#ifndef SD_H
#define SD_H
	
#include "main.h" 
#include "spi.h"
#include "stdio.h"
#include "usart.h"
#include "lv_app.h"
#include "stm32f4xx_it.h"
#define SD_CS_SET HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET)
#define SD_CS_CLR HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET)

#define MMC    0
#define V1     1
#define V2     2
#define V2HC   4

#define LEFT   1
#define RIGHT  2
#define NONE   3

#define CMD0 0
#define CMD1 1
#define CMD8 8		//CID
#define CMD9 9		//CSD
#define CMD10 10
#define CMD12 12
#define CMD16 16
#define CMD17 17
#define CMD18 18
#define CMD24 24
#define CMD25 25
#define CMD55 55
#define CMD58 58
#define ACMD41 41

extern char string_[10];
extern uint8_t SD_Type; //SD卡类型
extern uint8_t sd_init_sign;
extern uint8_t read_end_sign;
extern uint8_t write_end_sign;
extern uint8_t SD_Buffer[512];
void sd_reset();
void SD_Init();
uint8_t SD_ReadBlock(uint8_t *buffer, uint32_t adr);
uint8_t SD_ReadBlock_Multiple(uint8_t *buffer, uint32_t adr, uint16_t len);
uint8_t SD_ReadBlock_Multiple_DMA(uint8_t *buffer, uint32_t adr, uint16_t len);
uint8_t SD_WriteBlock(const uint8_t *buffer, uint32_t adr);
uint8_t SD_WriteBlock_Multiple(const uint8_t *buffer, uint32_t adr, uint16_t len);
void SD_GetCID(uint8_t data[16]);
void SD_GetCSD(uint8_t data[16]);
void read_multiple_dma();
#endif