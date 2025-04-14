/**
   ******************************************************************
   * @file    aw9523b.h
   * @author  FAE_WanYujian
   * @version V1.0.0
   * @date    2025年4月14日
   * @brief   aw9523b IO拓展器驱动
   ******************************************************************
   * @attention
   *	模仿乐鑫的tca9554驱动，修改为aw9523b驱动
*********************************************************************
   */
/* TAG ---------------------------------------------------------*/

#ifndef _AW9523B_H
#define _AW9523B_H

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/gpio.h"

typedef enum {
    AW9523B_GPIO_PORT_0 = 0,
    AW9523B_GPIO_PORT_1 = 1
} esp_aw9523b_gpio_port_t;

typedef enum {
    AW9523B_GPIO_NUM_0 = BIT(0),
    AW9523B_GPIO_NUM_1 = BIT(1),
    AW9523B_GPIO_NUM_2 = BIT(2),
    AW9523B_GPIO_NUM_3 = BIT(3),
    AW9523B_GPIO_NUM_4 = BIT(4),
    AW9523B_GPIO_NUM_5 = BIT(5),
    AW9523B_GPIO_NUM_6 = BIT(6),
    AW9523B_GPIO_NUM_7 = BIT(7),
    AW9523B_GPIO_NUM_MAX
} esp_aw9523b_gpio_num_t;

typedef enum {
    AW9523B_IO_LOW,
    AW9523B_IO_HIGH,
    AW9523B_LEVEL_ERROR
} esp_aw9523b_io_level_t;

typedef enum {
    AW9523B_IO_OUTPUT,
    AW9523B_IO_INPUT
} esp_aw9523b_io_config_t;

typedef struct {
    gpio_num_t i2c_sda;
    gpio_num_t i2c_scl;
    gpio_num_t interrupt_output;
} esp_aw9523b_config_t;

typedef enum {
    AW9523B_PIN_LED_MODE,
    AW9523B_PIN_GPIO_MODE
} aw9523_mode_t;

typedef enum {
    AW9523B_0_0,
    AW9523B_0_1,
    AW9523B_0_2,
    AW9523B_0_3,
    AW9523B_0_4,
    AW9523B_0_5,
    AW9523B_0_6,
    AW9523B_0_7,
    AW9523B_1_0,
    AW9523B_1_1,
    AW9523B_1_2,
    AW9523B_1_3,
    AW9523B_1_4,
    AW9523B_1_5,
    AW9523B_1_6,
    AW9523B_1_7,
    AW9523B_MAX
} aw9523b_gpio_num_t;//0-7,1-7保留，暂不使用



/**
 * @brief 初始化 AW9523B IO 拓展器
 * 
 * 该函数根据传入的配置结构体对 AW9523B 进行初始化操作。
 * 
 * @param cfg 指向 esp_aw9523b_config_t 结构体的指针，包含 I2C 的 SDA 引脚、SCL 引脚和中断输出引脚等配置信息。
 * @return esp_err_t 初始化结果，ESP_OK 表示成功，其他值表示失败。
 */
esp_err_t aw9523b_init(esp_aw9523b_config_t *cfg);

/**
 * @brief 获取指定 GPIO 端口和引脚的输入状态
 * 
 * 该函数从指定的 GPIO 端口和引脚读取输入电平状态。
 * 
 * @param gpio_port AW9523B 的 GPIO 端口号，取值范围由 esp_aw9523b_gpio_port_t 枚举定义。
 * @param gpio_num AW9523B 的 GPIO 引脚号，取值范围由 esp_aw9523b_gpio_num_t 枚举定义。
 * @return esp_aw9523b_io_level_t 读取到的电平状态，可能为低电平、高电平或错误状态。
 */
esp_aw9523b_io_level_t aw9523b_get_input_state(esp_aw9523b_gpio_port_t gpio_port,esp_aw9523b_gpio_num_t gpio_num);


/**
 * @brief 获取指定 GPIO 端口和引脚的输出状态
 * 
 * 该函数从指定的 GPIO 端口和引脚读取输出电平状态。
 * 
 * @param gpio_port AW9523B 的 GPIO 端口号，取值范围由 esp_aw9523b_gpio_port_t 枚举定义。
 * @param gpio_num AW9523B 的 GPIO 引脚号，取值范围由 esp_aw9523b_gpio_num_t 枚举定义。
 * @return esp_aw9523b_io_level_t 读取到的电平状态，可能为低电平、高电平或错误状态。
 */
