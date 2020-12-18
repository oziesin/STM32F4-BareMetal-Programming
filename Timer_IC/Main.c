#include "stm32f4xx.h"                  // Device header

#define	CLK	8000000UL

	//	Function prototype	//
void 	RCC_Config(void);
void	GPIO_Init(void);
void	TIM5_Init(void);

uint32_t	capture_var = 0 , counter =	0  ;
unsigned	long delay	=	0;

void TIM5_IRQHandler()
{	
	if(TIM5->SR	&	(1<<1))
	{
		TIM5->SR	&=	~(1<<1);	//	Capture interrupt flag clear
		capture_var	=	TIM5->CCR1;		
		TIM5->CNT	=	0	;
	}		
}

int	main()
{
	RCC_Config();
	GPIO_Init();
	TIM5_Init();

 	while(1)
	{

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
	RCC->AHB1ENR	|=	(1<<0);		// GPIOA clock enable
	GPIOA->MODER	|=	(1<<1);		// PA0 Alternate Function mode
	GPIOA->OTYPER	&=	~(1<<1);	// Push-Pull select
	GPIOA->PUPDR	&=	~(3<<0);	// No pull-up - No pull-down
	GPIOA->AFR[0]	=	(1<<1);			// PA0 AF2(TIM5) alternate function
}

void TIM5_Init()
{
	RCC->APB1ENR	|=	(1<<3);		// TIM5 clock enable
	
	TIM5->PSC	|=	31999;		//	Prescaler value
	
	TIM5->ARR	= 99999;		//	Auto reload value
		
	TIM5->CCMR1	|=	(1<<0);	//	CC1 channel is configured as input, IC1 is mapped on TI1
	TIM5->CCMR1	|=	(3<<4);	//	fSAMPLING=fCK_INT, N=8
	
	TIM5->CCER	|=	(1<<0);	//	Capture enable		
	
	TIM5->DIER	|=	(1<<0);	//	Interrupt enable
	TIM5->DIER	|=	(1<<1);	//	CC1 interrupt enable
	
	NVIC_EnableIRQ(TIM5_IRQn);	// TIM5 global interrupt enable
	
	TIM5->SR	&=	~(1<<1);	//	Capture interrupt flag clear
	
	TIM5->CR1	|=	(1U<<0);	//	Run timer5
	
}
