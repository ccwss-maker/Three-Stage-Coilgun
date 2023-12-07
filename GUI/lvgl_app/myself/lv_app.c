#include "lv_app.h"

lv_obj_t 	* scr;
lv_obj_t 	* tabview;
lv_obj_t 	* tab1, *tab2,*tab3,*tab4;
lv_obj_t 	* Data;
lv_obj_t 	* V_1_label,* V_2_label,* V_3_label,
					* E_1_label,* E_2_label,* E_3_label,
					* P_1_label,* P_2_label,* P_3_label,
					* VV_1_label,* VV_2_label,* VV_3_label;
lv_obj_t 	* ta1;
lv_obj_t 	* btnm1;
lv_obj_t 	* bg_img;
static lv_obj_t 	* slider_KC1_label,* slider_KC2_label,* slider_KC3_label;
static lv_obj_t 	* line3,* line4,* line5;
static uint8_t memory[512];
///////////////////////////////Printf重定义/////////////////////////////
#if (defined(__GNUC__) && !defined(__CC_ARM))
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop until the end of transmission */
  while (HAL_OK != HAL_UART_Transmit_DMA(&huart1, (uint8_t *) &ch, 1))
  {
    ;
  }
  return ch;
}
///////////////////////////////////////////////////////////////////////////

static void slider_event_handler(lv_obj_t * obj, lv_event_t event)
{									
	if(event == LV_EVENT_VALUE_CHANGED) 
	{
			int num=lv_slider_get_value(obj);
			LCD_LED=num;
	}
	else if(event == LV_EVENT_RELEASED) 
	{
			int num=lv_slider_get_value(obj);
			memory[0]=num;
			SD_WriteBlock(memory, 0x01);
	}
}

static void arc_event_handler(lv_obj_t * obj, lv_event_t event)
{
	static int angle = 100;											
	if(event == LV_EVENT_VALUE_CHANGED) 
	{				// 角度改变事件，手触摸/拖动进度条
			angle = (lv_arc_get_angle_end(obj)-180)*5/9;					// 获取事件对象改变的角度
			LCD_LED=angle;
			static char buf[8];
			lv_snprintf(buf, sizeof(buf), "%d", angle);		// 将值变为字符串
			lv_obj_t * label = lv_obj_get_child(obj, NULL);		// 获取事件对象的标签子对象
			lv_label_set_text(label, buf);						// 设置标签文本
			lv_obj_align(label, obj, LV_ALIGN_CENTER, 0, 0);	// 标签文件有改变要重新设置对齐
	}
}

static void btn_event_handler_1(lv_obj_t * obj, lv_event_t event)
{
	static int num=0;
	if(event == LV_EVENT_PRESSED) 
	{
		if(num==0)
		{
			sign_C[0]=1;
			lv_btn_toggle(obj);
			num=1;
		}
	}
	else if(event == LV_EVENT_RELEASED||event ==LV_EVENT_PRESS_LOST) 
  {
		if(num==1)
		{
			sign_C[0]=0;
			lv_btn_toggle(obj);
			num=0;
		}
  }
}

static void btn_event_handler_2(lv_obj_t * obj, lv_event_t event)
{
	static int num=0;
	if(event == LV_EVENT_PRESSING) 
	{
		if(num==0)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
			lv_btn_toggle(obj);
			num=1;
		}
	}
  else if(event == LV_EVENT_RELEASED||event ==LV_EVENT_PRESS_LOST) 
  {
    
		if(num==1)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
			lv_btn_toggle(obj);
			num=0;
		}
  }
}

static void btn_event_handler_3(lv_obj_t * obj, lv_event_t event)
{
	static int num=0;
	if(event == LV_EVENT_PRESSING) 
	{
		if(num==0)
		{
			sign_C[1]=1;
			lv_btn_toggle(obj);
			num=1;
		}
	}
  else if(event == LV_EVENT_RELEASED||event ==LV_EVENT_PRESS_LOST) 
  {
		if(num==1)
		{
			sign_C[1]=0;
			lv_btn_toggle(obj);
			num=0;
		}
  }
}

