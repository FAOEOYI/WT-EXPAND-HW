/**
   ******************************************************************
   * @file    aw9523b.c
   * @author  FAE_WanYujian
   * @version V1.0.0
   * @date    2025年4月14日
   * @brief   aw9523b IO拓展器驱动
   ******************************************************************
   * @attention
   *	模仿乐鑫的tca9554驱动，修改为aw9523b驱动
*********************************************************************
   */
/* TAG ---------------------------------------------------------*/
#include <string.h>
#include "i2c_bus.h"
#include "board.h"
#include "esp_log.h"
#include "aw9523b.h"

static char *TAG = "AW9523B";
static i2c_bus_handle_t i2c_handle;

#define SET_BITS(_m, _s, _v)  ((_v) ? (_m)|((_s)) : (_m)&~((_s)))
#define GET_BITS(_m, _s)      (((_m) & (_s)) ? true : false)

static uint8_t AW9523B_addr_wr = 0xB2;
static uint8_t AW9523B_addr_re = 0xB3;

// AW9523B register
#define P0_PORT_INPUT_STATE             0x00
#define P1_PORT_INPUT_STATE             0x01
#define P0_PORT_OUTPUT_STATE            0x02
#define P1_PORT_OUTPUT_STATE            0x03
#define P0_PORT_DIRECTION_CONFIGURE     0x04
#define P1_PORT_DIRECTION_CONFIGURE     0x05
#define P0_PORT_INTERRUPT_ENABLE        0x06
#define P1_PORT_INTERRUPT_ENABLE        0x07
#define ID_REGISTER                     0x10
#define GLOBAL_CONTROL_REGISTER         0x11
#define P0_PORT_MODE_CONFIGURE          0x12
#define P1_PORT_MODE_CONFIGURE          0x13
#define P1_0_LED_CURRENT_CONTROL        0X20
#define P1_1_LED_CURRENT_CONTROL        0X21
#define P1_2_LED_CURRENT_CONTROL        0X22
#define P1_3_LED_CURRENT_CONTROL        0X23
#define P0_0_LED_CURRENT_CONTROL        0X24
#define P0_1_LED_CURRENT_CONTROL        0X25
#define P0_2_LED_CURRENT_CONTROL        0X26
#define P0_3_LED_CURRENT_CONTROL        0X27
#define P0_4_LED_CURRENT_CONTROL        0X28
#define P0_5_LED_CURRENT_CONTROL        0X29
#define P0_6_LED_CURRENT_CONTROL        0X2A
#define P0_7_LED_CURRENT_CONTROL        0X2B
#define P1_4_LED_CURRENT_CONTROL        0X2C
#define P1_5_LED_CURRENT_CONTROL        0X2D
#define P1_6_LED_CURRENT_CONTROL        0X2E
#define P1_7_LED_CURRENT_CONTROL        0X2F
#define SOFT_RESETN                     0X7F

static esp_err_t aw9523b_write_reg(uint8_t reg_addr, uint8_t data)
{
    return i2c_bus_write_bytes(i2c_handle, AW9523B_addr_wr, &reg_addr, sizeof(reg_addr), &data, sizeof(data));
}

static char aw9523b_read_reg(uint8_t reg_addr)
{
    uint8_t data;
    i2c_bus_read_bytes(i2c_handle, AW9523B_addr_re, &reg_addr, sizeof(reg_addr), &data, sizeof(data));
    return data;
}

static int i2c_init(gpio_num_t clk, gpio_num_t sda)
{
    int res = 0;
    i2c_config_t es_i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
        .scl_io_num = clk,
        .sda_io_num = sda,
    };
    i2c_handle = i2c_bus_create(I2C_NUM_0, &es_i2c_cfg);
    return res;
}

esp_aw9523b_io_level_t aw9523b_get_input_state(esp_aw9523b_gpio_port_t gpio_port,esp_aw9523b_gpio_num_t gpio_num)
{
    char data = 0;
    if (gpio_num < AW9523B_GPIO_NUM_MAX) {
        if(gpio_port == AW9523B_GPIO_PORT_0){
            data = aw9523b_read_reg(P0_PORT_INPUT_STATE);
        }else{
            data = aw9523b_read_reg(P1_PORT_INPUT_STATE);
        }  
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
        return AW9523B_LEVEL_ERROR;
    }
    return GET_BITS(data, gpio_num);
}

esp_aw9523b_io_level_t aw9523b_get_output_state(esp_aw9523b_gpio_port_t gpio_port,esp_aw9523b_gpio_num_t gpio_num)
{
    char data = 0;
    if (gpio_num < AW9523B_GPIO_NUM_MAX) {
        if(gpio_port == AW9523B_GPIO_PORT_0){
            data = aw9523b_read_reg(P0_PORT_OUTPUT_STATE);
        }else{
            data = aw9523b_read_reg(P1_PORT_OUTPUT_STATE);
        }  
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
        return AW9523B_LEVEL_ERROR;
    }
    return GET_BITS(data, gpio_num);
}

