# AW9523B IO拓展器获取拓展器设备ID代码例程

- [English Version](./README.md)
- 例程难度：![alt text](../../../docs/_static/level_basic.png "初级")

## 例程简介

本例程的功能是简单初始化AW9523B IO拓展器，并获取打印拓展器设备ID代码例程

### 硬件要求

本例程支持的开发板在 `$ADF_PATH/examples/README_CN.md` 文档中[例程与乐鑫音频开发板的兼容性表格](../../README_CN.md#例程与乐鑫音频开发板的兼容性)中有标注，表格中标有绿色复选框的开发板均可运行本例程。请记住，如下面的 [配置](#配置) 一节所述，可以在 `menuconfig` 中选择开发板。


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
I (23) boot: compile time Apr 14 2025 16:11:48
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
I (92) esp_image: segment 0: paddr=00010020 vaddr=3c020020 size=0bda8h ( 48552) map
I (109) esp_image: segment 1: paddr=0001bdd0 vaddr=3fc93000 size=02b3ch ( 11068) load
I (112) esp_image: segment 2: paddr=0001e914 vaddr=40374000 size=01704h (  5892) load
I (119) esp_image: segment 3: paddr=00020020 vaddr=42000020 size=1bbfch (113660) map
I (146) esp_image: segment 4: paddr=0003bc24 vaddr=40375704 size=0d890h ( 55440) load
I (164) boot: Loaded app from partition at offset 0x10000
I (164) boot: Disabling RNG early entropy source...
I (176) cpu_start: Multicore app
I (185) cpu_start: Pro cpu start user code
I (185) cpu_start: cpu freq: 160000000 Hz
I (186) app_init: Application information:
I (188) app_init: Project name:     AW9523B_GET_ID
I (194) app_init: App version:      v2.7-1-g1f836502-dirty
I (200) app_init: Compile time:     Apr 14 2025 16:11:42
I (206) app_init: ELF file SHA256:  9792b801b...
I (211) app_init: ESP-IDF:          v5.3.2-dirty
I (217) efuse_init: Min chip rev:     v0.0
I (221) efuse_init: Max chip rev:     v0.99
I (226) efuse_init: Chip rev:         v0.2
I (231) heap_init: Initializing. RAM available for dynamic allocation:
I (238) heap_init: At 3FC96438 len 000532D8 (332 KiB): RAM
I (244) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (251) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (257) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (264) spi_flash: detected chip: gd
I (267) spi_flash: flash io: dio
I (271) sleep: Configure to isolate all GPIO pins in sleep state
I (278) sleep: Enable automatic switching of GPIO sleep configuration
I (285) main_task: Started on CPU0
I (315) main_task: Calling app_main()
W (315) i2c_bus_v2: I2C master handle is NULL, will create new one
I (315) gpio: GPIO[5]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (325) gpio: GPIO[4]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (335) AW9523B: [--w9523b init success/--]
AW9523B_ID:[0x23]
I (345) main_task: Returned from app_main()
```

