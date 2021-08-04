





#ifndef _ORDERSTRUCT_H
#define _ORDERSTRUCT_H



#include "GlobalRef.h"


#define GRADE				(20)


typedef enum {
	LOOP_STOP = 0x0,
}Ctl_t;


//元素属性
typedef struct {
	List* Sline;		//起始行
	List* Cline;		//当前行
	u32 Depth;			//loop深度
	int Repeat;			//loop重复次数
	int count;			//计数器
}__attribute__((aligned(4)))loop_t;


typedef struct {
	loop_t * top;		//栈顶元素指针
	int real;			//实际数据量
	int capcity;		//容量
}__attribute__((aligned(4)))Stack_t;


extern loop_t *loop_ptr;

//复位栈
int Initloop(void);

//入栈
int PushStack(loop_t *top);

//出栈
int PopStack(loop_t *top);







#endif

