//*************************************************DAC PRACTISE***************************************************************************//
#include "stm32f4xx.h"                  // Device header

#define	CLK	8000000UL

	//	Function prototype	//
void 	RCC_Config(void);
void	GPIO_Init(void);
void	DAC_Init(void);


int	main()
{
	RCC_Config();
	GPIO_Init();
	DAC_Init();

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
	GPIOA->MODER	|=	(3<<8);		// PA0 Analog mode
	GPIOA->OTYPER	&=	~(1<<4);	// Push-Pull select
	GPIOA->PUPDR	&=	~(3<<8);	// No push-No pull select
}

void DAC_Init()
{
	RCC->APB1ENR	|=	(1<<29);	//	DAC interface clock enable.
	DAC->CR	&=	~(1<<1);				//	DAC channel 1 output buffer enabled
	DAC->CR	&=	~(1<<2);					//	DAC channel 1 trigger enabled
//	DAC->CR	|=	(7<<3);					//	Choose software trigger	
	
	//	Vref*(dac_value/0xFFF)
	DAC->DHR12R1	|=	0xF00;				//	DAC value
//	DAC->SWTRIGR	|=	(1<<0);		//	Trigger Ch1
	DAC->CR	|=	(1<<0);					//	DAC channel 1 enabled	
}

