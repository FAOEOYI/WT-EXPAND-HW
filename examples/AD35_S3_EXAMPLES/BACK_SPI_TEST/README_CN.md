# AD35-S3背后拓展SPI基本通讯例程

- 例程难度：![alt text](../../../docs/_static/level_basic.png "初级")

## 例程简介

![image-20250429152034380](https://github.com/FAOEOYI/WT-EXPAND-HW/blob/main/examples/AD35_S3_EXAMPLES/BACK_SPI_TEST/img/image-20250429152034380.png)

本例程的功能是AD35-S3做SPI主机，通过背后拓展SPI接口，与另一个ESP32S3作为SPI从机进行简单的通讯演示。

### 硬件要求

本例程支持的开发板为AD35-S3,并且还需要一个EPSP32S3作为从机。


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

```bash
.
├── README_CN.md
├── SPI_RE_M
│   ├── CMakeLists.txt
│   ├── build
│   ├── main
│   ├── sdkconfig
│   └── sdkconfig.defaults
├── SPI_RE_S
│   ├── CMakeLists.txt
│   ├── build
│   ├── main
│   ├── sdkconfig
│   └── sdkconfig.defaults
└── img
    └── image-20250429152034380.png

7 directories, 8 files
```

### 功能和用法
- SPI_RE_M为主机例程，下载运行后，AD35-S3开发板作主机输出以下日志：


```c
I (23) boot: ESP-IDF v5.3.2-dirty 2nd stage bootloader
I (23) boot: compile time Apr 29 2025 14:19:18
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
I (92) esp_image: segment 0: paddr=00010020 vaddr=3c020020 size=0ce80h ( 52864) map
I (110) esp_image: segment 1: paddr=0001cea8 vaddr=3fc94f00 size=02ca4h ( 11428) load
I (112) esp_image: segment 2: paddr=0001fb54 vaddr=40374000 size=004c4h (  1220) load
I (118) esp_image: segment 3: paddr=00020020 vaddr=42000020 size=1cfb4h (118708) map
I (147) esp_image: segment 4: paddr=0003cfdc vaddr=403744c4 size=10970h ( 67952) load
I (169) boot: Loaded app from partition at offset 0x10000
I (169) boot: Disabling RNG early entropy source...
I (181) cpu_start: Multicore app
I (190) cpu_start: Pro cpu start user code
I (190) cpu_start: cpu freq: 160000000 Hz
I (190) app_init: Application information:
I (193) app_init: Project name:     SPI_RE_M
I (198) app_init: App version:      v2.7-12-g2c7f8c4d-dirty
I (204) app_init: Compile time:     Apr 29 2025 14:19:15
I (210) app_init: ELF file SHA256:  f93aa7583...
I (215) app_init: ESP-IDF:          v5.3.2-dirty
I (220) efuse_init: Min chip rev:     v0.0
I (225) efuse_init: Max chip rev:     v0.99
I (230) efuse_init: Chip rev:         v0.2
I (235) heap_init: Initializing. RAM available for dynamic allocation:
I (242) heap_init: At 3FC985D0 len 00051140 (324 KiB): RAM
I (248) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (254) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (261) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (268) spi_flash: detected chip: gd
I (271) spi_flash: flash io: dio
W (275) ADC: legacy driver is deprecated, please migrate to `esp_adc/adc_oneshot.h`
I (283) sleep: Configure to isolate all GPIO pins in sleep state
I (290) sleep: Enable automatic switching of GPIO sleep configuration
I (298) main_task: Started on CPU0
I (318) main_task: Calling app_main()
I (318) gpio: GPIO[39]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0
I (318) gpio: GPIO[40]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0
I (328) gpio: GPIO[41]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0
I (338) gpio: GPIO[42]| InputEn: 0| OutputEn: 1| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0
I (348) ESP32_S3_KORVO_2: SPI bus initialize: ESP_OK
I (358) AUDIO_BOARD: SPI bus add device: ESP_OK
I (358) SPI_RE_M: SPI transmit: ESP_OK
I (368) SPI_RE_M: Received data: ff ff ff ff
I (1368) SPI_RE_M: SPI transmit: ESP_OK
I (1368) SPI_RE_M: Received data: ff ff ff ff
I (2368) SPI_RE_M: SPI transmit: ESP_OK
I (2368) SPI_RE_M: Received data: ff ff ff ff
I (3368) SPI_RE_M: SPI transmit: ESP_OK
I (3368) SPI_RE_M: Received data: ff ff ff ff
I (4368) SPI_RE_M: SPI transmit: ESP_OK
I (4368) SPI_RE_M: Received data: ff ff ff ff
I (5368) SPI_RE_M: SPI transmit: ESP_OK
I (5368) SPI_RE_M: Received data: ff ff ff ff
I (6368) SPI_RE_M: SPI transmit: ESP_OK
I (6368) SPI_RE_M: Received data: ff ff ff ff
I (7368) SPI_RE_M: SPI transmit: ESP_OK
I (7368) SPI_RE_M: Received data: fe 00 00 00
I (8368) SPI_RE_M: SPI transmit: ESP_OK
I (8368) SPI_RE_M: Received data: ff ff ff ff
I (9368) SPI_RE_M: SPI transmit: ESP_OK
I (9368) SPI_RE_M: Received data: aa 55 00 00
I (10368) SPI_RE_M: SPI transmit: ESP_OK
I (10368) SPI_RE_M: Received data: aa 55 07 0f
I (11368) SPI_RE_M: SPI transmit: ESP_OK
I (11368) SPI_RE_M: Received data: aa 55 07 0f
I (12368) SPI_RE_M: SPI transmit: ESP_OK
I (12368) SPI_RE_M: Received data: aa 55 07 0f
I (13368) SPI_RE_M: SPI transmit: ESP_OK
I (13368) SPI_RE_M: Received data: aa 55 07 0f
I (14368) SPI_RE_M: SPI transmit: ESP_OK
I (14368) SPI_RE_M: Received data: aa 55 07 0f
I (15368) SPI_RE_M: SPI transmit: ESP_OK
I (15368) SPI_RE_M: Received data: aa 55 07 3f
I (16368) SPI_RE_M: SPI transmit: ESP_OK
I (16368) SPI_RE_M: Received data: aa 55 07 00
I (17368) SPI_RE_M: SPI transmit: ESP_OK
I (17368) SPI_RE_M: Received data: aa 55 07 0f
I (18368) SPI_RE_M: SPI transmit: ESP_OK
I (18368) SPI_RE_M: Received data: aa 55 3f ff
I (19368) SPI_RE_M: SPI transmit: ESP_OK
I (19368) SPI_RE_M: Received data: aa 55 00 00
I (20368) SPI_RE_M: SPI transmit: ESP_OK
I (20368) SPI_RE_M: Received data: aa 55 07 0f
I (21368) SPI_RE_M: SPI transmit: ESP_OK
I (21368) SPI_RE_M: Received data: aa 55 07 0f
I (22368) SPI_RE_M: SPI transmit: ESP_OK
I (22368) SPI_RE_M: Received data: aa 55 3f ff
I (23368) SPI_RE_M: SPI transmit: ESP_OK
I (23368) SPI_RE_M: Received data: aa 55 00 00
I (24368) SPI_RE_M: SPI transmit: ESP_OK
I (24368) SPI_RE_M: Received data: aa 55 07 0f
I (25368) SPI_RE_M: SPI transmit: ESP_OK
I (25368) SPI_RE_M: Received data: aa 55 07 0f
I (26368) SPI_RE_M: SPI transmit: ESP_OK
I (26368) SPI_RE_M: Received data: aa 55 07 0f
I (27368) SPI_RE_M: SPI transmit: ESP_OK
I (27368) SPI_RE_M: Received data: aa 55 07 0f
I (28368) SPI_RE_M: SPI transmit: ESP_OK
I (28368) SPI_RE_M: Received data: aa 55 07 0f
```

- SPI_RE_S为从机例程，做从机SPI设备的ESP32S3输出如下日志：

```c
I (27) boot: ESP-IDF v5.3.2-dirty 2nd stage bootloader
I (27) boot: compile time Apr 28 2025 18:03:39
I (27) boot: Multicore bootloader
I (30) boot: chip revision: v0.1
I (34) boot: efuse block revision: v1.2
I (39) boot.esp32s3: Boot SPI Speed : 80MHz
I (44) boot.esp32s3: SPI Mode       : DIO
I (48) boot.esp32s3: SPI Flash Size : 2MB
I (53) boot: Enabling RNG early entropy source...
I (58) boot: Partition Table:
I (62) boot: ## Label            Usage          Type ST Offset   Length
I (69) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (77) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (84) boot:  2 factory          factory app      00 00 00010000 00100000
I (92) boot: End of partition table
I (96) esp_image: segment 0: paddr=00010020 vaddr=3c020020 size=0b6c0h ( 46784) map
I (113) esp_image: segment 1: paddr=0001b6e8 vaddr=3fc92e00 size=02a64h ( 10852) load
I (115) esp_image: segment 2: paddr=0001e154 vaddr=40374000 size=01ec4h (  7876) load
I (123) esp_image: segment 3: paddr=00020020 vaddr=42000020 size=1ae3ch (110140) map
I (149) esp_image: segment 4: paddr=0003ae64 vaddr=40375ec4 size=0ce68h ( 52840) load
I (167) boot: Loaded app from partition at offset 0x10000
I (167) boot: Disabling RNG early entropy source...
I (179) cpu_start: Multicore app
I (188) cpu_start: Pro cpu start user code
I (188) cpu_start: cpu freq: 160000000 Hz
I (188) app_init: Application information:
I (191) app_init: Project name:     SPI_RE_S
I (196) app_init: App version:      1
I (200) app_init: Compile time:     Apr 28 2025 18:03:33
I (206) app_init: ELF file SHA256:  0cf8a64f9...
I (212) app_init: ESP-IDF:          v5.3.2-dirty
I (217) efuse_init: Min chip rev:     v0.0
I (222) efuse_init: Max chip rev:     v0.99
I (227) efuse_init: Chip rev:         v0.1
I (231) heap_init: Initializing. RAM available for dynamic allocation:
I (239) heap_init: At 3FC96130 len 000535E0 (333 KiB): RAM
I (245) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (251) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (257) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (264) spi_flash: detected chip: generic
I (268) spi_flash: flash io: dio
W (272) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (285) sleep: Configure to isolate all GPIO pins in sleep state
I (292) sleep: Enable automatic switching of GPIO sleep configuration
I (299) main_task: Started on CPU0
I (319) main_task: Calling app_main()
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x00,标志=0x00
写入寄存器0x03的值: 0x00
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x00,标志=0x00
写入寄存器0x03的值: 0x00
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x00,标志=0x00
写入寄存器0x03的值: 0x00
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x00,标志=0x00
写入寄存器0x03的值: 0x00
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
接收到SPI事务: 命令=0x01, 地址=0x03, 数据=0x07,标志=0x0f
写入寄存器0x03的值: 0x07
```

