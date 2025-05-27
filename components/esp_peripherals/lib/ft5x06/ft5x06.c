/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include "driver/i2c.h"
#include "i2c_bus.h"
#include "ft5x06.h"
#include "esp_log.h"
#include "string.h"

static const char *TAG = "FT5x06";

#define TOUCH_CHECK(a, str, ret)  if(!(a)) {                                   \
        ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str);   \
        return (ret);                                                           \
    }

#define WRITE_BIT          (I2C_MASTER_WRITE)       /*!< I2C master write */
#define READ_BIT           (I2C_MASTER_READ)        /*!< I2C master read */
#define ACK_CHECK_EN       0x1                      /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS      0x0                      /*!< I2C master will not check ack from slave */
#define ACK_VAL            0x0                      /*!< I2C ack value */
#define NACK_VAL           0x1                      /*!< I2C nack value */

#define FT5x06_DEVICE_MODE                0x00
#define FT5x06_GESTURE_ID                 0x01
#define FT5x06_TOUCH_POINTS               0x02

#define FT5x06_TOUCH1_EV_FLAG             0x03
#define FT5x06_TOUCH1_XH                  0x03
#define FT5x06_TOUCH1_XL                  0x04
#define FT5x06_TOUCH1_YH                  0x05
#define FT5x06_TOUCH1_YL                  0x06

#define FT5x06_TOUCH2_EV_FLAG             0x09
#define FT5x06_TOUCH2_XH                  0x09
#define FT5x06_TOUCH2_XL                  0x0A
#define FT5x06_TOUCH2_YH                  0x0B
#define FT5x06_TOUCH2_YL                  0x0C

#define FT5x06_TOUCH3_EV_FLAG             0x0F
#define FT5x06_TOUCH3_XH                  0x0F
#define FT5x06_TOUCH3_XL                  0x10
#define FT5x06_TOUCH3_YH                  0x11
#define FT5x06_TOUCH3_YL                  0x12

#define FT5x06_TOUCH4_EV_FLAG             0x15
#define FT5x06_TOUCH4_XH                  0x15
#define FT5x06_TOUCH4_XL                  0x16
#define FT5x06_TOUCH4_YH                  0x17
#define FT5x06_TOUCH4_YL                  0x18

#define FT5x06_TOUCH5_EV_FLAG             0x1B
#define FT5x06_TOUCH5_XH                  0x1B
#define FT5x06_TOUCH5_XL                  0x1C
#define FT5x06_TOUCH5_YH                  0x1D
#define FT5x06_TOUCH5_YL                  0x1E

#define FT5X0X_REG_THGROUP                0x80   /* touch threshold related to sensitivity */
#define FT5X0X_REG_THPEAK                 0x81
#define FT5X0X_REG_THCAL                  0x82
#define FT5X0X_REG_THWATER                0x83
#define FT5X0X_REG_THTEMP                 0x84
#define FT5X0X_REG_THDIFF                 0x85
#define FT5X0X_REG_CTRL                   0x86
#define FT5X0X_REG_TIMEENTERMONITOR       0x87
#define FT5X0X_REG_PERIODACTIVE           0x88   /* report rate */
#define FT5X0X_REG_PERIODMONITOR          0x89
#define FT5X0X_REG_HEIGHT_B               0x8a
#define FT5X0X_REG_MAX_FRAME              0x8b
#define FT5X0X_REG_DIST_MOVE              0x8c
#define FT5X0X_REG_DIST_POINT             0x8d
#define FT5X0X_REG_FEG_FRAME              0x8e
#define FT5X0X_REG_SINGLE_CLICK_OFFSET    0x8f
#define FT5X0X_REG_DOUBLE_CLICK_TIME_MIN  0x90
#define FT5X0X_REG_SINGLE_CLICK_TIME      0x91
#define FT5X0X_REG_LEFT_RIGHT_OFFSET      0x92
#define FT5X0X_REG_UP_DOWN_OFFSET         0x93
#define FT5X0X_REG_DISTANCE_LEFT_RIGHT    0x94
#define FT5X0X_REG_DISTANCE_UP_DOWN       0x95
#define FT5X0X_REG_ZOOM_DIS_SQR           0x96
#define FT5X0X_REG_RADIAN_VALUE           0x97
#define FT5X0X_REG_MAX_X_HIGH             0x98
#define FT5X0X_REG_MAX_X_LOW              0x99
#define FT5X0X_REG_MAX_Y_HIGH             0x9a
#define FT5X0X_REG_MAX_Y_LOW              0x9b
#define FT5X0X_REG_K_X_HIGH               0x9c
#define FT5X0X_REG_K_X_LOW                0x9d
#define FT5X0X_REG_K_Y_HIGH               0x9e
#define FT5X0X_REG_K_Y_LOW                0x9f
#define FT5X0X_REG_AUTO_CLB_MODE          0xa0
#define FT5X0X_REG_LIB_VERSION_H          0xa1
#define FT5X0X_REG_LIB_VERSION_L          0xa2
#define FT5X0X_REG_CIPHER                 0xa3
#define FT5X0X_REG_MODE                   0xa4
#define FT5X0X_REG_PMODE                  0xa5   /* Power Consume Mode        */
#define FT5X0X_REG_FIRMID                 0xa6   /* Firmware version */
#define FT5X0X_REG_STATE                  0xa7
#define FT5X0X_REG_FT5201ID               0xa8
#define FT5X0X_REG_ERR                    0xa9
#define FT5X0X_REG_CLB                    0xaa

