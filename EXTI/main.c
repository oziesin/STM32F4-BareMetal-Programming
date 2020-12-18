#include "stm32f4xx.h"                  // Device header
void	delay(uint32_t	time);
void	GPIO_Init(void);
void	Interrupt_Enable(void);

int 	main	(){
	
	GPIO_Init();
	Interrupt_Enable();
	while(1)
	{
	}
}


void	EXTI1_IRQHandler(void){
	
	EXTI->PR	|=	1<<1	;					//	interrupt flag cleared
	GPIOA->BSRR	|= 1<<5;					// Led on
	delay(1000000);
	GPIOA->BSRR	|= 1<<(16+5);			// Led off
}


void	delay(uint32_t	time){
	
	while(time--);
}

void	GPIO_Init(void){
	
	GPIOA->MODER |= 1<<10;	//	GPIOA pin5 output mode
}
	void Interrupt_Enable(){
		
	RCC->APB2ENR	|=	1<<14	;		// SYSCFGR clock enabled
	SYSCFG->EXTICR[0]	&=	~(15<<4)	;	// EXTI line1 PORTA enable
	EXTI->RTSR		|=	1<<1	;		// Trigger selected rising trigger
	EXTI->IMR			|=	1<<1	;
	NVIC_EnableIRQ(EXTI1_IRQn);	// NVIC EXTI line1 enable	
}

