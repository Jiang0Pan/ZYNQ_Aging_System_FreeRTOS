

#include "gpio.h"
#include "error.h"



/***********************************************************************
	[功	       能	]gpio引脚方向设置
	[参            数	]pin:引脚编号, IO_Dir:方向
	[返    回    值	]无
***********************************************************************/
void Gpio_Set_Dir(u8 pin, IO_Dir dir)
{
	//设置IO方向
	XGpioPs_SetDirectionPin(Get_Emio_Point(), pin, dir);

	//使能IO
	XGpioPs_SetOutputEnablePin(Get_Emio_Point(), pin, 1);
}

/***********************************************************************
	[功	       能	]设置引脚电平
	[参            数	]pin:引脚编号, value:电平
	[返    回    值	]无
***********************************************************************/
void Set_pin_value(u8 pin, u8 value)
{
	XGpioPs_WritePin(Get_Emio_Point(), pin, value);
}

/***********************************************************************
	[功	       能	]读取引脚电平
	[参            数	]pin:引脚编号
	[返    回    值	]电平值
***********************************************************************/
u8 Get_pin_value(u8 pin)
{
	return (XGpioPs_ReadPin(Get_Emio_Point(), pin));
}



