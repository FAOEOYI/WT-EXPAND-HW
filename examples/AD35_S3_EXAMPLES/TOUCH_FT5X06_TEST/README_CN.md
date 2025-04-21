# I2c总线、FT5X06触摸驱动触摸屏幕打印坐标例程

- 例程难度：![alt text](../../../docs/_static/level_basic.png "初级")


## 例程简介

本例程主要初始化LCD和基本的触摸功能，触摸屏幕打印对应的屏幕坐标值


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

- 例程开始运行后，LCD屏幕初始化为白色，FT5X06触摸驱动初始化成功，触摸屏幕将打印对应的坐标值。

```c
I (133581) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (98, 247)
【触摸事件】
【触摸事件】
I (133831) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (104, 234)
【触摸事件】
【触摸事件】
I (134081) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (92, 233)
【触摸事件】
I (134131) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (92, 233)
【触摸事件】
I (134381) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (229, 109)
【触摸事件】
I (134431) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (229, 109)
```


### 日志输出
以下是本例程的部分日志。

```c
I (23) boot: ESP-IDF v5.3.2-dirty 2nd stage bootloader
I (23) boot: compile time Apr 21 2025 17:25:16
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
I (92) esp_image: segment 0: paddr=00010020 vaddr=3c030020 size=0f8c0h ( 63680) map
I (112) esp_image: segment 1: paddr=0001f8e8 vaddr=3fc95a00 size=00730h (  1840) load
I (112) esp_image: segment 2: paddr=00020020 vaddr=42000020 size=22e74h (142964) map
I (143) esp_image: segment 3: paddr=00042e9c vaddr=3fc96130 size=02c8ch ( 11404) load
I (145) esp_image: segment 4: paddr=00045b30 vaddr=40374000 size=11954h ( 72020) load
I (171) boot: Loaded app from partition at offset 0x10000
I (171) boot: Disabling RNG early entropy source...
I (183) octal_psram: vendor id    : 0x0d (AP)
I (183) octal_psram: dev id       : 0x02 (generation 3)
I (184) octal_psram: density      : 0x03 (64 Mbit)
I (189) octal_psram: good-die     : 0x01 (Pass)
I (194) octal_psram: Latency      : 0x01 (Fixed)
I (199) octal_psram: VCC          : 0x01 (3V)
I (204) octal_psram: SRF          : 0x01 (Fast Refresh)
I (210) octal_psram: BurstType    : 0x01 (Hybrid Wrap)
I (216) octal_psram: BurstLen     : 0x01 (32 Byte)
I (221) octal_psram: Readlatency  : 0x02 (10 cycles@Fixed)
I (227) octal_psram: DriveStrength: 0x00 (1/1)
I (233) MSPI Timing: PSRAM timing tuning index: 4
I (238) esp_psram: Found 8MB PSRAM device
I (243) esp_psram: Speed: 80MHz
I (247) cpu_start: Multicore app
I (696) esp_psram: SPI SRAM memory test OK
I (705) cpu_start: Pro cpu start user code
I (705) cpu_start: cpu freq: 160000000 Hz
I (705) app_init: Application information:
I (708) app_init: Project name:     TOUCH_FT5X06_TEST
I (714) app_init: App version:      v2.7-5-gd8bafcad-dirty
I (720) app_init: Compile time:     Apr 21 2025 17:25:11
I (726) app_init: ELF file SHA256:  3cae77c54...
I (731) app_init: ESP-IDF:          v5.3.2-dirty
I (736) efuse_init: Min chip rev:     v0.0
I (741) efuse_init: Max chip rev:     v0.99
I (746) efuse_init: Chip rev:         v0.2
I (751) heap_init: Initializing. RAM available for dynamic allocation:
I (758) heap_init: At 3FC998B0 len 0004FE60 (319 KiB): RAM
I (764) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (770) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (776) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (783) esp_psram: Adding pool of 8192K of PSRAM memory to heap allocator
I (791) spi_flash: detected chip: gd
I (794) spi_flash: flash io: dio
W (799) ADC: legacy driver is deprecated, please migrate to `esp_adc/adc_oneshot.h`
I (807) sleep: Configure to isolate all GPIO pins in sleep state
I (814) sleep: Enable automatic switching of GPIO sleep configuration
I (821) main_task: Started on CPU0
I (841) esp_psram: Reserving pool of 32K of internal memory for DMA/internal allocations
I (841) main_task: Calling app_main()
W (841) i2c_bus_v2: I2C master handle is NULL, will create new one
I (851) gpio: GPIO[5]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (861) gpio: GPIO[4]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (871) AW9523B: [--w9523b init success/--]
AW9523B_ID:[0x23]
I (881) st7796: version: 1.2.1
I (881) st7796_general: LCD panel create success, version: 1.2.1
I (891) AW9523B: [--w9523b init success/--]
AW9523B_ID:[0x23]
I (1291) AUDIO_THREAD: The esp_periph task allocate stack on internal memory
I (1311) 【TOUCH_FT5X06_TEST】: 绘制成功
I (2311) FT5x06: i2c init success
I (2311) FT5x06: GPIO INT:6
I (2311) FT5x06: FT5x06 frameware version [11]
I (2311) FT5x06: Touch panel size width: 320, height: 480
I (2321) FT5x06: Initial successful | GPIO INT:6 | ADDR:0x38 | dir:0
I (2321) 【TOUCH_FT5X06_TEST】: 初始化FT5x06触摸屏驱动成功
I (2331) 【TOUCH_FT5X06_TEST】: 触摸检测任务开始
I (2331) main_task: Returned from app_main()
【触摸事件】
I (112931) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (83, 246)
【触摸事件】
I (112981) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (83, 246)
【触摸事件】
I (113831) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (114, 275)
【触摸事件】
I (113881) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (114, 275)
【触摸事件】
I (114331) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (115, 275)
【触摸事件】
I (114381) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (115, 275)
【触摸事件】
I (114781) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (111, 254)
【触摸事件】
I (115231) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (109, 255)
【触摸事件】
I (115281) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (109, 255)
【触摸事件】
【触摸事件】
I (115781) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (213, 336)
【触摸事件】
I (115831) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (213, 336)
I (116231) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (319, 432)
【触摸事件】
I (116581) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (39, 199)
【触摸事件】
I (116631) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (39, 199)
【触摸事件】
【触摸事件】
I (117681) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (3, 5)
【触摸事件】
I (117731) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (3, 5)
【触摸事件】
【触摸事件】
I (118481) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (184, 295)
【触摸事件】
I (118981) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (96, 144)
【触摸事件】
【触摸事件】
I (133281) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (96, 251)
【触摸事件】
【触摸事件】
I (133581) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (98, 247)
【触摸事件】
【触摸事件】
I (133831) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (104, 234)
【触摸事件】
【触摸事件】
I (134081) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (92, 233)
【触摸事件】
I (134131) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (92, 233)
【触摸事件】
I (134381) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (229, 109)
【触摸事件】
I (134431) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (229, 109)
【触摸事件】
I (134731) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (0, 265)
【触摸事件】
【触摸事件】
I (135031) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (48, 356)
【触摸事件】
I (135081) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (48, 356)
【触摸事件】
【触摸事件】
I (135431) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (212, 115)
【触摸事件】
【触摸事件】
I (135831) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (0, 272)
I (135881) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (0, 272)
【触摸事件】
【触摸事件】
I (137631) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (202, 392)
【触摸事件】
I (137681) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (202, 392)
【触摸事件】
I (138081) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (166, 142)
【触摸事件】
I (138131) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (166, 142)
【触摸事件】
【触摸事件】
I (138731) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (58, 289)
【触摸事件】
I (138781) 【TOUCH_FT5X06_TEST】: 触摸点坐标: (58, 289)
```
