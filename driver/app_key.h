#ifndef __APP_KEY_H
#define __APP_KEY_H

#include "key.h"
//#include "app_show.h"

extern void KEY_Init(void);
//extern uint8_t KEY_scan_result(void);
extern uint8_t APP_KEY_Scan(void);
extern void KET_Timeout(void *p);
extern void TIM2_Init(void);

#endif

