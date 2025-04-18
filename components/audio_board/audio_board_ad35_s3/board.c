/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2021 <ESPRESSIF SYSTEMS (SHANGHAI) CO., LTD>
 *
 * Permission is hereby granted for use on all ESPRESSIF SYSTEMS products, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "esp_log.h"
#include "board.h"
#include "audio_mem.h"
#include "periph_sdcard.h"
#include "periph_adc_button.h"
#include "tca9554.h"
#include "aw9523b.h"
#include "esp_err.h"


static const char *TAG = "AUDIO_BOARD";

static audio_board_handle_t board_handle = 0;

#define DMA_BURST_SIZE 64 //16,32,64。当DMA缓冲区来自PSRAM时，更高的突发大小可以提高性能

audio_board_handle_t audio_board_init(void)
{
    if (board_handle) {
        ESP_LOGW(TAG, "The board has already been initialized!");
        return board_handle;
    }
    board_handle = (audio_board_handle_t) audio_calloc(1, sizeof(struct audio_board_handle));
    AUDIO_MEM_CHECK(TAG, board_handle, return NULL);
    board_handle->audio_hal = audio_board_codec_init();
    board_handle->adc_hal = audio_board_adc_init();
    return board_handle;
}

audio_hal_handle_t audio_board_adc_init(void)
{
    audio_hal_codec_config_t audio_codec_cfg = AUDIO_CODEC_DEFAULT_CONFIG();
    audio_codec_cfg.codec_mode = AUDIO_HAL_CODEC_MODE_ENCODE;
    audio_hal_handle_t adc_hal = NULL;
    adc_hal = audio_hal_init(&audio_codec_cfg, &AUDIO_CODEC_ES7210_DEFAULT_HANDLE);
    AUDIO_NULL_CHECK(TAG, adc_hal, return NULL);
    return adc_hal;
}

audio_hal_handle_t audio_board_codec_init(void)
{
    audio_hal_codec_config_t audio_codec_cfg = AUDIO_CODEC_DEFAULT_CONFIG();
    audio_hal_handle_t codec_hal = audio_hal_init(&audio_codec_cfg, &AUDIO_CODEC_ES8311_DEFAULT_HANDLE);
    AUDIO_NULL_CHECK(TAG, codec_hal, return NULL);
    return codec_hal;
}

esp_err_t _lcd_rest(esp_periph_handle_t self, void *ctx)
{
    // Reset the LCD
    // tca9554_set_output_state(LCD_RST_GPIO, TCA9554_IO_LOW);
    // vTaskDelay(100 / portTICK_PERIOD_MS);
    // tca9554_set_output_state(LCD_RST_GPIO, TCA9554_IO_HIGH);
    // vTaskDelay(200 / portTICK_PERIOD_MS);
    i2c_config_t i2c_config ;
    get_i2c_pins(I2C_NUM_0, &i2c_config);

    esp_aw9523b_config_t cfg = {
        .i2c_sda = i2c_config.sda_io_num,
        .i2c_scl = i2c_config.scl_io_num,
        .interrupt_output = -1,
    };

    aw9523b_init(&cfg);
    
    aw9523b_set_output_state(AW9523B_GPIO_PORT_1,LCD_RST_GPIO,AW9523B_IO_LOW);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    aw9523b_set_output_state(AW9523B_GPIO_PORT_1,LCD_RST_GPIO,AW9523B_IO_HIGH);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    return ESP_OK;
}



display_service_handle_t audio_board_led_init(void)
{
    return NULL;
}

// 修改为I80接口的获取函数
esp_err_t _get_lcd_io_bus(void *bus, esp_lcd_panel_io_i80_config_t *io_config,esp_lcd_panel_io_handle_t *out_panel_io)
{
    return esp_lcd_new_panel_io_i80((esp_lcd_i80_bus_handle_t)bus, io_config, out_panel_io);
}

static SemaphoreHandle_t refresh_finish = NULL;

IRAM_ATTR static bool notify_refresh_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    BaseType_t need_yield = pdFALSE;

    xSemaphoreGiveFromISR(refresh_finish, &need_yield);
    return (need_yield == pdTRUE);
}

