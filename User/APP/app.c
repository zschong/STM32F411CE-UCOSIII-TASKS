/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : YS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include <includes.h>
#include <string.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

OS_MEM  mem;                    //�����ڴ�������
uint8_t ucArray [ 400 ] [ 256 ];   //�����ڴ������С


/*
*********************************************************************************************************
*                                                 TCB
*********************************************************************************************************
*/

static  OS_TCB   thread1_TCB;    //������ƿ�
static  OS_TCB   thread2_TCB;
static  OS_TCB   thread3_TCB;


/*
*********************************************************************************************************
*                                                STACKS
*********************************************************************************************************
*/

static  CPU_STK  thread1_stack [ THREAD_STACK_SIZE ];
static  CPU_STK  thread2_stack [ THREAD_STACK_SIZE ];
static  CPU_STK  thread3_stack [ THREAD_STACK_SIZE ];


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  thread1_func  ( void * p_arg )
{
	CPU_INT32U  cpu_clk_freq;
	CPU_INT32U  cnts;
	OS_ERR err;

    CPU_Init();                                                 //��ʼ�� CPU �����ʱ��������ж�ʱ���������������
    BSP_Init();                                                 //�弶��ʼ��

    cpu_clk_freq = BSP_CPU_ClkFreq();                           //��ȡ CPU �ں�ʱ��Ƶ�ʣ�SysTick ����ʱ�ӣ�
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        //�����û��趨��ʱ�ӽ���Ƶ�ʼ��� SysTick ��ʱ���ļ���ֵ
    OS_CPU_SysTickInit(cnts);                                   //���� SysTick ��ʼ�����������ö�ʱ������ֵ��������ʱ��

	while(1)
	{
		OSTimeDlyHMSM(0, 0, 1, 0, 0, &err);
		printf("%s\n", __func__);
	}
}
static  void  thread2_func  ( void * p_arg )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	while(1)
	{
		OS_ERR err;
		OSTimeDlyHMSM(0, 0, 1, 0, 0, &err);
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		OSTimeDlyHMSM(0, 0, 1, 0, 0, &err);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}
}
static  void  thread3_func  ( void * p_arg )
{
	while(1)
	{
	}
}

int  main (void)
{
    OS_ERR  err;
	
    OSInit(&err);                                                           //��ʼ�� uC/OS-III

	/* ������ʼ���� */
    OSTaskCreate((OS_TCB     *)&thread1_TCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Start",                            //��������
                 (OS_TASK_PTR ) thread1_func,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&thread1_stack[0],                           //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) THREAD_STACK_SIZE / 10,                     //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) THREAD_STACK_SIZE,                          //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
				 
				 
	/* ������ʼ���� */
    OSTaskCreate((OS_TCB     *)&thread2_TCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Start",                            //��������
                 (OS_TASK_PTR ) thread2_func,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&thread2_stack[0],                           //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) THREAD_STACK_SIZE / 10,                     //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) THREAD_STACK_SIZE,                          //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
				 
				 
	/* ������ʼ���� */
    OSTaskCreate((OS_TCB     *)&thread3_TCB,                            //������ƿ��ַ
                 (CPU_CHAR   *)"App Task Start",                            //��������
                 (OS_TASK_PTR ) thread3_func,                               //������
                 (void       *) 0,                                          //���ݸ����������β�p_arg����ʵ��
                 (OS_PRIO     ) APP_TASK_START_PRIO,                        //��������ȼ�
                 (CPU_STK    *)&thread3_stack[0],                           //�����ջ�Ļ���ַ
                 (CPU_STK_SIZE) THREAD_STACK_SIZE / 10,                     //�����ջ�ռ�ʣ��1/10ʱ����������
                 (CPU_STK_SIZE) THREAD_STACK_SIZE,                          //�����ջ�ռ䣨��λ��sizeof(CPU_STK)��
                 (OS_MSG_QTY  ) 5u,                                         //����ɽ��յ������Ϣ��
                 (OS_TICK     ) 0u,                                         //�����ʱ��Ƭ��������0��Ĭ��ֵOSCfg_TickRate_Hz/10��
                 (void       *) 0,                                          //������չ��0����չ��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), //����ѡ��
                 (OS_ERR     *)&err);                                       //���ش�������
				 
				 
    OSStart(&err);                                                          //�����������������uC/OS-III���ƣ�
}

