/**
   ******************************************************************
   * @file    BACK_IIC_AHT20_TEST
   * @author  FAE_WanYujian
   * @version V1.0.0
   * @date    2025年4月24日
   * @brief   AD35-S3 后背IIC测试

   ******************************************************************
   * @attention
        本例程使用的是AD35-S3的ESP32S3 的第二个 I2C接口
        每5秒读取一次AHT20的温度和湿度数据，并打印到串口日志
*********************************************************************
   */
/* TAG ---------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "stdbool.h"
#include "esp_bit_defs.h"
#include "freertos/event_groups.h"
#include "board.h"
#include "i2c_bus.h"
#define TAG "AHT20"

#define AHT20_STATUS_REG        0x00 //状态寄存器
#define AHT20_INIT_REG          0xBE //初始化寄存器
#define AHT20_SOFTRESET_REG     0xBA //软复位寄存器
#define AHT20_MEASURE_REG       0xAC //触发测量寄存器

static uint8_t AHT20_addr_wr = 0x70;
static uint8_t AHT20_addr_re = 0x71;

static i2c_bus_handle_t i2c_handle;

static int i2c_init()
{
    int res = 0;
    i2c_config_t es_i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };
    res = get_i2c_pins(I2C_NUM_1, &es_i2c_cfg);
    i2c_handle = i2c_bus_create(I2C_NUM_1, &es_i2c_cfg);
    return res;
}

// 修改为支持写入多个字节的函数
static esp_err_t AHT20_write_regs(uint8_t reg_addr, uint8_t *data, size_t data_len)
{
    return i2c_bus_write_bytes(i2c_handle, AHT20_addr_wr, &reg_addr, sizeof(reg_addr), data, data_len);
}

// 修改为支持读取多个字节的函数
static esp_err_t AHT20_read_regs(uint8_t reg_addr, uint8_t *data, size_t data_len)
{
    return i2c_bus_read_bytes(i2c_handle, AHT20_addr_re, &reg_addr, sizeof(reg_addr), data, data_len);
}

uint8_t ATH20_Read_Status(void)//读取AHT10的状态寄存器
{
    uint8_t status;
    AHT20_read_regs(AHT20_STATUS_REG, &status, sizeof(status));
    return status;
}

uint8_t ATH20_Read_Cal_Enable(void)
{
    uint8_t val = 0;//ret = 0,

    val = ATH20_Read_Status();
    if((val & 0x68) == 0x08)  //判断NOR模式和校准输出是否有效
        return 1;
    else
        return 0;
}

void ATH20_Read_CTdata(uint32_t *ct) //读取AHT10的温度和湿度数据
{
    uint32_t RetuData = 0;
	uint16_t cnt = 0;
    uint8_t Data[10];
    uint8_t tmp[10];

    tmp[0] = 0x33;
    tmp[1] = 0x00;
    AHT20_write_regs(AHT20_MEASURE_REG, tmp, 2);//P0 口中断不使能
    vTaskDelay(75 / portTICK_PERIOD_MS);

    cnt = 0;
	while(((ATH20_Read_Status()&0x80) == 0x80))//等待忙状态结束
	{
        vTaskDelay(1 / portTICK_PERIOD_MS);
        if(cnt++ >= 100)
        {
            break;
        }
	}
    AHT20_read_regs(0x00, Data, 7);

	RetuData = 0;
    RetuData = (RetuData|Data[1]) << 8;
	RetuData = (RetuData|Data[2]) << 8;
	RetuData = (RetuData|Data[3]);
	RetuData = RetuData >> 4;
	ct[0] = RetuData;

    RetuData = 0;
	RetuData = (RetuData|Data[3]) << 8;
	RetuData = (RetuData|Data[4]) << 8;
	RetuData = (RetuData|Data[5]);
	RetuData = RetuData&0xfffff;
	ct[1] = RetuData;
}
uint8_t count;

static esp_err_t AHT20_init()
{
    esp_err_t err = i2c_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "i2c init error");
        return err;
    }
    vTaskDelay(40 / portTICK_PERIOD_MS);
    uint8_t tmp[10];
    tmp[0] = 0x08;
    tmp[1] = 0x00;
    AHT20_write_regs(AHT20_INIT_REG, tmp, 2);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    count = 0;

    while(ATH20_Read_Cal_Enable() == 0)//需要等待状态字status的Bit[3]=1时才去读数据。如果Bit[3]不等于1 ，发软件复位0xBA给AHT10，再重新初始化AHT10，直至Bit[3]=1
    {
        AHT20_write_regs(AHT20_SOFTRESET_REG, tmp, 0);
        vTaskDelay(200 / portTICK_PERIOD_MS);
        AHT20_write_regs(AHT20_INIT_REG, tmp, 2);
        count++;
        if(count >= 10)
            return 0;
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    return ESP_OK;
}


void app_main(void)
{
    /*
        由于AHT20的初始化条件需要重复复位AHT20，判断状态寄存器第三位的校准使能位是否校准
        所以本例程会导致'i2c.master'的日志报错
        因此需要在初始化AHT20之前关闭'i2c.master'的日志输出
        以避免出现I2C读写错误报错
    */
    esp_log_level_set("i2c.master", ESP_LOG_NONE); // 关闭 I2C 日志输出
    AHT20_init();
    int  c1,t1;
    uint32_t CT_data[2];
    while(1)
    {
         /* 读取 ATH20 传感器数据*/
        while(ATH20_Read_Cal_Enable() == 0)
        {
            AHT20_init();//如果为0再使能一次
            vTaskDelay(30 / portTICK_PERIOD_MS);
        }
        ATH20_Read_CTdata(CT_data);  //读取温度和湿度
        c1 = CT_data[0] * 1000 / 1024 / 1024;  //计算得到湿度值（放大了10倍,如果c1=523，表示现在湿度为52.3%）
        t1 = CT_data[1] * 200 *10 / 1024 / 1024 - 500;//计算得到温度值（放大了10倍，如果t1=245，表示现在温度为24.5℃）
        printf("***************************\n");
        printf("AHT20温湿度传感器测试数据:\n");
        printf("温度: %d.%d ℃\n",(t1/10),(t1%10));
        printf("湿度: %d.%d %%\n",(c1/10),(c1%10));
        printf("\n");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}
