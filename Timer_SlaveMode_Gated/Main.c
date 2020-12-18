//*************************************************SLAVE MODE GATED MODE PRACTISE***************************************************************************//
#include "stm32f4xx.h"                  // Device header

#define	CLK	8000000UL

	//	Function prototype	//
void 	RCC_Config(void);
void	GPIO_Init(void);
void	TIM5_Init(void);

uint32_t count =	0 , cap_var = 0 ;
unsigned	long delay	=	0;

int	main()
{
	RCC_Config();
	GPIO_Init();
	TIM5_Init();

 	while(1)
	{
		count=TIM5->CNT;
		while((TIM5->SR	&	(1<<6)))
		{
			GPIOA->ODR	^=	(1<<1);
			TIM5->SR	&=	~(1<<6);
		}
	}		
		
}

void 	RCC_Config()	// Fosc=8MHz 
{
	RCC->CR	|=	(1<<16);				// HSE ON
	while(!(RCC->CR	&(1<<17)));	// HSE ready waiting
	RCC->CFGR	|=	(1<<12);			// APB1 PSC=0
	RCC->CFGR	&=	~(7<<13);			// APB2 PSC=0
	RCC->CFGR	|=	(10<<4);		  // AHB	PSC=8
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
	RCC->AHB1ENR	|=	(1<<0);		// GPIOA clock enable
	GPIOA->MODER	|=	(1<<1);		// PA0 Alternate Function mode
	GPIOA->OTYPER	&=	~(1<<0);	// Push-Pull select
	GPIOA->PUPDR	&=	~(3<<0);	// No push-No pull select
	GPIOA->AFR[0]	=	(1<<1);			// PA0 AF2(TIM5) alternate function
	
	GPIOA->MODER	|=	(1<<2);		// PA1 Output mode
	GPIOA->OTYPER	&=	~(1<<1);	// Push-Pull select
	GPIOA->PUPDR	&=	~(3<<2);	// No push-No pull select
}

void TIM5_Init()
{
	RCC->APB1ENR	|=	(1<<3);	//	TIM5 clock enable
	
	TIM5->PSC	|=	8000;				//	Prescaler value
	
	TIM5->ARR	= 10000;					//	Auto reload value
	
	TIM5->CR1	&=	~(1<<2);		//	Any of the following events generate an update interrupt or DMA request if enabled.

	TIM5->CCMR1	|=	(1<<0);		//	CC1 channel is configured as input, IC1 is mapped on TI1
	TIM5->CCMR1	|=	(3<<4);		//  fSAMPLING=fCK_INT, N=8
	
	TIM5->CCER	&=	~(1<<1);	//	Circuit is sensitive to TIxFP1 rising edge (capture, trigger in reset, external clock or trigger mode), TIxFP1 is not inverted (trigger in gated mode, encoder mode).
	TIM5->CCER	&=	~(1<<3);	//	This bit is used in conjunction with CC1P to define TI1FP1/TI2FP1 polarity. refer to CC1P description.
	
	TIM5->SMCR	|=	(5<<0);		//	Gated Mode - The counter clock is enabled when the trigger input (TRGI) is high. The counter stops (but is not reset) as soon as the trigger becomes low. Both start and stop of the counter are controlled.
	TIM5->SMCR	|=	(5<<4);		//	Filtered Timer Input 1 (TI1FP1)
	
	TIM5->DIER	|=	(1<<6);		//	Trigger interrupt enabled.
	TIM5->DIER	|=	(1<<0);		//	Update interrupt enabled
	
	TIM5->SR	&=	~(1<<6);		//	Trigger interrupt flag clear
	
	TIM5->CR1	|=	(1U<<0);		//	Run timer5
	
}
