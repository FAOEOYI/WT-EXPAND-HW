# MKPB2016接近传感器检测接近例程

- 例程难度：![alt text](../../../docs/_static/level_basic.png "初级")


## 例程简介

本例程主要创建一个接近检测任务，当遮挡物遮挡红色桌宠的正面，日志将打印【检测到接近】，将红色桌宠拿起面朝自己,也可以看到日志打印【检测到接近】。


## 环境配置

### 硬件要求

本例程支持的开发板为AD35-S3红色桌宠；


## 编译和下载

### IDF 默认分支

本例程支持 IDF release/v5.0 及以后的分支，例程默认使用 ADF 的內建分支 `esp-idf v5.3.2`。

### 配置

本例程默认选择的开发板是 `AD35-S3`，在其他开发板都不支持；

### 编译和下载

请先编译版本并烧录到开发板上，然后运行 monitor 工具来查看串口输出（替换 PORT 为端口名称）：

```
idf.py -p PORT flash monitor
```

退出调试界面使用 ``Ctrl-]``。

有关配置和使用 ESP-IDF 生成项目的完整步骤，请参阅 [《ESP-IDF 编程指南》](https://docs.espressif.com/projects/esp-idf/zh_CN/release-v5.3/esp32/index.html)。

## 如何使用例程

### 功能和用法

- 例程开始运行后，用遮挡物遮挡，或者是手持面朝自己逐步靠近，最终当传感器检测到接近，将会在日志打印【检测到接近】

```c
……
【检测到接近】
【检测到接近】
【检测到接近】
……
```


### 日志输出
以下是本例程的完整日志。

```c
I (23) boot: ESP-IDF v5.3.2-dirty 2nd stage bootloader
I (23) boot: compile time Apr 15 2025 14:45:24
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
I (92) esp_image: segment 0: paddr=00010020 vaddr=3c020020 size=0c1d0h ( 49616) map
I (109) esp_image: segment 1: paddr=0001c1f8 vaddr=3fc94400 size=03008h ( 12296) load
I (112) esp_image: segment 2: paddr=0001f208 vaddr=40374000 size=00e10h (  3600) load
I (118) esp_image: segment 3: paddr=00020020 vaddr=42000020 size=1c820h (116768) map
I (146) esp_image: segment 4: paddr=0003c848 vaddr=40374e10 size=0f5a8h ( 62888) load
I (167) boot: Loaded app from partition at offset 0x10000
I (167) boot: Disabling RNG early entropy source...
I (179) octal_psram: vendor id    : 0x0d (AP)
I (179) octal_psram: dev id       : 0x02 (generation 3)
I (179) octal_psram: density      : 0x03 (64 Mbit)
I (184) octal_psram: good-die     : 0x01 (Pass)
I (189) octal_psram: Latency      : 0x01 (Fixed)
I (195) octal_psram: VCC          : 0x01 (3V)
I (200) octal_psram: SRF          : 0x01 (Fast Refresh)
I (206) octal_psram: BurstType    : 0x01 (Hybrid Wrap)
I (212) octal_psram: BurstLen     : 0x01 (32 Byte)
I (217) octal_psram: Readlatency  : 0x02 (10 cycles@Fixed)
I (223) octal_psram: DriveStrength: 0x00 (1/1)
I (229) MSPI Timing: PSRAM timing tuning index: 10
I (234) esp_psram: Found 8MB PSRAM device
I (238) esp_psram: Speed: 80MHz
I (242) cpu_start: Multicore app
I (692) esp_psram: SPI SRAM memory test OK
I (701) cpu_start: Pro cpu start user code
I (701) cpu_start: cpu freq: 160000000 Hz
I (701) app_init: Application information:
I (704) app_init: Project name:     MK2016_Proximity_Sensor_TEST
I (710) app_init: App version:      v2.7-3-g483af51f-dirty
I (717) app_init: Compile time:     Apr 15 2025 14:45:19
I (723) app_init: ELF file SHA256:  9f116dbf2...
I (728) app_init: ESP-IDF:          v5.3.2-dirty
I (733) efuse_init: Min chip rev:     v0.0
I (738) efuse_init: Max chip rev:     v0.99
I (743) efuse_init: Chip rev:         v0.2
I (748) heap_init: Initializing. RAM available for dynamic allocation:
I (755) heap_init: At 3FC97D88 len 00051988 (326 KiB): RAM
I (761) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (767) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (773) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (780) esp_psram: Adding pool of 8192K of PSRAM memory to heap allocator
I (788) spi_flash: detected chip: gd
I (791) spi_flash: flash io: dio
I (795) sleep: Configure to isolate all GPIO pins in sleep state
I (802) sleep: Enable automatic switching of GPIO sleep configuration
I (809) main_task: Started on CPU0
I (839) esp_psram: Reserving pool of 32K of internal memory for DMA/internal allocations
I (839) main_task: Calling app_main()
i2c_config.sda_io_num = 5
i2c_config.scl_io_num = 4
W (849) i2c_bus_v2: I2C master handle is NULL, will create new one
I (849) gpio: GPIO[5]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (859) gpio: GPIO[4]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
mkpb2016 初始化完成
I (879) main_task: Returned from app_main()
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
【检测到接近】
```