#define TOUCH_CHECK(a, str, ret)  if(!(a)) {                                      \
    ESP_LOGE(TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str);   \
    return (ret);                                                                   \
}



#define CONFIG_TOUCH_DRIVER_FT5X06 

#ifdef CONFIG_TOUCH_DRIVER_FT5X06
extern touch_panel_driver_t ft5x06_default_driver;
#endif
#ifdef CONFIG_TOUCH_DRIVER_EXAMPLE
extern touch_panel_driver_t example_default_driver;
#endif
#ifdef CONFIG_TOUCH_DRIVER_MAX
extern touch_panel_driver_t max_default_driver;
#endif

esp_err_t touch_panel_find_driver(touch_panel_controller_t controller, touch_panel_driver_t *out_driver)
{
    TOUCH_CHECK(NULL != out_driver, "Pointer of Touch driver is invalid", ESP_ERR_INVALID_ARG);

    esp_err_t ret = ESP_OK;
    switch (controller) {
#ifdef CONFIG_TOUCH_DRIVER_FT5X06
    case TOUCH_PANEL_CONTROLLER_FT5X06:
        *out_driver = ft5x06_default_driver;
        break;
#endif
#ifdef CONFIG_TOUCH_DRIVER_EXAMPLE
    case TOUCH_PANEL_CONTROLLER_XPT2046:
        *out_driver = xpt2046_default_driver;
        break;
#endif
#ifdef CONFIG_TOUCH_DRIVER_MAX
    case TOUCH_PANEL_CONTROLLER_NS2016:
        *out_driver = ns2016_default_driver;
        break;
#endif
    default:
        ESP_LOGE(TAG, "Not support touch panel controller, it may not be enabled in menuconfig");
        ret = ESP_ERR_NOT_FOUND;
        break;
    }

    return ret;
}

// static i2c_bus_handle_t i2c_handle = NULL;

static uint8_t ft5x06_addr_wr = 0x70;
static uint8_t ft5x06_addr_re = 0x71;

typedef struct {
    i2c_bus_handle_t i2c_dev;
    int pin_num_int;
    touch_panel_dir_t direction;
    uint16_t width;
    uint16_t height;
} ft5x06_dev_t;

static ft5x06_dev_t g_dev;

static int i2c_init(gpio_num_t clk, gpio_num_t sda)
{
    i2c_config_t es_i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
        .scl_io_num = clk,
        .sda_io_num = sda,
    };
    g_dev.i2c_dev = i2c_bus_create(I2C_NUM_0, &es_i2c_cfg);
    return ESP_OK;
}



// static esp_err_t ft5x06_calibration_run(esp_lcd_panel_handle_t screen, bool recalibrate);

// touch_panel_driver_t ft5x06_default_driver = {
//     .init = ft5x06_init,
//     .deinit = ft5x06_deinit,
//     .calibration_run = ft5x06_calibration_run,
//     .set_direction = ft5x06_set_direction,
//     .read_point_data = ft5x06_sample,
// };

static esp_err_t ft5x06_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
     // 检查传入的指针是否有效
     if (data == NULL || len == 0) {
        return ESP_ERR_INVALID_ARG;
    }

    // 使用 I2C 总线读取指定寄存器地址的数据
    esp_err_t ret = i2c_bus_read_bytes(g_dev.i2c_dev, ft5x06_addr_re, &reg_addr, sizeof(reg_addr), data, len);
    if (ret != ESP_OK) {
        // 如果读取失败，输出错误日志（可以根据需要添加）
        return ret;
    }

    return ESP_OK;
}

