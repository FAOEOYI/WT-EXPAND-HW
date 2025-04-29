// #include <stdio.h>
// #include <string.h>

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"

// #include "driver/spi_slave.h"
// #include "driver/gpio.h"

// #include "esp_system.h"
// #include "esp_err.h"
// #include "esp_log.h"

// static const char *TAG = "SPI_RE_S"; // 日志标签

// // 定义SPI引脚
// #define PIN_NUM_MOSI 17  // MOSI引脚的GPIO编号
// #define PIN_NUM_MISO 8  // MISO引脚的GPIO编号
// #define PIN_NUM_CLK  46  // SCLK引脚的GPIO编号
// #define PIN_NUM_CS   20
//   // CS引脚的GPIO编号

// // 定义发送和接收缓冲区
// uint8_t tx_data[4] = {0xF0, 0x70, 0x30, 0x10}; // 从机发送缓冲区
// uint8_t rx_data[4] = {0};                      // 从机接收缓冲区

// void app_main(void)
// {
//     esp_err_t ret;

//     // 配置SPI从机总线
//     spi_bus_config_t bus_config = {
//         .miso_io_num = PIN_NUM_MISO,
//         .mosi_io_num = PIN_NUM_MOSI,
//         .sclk_io_num = PIN_NUM_CLK,
//         .quadwp_io_num = -1,
//         .quadhd_io_num = -1,
//         .max_transfer_sz = 32,
//     };

//     // 配置SPI从机设备
//     spi_slave_interface_config_t slave_config = {
//         .mode = 0,                  // SPI模式，0表示模式0
//         .spics_io_num = PIN_NUM_CS, // CS引脚的GPIO编号
//         .queue_size = 3,            // 事务队列的大小
//         .flags = 0,                 // 从机标志，0表示没有特殊标志
//     };

//     // 初始化SPI从机设备
//     ret = spi_slave_initialize(SPI2_HOST, &bus_config, &slave_config, SPI_DMA_CH_AUTO);
//     ESP_LOGI(TAG, "SPI slave initialize: %s", esp_err_to_name(ret));

//     // 创建SPI事务
//     spi_slave_transaction_t trans_desc;

//     while(1) {
//         // 准备事务
//         memset(&trans_desc, 0, sizeof(trans_desc));
//         trans_desc.length = 8 * 4;       // 事务的长度，单位为位
//         trans_desc.tx_buffer = tx_data;  // 发送缓冲区的指针
//         trans_desc.rx_buffer = rx_data;  // 接收缓冲区的指针

//         // 等待并处理事务
//         ret = spi_slave_transmit(SPI2_HOST, &trans_desc, portMAX_DELAY);
//         if (ret != ESP_OK) {
//             ESP_LOGE(TAG, "SPI slave transmit failed: %s", esp_err_to_name(ret));
//             continue;
//         }

//         // 打印接收到的数据
//         ESP_LOGI(TAG, "Received data: %02x %02x %02x %02x", 
//                  rx_data[0], rx_data[1], rx_data[2], rx_data[3]);
        
//         // 更新发送缓冲区（可选，这里简单地将接收到的数据作为下一次的发送数据）
//         for (int i = 0; i < 4; i++) {
//             tx_data[i] = rx_data[i] ^ 0xFF; // 对接收到的数据进行异或操作作为响应
//         }
        
//         ESP_LOGI(TAG, "Will send data: %02x %02x %02x %02x", 
//                  tx_data[0], tx_data[1], tx_data[2], tx_data[3]);
//     }

//     // 注意：以下代码在while(1)循环中永远不会执行
//     // 释放SPI从机设备
//     spi_slave_free(SPI2_HOST);
// }


#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_slave.h"

#define GPIO_MOSI   4
#define GPIO_MISO   6
#define GPIO_SCLK   15
#define GPIO_CS     17

#define CMD_WRITE_REG 0x01
#define REG_ADDR      0x03

void app_main(void) {
    // 配置SPI总线
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_MOSI,
        .miso_io_num = GPIO_MISO,
        .sclk_io_num = GPIO_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };

    // 配置SPI从机接口
    spi_slave_interface_config_t slvcfg = {
        .mode = 0,
        .spics_io_num = GPIO_CS,
        .queue_size = 3,
        .flags = 0,
        .post_setup_cb = NULL,
        .post_trans_cb = NULL
    };

    // 初始化SPI从机
    esp_err_t ret = spi_slave_initialize(SPI2_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    // 准备接收/发送缓冲区
    uint8_t recvbuf[4] = {0};
    uint8_t sendbuf[4] = {0};

    // 配置事务
    spi_slave_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 4 * 8;  // 总共传输4字节（32位）
    t.tx_buffer = sendbuf;
    t.rx_buffer = recvbuf;

    // 回传验证数据
    sendbuf[0] = 0xAA;
    sendbuf[1] = 0x55;
    

    // 等待主机连接
    while (1) {
        // 等待传输事务
        ret = spi_slave_transmit(SPI2_HOST, &t, portMAX_DELAY);

        if (ret == ESP_OK) {
            // 解析接收到的数据
            uint8_t cmd = recvbuf[0];  // 命令字节
            uint8_t addr = recvbuf[1]; // 地址字节
            uint8_t data = recvbuf[2]; // 数据字节
            uint8_t flag = recvbuf[3]; // 标志字节

            printf("接收到SPI事务: 命令=0x%02x, 地址=0x%02x, 数据=0x%02x,标志=0x%02x\n", 
                   cmd, addr, data, flag);

            // 如果是写寄存器命令
            if (cmd == CMD_WRITE_REG && addr == REG_ADDR) {
                printf("写入寄存器0x%02x的值: 0x%02x\n", addr, data);
                sendbuf[2] = data;
                sendbuf[3] = flag;
            }
        }

        // 重置事务以准备下一次传输
        memset(recvbuf, 0, sizeof(recvbuf));
    }
}