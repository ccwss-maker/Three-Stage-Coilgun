#include "Fire.h"

static char buf[20]={0};
static float VV[3]={0};
static float EE[3]={0};
static float PP[3]={0};
static float DY[3]={0};
static double m_0=0.8832,m_1=2.68,m_2=100,m_3=1000;		/*g*/
static uint8_t sign[3]={0};
uint8_t sign_V=0;
uint8_t sign_VV=0;
uint8_t sign_EE=0;
uint8_t sign_PP=0;
uint8_t sign_C[3]={0};
uint8_t sign_F[3]={0};
uint16_t C_MAX[3]={0};
void cdy()
{
	DY[0]=3303.3/4096*adcData[7];
	DY[1]=3303.3/4096*adcData[8];
	DY[2]=3303.3/4096*adcData[9];
	sprintf(buf,"V1=%5.0f",DY[0]);
	lv_label_set_text(VV_1_label, buf);
	
	sprintf(buf,"V2=%5.0f",DY[1]);
	lv_label_set_text(VV_2_label, buf);
	
	sprintf(buf,"V3=%5.0f",DY[2]);
	lv_label_set_text(VV_3_label, buf);
}

void fire()
{
	if(adcData[0]>=300)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
	}
	else if(adcData[2]>=300)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);
	}
	else if(adcData[4]>=300)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
	}
	
	if(adcData[1]>=300)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
	}
	else if(adcData[3]>=300)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
	}
	else if(adcData[5]>=300)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
	}
}

