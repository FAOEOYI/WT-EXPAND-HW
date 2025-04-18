// /**
//    ******************************************************************
//    * @file    LCD_ST7796_TEST
//    * @author  FAE_万宇健
//    * @version V1.0.0
//    * @date    2025年4月14日
//    * @brief   LCD_ST7796 驱动测试例程
//    * @note    此例程用于测试 LCD_ST7796 驱动的功能。
//    *          请确保在使用前正确初始化 LCD_ST7796 并调用相应的函数进行测试。
//    ******************************************************************
//    * @attention
//    *	此程序用于测试 LCD_ST7796 驱动的功能。
// *********************************************************************
//  */
// /* TAG ---------------------------------------------------------*/
static const char *TAG = "【LCD_ST7796_TEST】";
/* Includes ----------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "board.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "board.h"
#include "esp_lcd_panel_ops.h"
// #include "esp_lcd_st7796.h"
#include "esp_lcd_types.h"
#include "esp_lcd_panel_interface.h"

#include "esp_psram.h"


// 全局变量，用于存储颜色数据
uint16_t *color_data;

/**
 * @brief 将 RGB888 格式颜色转换为 RGB565 格式。
 * 
 * @param r 红色分量 (0-255)
 * @param g 绿色分量 (0-255)
 * @param b 蓝色分量 (0-255)
 * @return uint16_t 转换后的 RGB565 颜色值
 */
uint16_t rgb888_to_rgb565(uint8_t r, uint8_t g, uint8_t b) {
    // 截取高有效位并进行位移操作
    uint16_t rgb565 = (((r & 0xF8) << 8) |  // 取红色的高 5 位，并左移 8 位
                       ((g & 0xFC) << 3) |  // 取绿色的高 6 位，并左移 3 位
                       (b >> 3));           // 取蓝色的高 5 位
    return rgb565;
}



/**
 * @brief 将 RGB888 格式颜色（24 位十六进制值）转换为 RGB565 格式。
 * 
 * @param rgb888 输入的 RGB888 颜色值（如 0xFFFFFF）
 * @return uint16_t 转换后的 RGB565 颜色值
 */
uint16_t rgb888_to_rgb565_hex(uint32_t rgb888) {
    // 提取 R、G、B 分量
    uint8_t r = (rgb888 >> 16) & 0xFF;          // 提取红色分量 
    uint8_t g = (rgb888 >> 8) & 0xFF;           // 提取绿色分量
    uint8_t b = rgb888 & 0xFF;                  // 提取蓝色分量

    // 转换为 RGB565 格式
    uint16_t rgb565 = (((b & 0xF8) << 8) |  // 取红色的高 5 位，并左移 8 位
                       ((r & 0xF8) << 3) |  // 取绿色的高 6 位，并左移 3 位
                       (g >> 1));           // 取蓝色的高 5 位
    printf("RGB888: %#lx -> RGB565: %#x\n", rgb888, rgb565);

    return rgb565;
}



void app_main(void)
{
    if (!esp_psram_is_initialized()) {
        ESP_LOGE(TAG, "PSRAM 初始化失败");
        return;
    }
    ESP_LOGI(TAG, "PSRAM 初始化成功");


    // 初始化外设集合
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();       // 默认配置
    esp_periph_set_handle_t set = esp_periph_set_init(&periph_cfg);         // 初始化外设集合

    // 初始化屏幕
    esp_lcd_panel_handle_t panel_handle = audio_board_lcd_init(set, NULL);  // 初始化屏幕
    if (!panel_handle) {
        ESP_LOGE(TAG, "屏幕初始化失败");
        return;
    }

    color_data = heap_caps_malloc(LCD_H_RES * LCD_V_RES * LCD_BIT_PER_PIXEL/8, MALLOC_CAP_SPIRAM);
    if (!color_data) {
        ESP_LOGE(TAG, "未能为颜色数据分配 PSRAM 内存");
        return;
    }
    uint16_t RED_COLOR = rgb888_to_rgb565_hex(0xFF0000); // 红色
    uint16_t GREEN_COLOR = rgb888_to_rgb565_hex(0x00FF00); // 绿色
    uint16_t BLUE_COLOR = rgb888_to_rgb565_hex(0x0000FF); // 蓝色

    uint16_t white_color = rgb888_to_rgb565_hex(0xFFFFFF); // 白色
    uint16_t black_COLOR = rgb888_to_rgb565_hex(0x000000); // 黑色
    uint16_t YELLOW_COLOR = rgb888_to_rgb565_hex(0xFFFF00); // 黄色

    while(1){
        // 填充颜色数据为蓝色
        for (int i = 0; i < LCD_H_RES * LCD_V_RES; i++) {
            color_data[i] = BLUE_COLOR;
        }

        // 调用 esp_lcd_panel_draw_bitmap 绘制蓝色矩形
        esp_err_t ret = esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, color_data);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "蓝色矩形绘制成功");
        } else {
            ESP_LOGE(TAG, "绘制蓝色矩形失败: %s", esp_err_to_name(ret));
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        memset(color_data, 0, LCD_H_RES * LCD_V_RES * sizeof(uint16_t)/8);
        // 填充颜色数据为红色
        for (int i = 0; i < LCD_H_RES * LCD_V_RES; i++) {
            color_data[i] = RED_COLOR;
        }
        // 调用 esp_lcd_panel_draw_bitmap 绘制红色矩形
        ret = esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, color_data);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "红色矩形绘制成功");
        }else {
            ESP_LOGE(TAG, "绘制红色矩形失败: %s", esp_err_to_name(ret));    
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        memset(color_data, 0, LCD_H_RES * LCD_V_RES * sizeof(uint16_t)/8);
        // 填充颜色数据为绿色
        for (int i = 0; i < LCD_H_RES * LCD_V_RES; i++) {
            color_data[i] = GREEN_COLOR;
        }
        // 调用 esp_lcd_panel_draw_bitmap 绘制绿色矩形
        ret = esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, color_data);
        if (ret == ESP_OK) {
            
            ESP_LOGI(TAG, "绿色矩形绘制成功");
        } else {
            ESP_LOGE(TAG, "绘制绿色矩形失败: %s", esp_err_to_name(ret));
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        memset(color_data, 0, LCD_H_RES * LCD_V_RES * sizeof(uint16_t)/8);
        // 填充颜色数据为黄色
        for (int i = 0; i < LCD_H_RES * LCD_V_RES; i++) {
            color_data[i] = YELLOW_COLOR;
        }
        // 调用 esp_lcd_panel_draw_bitmap 绘制黄色矩形
        ret = esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, color_data);
        if (ret == ESP_OK) {
            
            ESP_LOGI(TAG, "黄色矩形绘制成功");
        } else {
            ESP_LOGE(TAG, "绘制黄色矩形失败: %s", esp_err_to_name(ret));
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        memset(color_data, 0, LCD_H_RES * LCD_V_RES * sizeof(uint16_t)/8);
    }
    
    // 释放内存
    free(color_data);

    // 销毁屏幕句柄
    esp_lcd_panel_del(panel_handle);
}