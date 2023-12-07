#include "sd.h"
uint8_t rcv[4];
uint8_t FF[512];
uint8_t SD_Buffer[512];
uint8_t FE=0xfe,FC=0xfc,FD=0xfd;
uint8_t SD_Type = 5; //SD卡类型
uint8_t sd_init_sign=0;
uint8_t error=0xff;
uint8_t read_end_sign=0,write_end_sign=0;
uint32_t nummber_bak=0xffffffff;
char string_[10];

struct 
{
	uint8_t *buffer;
  uint16_t len;
} sd ;

void SD_SendCMD(uint8_t cmd,uint32_t arg,uint8_t crc,uint8_t back,uint8_t reset)
{
	
	uint8_t data[6]={cmd|0x40,arg>>24,arg>>16,arg>>8,arg,crc};
	HAL_SPI_Transmit(&hspi3,data,6,HAL_MAX_DELAY);
	int i=200;
	if(back==NONE)
	{
		do
		{
			HAL_SPI_TransmitReceive(&hspi3,FF,rcv,1,HAL_MAX_DELAY);
		}while(*rcv==0xff&&i--);
	}
	else
	{
		do
		{
			HAL_SPI_TransmitReceive(&hspi3,FF,rcv,1,HAL_MAX_DELAY);
		}while(*rcv!=back&&i--);
	}
	
	if(back==0xff)	error=0xff;
	else 						error=back+0x01;
	
	if(i==0)	
	{
		sprintf(string_,"%s%x\n","ERROR=",error);
		lv_textarea_add_text(ta1, string_);
	}
	
	if (reset)	//根据要求保持或失能SD卡
	{
			SD_CS_SET;
			HAL_SPI_Transmit(&hspi3,FF,1,HAL_MAX_DELAY); //增加8个时钟确保本次操作完成
	}
}

void sd_reset()
{
	SD_CS_SET;
	for(int i=0;i<10;i++)
	{
		HAL_SPI_Transmit(&hspi3,FF,1,HAL_MAX_DELAY);
	}
	SD_CS_CLR;
	SD_SendCMD(CMD0,0,0x95,0x01,1);
}
//---------------------------------------------------------------------------------------------------------
//读SD卡应答并判断
//response:正确回应值
//成功返回0,失败返回1
uint8_t SD_GetResponse(uint8_t direction,uint8_t dir_num,uint16_t response)
{
		
    uint16_t ii = 5000; //读应答最多5000次
		if(direction==LEFT)
		{
			do
			{
				HAL_SPI_TransmitReceive(&hspi3,FF,rcv,1,HAL_MAX_DELAY);
			}while (*rcv << dir_num!=response && ii--);
		}
		else if(direction==RIGHT)
		{
			do
			{
				HAL_SPI_TransmitReceive(&hspi3,FF,rcv,1,HAL_MAX_DELAY);
			}while (*rcv >> dir_num!=response && ii--);
		}
		else if(direction==NONE)
		{
			do
			{
				HAL_SPI_TransmitReceive(&hspi3,FF,rcv,1,HAL_MAX_DELAY);
			}while (*rcv !=response && ii--);
		}
        
    if (ii == 0)
		{
				sprintf(string_,"%s%x\n","ERROR=",response);
				lv_textarea_add_text(ta1, string_);
        return 1; //返回失败
		}
    else
		{
			return 0; //返回成功
		}      
}
//---------------------------------------------------------------------------------------------------------
uint8_t SD_ReadBlock_Multiple_DMA(uint8_t *buffer, uint32_t adr, uint16_t len)
{
	read_end_sign=0;
	SD_CS_CLR;
	SD_SendCMD(CMD18,adr,0xff,0x00,0);

	if (SD_GetResponse(NONE,0,0xfe)) //等待SD卡发回0xfe
	{
			SD_CS_SET;  //错误退出
			sprintf(string_,"%s\n","ERROR=CMD17");
			lv_textarea_add_text(ta1, string_);
			return 0;
	}
	HAL_SPI_TransmitReceive_DMA(&hspi3,FF,buffer,512);
	sd.len=len;
	sd.buffer=buffer;
  return 1;
}

