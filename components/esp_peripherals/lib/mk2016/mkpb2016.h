/**
   ******************************************************************
   * @file    mkpb2016.hh
   * @author  FAE_WanYujian
   * @version V1.0.0
   * @date    2025年4月15日
   * @brief   mkpb2016 接近传感器驱动
   ******************************************************************
   * @attention
   *	mkpb2016 接近传感器驱动
*********************************************************************
   */
// 确保头文件只被包含一次，防止重复包含
#pragma once

// 包含标准整数类型定义头文件
#include "stdint.h"
// 包含布尔类型定义头文件
#include <stdbool.h>

// 如果是 C++ 编译环境，使用 extern "C" 确保函数名不被 C++ 编译器修改
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 mkpb2016 设备
 * 
 * @param sda I2C 数据线引脚
 * @param scl I2C 时钟线引脚
 */
void mktp2016_init(uint8_t sda, uint8_t scl);

/**
 * @brief 从 mkpb2016 设备读取接近值
 * 
 * @param proximity 用于存储接近值的指针
 */
void mktp2016_read_value(uint16_t *proximity);

/**
 * @brief 判断是否有物体接近
 * 
 * @return true 有物体接近
 * @return false 没有物体接近
 */
bool Contact_Determination(void);

/**
 * @brief 接近检测任务
 * 
 * @param arg 任务参数
 * @return void* 无返回值
 */
void *proximity_detection_task(void *arg);

// 如果是 C++ 编译环境，结束 extern "C" 块
#ifdef __cplusplus
}
#endif
