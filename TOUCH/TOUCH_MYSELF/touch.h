#ifndef _TOUCH_H_  
#define _TOUCH_H_ 
	
#include "main.h"

	void send_16way(uint16_t data);
	uint16_t receiv_16way(void);
	int cmp(const void *a, const void *b);
	void XPT2046_Rd_Addata(void);
	void IQR(void);
	uint8_t TOUCH_ReadXY(uint16_t *xValue, uint16_t *yValue);
	void TOUCH_TEST(void);
#endif
