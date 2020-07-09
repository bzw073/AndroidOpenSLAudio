#include <jni.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <stdio.h>
#include <malloc.h>
#include "AndroidLog.h"

SLObjectItf engineObject = NULL;
SLEngineItf engineEngine = NULL;

SLObjectItf outputMixObject = NULL;
//混响环境设置
SLEnvironmentalReverbItf outputMixEnvironmentalReverb;
SLEnvironmentalReverbSettings slEnvironmentalReverbSettings = SL_I3DL2_ENVIRONMENT_PRESET_STONECORRIDOR;
SLObjectItf pcmPlayerObject = NULL;
SLPlayItf pcmPlayerPlay;
SLAndroidSimpleBufferQueueItf pcmBufferQueue;
//读取PCM文件
FILE *pcmFile;
//设置缓存
void *buffer;
//设置输出缓存
uint8_t *out_buffer;

//获取PCM数据
int getPCMData(void **pcm) {
    int size = 0;
    while (!feof(pcmFile)) {
        size = fread(out_buffer, 1, 44100 * 2 * 2, pcmFile);
        if (out_buffer == NULL) {
            LOGE("%s", "No readable data.END");
            break;
        } else {
            LOGE("%s", "Reading data.READING");
        }
        *pcm = out_buffer;
        break;
    }
    return size;
}

//PCM缓存队列回调函数
void pcmBufferCallback(SLAndroidSimpleBufferQueueItf simpleBufferQueue, void *const context) {
    int size = getPCMData(&buffer);
    if (buffer != NULL) {
        (*pcmBufferQueue)->Enqueue(pcmBufferQueue, buffer, size);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_rainbow_androidopenslaudio_MainActivity_playAudioWithPCM(JNIEnv *env, jobject instance,
                                                                  jstring url_) {

    const char *url = env->GetStringUTFChars(url_, 0);
    LOGD("#请求音频路径:%s", url);
    pcmFile = fopen(url, "r+");

    if (pcmFile == NULL) {
        LOGE("#音频路径%s不存在文件", url);
        return;
    }
    //初始化音频缓存
    out_buffer = (uint8_t *) malloc(44100 * 2 * 2);
    //创建SL三部曲
    slCreateEngine(&engineObject, 0, 0, 0, 0, 0);
    (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    //设置混音器
    const SLInterfaceID mids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean mrep[1] = {SL_BOOLEAN_FALSE};

    (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, mids, mrep);
    (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    (*outputMixObject)->GetInterface(outputMixObject, SL_IID_ENVIRONMENTALREVERB,
                                     &outputMixEnvironmentalReverb);


    (*outputMixEnvironmentalReverb)->SetEnvironmentalReverbProperties(outputMixEnvironmentalReverb,
                                                                      &slEnvironmentalReverbSettings);
    SLDataLocator_AndroidSimpleBufferQueue android_queue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
                                                            2};

    SLDataFormat_PCM format_pcm = {
            SL_DATAFORMAT_PCM,
            2,
            SL_SAMPLINGRATE_44_1,
            //bitsPerSample
            SL_PCMSAMPLEFORMAT_FIXED_16,
            //containerSize
            SL_PCMSAMPLEFORMAT_FIXED_16,
            SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT,
            SL_BYTEORDER_LITTLEENDIAN
    };

    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSource slDataSource = {&android_queue, &format_pcm};
    SLDataSink audioSink = {&outputMix, NULL};

    const SLInterfaceID ids[1] = {SL_IID_BUFFERQUEUE};
    const SLboolean req[1] = {SL_BOOLEAN_TRUE};

    //创建音频播放器
    (*engineEngine)->CreateAudioPlayer(
            engineEngine, &pcmPlayerObject, &slDataSource, &audioSink, 1, ids, req
    );
    (*pcmPlayerObject)->Realize(pcmPlayerObject, SL_BOOLEAN_FALSE);
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_PLAY, &pcmPlayerPlay);

    //设置缓冲队列和回调函数
    (*pcmPlayerObject)->GetInterface(pcmPlayerObject, SL_IID_BUFFERQUEUE, &pcmBufferQueue);
    //注册回调函数
    (*pcmBufferQueue)->RegisterCallback(pcmBufferQueue, &pcmBufferCallback, NULL);
    //设置播放状态
    (*pcmPlayerPlay)->SetPlayState(pcmPlayerPlay, SL_PLAYSTATE_PLAYING);
    //调用回调函数
    pcmBufferCallback(pcmBufferQueue, NULL);
    env->ReleaseStringUTFChars(url_, url);
}