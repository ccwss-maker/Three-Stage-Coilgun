#ifndef FIRE_H
#define FIRE_H

#include "main.h"
#include "stdio.h"
#include "lv_app.h"

#define _1S 1000000 
extern uint16_t adcData[10];
extern lv_obj_t 	* V_1_label,* V_2_label,* V_3_label,
									* E_1_label,* E_2_label,* E_3_label,
									* P_1_label,* P_2_label,* P_3_label,
									* VV_1_label,* VV_2_label,* VV_3_label;
extern uint8_t sign_V;
extern uint8_t sign_VV;
extern uint8_t sign_EE;
extern uint8_t sign_PP;
extern uint8_t sign_C[3];
extern uint8_t sign_F[3];
extern uint16_t C_MAX[3];
void CS();
#endif