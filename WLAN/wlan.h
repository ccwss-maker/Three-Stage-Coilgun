#ifndef WLAN_H
#define WLAN_H
#include "main.h"
#include "lv_app.h"
#include "tim.h"
#include "fire.h"
#define ms 10

	static char *KC1="+IPD,0,1:1";
	static char *KC2="+IPD,0,1:2";
	static char *KC3="+IPD,0,1:3";
	static char *KF1="+IPD,0,1:4";
	static char *KF2="+IPD,0,1:5";
	static char *KF3="+IPD,0,1:6";
	static char *GET_V="+IPD,0,1:7";
	static char *GET_VV="+IPD,0,1:8";
	static char *GET_EE="+IPD,0,1:9";
	static char *GET_PP="+IPD,0,1:a";
	static char *STOP="+IPD,0,1:0";
	
	extern uint8_t ESP_sign;
	
	void ESP_Init();
	void control_test_0(char *string);
	void control_test_1(char *string);
	
#endif