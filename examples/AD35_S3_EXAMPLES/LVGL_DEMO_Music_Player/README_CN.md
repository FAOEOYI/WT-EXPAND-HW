# LVGL GUI 显示示例
- 例程难度：![alt text](../../../docs/_static/level_basic.png "初级")

## 例程简介

本例程使用 LVGL 图形库绘制了一个音乐播放器的界面，支持触摸屏控制。本例程参考 LVGL 原始 lv_demo_music 工程。

![image-20250422173851506](.\PNG\image-20250422173851506.png)

![image-20250422173907197](.\PNG\image-20250422173907197.png)

## 环境配置

### 硬件要求

本例程支持的开发板为红色桌宠AD35-S3

## 编译和下载


### IDF 默认分支

本例程默认使用 `IDF v5.3.2`。

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

例程运行时会首先进入欢迎界面，待进入播放界面后，向上滑动 `ALL TRACKS` 会出现播放列表。点击一首模拟的音乐，再向下滑动折叠的播放界面，即可看到音乐正在播放。

### 日志输出

以下为本例程的完整日志。

```c
I (23) boot: ESP-IDF v5.3.2-dirty 2nd stage bootloader
I (23) boot: compile time Apr 22 2025 17:28:24
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
I (92) esp_image: segment 0: paddr=00010020 vaddr=3c050020 size=73cf0h (474352) map
I (184) esp_image: segment 1: paddr=00083d18 vaddr=3fc94800 size=03090h ( 12432) load
I (187) esp_image: segment 2: paddr=00086db0 vaddr=40374000 size=09268h ( 37480) load
I (198) esp_image: segment 3: paddr=00090020 vaddr=42000020 size=49f48h (302920) map
I (252) esp_image: segment 4: paddr=000d9f70 vaddr=4037d268 size=0757ch ( 30076) load
I (265) boot: Loaded app from partition at offset 0x10000
I (265) boot: Disabling RNG early entropy source...
I (278) cpu_start: Multicore app
I (287) cpu_start: Pro cpu start user code
I (287) cpu_start: cpu freq: 160000000 Hz
I (287) app_init: Application information:
I (290) app_init: Project name:     LVGL_DEMO_Music_Player
I (296) app_init: App version:      v2.7-7-g01ae6c2b-dirty
I (302) app_init: Compile time:     Apr 22 2025 17:28:18
I (308) app_init: ELF file SHA256:  c0a5da6e7...
I (313) app_init: ESP-IDF:          v5.3.2-dirty
I (319) efuse_init: Min chip rev:     v0.0
I (323) efuse_init: Max chip rev:     v0.99
I (328) efuse_init: Chip rev:         v0.2
I (333) heap_init: Initializing. RAM available for dynamic allocation:
I (340) heap_init: At 3FCAB4B8 len 0003E258 (248 KiB): RAM
I (347) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (353) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (359) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (366) spi_flash: detected chip: gd
I (369) spi_flash: flash io: dio
W (374) ADC: legacy driver is deprecated, please migrate to `esp_adc/adc_oneshot.h`
I (382) sleep: Configure to isolate all GPIO pins in sleep state
I (388) sleep: Enable automatic switching of GPIO sleep configuration
I (396) main_task: Started on CPU0
I (416) main_task: Calling app_main()
W (416) i2c_bus_v2: I2C master handle is NULL, will create new one
I (416) gpio: GPIO[5]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (426) gpio: GPIO[4]| InputEn: 1| OutputEn: 1| OpenDrain: 1| Pullup: 1| Pulldown: 0| Intr:0
I (436) AW9523B: [--w9523b init success/--]
AW9523B_ID:[0x23]
I (446) st7796: version: 1.2.1
I (446) st7796_general: LCD panel create success, version: 1.2.1
I (456) AW9523B: [--w9523b init success/--]
AW9523B_ID:[0x23]
I (856) AUDIO_THREAD: The esp_periph task allocate stack on internal memory
I (856) FT5x06: i2c init success
I (856) FT5x06: GPIO INT:6
I (856) FT5x06: FT5x06 frameware version [11]
I (856) FT5x06: Touch panel size width: 320, height: 480
I (866) FT5x06: Initial successful | GPIO INT:6 | ADDR:0x38 | dir:0
I (876) lv_port: 初始化FT5x06触摸屏驱动成功
I (876) lv_port: LVGL initialized
I (886) lv_port: LVGL display initialized
I (886) lv_port: LVGL input device initialized
I (896) lv_port: LVGL tick initialized
I (896) DEMO: LVGL init done
I (1106) DEMO: LVGL music demo done
```
