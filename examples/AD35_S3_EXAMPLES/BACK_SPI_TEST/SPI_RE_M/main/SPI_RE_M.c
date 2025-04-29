#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h" // 引入FreeRTOS的头文件
#include "freertos/task.h"     // 引入FreeRTOS任务相关的头文件

#include "driver/spi_master.h" // 引入SPI主设备驱动程序的头文件
#include "driver/spi_common.h" // 引入SPI通用驱动程序的头文件
#include "driver/gpio.h"       // 引入GPIO驱动程序的头文件
#include "hal/spi_types.h"     // 引入ISP类型的头文件

#include "esp_system.h"        // 引入ESP-IDF系统相关的头文件
#include "esp_err.h"           // 引入ESP-IDF错误码的头文件
#include "esp_log.h"           // 引入ESP-IDF日志库的头文件

#include "board.h"             // 引入板级配置的头文件

static const char *TAG = "SPI_RE_M"; // 日志标签

/*
	定义发送和接收缓冲区
	tx_data是一个指向uint8_t类型的指针，用于存储要发送的数据
	rx_data是一个指向uint8_t类型的指针，用于存储接收到的数据
*/
uint8_t tx_data[4] = {0x01, 0x03, 0x07, 0x0F}; // 发送缓冲区
uint8_t rx_data[4] = {0};                      // 接收缓冲区
/*
	主函数
*/
void app_main(void)
{
    esp_err_t ret;
    spi_device_handle_t handle; // SPI设备的句柄
    ret = audio_board_back_SPI_init(&handle); // 初始化SPI总线和设备
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI: %s", esp_err_to_name(ret));
        return;
    }
    // 创建SPI事务
    spi_transaction_t trans_desc;
    memset(&trans_desc, 0, sizeof(trans_desc));
    trans_desc.addr = 0;             // 地址阶段的值，0表示没有地址阶段
    trans_desc.cmd = 0;              // 命令阶段的值，0表示没有命令阶段
    trans_desc.flags = 0;            // 事务的标志，0表示没有特殊标志
    trans_desc.length = 8 * 4;       // 事务的长度，单位为位
    trans_desc.rxlength = 8 * 4;     // 接收的长度，单位为位
    trans_desc.tx_buffer = tx_data;  // 发送缓冲区的指针
    trans_desc.rx_buffer = rx_data;  // 接收缓冲区的指针

    /*
        发送SPI事务
        spi_device_transmit函数使用指定的SPI设备发送事务
        第一个参数是SPI设备的句柄
        第二个参数是一个指向spi_transaction_t结构体的指针，该结构体定义了一个SPI事务
    */
    ret = spi_device_transmit(handle, &trans_desc);
    ESP_LOGI(TAG, "SPI transmit: %s", esp_err_to_name(ret));

    // 打印接收到的数据
    ESP_LOGI(TAG, "Received data: %02x %02x %02x %02x",rx_data[0], rx_data[1], rx_data[2], rx_data[3]);
    /*
        移除SPI设备
        spi_bus_remove_device函数从SPI总线上移除指定的SPI设备
        参数是要移除的SPI设备的句柄
    */
   while(1){
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 延时1秒
        ret = spi_device_transmit(handle, &trans_desc);
        ESP_LOGI(TAG, "SPI transmit: %s", esp_err_to_name(ret));
        // 打印接收到的数据
        ESP_LOGI(TAG, "Received data: %02x %02x %02x %02x",rx_data[0], rx_data[1], rx_data[2], rx_data[3]);
    }
    spi_bus_remove_device(handle);

    /*
        释放SPI总线
        spi_bus_free函数释放HSPI总线
        参数是要释放的SPI总线的主机ID
    */
    spi_bus_free(SPI2_HOST);
}
