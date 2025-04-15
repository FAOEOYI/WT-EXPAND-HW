# 使用 SPIFFS 保存的事件监控循环录音和回放例程

- 例程难度：![alt text](../../../docs/_static/level_basic.png "初级")


## 例程简介

本例程演示了使用 ADF 的 SPIFFS 流循环保存录音文件并读取、播放音频文件的方法，录音时，I2S 参数为 48000 Hz、16 位、双通道，重采样为 8000 Hz、16 位、单通道的数据，然后将数据编码为 AMR-NB 格式并写入 SPIFFS 中。回放时，读取 SPIFFS 中的 8000 Hz、16 位、单通道的 AMR-NB 文件，数据解码重采样为 48000 Hz、16 位、双通道，然后通过 I2S 外设把录音数据播放出来。

1. 对于录音过程：

  - 设置 I2S 并以 48000 Hz、16 位、立体声的采样率获取音频。
  - 使用重采样过滤器转换数据为 8000 Hz、16 位、单通道。
  - 使用 AMR-NB 编码器进行数据编码。
  - 写入 SPIFFS。

  录音的重采样并保存到 SPIFFS 的管道如下：

```
    [mic] ---> codec_chip ---> i2s_stream ---> resample_filter ---> amrnb_encoder ---> spiffs_stream ---> [flash]
```

2. 对于录音回放过程：

  - 读取 SPIFFS 录制的文件，采样率为 8000 Hz、16 位、单通道。
  - 用 AMR-NB 解码器解码文件数据。
  - 使用重采样过滤器转换数据为 48000 Hz、16 位、双通道。
  - 将音频写入 I2S 外设播放。

  读取 SPIFFS 的文件重采样后通过 I2S 播放管道如下：

```
    [flash] ---> spiffs_stream ---> amrnb_decoder ---> resample_filter ---> i2s_stream ---> codec_chip ---> [speaker]
```

可以在宏定义设置录制秒数时间；

可以在宏定义选择是时间监控实现循环，还是顺序逻辑控制循环；

## 环境配置

### 硬件要求

本例程支持的开发板在 `$ADF_PATH/examples/README_CN.md` 文档中[例程与乐鑫音频开发板的兼容性表格](../../README_CN.md#例程与乐鑫音频开发板的兼容性)中有标注，表格中标有绿色复选框的开发板均可运行本例程。请记住，如下面的 [配置](#配置) 一节所述，可以在 `menuconfig` 中选择开发板。


## 编译和下载

### IDF 默认分支

本例程支持 IDF release/v5.0 及以后的分支，例程默认使用`esp-idf v5.3.2`。

### 配置

本例程默认选择的开发板是 `AD35-S3`，如果需要在其他的开发板上运行此例程，则需要在 menuconfig 中选择开发板的配置，例如选择 `ESP32-Lyrat-Mini V1.1`。

```
menuconfig > Audio HAL > ESP32-Lyrat-Mini V1.1
```

### 编译和下载
请先编译版本并烧录到开发板上，然后运行 monitor 工具来查看串口输出 (替换 PORT 为端口名称)：

```
idf.py -p PORT flash monitor
```

退出调试界面使用 ``Ctrl-]``。

