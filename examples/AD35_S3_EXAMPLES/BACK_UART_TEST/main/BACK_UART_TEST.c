#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "stdbool.h"
#include "esp_bit_defs.h"
#include "freertos/event_groups.h"
#include "board.h"

static const char *TAG = "TX_RX_TEST";

//事件标志组
EventGroupHandle_t event_group;

static const int RX_BUF_SIZE = 1024;

#define TXD_PIN BACK_UART_TX
#define RXD_PIN BACK_UART_RX

/**
  *************************
  * @brief  UART串口配置初始化函数
  * @note  	None
  * @param  None
  * @retval None
  *************************
  */
void UART_Config_init(void)
{
    const uart_config_t uart_config = {
                                        .baud_rate = 115200,                            //波特率
                                        .data_bits = UART_DATA_8_BITS,                  //数据位
                                        .parity = UART_PARITY_DISABLE,                  //奇偶校验
                                        .stop_bits = UART_STOP_BITS_1,                  //停止位
                                        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,          //硬件流控
                                        .source_clk = UART_SCLK_DEFAULT,                //时钟源
                                      };
    //不使用缓冲区来发送数据。
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);                    //安装驱动程序
    uart_param_config(UART_NUM_1, &uart_config);                                        //配置uart参数
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); //配置引脚
}


/**
  *************************
  * @brief  UART串口发送数据函数
  * @note  	None
  * @param  None
  * @retval None
  *************************
  */
int sendData(char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    return txBytes;
}

/**
  *************************
  * @brief  串口接受信息函数
  * @note  	rx->GPIO5
  * @param  None
  * @retval None
  *************************
  */
 static void rx_task(void *arg)
 {
    uint8_t* RX_data = (uint8_t*) arg;
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
    while (1) {
        const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 100 / portTICK_PERIOD_MS);
        if (rxBytes > 0) {
            data[rxBytes] = 0;
            ESP_LOGI(TAG, "Read %d bytes: '%s'", rxBytes, data);
            // printf("%s\n", data);
            memcpy(RX_data, data, rxBytes);
            // printf("%s\n", RX_data);
            sendData((char*) RX_data);
            
        }
    }
    free(data);
 }
 
void app_main(void)
{
    uint8_t *DATA = (uint8_t *)malloc(1024);
    memset(DATA, 0, 1024);
    UART_Config_init();     //初始化串口
    //创建任务
    xTaskCreate(rx_task, "uart_rx_task", 1024 * 2, (void*)DATA, configMAX_PRIORITIES - 1, NULL);
}

//-----------------------------------------------------------------------------------------------------------

