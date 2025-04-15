/**
   ******************************************************************
   * @file    mkpb2016.c
   * @author  FAE_WanYujian
   * @version V1.0.0
   * @date    2025年4月15日
   * @brief   mkpb2016 接近传感器驱动
   ******************************************************************
   * @attention
   *	mkpb2016 接近传感器驱动
*********************************************************************
   */
// 包含 mkpb2016 设备的头文件
#include "mkpb2016.h"
// 包含 I2C 设备操作的头文件
#include "i2c_bus.h"

// 包含 FreeRTOS 操作系统的头文件
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"

static uint8_t MK2016_addr_wr = 0x72;
static uint8_t MK2016_addr_re = 0x73;

// 静态变量，用于存储 mkpb2016 设备检测到的接近值
static uint32_t mkpb2016_proximity = 0;

// 静态变量，用于表示 mkpb2016 设备的 I2C 总线句柄
static i2c_bus_handle_t mktp2016_device;


static esp_err_t mkpb2016_write_reg(uint8_t reg_addr, uint8_t data)
{
    return i2c_bus_write_bytes(mktp2016_device, MK2016_addr_wr, &reg_addr, sizeof(reg_addr), &data, sizeof(data));
}

static char mkpb2016_read_reg(uint8_t reg_addr)
{
    uint8_t data;
    i2c_bus_read_bytes(mktp2016_device, MK2016_addr_re, &reg_addr, sizeof(reg_addr), &data, sizeof(data));
    return data;
}

static int i2c_init(gpio_num_t clk, gpio_num_t sda)
{
    int res = 0;
    i2c_config_t mktp2016_i2c_cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
        .scl_io_num = clk,
        .sda_io_num = sda,
    };
    mktp2016_device = i2c_bus_create(I2C_NUM_0, &mktp2016_i2c_cfg);
    return res;
}
/**
 * @brief 初始化 mkpb2016 设备
 * 
 * @param sda I2C 数据线引脚
 * @param scl I2C 时钟线引脚
 */
void mktp2016_init(uint8_t sda, uint8_t scl) {
    // 分配 I2C 设备资源，初始化 I2C 总线
    i2c_init(scl, sda);
    // 向设备的寄存器写入配置值
    mkpb2016_write_reg(0x02, 0xFE);
    mkpb2016_write_reg(0x03, 0xF3);
    mkpb2016_write_reg(0x0C, 0x20);
    mkpb2016_write_reg(0x0D, 0x00);
    mkpb2016_write_reg(0x0E, 0x01);
    mkpb2016_write_reg(0x0F, 0x08);
    mkpb2016_write_reg(0x10, 0x07);
    mkpb2016_write_reg(0x16, 0x3F);
    mkpb2016_write_reg(0x17, 0x1c);
    mkpb2016_write_reg(0x1A, 0x03);
    mkpb2016_write_reg(0x00, 0x2D);
    // 打印初始化完成信息
    printf("mkpb2016 初始化完成\n");
}

/**
 * @brief 从 mkpb2016 设备读取接近值
 * 
 * @param proximity 用于存储接近值的指针
 */
void mktp2016_read_value(uint16_t *proximity) {
    // 定义一个数组用于存储读取的数据
    uint8_t data[6] = {0x00};
    // 定义变量用于存储低字节和高字节的数据
    uint8_t prox_low = 0; 
    uint8_t prox_high = 0;
    // 从设备的 0x18 寄存器读取低字节数据
    prox_low = mkpb2016_read_reg(0x18);
    // 从设备的 0x19 寄存器读取高字节数据
    prox_high = mkpb2016_read_reg(0x19);
    // 将高字节和低字节组合成一个 16 位的接近值
    *proximity = (prox_high << 8) | prox_low;
    // 更新全局变量 mkpb2016_proximity 的值
    mkpb2016_proximity = *proximity;
}

/**
 * @brief 清除设备的中断
 */
// 清除中断
void ClearInterrupt(void) {
    // 定义一个数组用于存储读取的数据
    // 从设备的 0x65 寄存器读取数据以清除中断
    mkpb2016_read_reg(0x65);
}

/**
 * @brief 判断是否有物体接近
 * 
 * @return true 有物体接近
 * @return false 没有物体接近
 */
bool Contact_Determination(void)
{
    // 定义变量用于存储接近值
    uint16_t proximityData = 0;
    // 循环 20 次进行检测
    for(int i=0;i<20;i++)
    {
        // 清除设备的中断
        ClearInterrupt();
        // 从设备读取接近值
        mktp2016_read_value(&proximityData);
        // 如果接近值为 0，表示没有物体接近，返回 false
        if(proximityData==0)
        {
            return false;
        }
        // 重置接近值
        proximityData=0;
        // 延时 50 毫秒
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    // 循环结束后仍未检测到接近值为 0，表示有物体接近，返回 true
    return true;
}

/**
 * @brief 接近检测任务
 * 
 * @param arg 任务参数
 * @return void* 无返回值
 */
void *proximity_detection_task(void *arg)
{
    // 无限循环进行接近检测
    while(1)
    {
        // 调用 Contact_Determination 函数判断是否有物体接近
        if(Contact_Determination())
        {
            // 如果有物体接近，打印提示信息
            printf("【检测到接近】\n");
        }
        // 延时 100 毫秒
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}