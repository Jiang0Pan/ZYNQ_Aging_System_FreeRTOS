



#include "error.h"
#include "unistd.h"



/***********************************************************************
	[功	       能	]错误处理(回调),操作错误表
	[参            数	]无
	[返    回    值	]无
***********************************************************************/
void Deal_ERROR(void)
{

}

/***********************************************************************
	[功	       能	]超时函数(us)
	[参            数	]ms:计数器指针
	[返    回    值	]超时结果
***********************************************************************/
u8 TimeOutUS(u16 *us)
{
	usleep(1);
	*us -= 1;
	if(*us == 0)
	  return 1;		//时间到达
	return 0;
}

u8 TimeOutCount(u16 *count)
{
	*count -= 1;
	if(*count == 0)
	  return 1;		//计数到达
	return 0;
}
















