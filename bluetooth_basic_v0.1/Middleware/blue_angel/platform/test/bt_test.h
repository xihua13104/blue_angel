/******************************************************************************
  * @file           bt_platform.h
  * @author         Leonard-y.He(1027901556@qq.com)
  * @version        V0.1
  * @date           2021-05-17
  * @brief
******************************************************************************/

#ifndef __BT_TEST_H
#define __BT_TEST_H

#include "stm32f10x_conf.h"
#include "core_cmfunc.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "cunit_interface.h"

void bt_linknode_test(void);

void bt_memory_test(void); 

void bt_timer_test(void); 

#endif
