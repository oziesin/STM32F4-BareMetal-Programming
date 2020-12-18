		/*		2 Button interrupt priority example		*/
		
/***********************************************************************************************************/		
#include "stm32f4xx.h"                  // Device header
void	delay(uint32_t	time);
void	GPIO_Init(void);
void	Interrupt_Enable(void);

		
int 	main	()
{
	GPIO_Init();
	Interrupt_Enable();
	
	while(1)
	{
	GPIOA->BSRR	|= 1<<0;					// GPIOA Pin0 high
	GPIOA->BSRR	|= 1<<4;					// GPIOA Pin4 high
	}
}

void	EXTI1_IRQHandler(void)
{
	EXTI->PR	|=	1<<1	;					//	interrupt flag cleared
	GPIOA->BSRR	|= 1<<(16+4);					// GPIOA Pin4 low
	GPIOA->BSRR	|= 1<<0;					// GPIOA Pin0 high
	delay(10000000);
	GPIOA->BSRR	|= 1<<(16+0);					// GPIOA Pin0 low
}

void	EXTI15_10_IRQHandler(void)
{
	EXTI->PR	|=	1<<13	;					//	interrupt flag cleared
	GPIOA->BSRR	|= 1<<(16+0);					// GPIOA Pin0 low
	GPIOA->BSRR	|= 1<<4;					// GPIOA Pin4 high
	delay(10000000);
	GPIOA->BSRR	|= 1<<(16+4);					// GPIOA Pin4 low
}

void	delay(uint32_t	time)
{
	while(time--);
}

void	GPIO_Init(void){
	
	GPIOA->MODER |= 1<<0;	//	GPIOA pin0 output mode
	GPIOA->MODER |= 1<<8;	//	GPIOA pin4 output mode
	
}
	void Interrupt_Enable()
{
	RCC->APB2ENR	|=	1<<14	;		// SYSCFGR clock enabled
	SYSCFG->EXTICR[0]	&=	~(15<<4)	;	// EXTI line1 PORTA enable
	EXTI->RTSR		|=	1<<1	;		// Trigger selected rising trigger
	EXTI->IMR			|=	1<<1	;
	NVIC_EnableIRQ(EXTI1_IRQn);	// NVIC EXTI line1 enable
	
	SYSCFG->EXTICR[3]	|=	2<<4	;	// EXTI line4 PORTC enable
	EXTI->RTSR		|=	1<<13	;		// Trigger selected rising trigger
	EXTI->IMR			|=	1<<13	;
	NVIC_EnableIRQ(EXTI15_10_IRQn);	// NVIC EXTI line1 enable
	
	
	NVIC_SetPriority(EXTI1_IRQn,0)	;			//	Priority determined
	NVIC_SetPriority(EXTI15_10_IRQn,1)	;	//	Priority determined
}
/***********************************************************************************************************/		

		/*	IR sensor with EXTI example		*/		

/***********************************************************************************************************/	