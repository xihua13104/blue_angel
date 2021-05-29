/******************************************************************************
  * @file           bt_common.h
  * @author         Yu-ZhongJun(124756828@qq.com)
  * @Taobao link    https://shop220811498.taobao.com/
  * @version        V0.0.1
  * @date           2020-4-14
  * @brief          bt common header file
******************************************************************************/

#ifndef COMMON_H_H_H
#define COMMON_H_H_H
#include "bt_config.h"
#include "stdint.h"
#include "bt_os_layer_api.h"

#define BT_SHARE_BUFFER_LOCK()   //bt_os_layer_disable_interrupt()
#define BT_SHARE_BUFFER_UNLOCK() //bt_os_layer_enable_interrupt()

#endif

