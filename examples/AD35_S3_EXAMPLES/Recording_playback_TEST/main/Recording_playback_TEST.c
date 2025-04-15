/*  带重采样的录音和播放示例。

   此示例代码属于公共领域（或根据您的选择采用 CC0 许可）。

   除非适用法律要求或书面同意，否则本软件按“原样”分发，不附带任何形式的明示或暗示保证或条件。
*/

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "esp_log.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "audio_mem.h"
#include "audio_common.h"
#include "spiffs_stream.h"
#include "i2s_stream.h"
#include "amrnb_encoder.h"
#include "amr_decoder.h"
#include "board.h"
#include "filter_resample.h"
#include "esp_peripherals.h"
#include "periph_button.h"
#include "periph_spiffs.h"
#include "periph_adc_button.h"
#include "periph_touch.h"

// 日志标签
static const char *TAG = "SPIFFS_AMR_RESAMPLE_EXAMPLE";
// 外设集合句柄
static esp_periph_set_handle_t set;

// 录音采样率
#define RECORD_RATE         48000
// 录音声道数
#define RECORD_CHANNEL      2
// 录音位深度
#define RECORD_BITS         16

// 保存文件的采样率
#define SAVE_FILE_RATE      8000
// 保存文件的声道数
#define SAVE_FILE_CHANNEL   1
// 保存文件的位深度
#define SAVE_FILE_BITS      16

// 播放采样率
#define PLAYBACK_RATE       48000
// 播放声道数
#define PLAYBACK_CHANNEL    2
// 播放位深度
#define PLAYBACK_BITS       16

// 1: 事件监控模式，0: 顺序逻辑模式
#define RECORD_MODE         1  
// #define RECORD_MODE         0  
//录音事件（秒）
#define RECORD_TIME         15  

/**
 * @brief 创建重采样滤波器
 * 
 * @param source_rate 源采样率
 * @param source_channel 源声道数
 * @param dest_rate 目标采样率
 * @param dest_channel 目标声道数
 * @return audio_element_handle_t 音频元素句柄
 */
static audio_element_handle_t create_filter(int source_rate, int source_channel, int dest_rate, int dest_channel)
{
    // 获取默认的重采样滤波器配置
    rsp_filter_cfg_t rsp_cfg = DEFAULT_RESAMPLE_FILTER_CONFIG();
    rsp_cfg.src_rate = source_rate;
    rsp_cfg.src_ch = source_channel;
    rsp_cfg.dest_rate = dest_rate;
    rsp_cfg.dest_ch = dest_channel;
    return rsp_filter_init(&rsp_cfg);
}

/**
 * @brief 创建 SPIFFS 流
 * 
 * @param sample_rates 采样率
 * @param bits 位深度
 * @param channels 声道数
 * @param type 音频流类型
 * @return audio_element_handle_t 音频元素句柄
 */
static audio_element_handle_t create_spiffs_stream(int sample_rates, int bits, int channels, audio_stream_type_t type)
{
    // 获取默认的 SPIFFS 流配置
    spiffs_stream_cfg_t spiffs_cfg = SPIFFS_STREAM_CFG_DEFAULT();
    spiffs_cfg.type = type;
    audio_element_handle_t spiffs_stream = spiffs_stream_init(&spiffs_cfg);
    mem_assert(spiffs_stream);
    audio_element_info_t writer_info = {0};
    // 获取音频元素信息
    audio_element_getinfo(spiffs_stream, &writer_info);
    writer_info.bits = bits;
    writer_info.channels = channels;
    writer_info.sample_rates = sample_rates;
    // 设置音频元素信息
    audio_element_setinfo(spiffs_stream, &writer_info);
    return spiffs_stream;
}

/**
 * @brief 创建 I2S 流
 * 
 * @param sample_rates 采样率
 * @param bits 位深度
 * @param channels 声道数
 * @param type 音频流类型
 * @return audio_element_handle_t 音频元素句柄
 */
static audio_element_handle_t create_i2s_stream(int sample_rates, int bits, int channels, audio_stream_type_t type)
{
    // 获取默认的 I2S 流配置
    i2s_stream_cfg_t i2s_cfg = I2S_STREAM_CFG_DEFAULT_WITH_PARA(CODEC_ADC_I2S_PORT, sample_rates, bits, type);
    audio_element_handle_t i2s_stream = i2s_stream_init(&i2s_cfg);
    mem_assert(i2s_stream);
    // 设置音频信息
    audio_element_set_music_info(i2s_stream, sample_rates, channels, bits);
    return i2s_stream;
}

