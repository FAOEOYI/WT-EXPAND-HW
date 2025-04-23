# AD35-S3 背后串口调试例程
- 例程难度：![alt text](../../../docs/_static/level_basic.png "初级")

## 例程简介

本例程使用 ESP32S3 芯片，使用AD35-S3 红色桌宠，通过背后预留的串口，和串口工具，演示如何使用串口。

![image-20250423175219069](https://github.com/FAOEOYI/WT-EXPAND-HW/blob/main/examples/AD35_S3_EXAMPLES/BACK_UART_TEST/doc/image-20250423175219069.png)

## 环境配置

### 硬件要求

本例程支持的开发板在 `$ADF_PATH/examples/README_CN.md` 文档中 [例程与乐鑫音频开发板的兼容性表格](../../README_CN.md#例程与乐鑫音频开发板的兼容性) 中有标注，表格中标有绿色复选框的开发板均可运行本例程。请记住，如下面的 [配置](#配置) 一节所述，可以在 `menuconfig` 中选择开发板。

## 编译和下载


### IDF 默认分支

本例程支持默认使用 IDF v5.3.2 分支。

### 配置

本例程选择的开发板是 `AD35-S3`。

### 编译和下载

请先编译版本并烧录到开发板上，然后运行 monitor 工具来查看串口输出（替换 PORT 为端口名称）：

```
idf.py -p PORT flash monitor
```

退出调试界面使用 ``Ctrl-]``。

有关配置和使用 ESP-IDF 生成项目的完整步骤，请前往 [《ESP-IDF 编程指南》](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/get-started/index.html)，并在页面左上角选择芯片和版本，查看对应的文档。

## 如何使用例程


### 功能和用法

例程运行后，就可以同串口助手进行通信了。

### 日志输出

以下为本例程的完整日志。

```c
I (23) boot: ESP-IDF v5.3.2-dirty 2nd stage bootloader
I (23) boot: compile time Apr 23 2025 17:43:50
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
I (92) esp_image: segment 0: paddr=00010020 vaddr=3c020020 size=0b590h ( 46480) map
I (109) esp_image: segment 1: paddr=0001b5b8 vaddr=3fc93b00 size=02b2ch ( 11052) load
I (111) esp_image: segment 2: paddr=0001e0ec vaddr=40374000 size=01f2ch (  7980) load
I (119) esp_image: segment 3: paddr=00020020 vaddr=42000020 size=1ab18h (109336) map
I (145) esp_image: segment 4: paddr=0003ab40 vaddr=40375f2c size=0dae0h ( 56032) load
I (164) boot: Loaded app from partition at offset 0x10000
I (164) boot: Disabling RNG early entropy source...
I (176) cpu_start: Multicore app
I (185) cpu_start: Pro cpu start user code
I (185) cpu_start: cpu freq: 160000000 Hz
I (185) app_init: Application information:
I (188) app_init: Project name:     BACK_UART_TEST
I (193) app_init: App version:      1
I (198) app_init: Compile time:     Apr 23 2025 17:43:43
I (204) app_init: ELF file SHA256:  c70d23442...
I (209) app_init: ESP-IDF:          v5.3.2-dirty
I (214) efuse_init: Min chip rev:     v0.0
I (219) efuse_init: Max chip rev:     v0.99
I (224) efuse_init: Chip rev:         v0.2
I (229) heap_init: Initializing. RAM available for dynamic allocation:
I (236) heap_init: At 3FC96EF0 len 00052820 (330 KiB): RAM
I (242) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (248) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (254) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (262) spi_flash: detected chip: gd
I (265) spi_flash: flash io: dio
I (269) sleep: Configure to isolate all GPIO pins in sleep state
I (276) sleep: Enable automatic switching of GPIO sleep configuration
I (283) main_task: Started on CPU0
I (313) main_task: Calling app_main()
I (313) main_task: Returned from app_main()
I (6593) TX_RX_TEST: Read 3 bytes: 'rrr'
I (8303) TX_RX_TEST: Read 3 bytes: 'rrr'
I (9233) TX_RX_TEST: Read 3 bytes: 'rrr'
I (9923) TX_RX_TEST: Read 3 bytes: 'rrr'
I (27343) TX_RX_TEST: Read 11 bytes: 'HELLO WORLD'
I (32943) TX_RX_TEST: Read 11 bytes: 'HELLO WORLD'
I (38943) TX_RX_TEST: Read 11 bytes: 'HELLO WORLD'
I (59293) TX_RX_TEST: Read 11 bytes: 'HELLO WORLD'
```
