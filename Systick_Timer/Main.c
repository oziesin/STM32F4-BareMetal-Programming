#include "stm32f4xx.h"                  // Device header

#define	CLK	8000000UL

	//	Function prototype	//
void 	RCC_Config(void);
void	GPIO_Init(void);
void	SysTick_Init(void);
void  Delay_ms(uint32_t ms);
static volatile uint32_t tDelay =	0;
unsigned	long delay	=	0;

void SysTick_Handler(void)
{
	SysTick->VAL	=	0;	
	if(tDelay != 0 )
	{
		tDelay--;
	
	}
}

int	main()
{
	RCC_Config();
	GPIO_Init();
	SysTick_Init();
	uint32_t clk= SystemCoreClock;
 	while(1)
	{
		GPIOA->ODR	^=	(1<<0);
		Delay_ms(1000);
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
void	SysTick_Init()
{
	SysTick->CTRL	= 0;
	SysTick->CTRL	|=	1<<2;
	SysTick->CTRL	|=	1<<1;
	SysTick->LOAD	=	7999;
	SysTick->VAL	=	0;
	SysTick->CTRL	|=	1<<0;
}
void  Delay_ms(uint32_t ms)
{
	tDelay=ms;
	while(	tDelay!=0	);
}