/**
 * @brief 创建 AMR-NB 编码器
 * 
 * @return audio_element_handle_t 音频元素句柄
 */
static audio_element_handle_t create_amrnb_encoder()
{
    // 获取默认的 AMR-NB 编码器配置
    amrnb_encoder_cfg_t amrnb_cfg = DEFAULT_AMRNB_ENCODER_CONFIG();
    return amrnb_encoder_init(&amrnb_cfg);
}

/**
 * @brief 创建 AMR 解码器
 * 
 * @return audio_element_handle_t 音频元素句柄
 */
static audio_element_handle_t create_amr_decoder()
{
    // 获取默认的 AMR 解码器配置
    amr_decoder_cfg_t amr_cfg = DEFAULT_AMR_DECODER_CONFIG();
    return amr_decoder_init(&amr_cfg);
}

/**
 * @brief 录音和播放任务
 */
void record_playback_task()
{
    // 录音管道句柄
    audio_pipeline_handle_t pipeline_rec = NULL;
    // 播放管道句柄
    audio_pipeline_handle_t pipeline_play = NULL;
    // 获取默认的音频管道配置
    audio_pipeline_cfg_t pipeline_cfg = DEFAULT_AUDIO_PIPELINE_CONFIG();

    /**
     * 对于录音部分:
     * 我们将设置 I2S 以 48000Hz 采样率、16 位深度、2 声道获取音频。
     * 然后使用重采样滤波器转换为 8000Hz 采样率、16 位深度、1 声道。使用 AMR-NB 编码器进行编码
     * 最后写入 SPIFFS
     */
    ESP_LOGI(TAG, "[1.1] 初始化录音管道");
    pipeline_rec = audio_pipeline_init(&pipeline_cfg);
    pipeline_play = audio_pipeline_init(&pipeline_cfg);

    ESP_LOGI(TAG, "[1.2] 为录音管道创建音频元素");
    audio_element_handle_t i2s_reader_el = create_i2s_stream(RECORD_RATE, RECORD_BITS, RECORD_CHANNEL, AUDIO_STREAM_READER);
    audio_element_handle_t filter_downsample_el = create_filter(RECORD_RATE, RECORD_CHANNEL, SAVE_FILE_RATE, SAVE_FILE_CHANNEL);
    audio_element_handle_t amrnb_encoder_el = create_amrnb_encoder();
    audio_element_handle_t spiffs_writer_el = create_spiffs_stream(SAVE_FILE_RATE, SAVE_FILE_BITS, SAVE_FILE_CHANNEL, AUDIO_STREAM_WRITER);

    ESP_LOGI(TAG, "[1.3] 将音频元素注册到录音管道");
    audio_pipeline_register(pipeline_rec, i2s_reader_el,        "i2s_reader");
    audio_pipeline_register(pipeline_rec, filter_downsample_el, "filter_downsample");
    audio_pipeline_register(pipeline_rec, amrnb_encoder_el,     "amrnb_encoder");
    audio_pipeline_register(pipeline_rec, spiffs_writer_el,     "file_writer");

    /**
     * 对于播放部分:
     * 我们将读取录制的文件，采样率 8000Hz、16 位深度、1 声道，
     * 使用 AMR 解码器进行解码
     * 然后使用重采样滤波器转换为 48000Hz 采样率、16 位深度、2 声道。
     * 最后将音频发送到 I2S
     */
    ESP_LOGI(TAG, "[2.2] 为播放管道创建音频元素");
    audio_element_handle_t spiffs_reader_el = create_spiffs_stream(SAVE_FILE_RATE, SAVE_FILE_BITS, SAVE_FILE_CHANNEL, AUDIO_STREAM_READER);
    audio_element_handle_t amr_decoder_el = create_amr_decoder();
    audio_element_handle_t filter_upsample_el = create_filter(SAVE_FILE_RATE, SAVE_FILE_CHANNEL, PLAYBACK_RATE, PLAYBACK_CHANNEL);
    audio_element_handle_t i2s_writer_el = create_i2s_stream(PLAYBACK_RATE, PLAYBACK_BITS, PLAYBACK_CHANNEL, AUDIO_STREAM_WRITER);

    ESP_LOGI(TAG, "[2.3] 将音频元素注册到播放管道");
    audio_pipeline_register(pipeline_play, spiffs_reader_el,     "file_reader");
    audio_pipeline_register(pipeline_play, amr_decoder_el,       "amr_decoder");
    audio_pipeline_register(pipeline_play, filter_upsample_el,   "filter_upsample");
    audio_pipeline_register(pipeline_play, i2s_writer_el,        "i2s_writer");

    /**
     * 音频录音流程:
     * [编解码器芯片]-->i2s流--->滤波器-->AMR-NB编码器-->SPIFFS流-->[闪存]
     */
    ESP_LOGI(TAG, "链接音频元素使录音管道就绪");
    const char *link_rec[4] = {"i2s_reader", "filter_downsample", "amrnb_encoder", "file_writer"};
    audio_pipeline_link(pipeline_rec, &link_rec[0], 4);

    ESP_LOGI(TAG, "设置保存录制音频的文件路径");
    i2s_stream_set_clk(i2s_writer_el, RECORD_RATE, RECORD_BITS, RECORD_CHANNEL);
    audio_element_set_uri(spiffs_writer_el, "/spiffs/rec.amr");
    // audio_pipeline_run(pipeline_rec);
    
    /**
     * 音频播放流程:
     * [闪存]-->SPIFFS流-->AMR解码器-->滤波器-->i2s流-->[编解码器芯片]
     */
    ESP_LOGI(TAG, "链接音频元素使播放管道就绪");
    const char *link_play[4] = {"file_reader", "amr_decoder", "filter_upsample", "i2s_writer"};
    audio_pipeline_link(pipeline_play, &link_play[0], 4);

    ESP_LOGI(TAG, "设置要播放的 AMR 音频文件路径");
    i2s_stream_set_clk(i2s_writer_el, PLAYBACK_RATE, PLAYBACK_BITS, PLAYBACK_CHANNEL);
    audio_element_set_uri(spiffs_reader_el, "/spiffs/rec.amr");
    // audio_pipeline_run(pipeline_play);

    ESP_LOGI(TAG, "[ 3 ] 设置事件监听器");
    audio_event_iface_cfg_t evt_cfg = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    audio_event_iface_handle_t evt = audio_event_iface_init(&evt_cfg);

    audio_event_iface_set_listener(esp_periph_set_get_event_iface(set), evt);

    ESP_LOGI(TAG, "[4.1] 来自管道所有元素的监听事件");
    audio_pipeline_set_listener(pipeline_play, evt);
    audio_pipeline_set_listener(pipeline_rec, evt);
    
    while (1) {
        audio_pipeline_run(pipeline_rec);

        for(int i = 0; i < 3; i++)
        {
            printf("[ * ]录音准备中…………%d\n", i+1);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        printf("[ * ]录音开始……\n");
    
        for(int i = 0; i < RECORD_TIME; i++)
        {
            printf("[ * ]录音…………%d\n", i+1);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        for(int i = 0; i < 3; i++)
        {
            printf("[ * ]录音保存中…………%d\n", i+1);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        printf("[ * ]录音保存完成\n");
        ESP_LOGE(TAG, "[ ** ]停止录音并开始播放");
        audio_pipeline_stop(pipeline_rec);
        audio_pipeline_wait_for_stop(pipeline_rec);
        audio_pipeline_terminate(pipeline_rec);
        audio_pipeline_reset_ringbuffer(pipeline_rec);
        audio_pipeline_reset_elements(pipeline_rec);
        for(int i = 0; i < 3; i++)
        {
            printf("[ * ]回放准备中…………%d\n", i+1);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        audio_pipeline_run(pipeline_play);
        #if RECORD_MODE
        while(1)
        {
            printf("[ * ]回放中……\n");
            audio_event_iface_msg_t msg;
            // 监听事件
            esp_err_t ret = audio_event_iface_listen(evt, &msg, portMAX_DELAY);
            if (ret != ESP_OK) {
                // ESP_LOGE(TAG, "[ * ] 事件接口错误 : %d", ret);
                continue;
            }
            if(msg.source_type == AUDIO_ELEMENT_TYPE_ELEMENT && msg.source == (void*)i2s_writer_el && msg.cmd == AEL_MSG_CMD_REPORT_STATUS && (int)msg.data == AEL_STATUS_STATE_FINISHED)
            {
                printf("[ * ]播放完成\n");
                break;
            }
        }
        #elif RECORD_MODE == 0
        for(int i = 0; i < RECORD_TIME+6; i++)
        {
            printf("[ * ]播放中…………\n");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        #endif
        ESP_LOGE(TAG, "[ ** ]停止播放并开始录音");
        audio_pipeline_stop(pipeline_play);
        audio_pipeline_wait_for_stop(pipeline_play);
        audio_pipeline_terminate(pipeline_play);
        audio_pipeline_reset_ringbuffer(pipeline_play);
        audio_pipeline_reset_elements(pipeline_play);
    }

    ESP_LOGI(TAG, "[ 4 ] 停止音频管道");
    audio_pipeline_stop(pipeline_rec);
    audio_pipeline_wait_for_stop(pipeline_rec);
    audio_pipeline_terminate(pipeline_rec);
    audio_pipeline_stop(pipeline_play);
    audio_pipeline_wait_for_stop(pipeline_play);
    audio_pipeline_terminate(pipeline_play);

    /* 在移除监听器之前终止管道 */
    audio_pipeline_remove_listener(pipeline_rec);
    audio_pipeline_remove_listener(pipeline_play);

    /* 在移除监听器之前停止所有外设 */
    esp_periph_set_stop_all(set);
    audio_event_iface_remove_listener(esp_periph_set_get_event_iface(set), evt);

    /* 确保在销毁事件接口之前调用 audio_pipeline_remove_listener 和 audio_event_iface_remove_listener */
    audio_event_iface_destroy(evt);

    /* 释放所有资源 */
    audio_pipeline_unregister(pipeline_play, spiffs_reader_el);
    audio_pipeline_unregister(pipeline_play, amr_decoder_el);
    audio_pipeline_unregister(pipeline_play, filter_upsample_el);
    audio_pipeline_unregister(pipeline_play, i2s_writer_el);
    audio_element_deinit(spiffs_reader_el);
    audio_element_deinit(amr_decoder_el);
    audio_element_deinit(filter_upsample_el);
    audio_element_deinit(i2s_writer_el);

    audio_pipeline_unregister(pipeline_rec, i2s_reader_el);
    audio_pipeline_unregister(pipeline_rec, filter_downsample_el);
    audio_pipeline_unregister(pipeline_rec, amrnb_encoder_el);
    audio_pipeline_unregister(pipeline_rec, spiffs_writer_el);
    audio_element_deinit(i2s_reader_el);
    audio_element_deinit(filter_downsample_el);
    audio_element_deinit(amrnb_encoder_el);
    audio_element_deinit(spiffs_writer_el);
}

/**
 * @brief 应用程序入口函数
 */
void app_main(void)
{
    // 设置所有日志的日志级别为警告
    esp_log_level_set("*", ESP_LOG_WARN);
    // 设置当前模块的日志级别为信息
    esp_log_level_set(TAG, ESP_LOG_INFO);
    // 设置 SPIFFS 外设的日志级别为信息
    esp_log_level_set("PERIPH_SPIFFS", ESP_LOG_INFO);

    // 初始化外设管理
    esp_periph_config_t periph_cfg = DEFAULT_ESP_PERIPH_SET_CONFIG();
    set = esp_periph_set_init(&periph_cfg);

    // 初始化 SPIFFS 外设
    periph_spiffs_cfg_t spiffs_cfg = {
        .root = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true
    };
    esp_periph_handle_t spiffs_handle = periph_spiffs_init(&spiffs_cfg);
    // 启动 SPIFFS 和按钮外设
    esp_periph_start(set, spiffs_handle);
    // 等待 SPIFFS 挂载完成
    while (!periph_spiffs_is_mounted(spiffs_handle)) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    // // 初始化音频板按键
    // audio_board_key_init(set);

    // 设置音频编解码器
    audio_board_handle_t board_handle = audio_board_init();
    audio_hal_ctrl_codec(board_handle->audio_hal, AUDIO_HAL_CODEC_MODE_BOTH, AUDIO_HAL_CTRL_START);

    // 启动录音/播放任务
    record_playback_task();
    // 销毁外设集合
    esp_periph_set_destroy(set);
}
