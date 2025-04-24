# 后背IIC拓展接口获取AHT20温湿度例程

- 例程难度：![alt text](../../../docs/_static/level_basic.png "初级")

## 例程简介

本例程演示的主要功能是AD35-S3 后背IIC拓展接口与AHT20温湿度传感器通讯并获取温湿度，每5秒读取一次AHT20的温度和湿度数据，并打印到串口日志。

### 硬件要求

本例程支持的开发板为AD35-S3,如下面的 [配置](#配置) 一节所述，可以在 `menuconfig` 中选择开发板。


## 编译和下载

### IDF 默认分支

本例程支持 IDF release/v5.0 及以后的分支，例程默认使用 esp-idf v5.3.2。

### 配置

准备好红色桌宠开发板AD35-S3,在`menuconfig`中选择`audio_board_ad35_s3`开发板。

### 编译和下载

请先编译版本并烧录到开发板上，然后运行 monitor 工具来查看串口输出 (替换 PORT 为端口名称)：

```
idf.py -p PORT flash monitor
```

退出调试界面使用 ``Ctrl-]``。

有关配置和使用 ESP-IDF 生成项目的完整步骤，请参阅 [《ESP-IDF 编程指南》](https://docs.espressif.com/projects/esp-idf/zh_CN/release-v5.3/esp32/index.html)。

## 如何使用例程

### 功能和用法
下载运行后，开发板应该输出以下日志：

```c
I (23) boot: ESP-IDF v5.3.2-dirty 2nd stage bootloader
I (23) boot: compile time Apr 24 2025 15:49:05
I (23) boot: Multicore bootloader
I (26) boot: chip revision: v0.2
I (30) boot: efuse block revision: v1.3
I (35) boot.esp32s3: Boot SPI Speed : 80MHz
I (40) boot.esp32s3: SPI Mode       : DIO
I (44) boot.esp32s3: SPI Flash Size : 16MB
I (49) boot: Enabling RNG early entropy source...
I (55) boot: Partition Table:
I (58) boot: ## Label            Usage          Type ST Offset   Length
I (65) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (73) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (80) boot:  2 factory          factory app      00 00 00010000 00100000
I (88) boot: End of partition table
I (92) esp_image: segment 0: paddr=00010020 vaddr=3c020020 size=0bdd0h ( 48592) map
I (109) esp_image: segment 1: paddr=0001bdf8 vaddr=3fc93000 size=02b3ch ( 11068) load
I (112) esp_image: segment 2: paddr=0001e93c vaddr=40374000 size=016dch (  5852) load
I (119) esp_image: segment 3: paddr=00020020 vaddr=42000020 size=1c320h (115488) map
I (146) esp_image: segment 4: paddr=0003c348 vaddr=403756dc size=0d8e0h ( 55520) load
I (165) boot: Loaded app from partition at offset 0x10000
I (165) boot: Disabling RNG early entropy source...
I (177) cpu_start: Multicore app
I (186) cpu_start: Pro cpu start user code
I (186) cpu_start: cpu freq: 160000000 Hz
I (186) app_init: Application information:
I (189) app_init: Project name:     BACK_IIC_AHT20_TEST
I (195) app_init: App version:      v2.7-11-g4be6fb76
I (200) app_init: Compile time:     Apr 24 2025 15:48:59
I (206) app_init: ELF file SHA256:  daa39f50e...
I (212) app_init: ESP-IDF:          v5.3.2-dirty
I (217) efuse_init: Min chip rev:     v0.0
I (222) efuse_init: Max chip rev:     v0.99
I (227) efuse_init: Chip rev:         v0.2
I (232) heap_init: Initializing. RAM available for dynamic allocation:
I (239) heap_init: At 3FC96440 len 000532D0 (332 KiB): RAM
I (245) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (251) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (257) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (264) spi_flash: detected chip: gd
I (268) spi_flash: flash io: dio
I (272) sleep: Configure to isolate all GPIO pins in sleep state
I (278) sleep: Enable automatic switching of GPIO sleep configuration
I (286) main_task: Started on CPU0
I (316) main_task: Calling app_main()
W (316) i2c_bus_v2: I2C master handle is NULL, will create new one
I (316) gpio: GPIO[48]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (326) gpio: GPIO[38]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
***************************
AHT20温湿度传感器测试数据:
温度: 27.7 ℃
湿度: 78.2 %

***************************
AHT20温湿度传感器测试数据:
温度: 27.6 ℃
湿度: 78.2 %

***************************
AHT20温湿度传感器测试数据:
温度: 27.5 ℃
湿度: 78.2 %

***************************
AHT20温湿度传感器测试数据:
温度: 27.4 ℃
湿度: 78.2 %

***************************
AHT20温湿度传感器测试数据:
温度: 27.3 ℃
湿度: 78.2 %
```

