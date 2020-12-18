//*************************************************UART PRACTISE***************************************************************************//
#include "stm32f4xx.h"                  // Device header
#include "stdio.h"
#include "stdint.h"

volatile uint8_t msg[]="HELLO WORLD\n";
uint8_t adc=16;
volatile char data;
char MSG[12];
char Dma_value[1];
	//	Function prototype	//
void	GPIO_Init(void);
void	UartInit(void);
void SendChar(char c);
char GetChar(void);

void delay(uint32_t time){
	while(time--);
}

void USART2_IRQHandler(void){
	
	 if(USART2->SR & (1<<5)){
		//	Recieved data ready to read.
		data= USART2->DR;
		USART2->DR=data; 
	}
}


//void DMA1_Stream5_IRQHandler(void){
//	
//	if(DMA1->HISR & (1<<11)){
//		
//		DMA1->HIFCR &=~(1<<11);
//		
//		DMA1_Stream6->CR	|= (1<<0);		//	Stream6 Enable Tx
//	}
//}

//void DMA1_Stream6_IRQHandler(void){
//	
//	if(DMA1->HISR & (1<<21)){
//		
//		DMA1->HIFCR &=~(1<<21);
//		
//		DMA1_Stream5->CR	|= (1<<0);		//	Stream5 Enable Rx
//	}
//}





int	main(){
	
	GPIO_Init();
	UartInit();
	DMA1_Stream5->CR	|= (1<<0);		//	Stream5 Enable Rx
 	while(1)
	{
//		
//		SendChar(temp);
//		sprintf(MSG,"\ragirlik:%d\n",adc);
//		for(uint8_t i=0; i<sizeof(MSG);i++){	
//		SendChar(MSG[i]);
//		delay(250000);
		}

	}

void	GPIO_Init()
{
	RCC->AHB1ENR	|=	(1<<0);		// GPIOA clock enable
	GPIOA->MODER	|=	(1<<5)|(1<<7);		// PA2 and PA3 Alternate function mode
	GPIOA->AFR[0]	|=	(7<<8)|(7<<12);		// PA9 and PA10 usart1 selected
	GPIOA->OSPEEDR	|=	(3<<4 )|(3<<6);	//	Selected high speed for uart
	
	GPIOA->MODER	|= (1<<10);	// PA5 set to output
}

void UartInit()
{
	RCC->APB1ENR	|=	(1<<17);		//	Usart2 clock enabled
	USART2->CR1	|=(1<<2)|(1<<3);	//	Reciever enabled,Transmitter enabled
	USART2->BRR	=	0x683;			//	uart baudrate config
	USART2->CR1	|=	(1<<13);		//	Usart2 enabled
	USART2->CR1	&=	~(1<<12) & ~(1<<10);	//	Word length and parity control settings
	USART2->CR2	&=	~(3<<12);		//	Select stop bit quantity
	USART2->CR1	|=	(5<<5);			//	Enable TXE and RXNE interrupts
	NVIC_EnableIRQ(USART2_IRQn);	//	Enable global interrupt vector
	
	//	DMA configurations	//
//	RCC->AHB1ENR |=	1<<21;			//	Dma1 clock enable
//	
//  USART2->CR3	|= (3<<6);			//	Enable dma for reciever and transmitter
//	
//	DMA1_Stream5->CR	|=(4<<25);	// Select dma stream5 channel4 for Rx
//	DMA1_Stream6->CR	|=(4<<25);	// Select dma stream6 channel4 for Tx
//	
//	DMA1_Stream5->CR	|=(2<<13);	// Memory data size selected word
//	DMA1_Stream6->CR	|=(2<<13);	// Memory data size selected word

//	DMA1_Stream5->CR	|=(2<<11);	// Periperal data size selected word
//	DMA1_Stream6->CR	|=(2<<11);	// Periperal data size selected word
//	
//	DMA1_Stream6->CR	|=(1<<10);	// Enabled memory incremented mode for transmit
	
//	DMA1_Stream6->CR	|=(1<<8);		// Circular mode enable
//	DMA1_Stream5->CR	|=(1<<8);		// Circular mode enable
	
//	DMA1_Stream6->CR	|=(1<<6);		// Data direction selected memory to peripheral
//	DMA1_Stream5->CR	&=~(3<<6);	// Data direction selected peripheral to memory
//	
//	DMA1_Stream6->CR	|=(1<<4);		// Transfer complete interrupt enable
//	DMA1_Stream5->CR	|=(1<<4);		// Transfer complete interrupt enable
//	
//	DMA1_Stream6->NDTR =1;
//	DMA1_Stream5->NDTR =1;
//	
//	DMA1_Stream6->PAR	= (uint32_t)&USART2->DR;
//	DMA1_Stream5->PAR	= (uint32_t)&USART2->DR;
//	
//	DMA1_Stream6->M0AR = (uint32_t)Dma_value;
//	DMA1_Stream5->M0AR = (uint32_t)Dma_value;
//	
//	NVIC_EnableIRQ(DMA1_Stream5_IRQn);
//	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void SendChar(char c){
	
	USART2->DR |=	c;	//	Load the data in the DR.
	GPIOA->ODR |=(1<<5);
	while(!(USART2->SR & (1<<6)));	//	Wait untill transmit complete.
	
}

char GetChar(void){
	
	while(!(USART2->SR & (1<<5)));	//	Wait untill recieve complete.
	char temp= USART2->DR;
	GPIOA->ODR &=~(1<<5);
	return temp;
	
}
