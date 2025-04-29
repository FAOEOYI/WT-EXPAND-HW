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
#include "driver/gpio.h"
#include <string.h>
#include "board.h"
#include "audio_error.h"
#include "audio_mem.h"
#include "soc/soc_caps.h"

static const char *TAG = "ESP32_S3_KORVO_2";

esp_err_t get_i2c_pins(i2c_port_t port, i2c_config_t *i2c_config)
{
    AUDIO_NULL_CHECK(TAG, i2c_config, return ESP_FAIL);
    if (port == I2C_NUM_0) {
        i2c_config->sda_io_num = GPIO_NUM_5;
        i2c_config->scl_io_num = GPIO_NUM_4;
    } else if(port == I2C_NUM_1){
        i2c_config->sda_io_num = GPIO_NUM_48;
        i2c_config->scl_io_num = GPIO_NUM_38;
    }else{
        memset(i2c_config, 0, sizeof(i2c_config_t));
        ESP_LOGE(TAG, "i2c port %d is not supported", port);
        return ESP_FAIL;
    }
    return ESP_OK;
}

esp_err_t get_i2s_pins(int port, board_i2s_pin_t *i2s_config)
{
    AUDIO_NULL_CHECK(TAG, i2s_config, return ESP_FAIL);
    if (port == 0) {
        i2s_config->bck_io_num = GPIO_NUM_14;
        i2s_config->ws_io_num = GPIO_NUM_13;
        i2s_config->data_out_num = GPIO_NUM_12;
        i2s_config->data_in_num = GPIO_NUM_21;
        i2s_config->mck_io_num = GPIO_NUM_47;
    } else if (port == 1) {
        i2s_config->bck_io_num = -1;
        i2s_config->ws_io_num = -1;
        i2s_config->data_out_num = -1;
        i2s_config->data_in_num = -1;
        i2s_config->mck_io_num = -1;
    } else {
        memset(i2s_config, -1, sizeof(board_i2s_pin_t));
        ESP_LOGE(TAG, "i2s port %d is not supported", port);
        return ESP_FAIL;
    }

    return ESP_OK;
}

esp_err_t get_spi_pins(spi_bus_config_t *spi_config, spi_device_interface_config_t *spi_device_interface_config)
{
    AUDIO_NULL_CHECK(TAG, spi_config, return ESP_FAIL);
    AUDIO_NULL_CHECK(TAG, spi_device_interface_config, return ESP_FAIL);

    // SPI 引脚初始化
    gpio_config_t io_conf = {
        .pin_bit_mask   = (1ULL << GPIO_NUM_39) | (1ULL << GPIO_NUM_40) | (1ULL << GPIO_NUM_41) | (1ULL << GPIO_NUM_42),
        .mode           = GPIO_MODE_OUTPUT, // 设置为输出模式
        .pull_up_en     = GPIO_PULLUP_ENABLE, // 启用上拉电阻
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,
        .intr_type      = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    // 配置SPI总线
    spi_config->mosi_io_num = GPIO_NUM_39;  // MISO引脚的GPIO编号
    spi_config->miso_io_num = GPIO_NUM_40;  // MOSI引脚的GPIO编号
    spi_config->sclk_io_num = GPIO_NUM_41;  // SCLK引脚的GPIO编号
    spi_config->quadwp_io_num = -1;         // QUADWP引脚的GPIO编号，-1表示不使用
    spi_config->quadhd_io_num = -1;         // QUADHD引脚的GPIO编号，-1表示不使用
    spi_config->max_transfer_sz = 32;       // 最大传输大小，单位为字节，这里设置为32字节

    esp_err_t ret = spi_bus_initialize(SPI2_HOST, spi_config, SPI_DMA_CH_AUTO);
    ESP_LOGI(TAG, "SPI bus initialize: %s", esp_err_to_name(ret));

     // 配置SPI设备
    spi_device_interface_config->clock_speed_hz = 1000000; // SPI时钟速度，单位为Hz,这里设置为1MHz
    spi_device_interface_config->mode = 0;                  // SPI模式，0表示模式0
    spi_device_interface_config->queue_size = 7;            // 事务队列的大小，这里设置为7
    spi_device_interface_config->spics_io_num = GPIO_NUM_42;// CS引脚的GPIO编号
    spi_device_interface_config->flags = 0;                // 事务的标志，0表示没有特殊标志
    spi_device_interface_config->clock_source = SPI_CLK_SRC_DEFAULT; // SPI时钟源，默认值为SPI_CLK_SRC_DEFAULT
    
    return ESP_OK;
}

// sdcard

int8_t get_sdcard_intr_gpio(void)
{
    return SDCARD_INTR_GPIO;
}

int8_t get_sdcard_open_file_num_max(void)
{
    return SDCARD_OPEN_FILE_NUM_MAX;
}

int8_t get_sdcard_power_ctrl_gpio(void)
{
    return SDCARD_PWR_CTRL;
}

// input-output pins

int8_t get_headphone_detect_gpio(void)
{
    return HEADPHONE_DETECT;
}

int8_t get_pa_enable_gpio(void)
{
    return PA_ENABLE_GPIO;
}

// adc button id

int8_t get_input_rec_id(void)
{
    return BUTTON_REC_ID;
}

int8_t get_input_mode_id(void)
{
    return BUTTON_MODE_ID;
}

int8_t get_input_set_id(void)
{
    return BUTTON_SET_ID;
}

int8_t get_input_play_id(void)
{
    return BUTTON_PLAY_ID;
}

int8_t get_input_volup_id(void)
{
    return BUTTON_VOLUP_ID;
}

int8_t get_input_voldown_id(void)
{
    return BUTTON_VOLDOWN_ID;
}

// led pins

int8_t get_green_led_gpio(void)
{
    return GREEN_LED_GPIO;
}

int8_t get_blue_led_gpio(void)
{
    return BLUE_LED_GPIO;
}

int8_t get_es8311_mclk_src(void)
{
    return ES8311_MCLK_SOURCE;
}