static void btn_event_handler_4(lv_obj_t * obj, lv_event_t event)
{
	static int num=0;
	if(event == LV_EVENT_PRESSING) 
	{
		if(num==0)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);
			lv_btn_toggle(obj);
			num=1;
		}
	}
  else if(event == LV_EVENT_RELEASED||event ==LV_EVENT_PRESS_LOST) 
  {
		if(num==1)
		{
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_SET);
			lv_btn_toggle(obj);
			num=0;
		}
  }
}

static void btn_event_handler_5(lv_obj_t * obj, lv_event_t event)
{
	static int num=0;
	if(event == LV_EVENT_PRESSING) 
	{
		if(num==0)
		{
			sign_C[2]=1;
			lv_btn_toggle(obj);
			num=1;
		}
	}
  else if(event == LV_EVENT_RELEASED||event ==LV_EVENT_PRESS_LOST) 
  {
		if(num==1)
		{
			sign_C[2]=0;
			lv_btn_toggle(obj);
			num=0;
		}
  }
}

static void btn_event_handler_6(lv_obj_t * obj, lv_event_t event)
{
	static int num=0;
	if(event == LV_EVENT_PRESSING) 
	{
		if(num==0)
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_RESET);
			lv_btn_toggle(obj);
			num=1;
		}
	}
  else if(event == LV_EVENT_RELEASED||event ==LV_EVENT_PRESS_LOST) 
  {
		if(num==1)
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,GPIO_PIN_SET);
			lv_btn_toggle(obj);
			num=0;
		}
  }
}

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
	static uint8_t sign[6]={0};
	uint16_t num=lv_btnmatrix_get_active_btn(obj);
	if(event == LV_EVENT_PRESSING) 
	{
		switch(num)
		{
			case 0:	{
								if(sign[0]==0)	
									{
										printf("AT+CIPSEND=0,3\r\n");
										sign[0]=1;
									}
							}
							break;
			case 1:	{
								if(sign[1]==0)	
								{
									printf("AT+\r\n");
									sign[1]=1;
								}
							}
							break;
			case 2:	{
								if(sign[2]==0)	
								{
									printf("AT+CIPSEND=0,5\r\n");
									sign[2]=1;
								}
							}
							break;
			case 3:	{
								if(sign[3]==0)	
								{
									printf("HELLO\r\n");
									sign[3]=1;
								}
							}
							break;
			case 4:	{
								if(sign[4]==0)	ESP_sign=1;
								sign[4]=1;
							}
							break;
			case 5:	{
								if(sign[5]==0)	lv_textarea_set_text(ta1, "");
								sign[5]=1;
							}
							break;
		}
	}
	else if(event == LV_EVENT_RELEASED) 
	{
		switch(num)
		{
			case 0:	sign[0]=0;
							break;
			case 1:	sign[1]=0;
							break;
			case 2:	sign[2]=0;
							break;
			case 3:	sign[3]=0;
							break;
			case 4:	sign[4]=0;
							break;
			case 5:	sign[5]=0;
							break;
		}
	}
}

static void sw_event_handler_1(lv_obj_t * obj, lv_event_t event)
{
	if(event == LV_EVENT_VALUE_CHANGED)
	{
		if(lv_switch_get_state(obj)==true)
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
			ESP_sign=1;
			test_sign=1;
			memory[1]=0xff;
			SD_WriteBlock(memory, 0x01);
		}
		else if(lv_switch_get_state(obj)==false)
		{
			HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);
			test_sign=0;
			memory[1]=0x00;
			SD_WriteBlock(memory, 0x01);
		}
	}
}

static void slider_KC1_event_handler(lv_obj_t * obj, lv_event_t event)
{
	if(event == LV_EVENT_VALUE_CHANGED) 
	{
			int num=lv_slider_get_value(obj);
			char string[20];
			C_MAX[0]=num;
			sprintf(string,"MAX[C1]=%d",num);
			lv_label_set_text(slider_KC1_label, string);
			lv_obj_align(slider_KC1_label, line3, LV_ALIGN_CENTER, 0, 15);
	}
	else if(event == LV_EVENT_RELEASED) 
	{
			uint16_t num=lv_slider_get_value(obj);
			memory[2]=num>>8;
			memory[3]=num;
			SD_WriteBlock(memory, 0x01);
	}
}