void *audio_board_lcd_init(esp_periph_set_handle_t set, void *cb)
{
    // 初始化io拓展器部分
    // 对复位引脚进行操作
    i2c_config_t i2c_config;
    get_i2c_pins(I2C_NUM_0, &i2c_config);

    esp_aw9523b_config_t cfg_iic = {
        .i2c_sda = i2c_config.sda_io_num,
        .i2c_scl = i2c_config.scl_io_num,
        .interrupt_output = -1,
    };

    aw9523b_init(&cfg_iic);
    
    // aw9523b_set_output_state(AW9523B_GPIO_PORT_1,LCD_RST_GPIO,AW9523B_IO_LOW);
    // vTaskDelay(100 / portTICK_PERIOD_MS);
    // aw9523b_set_output_state(AW9523B_GPIO_PORT_1,LCD_RST_GPIO,AW9523B_IO_HIGH);
    // vTaskDelay(200 / portTICK_PERIOD_MS);

    aw9523_set_led_max_current(AW9523_37mA);
    aw9523b_set_pin_mode(AW9523B_GPIO_PORT_1, AW9523B_GPIO_NUM_0, AW9523B_PIN_LED_MODE);
    aw9523b_set_pin_mode(AW9523B_GPIO_PORT_1, AW9523B_GPIO_NUM_1, AW9523B_PIN_LED_MODE);
    aw9523b_set_pin_mode(AW9523B_GPIO_PORT_1, AW9523B_GPIO_NUM_2, AW9523B_PIN_LED_MODE);
    aw9523b_set_pin_mode(AW9523B_GPIO_PORT_1, AW9523B_GPIO_NUM_3, AW9523B_PIN_LED_MODE);
    aw9523_led_set_duty(AW9523B_GPIO_PORT_1 , AW9523B_GPIO_NUM_0,128);
    aw9523_led_set_duty(AW9523B_GPIO_PORT_1 , AW9523B_GPIO_NUM_1,128);
    aw9523_led_set_duty(AW9523B_GPIO_PORT_1 , AW9523B_GPIO_NUM_2,128);
    aw9523_led_set_duty(AW9523B_GPIO_PORT_1 , AW9523B_GPIO_NUM_3,128);


    
    // 初始化I80总线
    esp_lcd_i80_bus_handle_t i80_bus = NULL;
    esp_lcd_panel_io_handle_t io_handle = NULL;
    void *user_ctx = NULL;
    // 定义LCD总线配置
    esp_lcd_i80_bus_config_t bus_config = {
        .clk_src = LCD_CLK_SRC_PLL160M,
        .dc_gpio_num = LCD_I80_1_RS, // 数据/命令选择引脚
        .wr_gpio_num = LCD_1_WR_CLK_GPIO, // 写使能引脚
        .data_gpio_nums = {
            PIN_NUM_LCD_DATA0,
            PIN_NUM_LCD_DATA1,
            PIN_NUM_LCD_DATA2,
            PIN_NUM_LCD_DATA3,
            PIN_NUM_LCD_DATA4,
            PIN_NUM_LCD_DATA5,
            PIN_NUM_LCD_DATA6,
            PIN_NUM_LCD_DATA7, 
            PIN_NUM_LCD_DATA8, 
            PIN_NUM_LCD_DATA9, 
            PIN_NUM_LCD_DATA10, 
            PIN_NUM_LCD_DATA11,
            PIN_NUM_LCD_DATA12, 
            PIN_NUM_LCD_DATA13, 
            PIN_NUM_LCD_DATA14, 
            PIN_NUM_LCD_DATA15
        },
        .bus_width = LCD_DATA_WIDTH,
        .max_transfer_bytes = LCD_H_RES * LCD_V_RES * LCD_BIT_PER_PIXEL / 8,
        .dma_burst_size = DMA_BURST_SIZE,
        .psram_trans_align = 64,            
        .sram_trans_align = 4,  
    };
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&bus_config, &i80_bus));

    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = -1,
        .pclk_hz = 10 * 1000 * 1000,
        .trans_queue_depth = 10,
        .dc_levels = {
            .dc_idle_level = 0,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        .flags = {
            .swap_color_bytes = 0, // Swap can be done in LvGL (default) or DMA
            .reverse_color_bits = 0, // Reverse can be done in LvGL (default) or DMA
        },
        .on_color_trans_done = NULL,  // 使用传入的回调函数
        .user_ctx = NULL,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    
    // 直接初始化面板IO，不通过periph_lcd组件
    // ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_bus, &io_config, &io_handle));

    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = PIN_NUM_LCD_RST,
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
        .color_space = ESP_LCD_COLOR_SPACE_BGR,
#else
        .rgb_endian = LCD_RGB_ENDIAN_BGR,
#endif
        .bits_per_pixel = LCD_BIT_PER_PIXEL,
    };
    
    // 直接初始化ST7796面板
    // ESP_ERROR_CHECK(esp_lcd_new_panel_st7796(io_handle, &panel_config, &panel_handle));
    // ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    // ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    // ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
    // return (void *)panel_handle;
    // 如果仍然需要使用periph_lcd组件，则使用以下代码
    
    periph_lcd_cfg_t cfg = {
        .io_bus = (void *)i80_bus,
        .new_panel_io = _get_lcd_io_bus,
        .lcd_io_cfg = &io_config,
        .new_lcd_panel = esp_lcd_new_panel_st7796,
        .lcd_dev_cfg = &panel_config,
        .rest_cb = _lcd_rest,
        .rest_cb_ctx = NULL,
        .lcd_swap_xy = LCD_SWAP_XY,
        .lcd_mirror_x = LCD_MIRROR_X,
        .lcd_mirror_y = LCD_MIRROR_Y,
        .lcd_color_invert = LCD_COLOR_INV,
    };
    esp_periph_handle_t periph_lcd = periph_lcd_init(&cfg);
    AUDIO_NULL_CHECK(TAG, periph_lcd, return NULL);
    esp_periph_start(set, periph_lcd);
    return (void *)periph_lcd_get_panel_handle(periph_lcd);
    
    
    
}

