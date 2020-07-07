/**
 * Tony Li wabslygzj@163.com
 */
#ifndef FFMPEGDEMO_ANDROIDLOG_H
#define FFMPEGDEMO_ANDROIDLOG_H

#endif

#include <android/log.h>

#define LOGD(FORMAT, ...)    __android_log_print(ANDROID_LOG_DEBUG,"TonyLi2023",FORMAT,##__VA_ARGS__)

#define LOGE(FORMAT, ...)    __android_log_print(ANDROID_LOG_ERROR,"TonyLi2023",FORMAT,##__VA_ARGS__)