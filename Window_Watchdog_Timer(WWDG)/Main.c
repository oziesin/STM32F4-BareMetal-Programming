//*************************************************WINDOW WATCHDOG TIMER PRACTISE***************************************************************************//
#include "stm32f4xx.h"                  // Device header

#define	CLK	8000000UL

	//	Function prototype	//
void 	RCC_Config(void);
void	GPIO_Init(void);
void	WWDG_Init(void);
void	delay(uint32_t	time);
uint32_t count =	0 ;

int	main()
{
	RCC_Config();
	GPIO_Init();
	WWDG_Init();
 	while(1)
	{
		GPIOA->ODR	^=	(1<<5);
		delay(CLK/10);
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
	
	GPIOA->MODER	|=	(1<<10);		// PA5 Output mode
	GPIOA->OTYPER	&=	~(1<<5);	// Push-Pull select
	GPIOA->PUPDR	&=	~(3<<10);	// No push-No pull select
}

void	WWDG_Init(void)
{
	RCC->APB1ENR	|=	(1<<11);	//	Window watchdog clock enabled
	WWDG->CFR	|=	(1<<8);				//	CK Counter Clock (PCLK1 div 4096) div 4
	WWDG->CFR	|=	0x3F;					//	These bits contain the window value to be compared to the downcounter.
	WWDG->CR	|=	(1<<7);				//  Watchdog enabled
	WWDG->CR	|=	(0x3F);				//  These bits contain the value of the watchdog counter. It is decremented every (4096 x 2WDGTB[1:0]) PCLK1 cycles. A reset is produced when it rolls over from 0x40 to 0x3F (T6 becomes cleared).
}
void	delay(uint32_t	time)
{
	while(time--);
}

