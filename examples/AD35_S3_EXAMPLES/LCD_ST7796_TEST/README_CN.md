# I80总线、ST7796驱动循环显示三原色例程

- 例程难度：![alt text](../../../docs/_static/level_basic.png "初级")


## 例程简介

本例程主要创建一个LCD外设，并循环显示红、绿、蓝、黄四个颜色；


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

- 例程开始运行后，自动就开始循环显示红、绿、蓝、黄四个颜色；

```c
I (1338) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (2348) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (3358) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (4368) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (5378) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (6388) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (7398) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
```


### 日志输出
以下是本例程的部分日志。

```c
I (23) boot: ESP-IDF v5.3.2-dirty 2nd stage bootloader
I (23) boot: compile time Apr 18 2025 18:05:33
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
I (88) boot:  3 storage          Unknown data     01 82 00110000 00500000
I (95) boot: End of partition table
I (99) esp_image: segment 0: paddr=00010020 vaddr=3c030020 size=0f710h ( 63248) map
I (119) esp_image: segment 1: paddr=0001f738 vaddr=3fc95900 size=008e0h (  2272) load
I (120) esp_image: segment 2: paddr=00020020 vaddr=42000020 size=22980h (141696) map
I (150) esp_image: segment 3: paddr=000429a8 vaddr=3fc961e0 size=02acch ( 10956) load
I (152) esp_image: segment 4: paddr=0004547c vaddr=40374000 size=118c0h ( 71872) load
I (178) boot: Loaded app from partition at offset 0x10000
I (178) boot: Disabling RNG early entropy source...
I (190) octal_psram: vendor id    : 0x0d (AP)
I (191) octal_psram: dev id       : 0x02 (generation 3)
I (191) octal_psram: density      : 0x03 (64 Mbit)
I (196) octal_psram: good-die     : 0x01 (Pass)
I (201) octal_psram: Latency      : 0x01 (Fixed)
I (206) octal_psram: VCC          : 0x01 (3V)
I (211) octal_psram: SRF          : 0x01 (Fast Refresh)
I (217) octal_psram: BurstType    : 0x01 (Hybrid Wrap)
I (223) octal_psram: BurstLen     : 0x01 (32 Byte)
I (228) octal_psram: Readlatency  : 0x02 (10 cycles@Fixed)
I (235) octal_psram: DriveStrength: 0x00 (1/1)
I (240) MSPI Timing: PSRAM timing tuning index: 4
I (245) esp_psram: Found 8MB PSRAM device
I (250) esp_psram: Speed: 80MHz
I (254) cpu_start: Multicore app
I (703) esp_psram: SPI SRAM memory test OK
I (712) cpu_start: Pro cpu start user code
I (712) cpu_start: cpu freq: 160000000 Hz
I (712) app_init: Application information:
I (715) app_init: Project name:     LCD_ST7796_TEST
I (721) app_init: App version:      v2.7-4-g1ffe7f25-dirty
I (727) app_init: Compile time:     Apr 18 2025 18:05:27
I (733) app_init: ELF file SHA256:  14f6c0607...
I (738) app_init: ESP-IDF:          v5.3.2-dirty
I (743) efuse_init: Min chip rev:     v0.0
I (748) efuse_init: Max chip rev:     v0.99
I (753) efuse_init: Chip rev:         v0.2
I (758) heap_init: Initializing. RAM available for dynamic allocation:
I (765) heap_init: At 3FC99780 len 0004FF90 (319 KiB): RAM
I (771) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (777) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (783) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (790) esp_psram: Adding pool of 8192K of PSRAM memory to heap allocator
I (798) spi_flash: detected chip: gd
I (801) spi_flash: flash io: dio
W (806) ADC: legacy driver is deprecated, please migrate to `esp_adc/adc_oneshot.h`
I (814) sleep: Configure to isolate all GPIO pins in sleep state
I (821) sleep: Enable automatic switching of GPIO sleep configuration
I (828) main_task: Started on CPU0
I (848) esp_psram: Reserving pool of 32K of internal memory for DMA/internal allocations
I (848) main_task: Calling app_main()
I (848) 【LCD_ST7796_TEST】: PSRAM 初始化成功
W (858) i2c_bus_v2: I2C master handle is NULL, will create new one
I (858) gpio: GPIO[5]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (868) gpio: GPIO[4]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (878) AW9523B: [--w9523b init success/--]
AW9523B_ID:[0x23]
I (898) st7796: version: 1.2.1
I (898) st7796_general: LCD panel create success, version: 1.2.1
I (898) AW9523B: [--w9523b init success/--]
AW9523B_ID:[0x23]
I (1308) AUDIO_THREAD: The esp_periph task allocate stack on internal memory
RGB888: 0xff0000 -> RGB565: 0x7c0
RGB888: 0xff00 -> RGB565: 0x7f
RGB888: 0xff -> RGB565: 0xf800
RGB888: 0xffffff -> RGB565: 0xffff
RGB888: 0 -> RGB565: 0
RGB888: 0xffff00 -> RGB565: 0x7ff
I (1338) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (2348) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (3358) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (4368) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (5378) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (6388) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (7398) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (8408) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (9418) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (10428) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (11438) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (12448) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (13458) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (14468) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (15478) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (16488) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (17498) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (18508) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (19518) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (20528) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (21538) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (22548) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (23558) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (24568) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (25578) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (26588) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (27598) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (28608) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (29618) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (30628) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (31638) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (32648) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (33658) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (34668) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (35678) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (36688) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (37698) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (38708) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (39718) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (40728) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (41738) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (42748) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (43758) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (44768) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (45778) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (46788) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (47798) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (48808) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (49818) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (50828) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (51838) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (52848) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (53858) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (54868) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (55878) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (56888) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (57898) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (58908) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (59918) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (60928) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
I (61938) 【LCD_ST7796_TEST】: 蓝色矩形绘制成功
I (62948) 【LCD_ST7796_TEST】: 红色矩形绘制成功
I (63958) 【LCD_ST7796_TEST】: 绿色矩形绘制成功
I (64968) 【LCD_ST7796_TEST】: 黄色矩形绘制成功
```
