





#ifndef _ORDERSTRUCT_H
#define _ORDERSTRUCT_H



#include "GlobalRef.h"


#define GRADE				(20)


typedef enum {
	LOOP_STOP = 0x0,
}Ctl_t;


//Ԫ������
typedef struct {
	List* Sline;		//��ʼ��
	List* Cline;		//��ǰ��
	u32 Depth;			//loop���
	int Repeat;			//loop�ظ�����
	int count;			//������
}__attribute__((aligned(4)))loop_t;


typedef struct {
	loop_t * top;		//ջ��Ԫ��ָ��
	int real;			//ʵ��������
	int capcity;		//����
}__attribute__((aligned(4)))Stack_t;


extern loop_t *loop_ptr;

//��λջ
int Initloop(void);

//��ջ
int PushStack(loop_t *top);

//��ջ
int PopStack(loop_t *top);







#endif

