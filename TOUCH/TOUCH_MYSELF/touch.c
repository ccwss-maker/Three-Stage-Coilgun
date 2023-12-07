#include "touch.h"
#include "main.h"
#include "spi.h"
#include "stdlib.h"
#include "lv_port_disp.h"

#define TOUCH_MAX 3
#define TOUCH_X_MIN 240
#define TOUCH_X_MAX 3860
#define TOUCH_Y_MIN 130
#define TOUCH_Y_MAX 3690
#define X_CMD 0Xd3
#define Y_CMD 0X93
uint16_t t_x;
uint16_t t_y;
uint16_t t_x_;
uint16_t t_y_;

int sym=0;
static const uint8_t count = 3; //采集次数
uint16_t x_data[count], y_data[count];

void send_16way(uint16_t data)
{
    HAL_SPI_Transmit(&hspi2, (uint8_t*)&data, 1,HAL_MAX_DELAY); //不配置硬件片选（NSS）功能，手动片选
}
 
uint16_t receiv_16way(void)
{
    uint16_t data;
    HAL_SPI_Receive(&hspi2, (uint8_t*)&data, 1,HAL_MAX_DELAY);
    return data;
}


static uint16_t TOUCH_ReadData(uint8_t cmd)
{
	uint8_t i, j;
	uint16_t readValue[count], value;
	uint32_t totalValue;
		
	/* 读取 TOUCH_READ_TIMES 次触摸值 */
	for(i=0; i<count; i++)
	{              
		/* 打开片选 */ 
		 HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET); //片选
		/* 在差分模式下，XPT2046 转换需要 24 个时钟，8 个时钟输入命令，之后 1个时钟去除 */
		/* 忙信号，接着输出 12 位转换结果，剩下 3 个时钟是忽略位 */
		send_16way(cmd); // 发送命令，选择 X 轴或者 Y 轴
		/* 读取数据 */
		readValue[i] = receiv_16way();
		/* 将数据处理，读取到的 AD 值的只有 12 位，最低三位无用 */
		readValue[i] >>= 3; 
		send_16way(0x80);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET); //片选
	}
	
	for(i=0; i<(count - 1); i++)
	{
		for(j=i+1; j<count; j++)
		{
			/* 采样值从大到小排序排序 */
			if(readValue[i] < readValue[j])
			{
			value = readValue[i]; readValue[i] = readValue[j]; readValue[j] = value;
			}
		}
	}
		
	/* 去掉最大值，去掉最小值，求平均值 */
	j = count - 1;
	totalValue = 0;
	for(i=1; i<j; i++)/*="" 求="" y="" 的全部值*/
	{
		totalValue += readValue[i];
	}
	value = totalValue / (count - 2);
		
	return value;
}

uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue)
{
	uint16_t xValue1, yValue1, xValue2, yValue2;
	xValue1 = TOUCH_ReadData(X_CMD); 
	yValue1 = TOUCH_ReadData(Y_CMD); 
	xValue2 = TOUCH_ReadData(X_CMD); 
	yValue2 = TOUCH_ReadData(Y_CMD);

	/* 查看两个点之间的只采样值差距 */

	*xValue = abs(xValue1 - xValue2);
	*yValue = abs(yValue1 - yValue2);

	/* 判断采样差值是否在可控范围内 */
	if((*xValue > TOUCH_MAX) || (*yValue > TOUCH_MAX))
	{
		return false;
	}

	/* 求平均值 */
	*xValue = (xValue1 + xValue2) / 2;
	*yValue = (yValue1 + yValue2) / 2;

	/* 判断得到的值，是否在取值范围之内 */
	if((*xValue > TOUCH_X_MAX) || (*xValue < TOUCH_X_MIN)
	|| (*yValue > TOUCH_Y_MAX) || (*yValue < TOUCH_Y_MIN))
	{
		return false;
	}
	return true;
}

void IQR()
{
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)==GPIO_PIN_RESET)
	{
		sym=1;
		if(TOUCH_ReadXY(&t_x_,&t_y_))
		{
			t_x=(int)(0.0664*(4096-t_x_)-15.86+0.5);
			t_y=(int)(0.09*t_y_-11.7+0.5);
		}
	}
	else 
	{
		t_x=0;
		t_y=0;
		sym=0;
	}
}

void TOUCH_TEST()
{
	static lv_point_t line_points_0[2]={{0,0},{40,40}};
	static lv_point_t line_points_1[2]={{0,40},{40,0}};
	
	static lv_point_t line_points_2[2]={{200,0},{240,40}};
	static lv_point_t line_points_3[2]={{200,40},{240,0}};
	
	static lv_point_t line_points_4[2]={{0,280},{40,320}};
	static lv_point_t line_points_5[2]={{0,320},{40,280}};
	
	static lv_point_t line_points_6[2]={{200,280},{240,320}};
	static lv_point_t line_points_7[2]={{200,320},{240,280}};
	
	static lv_style_t style_line;
	lv_style_init(&style_line);
	lv_style_set_line_width(&style_line, LV_STATE_DEFAULT, 2);
	
	lv_obj_t * line1;
	line1 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line1, line_points_0, 2);     /*Set the points*/
	lv_obj_add_style(line1, LV_LINE_PART_MAIN, &style_line);     /*Set the points*/
	
	lv_obj_t * line2;
	line2 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line2, line_points_1, 2);     /*Set the points*/
	lv_obj_add_style(line2, LV_LINE_PART_MAIN, &style_line);     /*Set the points*/
	
	lv_obj_t * line3;
	line3 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line3, line_points_2, 2);     /*Set the points*/
	lv_obj_add_style(line3, LV_LINE_PART_MAIN, &style_line);     /*Set the points*/
	
	lv_obj_t * line4;
	line4 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line4, line_points_3, 2);     /*Set the points*/
	lv_obj_add_style(line4, LV_LINE_PART_MAIN, &style_line);     /*Set the points*/
	
	lv_obj_t * line5;
	line5 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line5, line_points_4, 2);     /*Set the points*/
	lv_obj_add_style(line5, LV_LINE_PART_MAIN, &style_line);     /*Set the points*/
	
	lv_obj_t * line6;
	line6 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line6, line_points_5, 2);     /*Set the points*/
	lv_obj_add_style(line6, LV_LINE_PART_MAIN, &style_line);     /*Set the points*/
	
	lv_obj_t * line7;
	line7 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line7, line_points_6, 2);     /*Set the points*/
	lv_obj_add_style(line7, LV_LINE_PART_MAIN, &style_line);     /*Set the points*/
	
	lv_obj_t * line8;
	line8 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line8, line_points_7, 2);     /*Set the points*/
	lv_obj_add_style(line8, LV_LINE_PART_MAIN, &style_line);     /*Set the points*/
}