void read_multiple_dma()
{
	if(read_end_sign==0)
	{
		if((sd.len-1)>0)
		{
			sd.buffer+=512;
			HAL_SPI_Transmit(&hspi3,FF,2,HAL_MAX_DELAY);  //丢弃2个CRC
			if (SD_GetResponse(NONE,0,0xfe)) //等待SD卡发回0xfe
			{
					SD_CS_SET;  //错误退出
					sprintf(string_,"%s\n","ERROR=CMD17");
					lv_textarea_add_text(ta1, string_);
			}
			HAL_SPI_TransmitReceive_DMA(&hspi3,FF,sd.buffer,512);
		}
		else
		{
			SD_SendCMD(CMD12,0,0xff,NONE,1);
			read_end_sign=1;
		}
	}
}
uint8_t SD_ReadBlock_Multiple(uint8_t *buffer, uint32_t adr, uint16_t len)
{
	SD_CS_CLR;
	SD_SendCMD(CMD18,adr,0xff,0x00,0);

	do
	{
		if (SD_GetResponse(NONE,0,0xfe)) //等待SD卡发回0xfe
		{
				SD_CS_SET;  //错误退出
				sprintf(string_,"%s\n","ERROR=CMD17");
				lv_textarea_add_text(ta1, string_);
				return 0;
		}
		HAL_SPI_TransmitReceive(&hspi3,FF,buffer,512,HAL_MAX_DELAY);

		HAL_SPI_Transmit(&hspi3,FF,2,HAL_MAX_DELAY);  //丢弃2个CRC
		buffer+=512;
	}while(--len);
	
	SD_SendCMD(CMD12,0,0xff,NONE,1);

  return 1;
}

uint8_t SD_ReadBlock(uint8_t *buffer, uint32_t adr)
{
	SD_CS_CLR;
	SD_SendCMD(CMD17,adr,0xff,0x00,0);

	if (SD_GetResponse(NONE,0,0xfe)) //等待SD卡发回0xfe
	{
			SD_CS_SET;  //错误退出
			sprintf(string_,"%s\n","ERROR=CMD17");
			lv_textarea_add_text(ta1, string_);
			return 0;
	}
	HAL_SPI_TransmitReceive(&hspi3,FF,buffer,512,HAL_MAX_DELAY);

	HAL_SPI_Transmit(&hspi3,FF,2,HAL_MAX_DELAY);  //丢弃2个CRC

  return 1;
}

//---------------------------------------------------------------------------------------------------------
uint8_t SD_WriteBlock(const uint8_t *buffer, uint32_t adr)
{
	SD_CS_CLR;
	SD_SendCMD(CMD24,adr,0xff,0x00,0);

	HAL_SPI_Transmit(&hspi3,&FE,1,HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi3,(uint8_t *)buffer,512,HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi3,FF,2,HAL_MAX_DELAY);
	
	if (SD_GetResponse(LEFT,3,40)) 
	{
			SD_CS_SET;  //错误退出
			return 0;
	}
	if (SD_GetResponse(NONE,0,0xff)) 
	{
			SD_CS_SET;  //错误退出
			return 0;
	}
  return 1;
}

