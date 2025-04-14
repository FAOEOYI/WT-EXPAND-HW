/**
   ******************************************************************
   * @file    AW9523B_GET_ID.c
   * @author  FAE_WanYujian
   * @version V1.0.0
   * @date    2025年4月14日
   * @brief   aw9523b IO拓展器获取设备ID代码
   ******************************************************************
   * @attention
   *	aw9523b IO拓展器获取设备ID代码
*********************************************************************
*/
#include <stdio.h>
#include "string.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "esp_err.h"
#include "esp_check.h"
#include "esp_system.h"
#include "aw9523b.h"
#include "driver/gpio.h"
#include "board.h"

void app_main(void)
{
    i2c_config_t i2c_config ;
    get_i2c_pins(I2C_NUM_0, &i2c_config);

    esp_aw9523b_config_t cfg = {
        .i2c_sda = i2c_config.sda_io_num,
        .i2c_scl = i2c_config.scl_io_num,
        .interrupt_output = -1,
    };
    aw9523b_init(&cfg);
}