static esp_err_t ft5x06_write(uint8_t reg_addr, uint8_t data)
{
    esp_err_t ret;

    ret = i2c_bus_write_bytes(g_dev.i2c_dev, ft5x06_addr_wr, &reg_addr, sizeof(reg_addr), &data, sizeof(data));
    if (ESP_OK != ret) {
        ESP_LOGI(TAG, "i2c error %s", esp_err_to_name(ret));
    }
    return ret;
}

// static esp_err_t ft5x06_write_reg(ft5x06_dev_t *dev, uint8_t reg, uint8_t val)
// {
//     return ft5x06_write(dev, reg, 1, &val);
// }

// static esp_err_t ft5x06_read_reg(ft5x06_dev_t *dev, uint8_t reg, uint8_t *val)
// {
//     return ft5x06_read(dev, reg, 1, val);
// }

static uint8_t ft5x06_read_fw_ver(void)
{
    uint8_t ver;
    // ft5x06_read_reg(dev, FT5X0X_REG_FIRMID, &ver);
    ft5x06_read(FT5X0X_REG_FIRMID,&ver,1);
    return (ver);
}

esp_err_t ft5x06_init(void)
{
    esp_err_t ret = ESP_OK;
    #ifdef CONFIG_AUDIO_BOARD_AD35_S3
    touch_panel_config_t touch_cfg = {
         .interface_i2c = {
            .i2c_addr = 0x38,
        },
        .interface_type = TOUCH_PANEL_IFACE_I2C,
        .pin_num_int = TP_I2C_INT_PIN,
        .direction = TOUCH_DIR_LRTB,
        .width = LCD_H_RES,
        .height = LCD_V_RES,
    };
    
    i2c_config_t i2c_config ;
    get_i2c_pins(I2C_NUM_0, &i2c_config);
    i2c_init(i2c_config.scl_io_num,i2c_config.sda_io_num);

    if(g_dev.i2c_dev == NULL) {
        ESP_LOGE(TAG, "i2c init failed");
        return ESP_FAIL;
    }else{
        ESP_LOGI(TAG, "i2c init success");
    }

    if (touch_cfg.pin_num_int >= 0) {
        esp_rom_gpio_pad_select_gpio(touch_cfg.pin_num_int);
        gpio_set_direction(touch_cfg.pin_num_int, GPIO_MODE_INPUT);
        ESP_LOGI(TAG, "GPIO INT:%d", touch_cfg.pin_num_int);
        gpio_set_intr_type(touch_cfg.pin_num_int, GPIO_INTR_POSEDGE);
    }

    // g_dev.i2c_dev = i2c_dev;
    g_dev.pin_num_int = touch_cfg.pin_num_int;
    g_dev.width = touch_cfg.width;
    g_dev.height = touch_cfg.height;
    ft5x06_set_direction(touch_cfg.direction);

    // esp_err_t ret = ESP_OK;
    // ft5x06_dev_t *dev = &g_dev;
    ESP_LOGI(TAG, "FT5x06 frameware version [%x]", ft5x06_read_fw_ver());
    ESP_LOGI(TAG, "Touch panel size width: %d, height: %d", g_dev.width, g_dev.height);
    // Init default values. (From NHD-3.5-320240MF-ATXL-CTP-1 datasheet)
    // Valid touching detect threshold
    ret |= ft5x06_write( FT5X0X_REG_THGROUP, 0x16);

    // valid touching peak detect threshold
    ret |= ft5x06_write(FT5X0X_REG_THPEAK, 0x3C);

    // Touch focus threshold
    ret |= ft5x06_write(FT5X0X_REG_THCAL, 0xE9);

    // threshold when there is surface water
    ret |= ft5x06_write(FT5X0X_REG_THWATER, 0x01);

    // threshold of temperature compensation
    ret |= ft5x06_write(FT5X0X_REG_THTEMP, 0x01);

    // Touch difference threshold
    ret |= ft5x06_write(FT5X0X_REG_THDIFF, 0xA0);

    // Delay to enter 'Monitor' status (s)
    ret |= ft5x06_write(FT5X0X_REG_TIMEENTERMONITOR, 0x0A);

    // Period of 'Active' status (ms)
    ret |= ft5x06_write(FT5X0X_REG_PERIODACTIVE, 0x06);

    // Timer to enter 'idle' when in 'Monitor' (ms)
    ret |= ft5x06_write(FT5X0X_REG_PERIODMONITOR, 0x28);
    TOUCH_CHECK(ESP_OK == ret, "ft5x06 write reg failed", ESP_FAIL);

    ESP_LOGI(TAG, "Initial successful | GPIO INT:%d | ADDR:0x%x | dir:%d",
             touch_cfg.pin_num_int, touch_cfg.interface_i2c.i2c_addr, touch_cfg.direction);
    #endif
    return ret;
}

