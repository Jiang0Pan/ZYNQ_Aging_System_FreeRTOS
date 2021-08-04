








#include "orderstruct.h"
#include "string.h"
#include "operation.h"



//��Ч����Ϊ1-GRADE
static loop_t loop[GRADE];

static loop_t curr_loop;

loop_t *loop_ptr = &curr_loop;

static Stack_t Stack = {
	.top     = loop,
	.real    = 0,
	.capcity = GRADE,
};

//��λջ
int Initloop(void)
{
	memset(loop, 0, sizeof(loop_t) * (GRADE));
	memset(&curr_loop, 0, sizeof(loop_t));

	//��ʼ����ǰloop
	curr_loop.Sline  = (List*)LIST_ADDR;
	curr_loop.Cline  = (List*)LIST_ADDR;
	curr_loop.Depth  = Get_Cycle_Num();
	curr_loop.Repeat = 1;
	curr_loop.count  = 1;

	//ջ��ʼ��
	Stack.top     = loop;
	Stack.real    = 0;
	Stack.capcity = GRADE;

	return SUCCESS;
}

//��ջ
int PushStack(loop_t *top)
{
	if(Stack.real == Stack.capcity)	//full
		return FAILURE;

	if(Stack.real > 0)
		Stack.top += 1;

	Stack.real += 1;

	Stack.top->Sline  = top->Sline;
	Stack.top->Cline  = top->Cline;
	Stack.top->Depth  = top->Depth;
	Stack.top->Repeat = top->Repeat;
	Stack.top->count  = top->count;

	return SUCCESS;
}

//��ջ
int PopStack(loop_t *top)
{
	if(Stack.real == 0)
		return FAILURE;

	top->Sline  = Stack.top->Sline;
	top->Cline  = Stack.top->Cline;
	top->Depth  = Stack.top->Depth;
	top->Repeat = Stack.top->Repeat;
	top->count  = Stack.top->count;

	if(Stack.real < Stack.capcity && Stack.real > 1)
		Stack.top -= 1;

	Stack.real -= 1;

	return SUCCESS;
}