uint8_t SD_WriteBlock_Multiple(const uint8_t *buffer, uint32_t adr, uint16_t len)
{
	SD_CS_CLR;
	SD_SendCMD(CMD25,adr,0xff,0x00,0);

	do
	{
		HAL_SPI_Transmit(&hspi3,&FC,1,HAL_MAX_DELAY);
		HAL_SPI_Transmit(&hspi3,(uint8_t *)buffer,512,HAL_MAX_DELAY);
		HAL_SPI_Transmit(&hspi3,FF,2,HAL_MAX_DELAY);
		
		if (SD_GetResponse(LEFT,3,40)) 
    {
        SD_CS_SET;  //错误退出
        return 0;
    }
		if (SD_GetResponse(NONE,0,0xff)) 
    {
        SD_CS_SET;  //错误退出
        return 0;
    }
		buffer+=512;
	}while(--len);
	
	HAL_SPI_Transmit(&hspi3,&FD,1,HAL_MAX_DELAY);
	if (SD_GetResponse(NONE,0,0xff)) 
	{
			SD_CS_SET;  //错误退出
			return 0;
	}
  return 1;
}
//---------------------------------------------------------------------------------------------------------
//获取逻辑0扇区的物理扇区号
void SD_GetLogic0(void)
{
    uint32_t tmp = 0;

    SD_ReadBlock_Multiple(SD_Buffer, 0, 1);
    tmp += SD_Buffer[0x1c6];
    tmp += SD_Buffer[0x1c6 + 1] << 8;
    tmp += SD_Buffer[0x1c6 + 2] << 16;
    tmp += SD_Buffer[0x1c6 + 3] << 24;
		
		nummber_bak=tmp;
}
//---------------------------------------------------------------------------------------------------------
//获取CID
void SD_GetCID(uint8_t data[16])
{
	SD_CS_CLR;
	SD_SendCMD(CMD10,0,0xff,0x00,0);
	SD_GetResponse(NONE,0,0xfe);
	HAL_SPI_TransmitReceive(&hspi3,FF,data,16,HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi3,FF,2,HAL_MAX_DELAY);
	SD_CS_SET;
	HAL_SPI_Transmit(&hspi3,FF,1,HAL_MAX_DELAY);
}
//---------------------------------------------------------------------------------------------------------
//获取CSD
void SD_GetCSD(uint8_t data[16])
{
	SD_CS_CLR;
	SD_SendCMD(CMD9,0,0xff,0x00,0);
	SD_GetResponse(NONE,0,0xfe);
	HAL_SPI_TransmitReceive(&hspi3,FF,data,16,HAL_MAX_DELAY);
	HAL_SPI_Transmit(&hspi3,FF,2,HAL_MAX_DELAY);
	SD_CS_SET;
	HAL_SPI_Transmit(&hspi3,FF,1,HAL_MAX_DELAY);
}
void SD_Init()
{
	memset(FF,0xff,1024);
	sd_reset();
	
	//获取卡的版本信息
	SD_CS_CLR;
	SD_SendCMD(CMD8, 0x1aa, 0x87, NONE,0);
	if(*rcv==0x01)
	{
		HAL_SPI_Transmit(&hspi3,FF,4,HAL_MAX_DELAY);

		SD_CS_SET;

		HAL_SPI_TransmitReceive(&hspi3,FF,rcv,1,HAL_MAX_DELAY); //增加8个时钟确保本次操作完成

		int i = 200;
		
		do
		{
				SD_CS_CLR;
				SD_SendCMD(CMD55, 0, 0xff, 0x01,1);
				SD_CS_CLR;
				SD_SendCMD(ACMD41, 0x40000000, 0xff, NONE,1);
		} while (*rcv != 0&&i--);

		if(i==0)
		{
		sprintf(string_,"%s\n","ERROR=ACMD41");
		lv_textarea_add_text(ta1, string_);
		}
		
		SD_CS_CLR;
		SD_SendCMD(CMD58, 0, 0xff, 0x00,0);
		
		//接收OCR信息
		HAL_SPI_TransmitReceive(&hspi3,FF,rcv,4,HAL_MAX_DELAY);

		SD_CS_SET;
		HAL_SPI_Transmit(&hspi3,FF,1,HAL_MAX_DELAY); //增加8个时钟确保本次操作完成

		if (rcv[0] & 0x40)
				SD_Type = V2HC;
		else
				SD_Type = V2;
		
		HAL_UART_Transmit(&huart1, FF, 4,HAL_MAX_DELAY);
		
	}
		if(SD_Type<5)	sd_init_sign=1;

		MX_SPI3_High_Init();
		
//		SD_ReadBlock_Multiple_DMA(SD_Buffer, 0X801, 2);
//		while(read_end_sign==0)
//		{
//			DMA1_Stream2_IRQHandler();
//		}
//		uint8_t NUM[1024];
//		memset(NUM,0x06,1024);
//		SD_WriteBlock_Multiple(NUM, 0X801, 1);
//		SD_ReadBlock_Multiple_DMA(SD_Buffer, 0X801, 1);
//		
//		while(read_end_sign==0)
//		{
//			DMA1_Stream2_IRQHandler();
//		}
//		SD_GetLogic0();
//		sprintf(string_,"bank number=%x\n",nummber_bak);
//		lv_textarea_add_text(ta1, string_);
//		
//		SD_ReadBlock_Multiple(SD_Buffer,nummber_bak+3,2);
//		for(int i=0;i<5;i++)
//		{
//			sprintf(string_,"num%03d=%02x\n",i,SD_Buffer[i]);
//			lv_textarea_add_text(ta1, string_);
//		}
//		for(int i=512;i<512+5;i++)
//		{
//			sprintf(string_,"num%03d=%02x\n",i,SD_Buffer[i]);
//			lv_textarea_add_text(ta1, string_);
//		}
//		
//		
//		uint8_t NUM[1024];
//		memset(NUM,0x05,1024);
//		SD_WriteBlock_Multiple(NUM, nummber_bak+3, 2);
//		
//		SD_ReadBlock_Multiple(SD_Buffer,nummber_bak+3,2);
//		for(int i=0;i<5;i++)
//		{
//			sprintf(string_,"num%03d=%02x\n",i,SD_Buffer[i]);
//			lv_textarea_add_text(ta1, string_);
//		}
//		for(int i=512;i<512+5;i++)
//		{
//			sprintf(string_,"num%03d=%02x\n",i,SD_Buffer[i]);
//			lv_textarea_add_text(ta1, string_);
//		}
}

