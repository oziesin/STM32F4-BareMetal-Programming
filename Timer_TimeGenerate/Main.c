#include "stm32f4xx.h"                  // Device header

#define	CLK	8000000UL

	//	Function prototype	//
void 	RCC_Config(void);
void	GPIO_Init(void);

uint32_t count =	0,var = 0;
unsigned	long delay	=	0;

void TIM4_IRQHandler()
{	
//		if(TIM4->SR	&(1<<0))
//		{
			TIM4->SR	&=	~(1U<<0);
			var++;
//		}
		GPIOA->ODR	^=	(1<<0);
}

int	main()
{
	RCC_Config();
	GPIO_Init();
	
	RCC->APB1ENR	|=	(1<<2);		// TIM4 clock enable
	
	TIM4->PSC	|=	799;
	
	TIM4->ARR	=	9999;
	
	TIM4->DIER	|=	(1<<0);	//	Update interrupt enable

	NVIC_EnableIRQ(TIM4_IRQn);
	
	TIM4->CR1	|=	(1U<<0);
 	while(1)
	{
		count = TIM4->CNT;
	}		
		
}

void 	RCC_Config()	// Fosc=8MHz 
{
	RCC->CR	|=	(1<<16);				// HSE ON
	while(!(RCC->CR	&(1<<17)));	// HSE ready waiting
	RCC->CFGR	|=	(1<<12);			// APB1 PSC=0
	RCC->CFGR	&=	~(7<<13);			// APB2 PSC=0
	RCC->CFGR	|=	(10<<4);				  // AHB	PSC=8
	RCC->PLLCFGR	&=	~(0xFFFFFFFF);	// APB2 PSC=0
	RCC->PLLCFGR	|=	(1<<22);	//	HSE selected
	RCC->PLLCFGR	&=	~(3<<16);	// PLLP=2
	RCC->PLLCFGR	|=	(64<<6);	// PLLN=64
	RCC->PLLCFGR	|=	(1<<2);		// PLLM=4
	RCC->PLLCFGR	|=	(1<<29);	// PLLR=2
	RCC->PLLCFGR	|=	(1<<25);	// PLLQ=2
	
	RCC->CR	|=	(1<<24);				// PLL ON
	while(!(RCC->CR	&(1<<25)));	// PLL ready waiting	
	
}

void	GPIO_Init()
{
	RCC->AHB1ENR	|=	(1<<0);
	GPIOA->MODER	|=	(1<<0);
	GPIOA->OTYPER	&=	~(1<<0);
	GPIOA->PUPDR	&=	~(3<<0);
	
}




