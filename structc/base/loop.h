﻿#ifndef _H_LOOP
#define _H_LOOP

#include "q.h"
#include "atom.h"
#include "thread.h"

typedef struct loop * loop_t;

//
// loop_delete - 轮询对象销毁
// p        : 轮询对象
// return   : void
//
extern void loop_delete(loop_t p);

//
// loop_push - 消息压入轮询器
// p        : 轮询对象
// m        : 消息
// return   : void
//
extern void loop_push(loop_t p, void * m);

//
// loop_create - 创建轮询对象
// frun     : 消息处理行为
// fdie     : 消息销毁行为
// return   : 轮询对象
//
#define loop_create(frun, fdie)                     \
loop_create_((node_f)(frun), (node_f)(fdie))
extern loop_t loop_create_(node_f frun, node_f fdie);

#endif//_H_LOOP
