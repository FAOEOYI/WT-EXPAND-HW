// /**
//    ******************************************************************
//    * @file    TOUCH_FT6336U_TEST
//    * @author  FAE_WanYujian
//    * @version V1.0.0
//    * @date    2025年4月21日
//    * @brief   FT5X06 驱动测试例程
//    * @note    此例程用于测试 FT5X06 驱动的功能。
//    *          请确保在使用前正确初始化 FT5X06 并调用相应的函数进行测试。
//    ******************************************************************
//    * @attention
//    *	此程序用于测试 FT5X06 驱动的功能。
// *********************************************************************
//  */
// /* TAG ---------------------------------------------------------*/
static const char *TAG = "【TOUCH_FT5X06_TEST】";
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
#include "esp_lcd_st7796.h"
#include "esp_lcd_types.h"
#include "esp_lcd_panel_interface.h"

#include "esp_psram.h"

#include "ft5x06.h"



// 声明触摸屏驱动结构体
touch_panel_driver_t touch;
// 全局变量，用于存储颜色数据
uint16_t *color_data;

// 创建触摸检测任务
static void touch_task(void *pvParameters)
{
    ESP_LOGI(TAG, "触摸检测任务开始");

    while (1) {
        touch_panel_points_t points;
        touch.read_point_data(&points);

        if(ft5x06_is_press() == 1){
            printf("【触摸事件】\n");
        }
        
        if ( points.event == TOUCH_EVT_PRESS) {
            int32_t x = points.curx[0];
            int32_t y = points.cury[0];
            ESP_LOGI(TAG, "触摸点坐标: (%ld, %ld)", x, y);
            
            // 在这里处理触摸事件，例如绘制图形或执行其他操作
        }
        
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
// 初始化I2C总线和触摸屏
static void init_touch_panel(void)
{
    // 初始化FT5x06触摸屏驱动
    touch_panel_find_driver(TOUCH_PANEL_CONTROLLER_FT5X06, &touch);
    touch.init();
    ESP_LOGI(TAG, "初始化FT5x06触摸屏驱动成功");
}


void app_main(void)
{
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
     // 填充颜色数据
     for (int i = 0; i < LCD_H_RES * LCD_V_RES; i++) {
        color_data[i] = 0xffffff;
    }
     // 调用 esp_lcd_panel_draw_bitmap 绘制
     esp_err_t ret = esp_lcd_panel_draw_bitmap(panel_handle, 0, 0, LCD_H_RES, LCD_V_RES, color_data);
     if (ret == ESP_OK) {
         ESP_LOGI(TAG, "绘制成功");
     } else {
         ESP_LOGE(TAG, "绘制失败: %s", esp_err_to_name(ret));
     }
     vTaskDelay(1000 / portTICK_PERIOD_MS);
     // 释放内存
    free(color_data);

     // 初始化触摸屏
    init_touch_panel();
    
    // 创建触摸检测任务
    xTaskCreate(touch_task, "touch_task", 4096, NULL, 5, NULL);

}