有关配置和使用 ESP-IDF 生成项目的完整步骤，请参阅 [《ESP-IDF 编程指南》](https://docs.espressif.com/projects/esp-idf/zh_CN/release-v5.3/esp32/index.html)。

## 如何使用例程

### 功能和用法

- 例程开始运行后，如果 `SPIFFS` 是第一次挂载，将会进行格式化操作，打印如下：

```c
rst:0x1 (POWERON_RESET),boot:0x1f (SPI_FAST_FLASH_BOOT)
configsip: 0, SPIWP:0xee
clk_drv:0x00,q_drv:0x00,d_drv:0x00,cs0_drv:0x00,hd_drv:0x00,wp_drv:0x00
mode:DIO, clock div:2
load:0x3fff0030,len:7204
load:0x40078000,len:13212
load:0x40080400,len:4568
0x40080400: _init at ??:?

entry 0x400806f4
I (27) boot: ESP-IDF v4.2.2-1-g379ca2123 2nd stage bootloader
I (27) boot: compile time 16:55:50
I (27) boot: chip revision: 3
I (31) boot_comm: chip revision: 3, min. bootloader chip revision: 0
I (38) boot.esp32: SPI Speed      : 40MHz
I (43) boot.esp32: SPI Mode       : DIO
I (47) boot.esp32: SPI Flash Size : 2MB
I (52) boot: Enabling RNG early entropy source...
I (57) boot: Partition Table:
I (61) boot: ## Label            Usage          Type ST Offset   Length
I (68) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (76) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (83) boot:  2 factory          factory app      00 00 00010000 00100000
I (91) boot:  3 storage          Unknown data     01 82 00110000 000f0000
I (98) boot: End of partition table
I (102) boot_comm: chip revision: 3, min. application chip revision: 0
I (110) esp_image: segment 0: paddr=0x00010020 vaddr=0x3f400020 size=0x1b0ec (110828) map
I (161) esp_image: segment 1: paddr=0x0002b114 vaddr=0x3ffb0000 size=0x0240c (  9228) load
I (165) esp_image: segment 2: paddr=0x0002d528 vaddr=0x40080000 size=0x02af0 ( 10992) load
0x40080000: _WindowOverflow4 at /hengyongchao/esp-idfs/esp-idf-v4.2.2-psram/components/freertos/xtensa/xtensa_vectors.S:1730

I (172) esp_image: segment 3: paddr=0x00030020 vaddr=0x400d0020 size=0x45584 (284036) map
0x400d0020: _stext at ??:?

I (285) esp_image: segment 4: paddr=0x000755ac vaddr=0x40082af0 size=0x0a2e8 ( 41704) load
0x40082af0: _lock_try_acquire at /hengyongchao/esp-idfs/esp-idf-v4.2.2-psram/components/newlib/locks.c:175

I (310) boot: Loaded app from partition at offset 0x10000
I (310) boot: Disabling RNG early entropy source...
I (311) cpu_start: Pro cpu up.
I (314) cpu_start: Application information:
I (319) cpu_start: Project name:     spiffs_amr_resample_app
I (326) cpu_start: App version:      v2.2-216-g29c606a4
I (331) cpu_start: Compile time:     Nov  8 2021 16:55:43
I (338) cpu_start: ELF file SHA256:  6a240ffe175924a2...
I (343) cpu_start: ESP-IDF:          v4.2.2-1-g379ca2123
I (349) cpu_start: Starting app cpu, entry point is 0x40081900
0x40081900: call_start_cpu1 at /hengyongchao/esp-idfs/esp-idf-v4.2.2-psram/components/esp32/cpu_start.c:287

I (0) cpu_start: App cpu up.
I (360) heap_init: Initializing. RAM available for dynamic allocation:
I (367) heap_init: At 3FFAE6E0 len 00001920 (6 KiB): DRAM
I (373) heap_init: At 3FFB2CD0 len 0002D330 (180 KiB): DRAM
I (379) heap_init: At 3FFE0440 len 00003AE0 (14 KiB): D/IRAM
I (385) heap_init: At 3FFE4350 len 0001BCB0 (111 KiB): D/IRAM
I (392) heap_init: At 4008CDD8 len 00013228 (76 KiB): IRAM
I (398) cpu_start: Pro cpu start user code
I (416) spi_flash: detected chip: gd
I (417) spi_flash: flash io: dio
W (417) spi_flash: Detected size(8192k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (427) cpu_start: Starting scheduler on PRO CPU.
I (0) cpu_start: Starting scheduler on APP CPU.
W (440) SPIFFS: mount failed, -10025. formatting...
I (11690) PERIPH_SPIFFS: Partition size: total: 896321, used: 0
E (11690) gpio: gpio_install_isr_service(438): GPIO isr service already installed
W (11710) PERIPH_TOUCH: _touch_init
I (11730) SPIFFS_AMR_RESAMPLE_EXAMPLE: [1.1] Initialize recorder pipeline
I (11730) SPIFFS_AMR_RESAMPLE_EXAMPLE: [1.2] Create audio elements for recorder pipeline
I (11770) SPIFFS_AMR_RESAMPLE_EXAMPLE: [1.3] Register audio elements to recorder pipeline
I (11770) SPIFFS_AMR_RESAMPLE_EXAMPLE: [2.2] Create audio elements for playback pipeline
W (11780) I2S: I2S driver already installed
I (11800) SPIFFS_AMR_RESAMPLE_EXAMPLE: [2.3] Register audio elements to playback pipeline
I (11800) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ 3 ] Set up  event listener

```

- 观察日志打印，日志输出【录音开始】时，本例将进行录音，重采样数据后编码为 AMR-NB 文件，并写入到 SPIFFS 中保存，打印如下：

```c
[ * ]录音准备中…………1
[ * ]录音准备中…………2
[ * ]录音准备中…………3
[ * ]录音开始……
[ * ]录音…………1
[ * ]录音…………2
[ * ]录音…………3
[ * ]录音…………4
[ * ]录音…………5
[ * ]录音…………6
[ * ]录音…………7
[ * ]录音…………8
[ * ]录音…………9
[ * ]录音…………10
[ * ]录音…………11
[ * ]录音…………12
[ * ]录音…………13
[ * ]录音…………14
[ * ]录音…………15
[ * ]录音保存中…………1
[ * ]录音保存中…………2
[ * ]录音保存中…………3
[ * ]录音保存完成
```

- 观察日志打印，日志输出【回放中……】时，例程将读取 SPIFFS 中的 AMR-NB 文件，重采样后发送到 I2S 接口播放该录音文件，打印如下：

```c
E (22131) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ ** ]停止录音并开始播放
W (22131) AUDIO_ELEMENT: IN-[filter_downsample] AEL_IO_ABORT
W (22141) AUDIO_ELEMENT: [filter_downsample] Element already stopped
W (22141) AUDIO_ELEMENT: IN-[file_writer] AEL_IO_ABORT
W (22181) AUDIO_ELEMENT: IN-[amrnb_encoder] AEL_IO_ABORT
[ * ]回放准备中…………1
[ * ]回放准备中…………2
[ * ]回放准备中…………3
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
W (40431) SPIFFS_STREAM: No more data, ret:0
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]播放完成
E (45721) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ ** ]停止播放并开始录音
W (45721) AUDIO_ELEMENT: [file_reader] Element already stopped
W (45731) AUDIO_ELEMENT: [amr_decoder] Element already stopped
W (45731) AUDIO_ELEMENT: [filter_upsample] Element already stopped
W (45741) AUDIO_ELEMENT: [i2s_writer] Element already stopped
```


### 日志输出

以下是本例程的完整日志。

```c
I (23) boot: ESP-IDF v5.3.2-dirty 2nd stage bootloader
I (23) boot: compile time Apr 15 2025 10:09:22
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
I (88) boot:  3 storage          Unknown data     01 82 00110000 00100000
I (95) boot: End of partition table
I (99) esp_image: segment 0: paddr=00010020 vaddr=3c060020 size=22770h (141168) map
I (133) esp_image: segment 1: paddr=00032798 vaddr=3fc95c00 size=03118h ( 12568) load
I (136) esp_image: segment 2: paddr=000358b8 vaddr=40374000 size=0a760h ( 42848) load
I (148) esp_image: segment 3: paddr=00040020 vaddr=42000020 size=50578h (329080) map
I (206) esp_image: segment 4: paddr=000905a0 vaddr=4037e760 size=073f0h ( 29680) load
I (220) boot: Loaded app from partition at offset 0x10000
I (220) boot: Disabling RNG early entropy source...
I (232) octal_psram: vendor id    : 0x0d (AP)
I (233) octal_psram: dev id       : 0x02 (generation 3)
I (233) octal_psram: density      : 0x03 (64 Mbit)
I (238) octal_psram: good-die     : 0x01 (Pass)
I (243) octal_psram: Latency      : 0x01 (Fixed)
I (248) octal_psram: VCC          : 0x01 (3V)
I (253) octal_psram: SRF          : 0x01 (Fast Refresh)
I (259) octal_psram: BurstType    : 0x01 (Hybrid Wrap)
I (265) octal_psram: BurstLen     : 0x01 (32 Byte)
I (270) octal_psram: Readlatency  : 0x02 (10 cycles@Fixed)
I (276) octal_psram: DriveStrength: 0x00 (1/1)
I (282) MSPI Timing: PSRAM timing tuning index: 4
I (287) esp_psram: Found 8MB PSRAM device
I (292) esp_psram: Speed: 80MHz
I (296) cpu_start: Multicore app
I (745) esp_psram: SPI SRAM memory test OK
I (754) cpu_start: Pro cpu start user code
I (754) cpu_start: cpu freq: 160000000 Hz
I (754) app_init: Application information:
I (757) app_init: Project name:     Recording_playback_TEST
I (763) app_init: App version:      v2.7-3-g483af51f-dirty
I (769) app_init: Compile time:     Apr 15 2025 14:14:56
I (775) app_init: ELF file SHA256:  2b3b56bb1...
I (781) app_init: ESP-IDF:          v5.3.2-dirty
I (786) efuse_init: Min chip rev:     v0.0
I (791) efuse_init: Max chip rev:     v0.99
I (796) efuse_init: Chip rev:         v0.2
I (801) heap_init: Initializing. RAM available for dynamic allocation:
I (808) heap_init: At 3FC999C0 len 0004FD50 (319 KiB): RAM
I (814) heap_init: At 3FCE9710 len 00005724 (21 KiB): RAM
I (820) heap_init: At 3FCF0000 len 00008000 (32 KiB): DRAM
I (826) heap_init: At 600FE100 len 00001EE8 (7 KiB): RTCRAM
I (832) esp_psram: Adding pool of 8192K of PSRAM memory to heap allocator
I (841) spi_flash: detected chip: gd
I (844) spi_flash: flash io: dio
W (848) ADC: legacy driver is deprecated, please migrate to `esp_adc/adc_oneshot.h`
I (856) sleep: Configure to isolate all GPIO pins in sleep state
I (863) sleep: Enable automatic switching of GPIO sleep configuration
I (871) main_task: Started on CPU0
I (891) esp_psram: Reserving pool of 32K of internal memory for DMA/internal allocations
I (891) main_task: Calling app_main()
I (951) PERIPH_SPIFFS: Partition size: total: 956561, used: 191764
W (951) i2c_bus_v2: I2C master handle is NULL, will create new one
AW9523B_ID:[0x23]
W (1001) ES7210: Enable TDM mode. ES7210_SDP_INTERFACE2_REG12: 2
I (1011) SPIFFS_AMR_RESAMPLE_EXAMPLE: [1.1] 初始化录音管道
I (1011) SPIFFS_AMR_RESAMPLE_EXAMPLE: [1.2] 为录音管道创建音频元素
I (1021) SPIFFS_AMR_RESAMPLE_EXAMPLE: [1.3] 将音频元素注册到录音管道
I (1031) SPIFFS_AMR_RESAMPLE_EXAMPLE: [2.2] 为播放管道创建音频元素
I (1041) SPIFFS_AMR_RESAMPLE_EXAMPLE: [2.3] 将音频元素注册到播放管道
I (1041) SPIFFS_AMR_RESAMPLE_EXAMPLE: 链接音频元素使录音管道就绪
I (1051) SPIFFS_AMR_RESAMPLE_EXAMPLE: 设置保存录制音频的文件路径
I (1061) SPIFFS_AMR_RESAMPLE_EXAMPLE: 链接音频元素使播放管道就绪
I (1061) SPIFFS_AMR_RESAMPLE_EXAMPLE: 设置要播放的 AMR 音频文件路径
I (1071) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ 3 ] 设置事件监听器
I (1081) SPIFFS_AMR_RESAMPLE_EXAMPLE: [4.1] 来自管道所有元素的监听事件
[ * ]录音准备中…………1
[ * ]录音准备中…………2
[ * ]录音准备中…………3
[ * ]录音开始……
[ * ]录音…………1
[ * ]录音…………2
[ * ]录音…………3
[ * ]录音…………4
[ * ]录音…………5
[ * ]录音…………6
[ * ]录音…………7
[ * ]录音…………8
[ * ]录音…………9
[ * ]录音…………10
[ * ]录音…………11
[ * ]录音…………12
[ * ]录音…………13
[ * ]录音…………14
[ * ]录音…………15
[ * ]录音保存中…………1
[ * ]录音保存中…………2
[ * ]录音保存中…………3
[ * ]录音保存完成
E (22131) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ ** ]停止录音并开始播放
W (22131) AUDIO_ELEMENT: IN-[filter_downsample] AEL_IO_ABORT
W (22141) AUDIO_ELEMENT: [filter_downsample] Element already stopped
W (22141) AUDIO_ELEMENT: IN-[file_writer] AEL_IO_ABORT
W (22181) AUDIO_ELEMENT: IN-[amrnb_encoder] AEL_IO_ABORT
[ * ]回放准备中…………1
[ * ]回放准备中…………2
[ * ]回放准备中…………3
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
W (40431) SPIFFS_STREAM: No more data, ret:0
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]播放完成
E (45721) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ ** ]停止播放并开始录音
W (45721) AUDIO_ELEMENT: [file_reader] Element already stopped
W (45731) AUDIO_ELEMENT: [amr_decoder] Element already stopped
W (45731) AUDIO_ELEMENT: [filter_upsample] Element already stopped
W (45741) AUDIO_ELEMENT: [i2s_writer] Element already stopped
[ * ]录音准备中…………1
[ * ]录音准备中…………2
[ * ]录音准备中…………3
[ * ]录音开始……
[ * ]录音…………1
[ * ]录音…………2
[ * ]录音…………3
[ * ]录音…………4
[ * ]录音…………5
[ * ]录音…………6
[ * ]录音…………7
[ * ]录音…………8
[ * ]录音…………9
[ * ]录音…………10
[ * ]录音…………11
[ * ]录音…………12
[ * ]录音…………13
[ * ]录音…………14
[ * ]录音…………15
[ * ]录音保存中…………1
[ * ]录音保存中…………2
[ * ]录音保存中…………3
[ * ]录音保存完成
E (66831) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ ** ]停止录音并开始播放
W (66831) AUDIO_ELEMENT: IN-[filter_downsample] AEL_IO_ABORT
W (66831) AUDIO_ELEMENT: [filter_downsample] Element already stopped
W (66841) AUDIO_ELEMENT: IN-[file_writer] AEL_IO_ABORT
W (66871) AUDIO_ELEMENT: IN-[amrnb_encoder] AEL_IO_ABORT
[ * ]回放准备中…………1
[ * ]回放准备中…………2
[ * ]回放准备中…………3
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
W (85121) SPIFFS_STREAM: No more data, ret:0
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]播放完成
E (90411) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ ** ]停止播放并开始录音
W (90411) AUDIO_ELEMENT: [file_reader] Element already stopped
W (90411) AUDIO_ELEMENT: [amr_decoder] Element already stopped
W (90421) AUDIO_ELEMENT: [filter_upsample] Element already stopped
W (90431) AUDIO_ELEMENT: [i2s_writer] Element already stopped
[ * ]录音准备中…………1
[ * ]录音准备中…………2
[ * ]录音准备中…………3
[ * ]录音开始……
[ * ]录音…………1
[ * ]录音…………2
[ * ]录音…………3
[ * ]录音…………4
[ * ]录音…………5
[ * ]录音…………6
[ * ]录音…………7
[ * ]录音…………8
[ * ]录音…………9
[ * ]录音…………10
[ * ]录音…………11
[ * ]录音…………12
[ * ]录音…………13
[ * ]录音…………14
[ * ]录音…………15
[ * ]录音保存中…………1
[ * ]录音保存中…………2
[ * ]录音保存中…………3
[ * ]录音保存完成
E (111511) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ ** ]停止录音并开始播放
W (111511) AUDIO_ELEMENT: IN-[filter_downsample] AEL_IO_ABORT
W (111521) AUDIO_ELEMENT: [filter_downsample] Element already stopped
W (111521) AUDIO_ELEMENT: IN-[file_writer] AEL_IO_ABORT
W (111561) AUDIO_ELEMENT: IN-[amrnb_encoder] AEL_IO_ABORT
[ * ]回放准备中…………1
[ * ]回放准备中…………2
[ * ]回放准备中…………3
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
W (129821) SPIFFS_STREAM: No more data, ret:0
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]回放中……
[ * ]播放完成
E (135111) SPIFFS_AMR_RESAMPLE_EXAMPLE: [ ** ]停止播放并开始录音
W (135111) AUDIO_ELEMENT: [file_reader] Element already stopped
W (135121) AUDIO_ELEMENT: [amr_decoder] Element already stopped
W (135121) AUDIO_ELEMENT: [filter_upsample] Element already stopped
W (135131) AUDIO_ELEMENT: [i2s_writer] Element already stopped
```