esp_err_t audio_board_key_init(esp_periph_set_handle_t set)
{
    periph_adc_button_cfg_t adc_btn_cfg = PERIPH_ADC_BUTTON_DEFAULT_CONFIG();
    adc_arr_t adc_btn_tag = ADC_DEFAULT_ARR();
    adc_btn_tag.total_steps = 6;
    adc_btn_tag.adc_ch = ADC1_CHANNEL_4;
    int btn_array[7] = {190, 600, 1000, 1375, 1775, 2195, 3000};
    adc_btn_tag.adc_level_step = btn_array;
    adc_btn_cfg.arr = &adc_btn_tag;
    adc_btn_cfg.arr_size = 1;
    if (audio_mem_spiram_stack_is_enabled()) {
        adc_btn_cfg.task_cfg.ext_stack = true;
    }
    esp_periph_handle_t adc_btn_handle = periph_adc_button_init(&adc_btn_cfg);
    AUDIO_NULL_CHECK(TAG, adc_btn_handle, return ESP_ERR_ADF_MEMORY_LACK);
    return esp_periph_start(set, adc_btn_handle);
}

esp_err_t audio_board_sdcard_init(esp_periph_set_handle_t set, periph_sdcard_mode_t mode)
{
    if (mode != SD_MODE_1_LINE) {
        ESP_LOGE(TAG, "Current board only support 1-line SD mode!");
        return ESP_FAIL;
    }
    periph_sdcard_cfg_t sdcard_cfg = {
        .root = "/sdcard",
        .card_detect_pin = get_sdcard_intr_gpio(),
        .mode = mode
    };
    esp_periph_handle_t sdcard_handle = periph_sdcard_init(&sdcard_cfg);
    esp_err_t ret = esp_periph_start(set, sdcard_handle);
    int retry_time = 5;
    bool mount_flag = false;
    while (retry_time --) {
        if (periph_sdcard_is_mounted(sdcard_handle)) {
            mount_flag = true;
            break;
        } else {
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
    if (mount_flag == false) {
        ESP_LOGE(TAG, "Sdcard mount failed");
        return ESP_FAIL;
    }
    return ret;
}

audio_board_handle_t audio_board_get_handle(void)
{
    return board_handle;
}

esp_err_t audio_board_deinit(audio_board_handle_t audio_board)
{
    esp_err_t ret = ESP_OK;
    ret |= audio_hal_deinit(audio_board->audio_hal);
    ret |= audio_hal_deinit(audio_board->adc_hal);
    audio_free(audio_board);
    board_handle = NULL;
    return ret;
}
