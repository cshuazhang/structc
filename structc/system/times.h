﻿#ifndef _H_TIMES
#define _H_TIMES

#include <time.h>
#include <stdbool.h>

//
// ~ 力求最小时间业务单元 ~ 
// 1s(秒) = 1000ms(毫秒) = 1000000us(微秒) = 1000000000ns(纳秒)
//
#ifdef __GNUC__

#include <unistd.h>
#include <sys/time.h>

//
// msleep - 睡眠函数, 颗粒度是毫秒.
// m        : 待睡眠的毫秒数
// return   : void
//
inline void msleep(int ms) {
    usleep(ms * 1000);
}

#endif

#ifdef _MSC_VER

#include <windows.h>

inline void msleep(int ms) {
    Sleep(ms);
}

//
// localtime_r - 安全的得到当前时间结构体
// timep        : 输入的时间戳指针
// result       : 返回输出时间结构
// return       : 失败 NULL, 正常返回 result
//
inline struct tm * localtime_r(const time_t * timep, struct tm * result) {
    return localtime_s(result, timep) ? NULL : result;
}

//
// usleep - 微秒级别等待函数
// usec         : 等待的微秒
// return       : 0 on success.  On error, -1 is returned.
//
extern int usleep(unsigned usec);

#endif

// times_t - 时间串类型
#define INT_TIMES (64)
typedef char times_t[INT_TIMES];

//
// times_get - 解析时间串, 返回时间戳
// ns           : 时间串内容 
// ot           : 返回得到的时间戳
// om           : 返回得到的时间结构体
// return       : 返回 true 表示构造成功
//
extern bool times_get(times_t ns, time_t * ot, struct tm * om);

//
// time_get - 解析时间串, 返回时间戳
// ns           : 时间串内容  
// return       : < 0 is error
//
extern time_t time_get(times_t ns);

//
// time_day - 判断时间戳是否是同一天
// n            : 第一个时间戳
// t            : 第二个时间戳
// return       : true 表示同一天
//
extern bool time_day(time_t n, time_t t);

//
// time_now - 判断时间戳是否是今天
// t            : 待判断的时间戳
// return       : 返回当前时间戳, < 0 is error
//
extern time_t time_now(time_t t);

//
// time_week - 判断时间戳是否是同一周
// n            : 第一个时间戳
// t            : 第二个时间戳
// return       : true 表示同一周
//
extern bool time_week(time_t n, time_t t);

//
// times_day - 判断时间串是否是同一天
// ns           : 第一个时间串
// ts           : 第二个时间串
// return       : true 表示同一天
//
extern bool times_day(times_t ns, times_t ts);

//
// times_week - 判断时间串是否是同一周
// ns           : 第一个时间串
// ts           : 第二个时间串
// return       : true 表示同一周
//
extern bool times_week(times_t ns, times_t ts);

//
// times_fmt - 通过 fmt 格式最终拼接一个字符串
// fmt          : 必须包含 %04d %02d %02d %02d %02d %02d %03d
// out          : 最终保存的内容
// sz           : buf 长度
// return       : 返回生成串长度
//
int times_fmt(const char * fmt, char out[], size_t sz);

//
// times_buf - 存储毫秒串 "2016-07-10 22:38:34 500"
// ns           : 返回生成串
// return       : 返回生成串长度
#define STR_TIMES "%04d-%02d-%02d %02d:%02d:%02d %03d"
inline int times_buf(times_t ns) {
    return times_fmt(STR_TIMES, ns, sizeof(times_t));
}

//
// times_str - 得到毫秒串
// ns           : 返回生成串
// return       : 返回生成串
//
inline char * times_str(times_t ns) {
    times_buf(ns);
    return ns;
}

#endif//_H_TIMES
