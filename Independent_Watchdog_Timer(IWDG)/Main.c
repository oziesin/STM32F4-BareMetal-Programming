//*************************************************INDEPENDENT WATCHDOG TIMER PRACTISE***************************************************************************//
#include "stm32f4xx.h"                  // Device header

#define	CLK	8000000UL

	//	Function prototype	//
void 	RCC_Config(void);
void	GPIO_Init(void);
void	IWDG_Init(void);
void	delay(uint32_t	time);
uint32_t count =	0 , cap_var = 0 ;

int	main()
{
	RCC_Config();
	GPIO_Init();
	IWDG_Init();
 	while(1)
	{
		GPIOA->ODR	^=	(1<<5);
		delay(CLK/3);
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

void	IWDG_Init(void)
{
	IWDG->KR	|=	(0x5555);		// 	access to PR and RLR registers enable
	IWDG->PR	|=	(1<<0);			//	Prescaler value
	IWDG->RLR	|=	(0xFFF);		//	Reload value
	IWDG->KR	|=	(0x0000);		//	access to PR and RLR registers disable
	IWDG->KR	|=	(0xCCCC);		//	counter start

}
void	delay(uint32_t	time)
{
	while(time--);
}