esp_err_t aw9523b_set_output_state(esp_aw9523b_gpio_port_t gpio_port,esp_aw9523b_gpio_num_t gpio_num, esp_aw9523b_io_level_t level)
{
    char data;
    esp_err_t res = ESP_FAIL;
    if (gpio_num < AW9523B_GPIO_NUM_MAX) {
        if(gpio_port == AW9523B_GPIO_PORT_0)
        {
            data = aw9523b_read_reg(P0_PORT_OUTPUT_STATE);
            res = aw9523b_write_reg(P0_PORT_OUTPUT_STATE, SET_BITS(data, gpio_num, level));
        }
        if(gpio_port == AW9523B_GPIO_PORT_1)
        {
            data = aw9523b_read_reg(P1_PORT_OUTPUT_STATE);
            res = aw9523b_write_reg(P1_PORT_OUTPUT_STATE, SET_BITS(data, gpio_num, level));
        }
        
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
    }
    return res;
}

esp_err_t aw9523b_set_io_config(esp_aw9523b_gpio_port_t gpio_port , esp_aw9523b_gpio_num_t gpio_num, esp_aw9523b_io_config_t io_config)
{
    char data;
    esp_err_t res = ESP_FAIL;
    if (gpio_num < AW9523B_GPIO_NUM_MAX) {
        if(gpio_port==AW9523B_GPIO_PORT_0)
        {
            data = aw9523b_read_reg(P0_PORT_DIRECTION_CONFIGURE);
            res = aw9523b_write_reg(P0_PORT_DIRECTION_CONFIGURE, SET_BITS(data, gpio_num, io_config));
        }else{
            data = aw9523b_read_reg(P1_PORT_DIRECTION_CONFIGURE);
            res = aw9523b_write_reg(P1_PORT_DIRECTION_CONFIGURE, SET_BITS(data, gpio_num, io_config));
        }        
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
    }
    return res;
}

esp_err_t aw9523b_set_pin_mode(esp_aw9523b_gpio_port_t gpio_port ,esp_aw9523b_gpio_num_t gpio_num, aw9523_mode_t mode)
{
    char data;
    esp_err_t res = ESP_FAIL;
    if (gpio_num < AW9523B_GPIO_NUM_MAX) {
        if(gpio_port==AW9523B_GPIO_PORT_0)
        {
            data = aw9523b_read_reg(P0_PORT_MODE_CONFIGURE);
            res = aw9523b_write_reg(P0_PORT_MODE_CONFIGURE, SET_BITS(data, gpio_num, mode));
        }else{
            data = aw9523b_read_reg(P1_PORT_MODE_CONFIGURE);
            res = aw9523b_write_reg(P1_PORT_MODE_CONFIGURE, SET_BITS(data, gpio_num, mode));
        }        
    } else {
        ESP_LOGE(TAG, "gpio num is error, current gpio: %d", gpio_num);
    }
    return res;

}

esp_err_t aw9523b_read_ID(void)
{
    return aw9523b_read_reg(ID_REGISTER);
}

esp_err_t aw9523b_reset(void)
{
    return aw9523b_write_reg(SOFT_RESETN, 0x00);
}


esp_err_t aw9523b_init(esp_aw9523b_config_t *cfg)
{
    esp_err_t ret = ESP_OK;
    i2c_init(cfg->i2c_scl, cfg->i2c_sda);
    char data=aw9523b_read_ID();
    if (data == 0x23) {
        ESP_LOGI(TAG, "[--\aw9523b init success/--]");
        printf("AW9523B_ID:[%#x]\r\n",data);
    } else {
        ESP_LOGE(TAG, "aw9523b init fail");
        ret = ESP_FAIL;
    }
    return ret;
}

esp_err_t aw9523b_deinit()
{
    i2c_bus_delete(i2c_handle);
    return ESP_OK;
}

void aw9523b_test(void)
{
    printf("aw9523b_test\r\n");
}

void aw9523b_set_pa(esp_aw9523b_io_level_t level)
{
    if (level == AW9523B_IO_HIGH) {
        aw9523b_set_output_state(AW9523B_GPIO_PORT_1,AW9523B_GPIO_NUM_5,AW9523B_IO_HIGH);
    } else {
        aw9523b_set_output_state(AW9523B_GPIO_PORT_1,AW9523B_GPIO_NUM_5,AW9523B_IO_LOW);
    }
}

