#ifndef _GPIO_H_
#define _GPIO_H_

#include "system_emio.h"


//系统运行指示灯
#define SYS_LED		(54) //B6
#define BUZZER		(55) //H5

//#define GPIO0		(56) //B2
//#define GPIO1		(57) //B7
//#define GPIO2		(58) //A5
//#define GPIO3		(59) //C6
//#define GPIO4		(60) //C5
//#define GPIO5		(61) //H3
//#define GPIO6		(62) //F4
//#define GPIO7		(63) //E5
//#define GPIO8		(64) //C3
//#define GPIO9		(65) //D3


#define ADS_RST		(0)	 //MIO0


typedef enum {
	GPIO_OUT = 1,
	GPIO_IN  = 0,
}IO_Dir;


////////////////////////////////////////////////////////////////////////////////

//初始化其它引脚
s32 Gpio_Init(void);

//设置方向
void Gpio_Set_Dir(u8 pin, u8 dir);

void Set_pin_value(u8 pin, u8 value);

u8 Get_pin_value(u8 pin);


extern XGpioPs * Get_Emio_Point(void);





#endif
