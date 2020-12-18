//*************************************************ADC PRACTISE***************************************************************************//
#include "stm32f4xx.h"                  // Device header

#define	CLK	8000000UL

	//	Function prototype	//
void 	RCC_Config(void);
void	GPIO_Init(void);
void	ADC_Init(void);

uint16_t	data	=	0;

void	ADC_IRQHandler(void)
{
	data	=	ADC1->DR;
}

int	main()
{
	RCC_Config();
	GPIO_Init();
	ADC_Init();

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
	GPIOA->MODER	|=	(3<<0);		// PA0 Analog mode
	GPIOA->OTYPER	&=	~(1<<0);	// Push-Pull select
	GPIOA->PUPDR	&=	~(3<<0);	// No push-No pull select
}

void ADC_Init()
{
	RCC->APB2ENR	|=	(1<<8);		//	ADC1 clock enabled
	ADC1->CR1	|=	(1<<25);			//	Resolution 8-bit (minimum 11 ADCCLK cycles)
	ADC1->CR1	|=	(1<<5);				//	EOC interrupt enabled. An interrupt is generated when the EOC bit is set.
	ADC1->CR2	&=	~(1<<11);			//	Data alignment is right alignment
	ADC1->CR2	|=	(1<<10);			//	The EOC bit is set at the end of each regular conversion. Overrun detection is enabled.
	ADC1->CR2	|=	(1<<1);				//	Continuous conversion mode.
	ADC1->CR2	|=	(1<<0);				//	Enable ADC
	ADC1->SMPR2	|=	(7<<0);			//	Sampling time is 480 clock cycles.
	NVIC_EnableIRQ(ADC_IRQn);		//	ADC global interrupt enable.
	ADC1->CR2	|=	(1<<30);			//	Starts conversion of regular channels
}

