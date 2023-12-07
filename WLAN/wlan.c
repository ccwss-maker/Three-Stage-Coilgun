#include "wlan.h"
/////////////////////////////////////    ESP_初始化    ///////////////////////////////////
uint8_t tim_start_sign=1;
uint8_t ESP_sign= 0;
void ESP_Init()
{
	if(ESP_sign==1)
	{
		if(tim_start_sign==1)
		{
			TIM1->CNT=0;
			HAL_TIM_Base_Start(&htim1);
			tim_start_sign=0;
		}
		
		if(TIM1->CNT>=300*ms)
		{
			printf("AT+CIPMUX=1\r\n");
			HAL_Delay(1);
			printf("AT+CIPSERVERMAXCONN=1\r\n");
			HAL_Delay(1);
			printf("AT+CIPSERVER=1,5000\r\n");
			HAL_TIM_Base_Stop(&htim1);
			tim_start_sign=1;
			ESP_sign=0;
		}
	}
}	

void control_test_0(char *string)
{
	if(strstr(string,KC1)!=NULL)	
	{
		sign_C[0]=1;
	}
	else if(strstr(string,KC2)!=NULL)	
	{
		sign_C[1]=1;
	}
	else if(strstr(string,KC3)!=NULL)	
	{
		sign_C[2]=1;
	}
	else if(strstr(string,KF1)!=NULL)	
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
	}
	else if(strstr(string,KF2)!=NULL)	
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);
	}
	else if(strstr(string,KF3)!=NULL)	
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
	}
	else if(strstr(string,GET_V)!=NULL)	
	{
		sign_V=1;
	}
	else if(strstr(string,GET_VV)!=NULL)	
	{
		sign_VV=1;
	}
	else if(strstr(string,GET_EE)!=NULL)	
	{
		sign_EE=1;
	}
	else if(strstr(string,GET_PP)!=NULL)	
	{
		sign_PP=1;
	}
	else if(strstr(string,STOP)!=NULL)			
	{
		sign_C[0]=0;
		sign_C[1]=0;
		sign_C[2]=0;
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
	}
}

void control_test_1(char *string)
{
	extern lv_obj_t* bg_img;
	extern lv_obj_t* tab4;
	if(strstr(string,KC1)!=NULL)	
	{
		lv_img_set_src(bg_img, "0:/Picture/red.bin");
		lv_obj_align(bg_img,tab4,LV_ALIGN_CENTER,0,0);
	}
	else if(strstr(string,KC2)!=NULL)	
	{
		lv_img_set_src(bg_img, "0:/Picture/blue.bin");
		lv_obj_align(bg_img,tab4,LV_ALIGN_CENTER,0,0);
	}
	else if(strstr(string,KC3)!=NULL)	
	{
		lv_img_set_src(bg_img, "0:/Picture/green.bin");
		lv_obj_align(bg_img,tab4,LV_ALIGN_CENTER,0,0);
	}
	else if(strstr(string,KF1)!=NULL)	
	{
		lv_img_set_src(bg_img, "0:/Picture/yello_max.bin");
		lv_obj_align(bg_img,tab4,LV_ALIGN_CENTER,0,0);
	}
	else if(strstr(string,KF2)!=NULL)	
	{
		lv_img_set_src(bg_img, "0:/Picture/logo_40_40.bin");
		lv_obj_align(bg_img,tab4,LV_ALIGN_CENTER,10,0);
	}
	else if(strstr(string,KF3)!=NULL)	
	{
		
	}
	else if(strstr(string,STOP)!=NULL)			
	{

	}
}






