static void slider_KC2_event_handler(lv_obj_t * obj, lv_event_t event)
{
	if(event == LV_EVENT_VALUE_CHANGED) 
	{
			int num=lv_slider_get_value(obj);
			char string[20];
			C_MAX[1]=num;
			sprintf(string,"MAX[C2]=%d",num);
			lv_label_set_text(slider_KC2_label, string);
			lv_obj_align(slider_KC2_label, line4, LV_ALIGN_CENTER, 0, 15);
	}
	else if(event == LV_EVENT_RELEASED) 
	{
			uint16_t num=lv_slider_get_value(obj);
			memory[4]=num>>8;
			memory[5]=num;
			SD_WriteBlock(memory, 0x01);
	}
}

static void slider_KC3_event_handler(lv_obj_t * obj, lv_event_t event)
{
	if(event == LV_EVENT_VALUE_CHANGED) 
	{
			int num=lv_slider_get_value(obj);
		  char string[20];
			C_MAX[2]=num;
			sprintf(string,"MAX[C3]=%d",num);
			lv_label_set_text(slider_KC3_label, string);
			lv_obj_align(slider_KC3_label, line5, LV_ALIGN_CENTER, 0, 15);
	}
	else if(event == LV_EVENT_RELEASED) 
	{
			uint16_t num=lv_slider_get_value(obj);
			memory[6]=num>>8;
			memory[7]=num;
			SD_WriteBlock(memory, 0x01);
	}	
}
void tab1_init()
{
	tab1 = lv_tabview_add_tab(tabview, "Fire");
//////////////////  按钮  //////////////////
	
	lv_obj_t * btn_1=lv_btn_create(tab1,NULL);
	lv_obj_set_event_cb(btn_1, btn_event_handler_1);
  lv_obj_align(btn_1, tab1,LV_ALIGN_CENTER, -60,-90);
	lv_obj_t * btn1_label = lv_label_create(btn_1, NULL);		
	lv_label_set_text(btn1_label, "KC_1");
	lv_obj_align(btn1_label, btn_1, LV_ALIGN_CENTER, 0, 0);	
	
	lv_obj_t * btn_2=lv_btn_create(tab1,NULL);
	lv_obj_set_event_cb(btn_2, btn_event_handler_2);
  lv_obj_align(btn_2, tab1,LV_ALIGN_CENTER, 60,-90);
	lv_obj_t * btn2_label = lv_label_create(btn_2, NULL);
	lv_obj_align(btn2_label, btn_2, LV_ALIGN_CENTER, 0, 0);	
	lv_label_set_text(btn2_label, "KF_1");
	
	lv_obj_t * btn_3=lv_btn_create(tab1,NULL);
	lv_obj_set_event_cb(btn_3, btn_event_handler_3);
  lv_obj_align(btn_3, tab1,LV_ALIGN_CENTER, -60,-15);
	lv_obj_t * btn3_label = lv_label_create(btn_3, NULL);
	lv_obj_align(btn3_label, btn_3, LV_ALIGN_CENTER, 0, 0);	
	lv_label_set_text(btn3_label, "KC_2");
	
	lv_obj_t * btn_4=lv_btn_create(tab1,NULL);
	lv_obj_set_event_cb(btn_4, btn_event_handler_4);
  lv_obj_align(btn_4, tab1,LV_ALIGN_CENTER, 60,-15);
	lv_obj_t * btn4_label = lv_label_create(btn_4, NULL);	
	lv_obj_align(btn4_label, btn_4, LV_ALIGN_CENTER, 0, 0);	
	lv_label_set_text(btn4_label, "KF_2");
	
	lv_obj_t * btn_5=lv_btn_create(tab1,NULL);
	lv_obj_set_event_cb(btn_5, btn_event_handler_5);
  lv_obj_align(btn_5, tab1,LV_ALIGN_CENTER, -60,60);
	lv_obj_t * btn5_label = lv_label_create(btn_5, NULL);	
	lv_obj_align(btn5_label, btn_5, LV_ALIGN_CENTER, 0, 0);	
	lv_label_set_text(btn5_label, "KC_3");
	
	lv_obj_t * btn_6=lv_btn_create(tab1,NULL);
	lv_obj_set_event_cb(btn_6, btn_event_handler_6);
  lv_obj_align(btn_6, tab1,LV_ALIGN_CENTER, 60,60);
	lv_obj_t * btn6_label = lv_label_create(btn_6, NULL);	
	lv_obj_align(btn6_label, btn_6, LV_ALIGN_CENTER, 0, 0);	
	lv_label_set_text(btn6_label, "KF_3");

////////////////  文字  //////////////////
	static lv_style_t label_style;									// 创建一个风格
	lv_style_init(&label_style);									// 初始化风格
	lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &Hua_Wen_Fang_Song_In);// 设置风格的字体
	
	VV_1_label = lv_label_create(tab1, NULL);	
	lv_label_set_text(VV_1_label, "V1=0V");
	lv_obj_add_style(VV_1_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(VV_1_label, tab1, LV_ALIGN_CENTER, 0, -52);	
	
	VV_2_label = lv_label_create(tab1, NULL);	
	lv_label_set_text(VV_2_label, "V2=0V");
	lv_obj_add_style(VV_2_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(VV_2_label, tab1, LV_ALIGN_CENTER, 0, 23);
	
	VV_3_label = lv_label_create(tab1, NULL);	
	lv_label_set_text(VV_3_label, "V3=0V");
	lv_obj_add_style(VV_3_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(VV_3_label, tab1, LV_ALIGN_CENTER, 0, 98);
	
}

void tab2_init()
{
	
	tab2 = lv_tabview_add_tab(tabview, "Data");
//////////////////  矩阵  //////////////////	
	
//	Data = lv_label_create(tab2, NULL);	
//	lv_obj_align(Data, tab2, LV_ALIGN_CENTER, 0, -70);	
//	lv_label_set_text(Data, "F");
	
//	static const char * btnm_map[] = {"AT", "AT", "HELLO", "\n", 
//		"HELLO" , "TCP" , "CLR" , ""};
//	
//	btnm1 = lv_btnmatrix_create(tab2, NULL);
//	lv_btnmatrix_set_map(btnm1, btnm_map);
//	lv_obj_align(btnm1, tab2, LV_ALIGN_CENTER, 0, 70);
//	lv_obj_set_event_cb(btnm1, event_handler);
		
//////////////////  ARC  //////////////////
	
//	lv_obj_t * arc1=lv_arc_create(tab3,NULL);
//	lv_arc_set_type(arc1,LV_ARC_TYPE_NORMAL);
//	lv_arc_set_bg_angles(arc1,180,360);
//	lv_arc_set_angles(arc1,180,360);
//	lv_obj_set_size(arc1, 150, 150);
//	lv_obj_align(arc1, NULL, LV_ALIGN_CENTER, 0, -70);
//	lv_arc_set_adjustable(arc1,true);
//	lv_obj_t * arc1_label = lv_label_create(arc1, NULL);	// 在Arc1控件上创建一个标签
//	lv_obj_align(arc1_label, arc1, LV_ALIGN_CENTER, 0, 0);	// 标签对齐到Arc1控件中心
//	lv_label_set_text(arc1_label, "100");
//	lv_obj_set_event_cb(arc1, arc_event_handler);

////////////////  文字  //////////////////

	static lv_style_t label_style;									// 创建一个风格
	lv_style_init(&label_style);									// 初始化风格
	lv_style_set_text_font(&label_style, LV_STATE_DEFAULT, &Hua_Wen_Fang_Song_In);// 设置风格的字体
	
	lv_obj_t *V_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(V_label, "V");
	lv_obj_add_style(V_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(V_label, tab2, LV_ALIGN_CENTER, -70, -5);	
	
	lv_obj_t *E_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(E_label, "E");
	lv_obj_add_style(E_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(E_label, tab2, LV_ALIGN_CENTER, 0, -5);	
	
	lv_obj_t *P_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(P_label, "P");
	lv_obj_add_style(P_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(P_label, tab2, LV_ALIGN_CENTER, 70, -5);	
	
	lv_obj_t *_1_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(_1_label, "1");
	lv_obj_add_style(_1_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(_1_label, tab2, LV_ALIGN_CENTER, -105, 20);	
	
	lv_obj_t *_2_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(_2_label, "2");
	lv_obj_add_style(_2_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(_2_label, tab2, LV_ALIGN_CENTER, -105, 60);	
	
	lv_obj_t *_3_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(_3_label, "3");
	lv_obj_add_style(_3_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(_3_label, tab2, LV_ALIGN_CENTER, -105, 100);	
	
	V_1_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(V_1_label, "00.00");
	lv_obj_add_style(V_1_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(V_1_label, tab2, LV_ALIGN_CENTER, -70, 20);	
	
	E_1_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(E_1_label, "0.00");
	lv_obj_add_style(E_1_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(E_1_label, tab2, LV_ALIGN_CENTER,  0, 20);
	
	P_1_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(P_1_label, "0.00");
	lv_obj_add_style(P_1_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(P_1_label, tab2, LV_ALIGN_CENTER, 70, 20);
	
	V_2_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(V_2_label, "00.00");
	lv_obj_add_style(V_2_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(V_2_label, tab2, LV_ALIGN_CENTER, -70, 60);
	
	E_2_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(E_2_label, "0.00");
	lv_obj_add_style(E_2_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(E_2_label, tab2, LV_ALIGN_CENTER,  0, 60);
	
	P_2_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(P_2_label, "0.00");
	lv_obj_add_style(P_2_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(P_2_label, tab2, LV_ALIGN_CENTER, 70, 60);
	
	V_3_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(V_3_label, "00.00");
	lv_obj_add_style(V_3_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(V_3_label, tab2, LV_ALIGN_CENTER, -70, 100);
	
	E_3_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(E_3_label, "0.00");
	lv_obj_add_style(E_3_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(E_3_label, tab2, LV_ALIGN_CENTER,  0, 100);
	
	P_3_label = lv_label_create(tab2, NULL);	
	lv_label_set_text(P_3_label, "0.00");
	lv_obj_add_style(P_3_label, LV_LABEL_PART_MAIN, &label_style);
	lv_obj_align(P_3_label, tab2, LV_ALIGN_CENTER, 70, 100);

//////////////////  文本框  //////////////////

	ta1 = lv_textarea_create(tab2, NULL);
	lv_obj_set_size(ta1, 200, 100);
	lv_textarea_set_cursor_hidden(ta1, true);
	lv_obj_align(ta1, NULL, LV_ALIGN_CENTER, 0, -70);
	lv_textarea_set_text(ta1, "");
}

void tab3_init()
{
	tab3 = lv_tabview_add_tab(tabview, "Settings");
	//////////////////  线条  //////////////////	
	
	static lv_point_t line_points[] = { {10, 0}, {230, 0}};
	
	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, LV_STATE_DEFAULT, 1);
	lv_style_set_line_color(&style_line, LV_STATE_DEFAULT, LV_COLOR_GRAY);
	lv_style_set_line_rounded(&style_line, LV_STATE_DEFAULT, true);
	
	lv_obj_t * line1;
	line1 = lv_line_create(tab3, NULL);
	lv_line_set_points(line1, line_points, 2 );     /*Set the points*/
	lv_obj_add_style(line1, LV_LINE_PART_MAIN, &style_line);     /*Set the points*/
	lv_obj_align(line1, tab3, LV_ALIGN_CENTER, 0,-130);
	
	lv_obj_t * line2;
	line2 = lv_line_create(tab3, line1);
	lv_obj_align(line2, tab3, LV_ALIGN_CENTER, 0,-70);
	
	
	line3 = lv_line_create(tab3, line1);
	lv_obj_align(line3, tab3, LV_ALIGN_CENTER, 0,-10);
	
	line4 = lv_line_create(tab3, line1);
	lv_obj_align(line4, tab3, LV_ALIGN_CENTER, 0,50);
	
	line5 = lv_line_create(tab3, line1);
	lv_obj_align(line5, tab3, LV_ALIGN_CENTER, 0,110);
	
	lv_obj_t * line6;
	line6 = lv_line_create(tab3, line1);
	lv_obj_align(line6, tab3, LV_ALIGN_CENTER, 0,170);
	
//////////////////  滑杆  //////////////////

	lv_obj_t * slider = lv_slider_create(tab3, NULL);
	lv_obj_set_width(slider, 200);
	lv_slider_set_range(slider, 5, 100);
	lv_slider_set_value(slider,memory[0],LV_ANIM_OFF);
	lv_obj_set_event_cb(slider, slider_event_handler);
	lv_obj_align(slider, line1, LV_ALIGN_CENTER, 0, 40);
	
	lv_obj_t * slider_label = lv_label_create(tab3, NULL);	
	lv_label_set_text(slider_label, "Light");
	lv_obj_align(slider_label, line1, LV_ALIGN_CENTER, 0, 15);	
	
	lv_obj_t * slider_KC1 = lv_slider_create(tab3, NULL);
	lv_obj_set_width(slider_KC1, 200);
	lv_slider_set_range(slider_KC1, 30, 400);
	uint16_t MAX_C1=0;
	MAX_C1|=memory[2]<<8;
	MAX_C1|=memory[3];
	C_MAX[0]=MAX_C1;
	lv_slider_set_value(slider_KC1,MAX_C1,LV_ANIM_OFF);
	lv_obj_set_event_cb(slider_KC1, slider_KC1_event_handler);
	lv_obj_align(slider_KC1, line3, LV_ALIGN_CENTER, 0, 40);
	
	static char string[20];
	slider_KC1_label = lv_label_create(tab3, NULL);	
	sprintf(string,"MAX[C1]=%d",MAX_C1);
	lv_label_set_text(slider_KC1_label, string);
	lv_obj_align(slider_KC1_label, line3, LV_ALIGN_CENTER, 0, 15);	
	
	lv_obj_t * slider_KC2 = lv_slider_create(tab3, NULL);
	lv_obj_set_width(slider_KC2, 200);
	lv_slider_set_range(slider_KC2, 30, 400);
	uint16_t MAX_C2=0;
	MAX_C2|=memory[4]<<8;
	MAX_C2|=memory[5];
	C_MAX[1]=MAX_C2;
	lv_slider_set_value(slider_KC2,MAX_C2,LV_ANIM_OFF);
	lv_obj_set_event_cb(slider_KC2, slider_KC2_event_handler);
	lv_obj_align(slider_KC2, line4, LV_ALIGN_CENTER, 0, 40);
	
	slider_KC2_label = lv_label_create(tab3, NULL);	
	sprintf(string,"MAX[C2]=%d",MAX_C2);
	lv_label_set_text(slider_KC2_label, string);
	lv_obj_align(slider_KC2_label, line4, LV_ALIGN_CENTER, 0, 15);	
	
	lv_obj_t * slider_KC3 = lv_slider_create(tab3, NULL);
	lv_obj_set_width(slider_KC3, 200);
	lv_slider_set_range(slider_KC3, 30, 400);
	uint16_t MAX_C3=0;
	MAX_C3|=memory[6]<<8;
	MAX_C3|=memory[7];
	C_MAX[2]=MAX_C3;
	lv_slider_set_value(slider_KC3,MAX_C3,LV_ANIM_OFF);
	lv_obj_set_event_cb(slider_KC3, slider_KC3_event_handler);
	lv_obj_align(slider_KC3, line5, LV_ALIGN_CENTER, 0, 40);
	
	slider_KC3_label = lv_label_create(tab3, NULL);	
	sprintf(string,"MAX[C3]=%d",MAX_C3);
	lv_label_set_text(slider_KC3_label, string);
	lv_obj_align(slider_KC3_label, line5, LV_ALIGN_CENTER, 0, 15);	

//////////////////  开关  //////////////////	

	lv_obj_t *sw1 = lv_switch_create(tab3, NULL);
	lv_obj_set_event_cb(sw1,sw_event_handler_1);
	lv_obj_align(sw1, line2, LV_ALIGN_CENTER, 0, 40);
	if(memory[1]==0xff)
	{
		lv_switch_on(sw1,LV_ANIM_OFF);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET);
		ESP_sign=1;
		test_sign=1;
	}
	lv_obj_t * sw1_label = lv_label_create(tab3, NULL);	
	lv_label_set_text(sw1_label, "WLAN");
	lv_obj_align(sw1_label, line2, LV_ALIGN_CENTER, 0, 15);
}
	
void tab4_init()
{
	tab4 = lv_tabview_add_tab(tabview, "Picture");
	bg_img = lv_img_create(tab4, NULL);
	lv_img_set_src(bg_img, "0:/Picture/QR.bin");
	lv_obj_align(bg_img,tab4,LV_ALIGN_CENTER,0,0);

}
void lv_demo_myself()
{
	scr = lv_disp_get_scr_act(NULL);
	tabview = lv_tabview_create(scr, NULL);
	SD_ReadBlock(memory, 0x01);
	tab1_init();
	tab2_init();
	tab3_init();
	tab4_init();
}



















