/**
   ******************************************************************
   * @file    MK2016_Proximity_Sensor_TEST
   * @author  FAE_WanYujian
   * @version V1.0.0
   * @date    2025年4月15日
   * @brief   mkpb2016 接近传感器检测
   * @note    mkpb2016 接近传感器驱动测试程序
   ******************************************************************
   * @attention
   *  此程序用于测试 mkpb2016 接近传感器的功能。
   *  请确保在使用前正确初始化传感器，并调用检测函数进行接近检测。
   *  此程序还包含了一个接近检测任务，用于在后台执行接近检测。
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
#include "driver/gpio.h"
#include "board.h"
#include "mkpb2016.h"

void app_main(void)
{
    i2c_config_t i2c_config ;
    get_i2c_pins(I2C_NUM_0, &i2c_config);
    printf("i2c_config.sda_io_num = %d\n", i2c_config.sda_io_num);
    printf("i2c_config.scl_io_num = %d\n", i2c_config.scl_io_num);
    mktp2016_init(i2c_config.sda_io_num, i2c_config.scl_io_num);
    xTaskCreate(proximity_detection_task, "proximity_detection_task", 2048, NULL, 5, NULL);
}