esp_err_t ft5x06_deinit(void)
{
    i2c_bus_delete(g_dev.i2c_dev);
    memset(&g_dev, 0, sizeof(ft5x06_dev_t));
    return ESP_OK;
}

esp_err_t ft5x06_set_direction(touch_panel_dir_t dir)
{
    if (TOUCH_DIR_MAX < dir) {
        dir >>= 5;
    }
    g_dev.direction = dir;
    return ESP_OK;
}

int ft5x06_is_press(void)
{
    /**
     * @note There are two ways to determine weather the touch panel is pressed
     * 1. Read the IRQ line of touch controller
     * 2. Read number of points touched in the register
     */
    if ( g_dev.pin_num_int != -1 ) {
        return !gpio_get_level((gpio_num_t)g_dev.pin_num_int);
    }
    uint8_t points;
    ft5x06_read(FT5x06_TOUCH_POINTS, &points, 1);
    // points &= 0x07;
    // printf("points: %d\n", points);
    if (points != 1) {    // ignore no touch & multi touch
        return 0;
    }

    return 1;
}

static void ft5x06_apply_rotate(uint16_t *x, uint16_t *y)
{
    uint16_t _x = *x;
    uint16_t _y = *y;

    switch (g_dev.direction) {
    case TOUCH_DIR_LRTB:
        *x = _x;
        *y = _y;
        break;
    case TOUCH_DIR_LRBT:
        *x = _x;
        *y = g_dev.height - _y;
        break;
    case TOUCH_DIR_RLTB:
        *x = g_dev.width - _x;
        *y = _y;
        break;
    case TOUCH_DIR_RLBT:
        *x = g_dev.width - _x;
        *y = g_dev.height - _y;
        break;
    case TOUCH_DIR_TBLR:
        *x = _y;
        *y = _x;
        break;
    case TOUCH_DIR_BTLR:
        *x = _y;
        *y = g_dev.width - _x;
        break;
    case TOUCH_DIR_TBRL:
        *x = g_dev.height - _y;
        *y = _x;
        break;
    case TOUCH_DIR_BTRL:
        *x = g_dev.height - _y;
        *y = g_dev.width - _x;
        break;

    default:
        break;
    }
}

esp_err_t ft5x06_sample(touch_panel_points_t *info)
{
    TOUCH_CHECK(NULL != info, "Pointer invalid", ESP_FAIL);
    TOUCH_CHECK(NULL != g_dev.i2c_dev, "Uninitialized", ESP_ERR_INVALID_STATE);

    uint8_t data[4] = {0};
    // ft5x06_dev_t *dev = &g_dev;

    uint8_t point_num_data;
    ft5x06_read(FT5x06_TOUCH_POINTS, &point_num_data, 1);
    info->point_num= point_num_data;
    // printf("points: %#x\n", point_num_data);
    // printf("points: %#x\n", info->point_num);
    info->point_num &= 0x07;
    // printf("points: %d\n", info->point_num);

    if (info->point_num > 0 && info->point_num <= TOUCH_MAX_POINT_NUMBER) {
        uint16_t x[TOUCH_MAX_POINT_NUMBER];
        uint16_t y[TOUCH_MAX_POINT_NUMBER];

        for (size_t i = 0; i < info->point_num; i++) {
            ft5x06_read((FT5x06_TOUCH1_XH + i * 6), data,2 );
            x[i] = 0x0fff & ((uint16_t)(data[0]) << 8 | data[1]);
            ft5x06_read((FT5x06_TOUCH1_YH + i * 6), data,2 );
            y[i] = ((uint16_t)(data[0]) << 8 | data[1]);
            ft5x06_apply_rotate(&x[i], &y[i]);
        }
        memcpy(info->curx, x, sizeof(uint16_t) * info->point_num);
        memcpy(info->cury, y, sizeof(uint16_t) * info->point_num);

        info->event = TOUCH_EVT_PRESS;
    } else {
        info->curx[0] = 0;
        info->cury[0] = 0;
        info->event = TOUCH_EVT_RELEASE;
    }

    return ESP_OK;
}

esp_err_t ft5x06_calibration_run(esp_lcd_panel_handle_t screen, bool recalibrate)
{
    (void)screen;
    (void)recalibrate;
    /**
     * The capacitive touch screen does not need to be calibrated
     */
    return ESP_OK;
}

touch_panel_driver_t ft5x06_default_driver = {
    .init = ft5x06_init,
    .deinit = ft5x06_deinit,
    .calibration_run = ft5x06_calibration_run,
    .set_direction = ft5x06_set_direction,
    .read_point_data = ft5x06_sample,
};
