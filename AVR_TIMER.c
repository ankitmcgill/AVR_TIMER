///////////////////////////////////////////////////////
// AVR TIMER LIBRARY
//
// ONLY IMPLEMENTS THE FOLLOWING TIMER MODES:
//	1. NORMAL (TOP = MAX = 0xFF)
//		--- OCA (OPTIONAL)
//		--- OCB (OPTIONAL)
//		--- INTERRUPTS POSSIBLE : OVF, OCA, OCB
//	2. CLEAR TIMER ON COMPARE (CTC) (TOP = OCRA)
//		--- OCA (REQUIRED)
//      --- INTERRUPTS POSSIBLE : OCA
//
// * AT A GIVEN TIME, TIMER CAN BE NORMAL OR CTC MODE
// BUT NOT BOTH
// 
// *TIMER2 SAME AS TIMER0 EXCEPT IT SUPPORTS MORE
// CLOCK PRESCALING OPTIONS
//
// ONLY SUPPORTS THE FOLLOWING CLOCK SOURCES:
//	1. INTERNAL (FROM IO CLOCK)
//
// DECEMBER 22, 2016
//
// ANKIT BHATNAGAR
// ANKIT.BHATNAGARINDIA@GMAIL.COM
///////////////////////////////////////////////////////


#include "AVR_TIMER.h"

void AVR_TIMER_Enable_Mode_Normal(uint8_t timer_num, uint8_t timer_clock, uint8_t interrupt_enable)
{
	//INITIALIZE AND START THE TIMER IN NORMAL MODE WITH THE SPECIFIED
	//CLOCK. ENABLE INTERRUPT IF REQUESTED
	//
	//NORMAL MODE:
	//	BOTTOM = 0x00
	//	TOP = MAX = 0xFF
	//	INTERRUPT = OVERFLOW (IF ENABLED)
	
	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			//CLEAR MODE AND SET NORMAL MODE
			TCCR0A = 0x00;
			TCNT0 = 0x00;
			if(interrupt_enable == AVR_TIMER_INTERRUPT_ON)
			{
				TIMSK0 |= (1 << TOIE0);
			}
			//APPLY CLOCK. START THE TIMER
			TCCR0B = timer_clock;
			break;

		default:
			break;
	}
}

void AVR_TIMER_Enable_Mode_Ctc(uint8_t timer_num, uint8_t timer_clock)
{
	//INITIALIZE AND START THE TIMER IN CTC MODE WITH THE SPECIFIED
	//CLOCK AND SPECIFIED TOP VALUE
	
	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			//CLEAR MODE AND SET CTC MODE
			TCCR0A = 0x02;
			//CLEAR COUNT
			TCNT0 = 0x00;
			//APPLY CLOCK. START THE TIMER
			TCCR0B |= timer_clock;
			break;
			
		default:
			break;
	}
}

void AVR_TIMER_Set_Oca_parameters(uint8_t timer_num, uint8_t oc_mode, uint8_t top_value, uint8_t interrupt_enable)
{
	//SET THE CTC OC-A PARAMETERS FOR THE SPECIFIED TIMER
	
	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			//SET THE OC-A MODE 
			TCCR0A |= (oc_mode << 6);
			//SET THE TOP VALUE
			OCR0A = top_value;
			if(interrupt_enable == AVR_TIMER_INTERRUPT_ON)
			{
				TIMSK0 |= (1 << OCIE0A);
			}
			break;
			
		default:
			break;
	}
}

void AVR_TIMER_Set_Ocb_parameters(uint8_t timer_num, uint8_t oc_mode, uint8_t top_value, uint8_t interrupt_enable)
{
	//SET THE CTC OC-B PARAMETERS FOR THE SPECIFIED TIMER
	
	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			//SET THE OC-B MODE 
			TCCR0A |= (oc_mode << 4);
			//SET THE TOP VALUE
			OCR0B = top_value;
			if(interrupt_enable == AVR_TIMER_INTERRUPT_ON)
			{
				TIMSK0 |= (1 << OCIE0B);
			}
			break;
			
		default:
			break;
	}
}

uint8_t AVR_TIMER_Get_Flag_Value(uint8_t timer_num, uint8_t timer_flag)
{
	//RETURN THE SPECIFIED FLAG VALUE OF THE SPECIFIED TIMER
	
	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			return (((TIFR0 & timer_flag) != 0)? 1 : 0);
			break;
		
		default:
			break;
	}
	return 0;
}

void AVR_TIMER_Clear_Flag(uint8_t timer_num, uint8_t timer_flag)
{
	//CLEAR THE SPECIFIED FLAG OF THE SPECIFIED TIMER

	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			TIFR0 |= timer_flag;
			break;
		
		default:
			break;
	}

}

void AVR_TIMER_Disable(uint8_t timer_num)
{
	//DISABLE THE SPECIFIED TIMER AND RESET
	//ITS COUNT
	
	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			TCCR0A = 0x00;
			//STOP CLOCK TO TIMER
			TCCR0B = 0x00;
			//CLEAR COUNT
			TCNT0 = 0;
			//DISABLE INTERRUPT
			TIMSK0 = 0;
			break;
		
		default:
			break;
	}
}