esp_aw9523b_io_level_t aw9523b_get_output_state(esp_aw9523b_gpio_port_t gpio_port,esp_aw9523b_gpio_num_t gpio_num);

/**
 * @brief 设置指定 GPIO 端口和引脚的输出状态
 * 
 * 该函数将指定的 GPIO 端口和引脚设置为指定的输出电平状态。
 * 
 * @param gpio_port AW9523B 的 GPIO 端口号，取值范围由 esp_aw9523b_gpio_port_t 枚举定义。
 * @param gpio_num AW9523B 的 GPIO 引脚号，取值范围由 esp_aw9523b_gpio_num_t 枚举定义。
 * @param level 要设置的输出电平状态，取值范围由 esp_aw9523b_io_level_t 枚举定义。
 * @return esp_err_t 设置结果，ESP_OK 表示成功，其他值表示失败。
 */
esp_err_t aw9523b_set_output_state(esp_aw9523b_gpio_port_t gpio_port,esp_aw9523b_gpio_num_t gpio_num,esp_aw9523b_io_level_t level);

/**
 * @brief 配置指定 GPIO 端口和引脚的输入输出模式
 * 
 * 该函数将指定的 GPIO 端口和引脚配置为输入或输出模式。
 * 
 * @param gpio_port AW9523B 的 GPIO 端口号，取值范围由 esp_aw9523b_gpio_port_t 枚举定义。
 * @param gpio_num AW9523B 的 GPIO 引脚号，取值范围由 esp_aw9523b_gpio_num_t 枚举定义。
 * @param io_config 要配置的输入输出模式，取值范围由 esp_aw9523b_io_config_t 枚举定义。
 * @return esp_err_t 配置结果，ESP_OK 表示成功，其他值表示失败。
 */
esp_err_t aw9523b_set_io_config(esp_aw9523b_gpio_port_t gpio_port,esp_aw9523b_gpio_num_t gpio_num,esp_aw9523b_io_config_t io_config);

/**
 * @brief 设置指定 GPIO 端口和引脚的工作模式
 * 
 * 该函数将指定的 GPIO 端口和引脚设置为 LED 模式或 GPIO 模式。
 * 
 * @param gpio_port AW9523B 的 GPIO 端口号，取值范围由 esp_aw9523b_gpio_port_t 枚举定义。
 * @param gpio_num AW9523B 的 GPIO 引脚号，取值范围由 esp_aw9523b_gpio_num_t 枚举定义。
 * @param mode 要设置的工作模式，取值范围由 aw9523_mode_t 枚举定义。
 * @return esp_err_t 设置结果，ESP_OK 表示成功，其他值表示失败。
 */
esp_err_t aw9523b_set_pin_mode(esp_aw9523b_gpio_port_t gpio_port,esp_aw9523b_gpio_num_t gpio_num,aw9523_mode_t mode);

/**
 * @brief 对 AW9523B IO 拓展器进行复位操作
 * 
 * 该函数将 AW9523B 恢复到初始状态。
 * 
 * @return esp_err_t 复位结果，ESP_OK 表示成功，其他值表示失败。
 */
esp_err_t aw9523b_reset(void);

/**
 * @brief 反初始化 AW9523B IO 拓展器，释放相关资源
 * 
 * 该函数停止 AW9523B 的工作并释放占用的资源。
 * 
 * @return esp_err_t 反初始化结果，ESP_OK 表示成功，其他值表示失败。
 */
esp_err_t aw9523b_deinit(void);

/**
 * @brief 对 AW9523B IO 拓展器进行测试
 * 
 * 该函数执行一系列测试操作，验证 AW9523B 的功能是否正常。
 */
void aw9523b_test(void);

/**
 * @brief 设置功率放大器（PA）的电平状态(测试用，暂不使用)
 * 
 * 该函数将功率放大器的输出设置为指定的电平状态。
 * 
 * @param level 要设置的电平状态，取值范围由 esp_aw9523b_io_level_t 枚举定义。
 */
void aw9523b_set_pa(esp_aw9523b_io_level_t level);

#ifdef __cplusplus
}
#endif

#endif