void Charge()
{
	static uint8_t sign[3]={0};
	static uint8_t lock[3]={0};
	
	if(lock[0]==0&&sign[0]==0&&sign_C[0]==1)
	{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
		sign[0]=1;
	}
	else if(sign[0]==1&&sign_C[0]==0)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
		sign[0]=0;
	}
	
	if(lock[1]==0&&sign[1]==0&&sign_C[1]==1)
	{
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
		sign[1]=1;
	}
	else if(sign[1]==1&&sign_C[1]==0)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);
		sign[1]=0;
	}
	
	if(lock[2]==0&&sign[2]==0&&sign_C[2]==1)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);
		sign[2]=1;
	}
	else if(sign[2]==1&&sign_C[2]==0)
	{
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);
		sign[2]=0;
	}
	
	if(DY[0]>=C_MAX[0])
	{
		lock[0]=1;
		sign_C[0]=0;
	}
	else if(DY[0]<C_MAX[0])
	{
		lock[0]=0;
	}
	
	if(DY[1]>=C_MAX[1])
	{
		lock[1]=1;
		sign_C[1]=0;
	}
	else if(DY[1]<C_MAX[1])
	{
		lock[1]=0;
	}
	
	if(DY[2]>=C_MAX[2])
	{
		lock[2]=1;
		sign_C[2]=0;
	}
	else if(DY[2]<C_MAX[2])
	{
		lock[2]=0;
	}
}
void ESP_Value()
{
	if(sign_V==1)
	{
		printf("AT+CIPSEND=0,10\r\n");
		HAL_Delay(1);
		printf("V1=%5.0f\r\n",DY[0]);
		HAL_Delay(4);
		printf("AT+CIPSEND=0,10\r\n");
		HAL_Delay(1);
		printf("V2=%5.0f\r\n",DY[1]);
		HAL_Delay(4);
		printf("AT+CIPSEND=0,10\r\n");
		HAL_Delay(1);
		printf("V3=%5.0f\r\n",DY[2]);
		
		sign_V=0;
	}
	
	if(sign_VV==1)
	{
		printf("AT+CIPSEND=0,11\r\n");
		HAL_Delay(1);
		printf("VV0=%5.2f\r\n",VV[0]);
		HAL_Delay(4);
		printf("AT+CIPSEND=0,11\r\n");
		HAL_Delay(1);
		printf("VV1=%5.2f\r\n",VV[1]);
		HAL_Delay(4);
		printf("AT+CIPSEND=0,11\r\n");
		HAL_Delay(1);
		printf("VV2=%5.2f\r\n",VV[2]);
		
		sign_VV=0;
	}
	
	if(sign_EE==1)
	{
		printf("AT+CIPSEND=0,11\r\n");
		HAL_Delay(1);
		printf("EE0=%5.2f\r\n",EE[0]);
		HAL_Delay(4);
		printf("AT+CIPSEND=0,11\r\n");
		HAL_Delay(1);
		printf("EE1=%5.2f\r\n",EE[1]);
		HAL_Delay(4);
		printf("AT+CIPSEND=0,11\r\n");
		HAL_Delay(1);
		printf("EE2=%5.2f\r\n",EE[2]);
		
		sign_EE=0;
	}
	
	if(sign_PP==1)
	{
		printf("AT+CIPSEND=0,11\r\n");
		HAL_Delay(1);
		printf("PP0=%5.2f\r\n",PP[0]);
		HAL_Delay(4);
		printf("AT+CIPSEND=0,11\r\n");
		HAL_Delay(1);
		printf("PP1=%5.2f\r\n",PP[1]);
		HAL_Delay(4);
		printf("AT+CIPSEND=0,11\r\n");
		HAL_Delay(1);
		printf("PP2=%5.2f\r\n",PP[2]);
		
		sign_PP=0;
	}
	
}
void CS()
{
	if(adcData[1]>=500&&adcData[2]<=300&&sign[0]==0)
	{
		TIM5->CNT=0;
		HAL_TIM_Base_Start(&htim5);
		sign[0]=1;
	}
	else if(adcData[3]>=500&&adcData[4]<=300&&sign[1]==0)
	{
		TIM5->CNT=0;
		HAL_TIM_Base_Start(&htim5);
		sign[1]=1;
	}
	else if(adcData[5]>=500&&adcData[6]<=300&&sign[2]==0)
	{
		TIM5->CNT=0;
		HAL_TIM_Base_Start(&htim5);
		sign[2]=1;
	}
	
	if(adcData[2]>=500||TIM5->CNT>=_1S)
	{
		HAL_TIM_Base_Stop(&htim5);
		VV[0]=10000.0/TIM5->CNT;
		sign[0]=0;
	}
	else if(adcData[4]>=500||TIM5->CNT==_1S)
	{
		HAL_TIM_Base_Stop(&htim5);
		VV[1]=10000.0/TIM5->CNT;
		sign[1]=0;
	}
	else if(adcData[6]>=500||TIM5->CNT==_1S)
	{
		HAL_TIM_Base_Stop(&htim5);
		VV[2]=10000.0/TIM5->CNT;
		sign[2]=0;
	}
	
	if(TIM3->CNT>=1000)
	{
		sprintf(buf,"%.2f",VV[0]);
		lv_label_set_text(V_1_label, buf);
		lv_obj_align(V_1_label, tab2, LV_ALIGN_CENTER, -70, 20);
		
		EE[0]=m_0*VV[0]*VV[0]/2000.0;
		PP[0]=m_0*VV[0]/1000.0;
		
		sprintf(buf,"%2.2f",EE[0]);
		lv_label_set_text(E_1_label, buf);
		lv_obj_align(E_1_label, tab2, LV_ALIGN_CENTER,  0, 20);
		
		sprintf(buf,"%2.2f",PP[0]);
		lv_label_set_text(P_1_label, buf);
		lv_obj_align(P_1_label, tab2, LV_ALIGN_CENTER, 70, 20);
		
		sprintf(buf,"%.2f",VV[1]);
		lv_label_set_text(V_2_label, buf);
		lv_obj_align(V_2_label, tab2, LV_ALIGN_CENTER, -70, 60);
		
		EE[1]=m_0*VV[1]*VV[1]/2000.0;
		PP[1]=m_0*VV[1]/1000.0;
		
		sprintf(buf,"%2.2f",EE[1]);
		lv_label_set_text(E_2_label, buf);
		lv_obj_align(E_2_label, tab2, LV_ALIGN_CENTER,  0, 60);
		
		sprintf(buf,"%2.2f",PP[1]);
		lv_label_set_text(P_2_label, buf);
		lv_obj_align(P_2_label, tab2, LV_ALIGN_CENTER, 70, 60);
		
		sprintf(buf,"%.2f",VV[2]);
		lv_label_set_text(V_3_label, buf);
		lv_obj_align(V_3_label, tab2, LV_ALIGN_CENTER, -70, 100);
		
		EE[2]=m_0*VV[2]*VV[2]/2000.0;
		PP[2]=m_0*VV[2]/1000.0;
		
		sprintf(buf,"%2.2f",EE[2]);
		lv_label_set_text(E_3_label, buf);
		lv_obj_align(E_3_label, tab2, LV_ALIGN_CENTER,  0, 100);
		
		sprintf(buf,"%2.2f",PP[2]);
		lv_label_set_text(P_3_label, buf);
		lv_obj_align(P_3_label, tab2, LV_ALIGN_CENTER, 70, 100);
		
		cdy();
		TIM3->CNT=0;
	}
	Charge();
	fire();
	ESP_Value();
}