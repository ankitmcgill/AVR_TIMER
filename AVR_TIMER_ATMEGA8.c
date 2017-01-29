///////////////////////////////////////////////////////
// AVR TIMER LIBRARY
// FOR ATMEGA8
//
// ONLY IMPLEMENTS THE FOLLOWING TIMER MODES:
//	1. NORMAL (TOP = MAX = 0xFF)
//		IN NORMAL MODE, INTERRUPT/FLAG IS AT TOP VALUE
//		OF MAX. OPTIONALLY INTERRUPT/FLAGS CAN BE SET
//		FOR THE 2 OUTPUT COMPARE CHANNELS A & B
//		--- OCA (OPTIONAL)
//		--- OCB (OPTIONAL)
//		--- INTERRUPTS POSSIBLE : OVF, OCA, OCB
//	2. CLEAR TIMER ON COMPARE (CTC) (TOP = OCRA)
//		IN CTC MODE, TOP = OCRA. THE TIMER COUNT RESETS
//		AFTER REACHING THIS VALUE. THE INTERRUPT/FLAG IS
//		SET ONLY FOR THIS EVENT. NO OVERFLOW EVENT
//		--- OCA (REQUIRED)
//      --- INTERRUPTS POSSIBLE : OCA
//
// * AT A GIVEN TIME, TIMER CAN BE NORMAL OR CTC MODE
// BUT NOT BOTH
// 
// *TIMER2 SAME AS TIMER0 EXCEPT IT SUPPORTS MORE
// CLOCK PRESCALING OPTIONS
//
// MODES OF OPERATION
//	1. TIMER 0 - 8 BIT - OVF
//	2. TIMER 1 - 16 BIT - OVF, OC-A, OC-B [CTC MODE ON OC-A]
//	3. TIMER 2 - 8 BIT - OVF, OC-A [CTC MODE ON OC-A]
//
// ONLY SUPPORTS THE FOLLOWING CLOCK SOURCES:
//	1. INTERNAL (FROM IO CLOCK)
//
//	EXAMPLE USAGE:
//		* ALWAYS SET THE OC A/B CHANNELS BEFORE SETTING
//		  THE NORMAL / CTC MODES. SETTING THESE MODES
//		  STARTS THE TIMER sO OC A/B SHOULD BE SET BEFORE
//		  HAND
//	
//	AVR_TIMER_Set_Oca_parameters(AVR_TIMER_8BIT_TIMER0, AVR_TIMER_OPMODE_OC_TOGGLE, 100, AVR_TIMER_INTERRUPT_ON);
//	AVR_TIMER_Set_Ocb_parameters(AVR_TIMER_8BIT_TIMER0, AVR_TIMER_OPMODE_OC_TOGGLE, 200, AVR_TIMER_INTERRUPT_ON);
//	AVR_TIMER_Enable_Mode_Normal(AVR_TIMER_8BIT_TIMER0, AVR_TIMER_TIM0_CLOCK_PRESCALE_NONE, AVR_TIMER_INTERRUPT_ON);
//
//	AVR_TIMER_Ctc_Set_Oca_parameters(AVR_TIMER_8BIT_TIMER0, AVR_TIMER_OPMODE_CTC_TOGGLE, 200,AVR_TIMER_INTERRUPT_ON);
//	AVR_TIMER_Enable_Mode_Ctc(AVR_TIMER_8BIT_TIMER0, AVR_TIMER_TIM0_CLOCK_PRESCALE_NONE);
//
//
// DECEMBER 26, 2016
//
// ANKIT BHATNAGAR
// ANKIT.BHATNAGARINDIA@GMAIL.COM
//
// TODO:
//	(1) FIX CTC MODE IN TIMER1 - NOT WORKING RIGHT NOW
///////////////////////////////////////////////////////

#include "AVR_TIMER_ATMEGA8.h"

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
			//CLEAR COUNT AND SET CLOCK
			TCNT0 = 0x00;
			if(interrupt_enable == AVR_TIMER_INTERRUPT_ON)
			{
				TIMSK |= (1 << TOIE0);
			}
			//APPLY CLOCK. START THE TIMER
			TCCR0 |= timer_clock;
			break;

		case AVR_TIMER_8BIT_TIMER2:
			//CLEAR MODE AND SET NORMAL MODE
			TCCR2 &= ~((1 << WGM21) | (1 << WGM20));
			TCNT2 = 0x00;
			if(interrupt_enable == AVR_TIMER_INTERRUPT_ON)
			{
				TIMSK |= (1 << TOIE2);
			}
			//APPLY CLOCK. START THE TIMER
			TCCR2 |= timer_clock;
			break;

		case AVR_TIMER_16BIT_TIMER1:
			//CLEAR MODE AND SET NORMAL MODE
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCNT1 = 0x0000;
			if(interrupt_enable == AVR_TIMER_INTERRUPT_ON)
			{
				TIMSK |= (1 << TOIE1);
			}
			//APPLY CLOCK. START THE TIMER
			TCCR1B |= timer_clock;
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
			//TIMER0 IN ATMEGA8 DOES NOT HAVE CTC MODE
			return;
			break;

		case AVR_TIMER_8BIT_TIMER2:
			//CLEAR MODE AND SET CTC MODE
			TCCR2 &= ~((1 << WGM21) | (1 << WGM20));
			TCCR2 |= (1 << WGM21);
			//CLEAR COUNT
			TCNT2 = 0x00;
			//APPLY CLOCK. START THE TIMER
			TCCR2 |= timer_clock;
			break;
			
		case AVR_TIMER_16BIT_TIMER1:
			//CLEAR MODE AND SET CTC MODE
			TCCR1A &= ~((1 << WGM11) | (1 << WGM10));
			TCCR1B |= (1 << WGM12);
			//CLEAR COUNT
			TCNT1 = 0x00;
			//APPLY CLOCK. START THE TIMER
			TCCR1B |= timer_clock;
			break;

		default:
			break;
	}
}

void AVR_TIMER_Set_Oca_parameters(uint8_t timer_num, uint8_t oc_mode, uint16_t top_value, uint8_t interrupt_enable)
{
	//SET THE CTC OC-A PARAMETERS FOR THE SPECIFIED TIMER
	
	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			//TIMER0 IN ATMEGA8 DOES NOT HAVE CTC MODE
			return;
			break;

		case AVR_TIMER_8BIT_TIMER2:
			//SET THE OC-A MODE 
			TCCR2 |= (oc_mode << 4);
			//SET THE TOP VALUE
			OCR2 = top_value;
			if(interrupt_enable == AVR_TIMER_INTERRUPT_ON)
			{
				TIMSK |= (1 << OCIE2);
			}
			break;

		case AVR_TIMER_16BIT_TIMER1:
			//SET THE OC-A MODE 
			TCCR1A |= (oc_mode << 6);
			//SET THE TOP VALUE
			OCR1A = top_value;
			if(interrupt_enable == AVR_TIMER_INTERRUPT_ON)
			{
				TIMSK |= (1 << OCIE1A);
			}
			break;
			
		default:
			break;
	}
}

void AVR_TIMER_Set_Ocb_parameters(uint8_t timer_num, uint8_t oc_mode, uint16_t top_value, uint8_t interrupt_enable)
{
	//SET THE CTC OC-B PARAMETERS FOR THE SPECIFIED TIMER
	
	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			//TIMER0 IN ATMEGA8 DOES NOT HAVE CTC MODE
			return;
			break;

		case AVR_TIMER_8BIT_TIMER2:
			//SET THE OC-B MODE 
			//TIMER2 ONLY SUPPORTS OC-A MODE
			return;
			break;
		
		case AVR_TIMER_16BIT_TIMER1:
			//SET THE OC-B MODE 
			TCCR1A |= (oc_mode << 4);
			//SET THE TOP VALUE
			OCR1B = top_value;
			if(interrupt_enable == AVR_TIMER_INTERRUPT_ON)
			{
				TIMSK |= (1 << OCIE1B);
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
			return (((TIFR & timer_flag) != 0)? 1 : 0);
			break;
		
		case AVR_TIMER_8BIT_TIMER2:
			return (((TIFR & timer_flag) != 0)? 1 : 0);
			break;

		case AVR_TIMER_16BIT_TIMER1:
			return (((TIFR & timer_flag) != 0)? 1 : 0);
			break;
		
		default:
			break;
	}
	return 0;
}

void AVR_TIMER_Clear_Flag(uint8_t timer_num, uint8_t timer_flag)
{
	//CLEAR THE SPECIFIED FLAG OF THE SPECIFIED TIMER
	//TO CLEAR THE TIMER FLAG, WE HAVE TO WRITE 1 TO THE
	//FLAG POSITION

	switch(timer_num)
	{
		case AVR_TIMER_8BIT_TIMER0:
			TIFR |= timer_flag;
			break;

		case AVR_TIMER_8BIT_TIMER2:
			TIFR |= timer_flag;
			break;
		
		case AVR_TIMER_16BIT_TIMER1:
			TIFR |= timer_flag;
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
			//STOP CLOCK TO TIMER
			TCCR0 = 0x00;
			//CLEAR COUNT
			TCNT0 = 0;
			//DISABLE INTERRUPT
			TIMSK &= ~(1 << TOIE0);
			break;
		
		case AVR_TIMER_8BIT_TIMER2:
			//STOP CLOCK TO TIMER
			TCCR2 = 0x00;
			//CLEAR COUNT
			TCNT2 = 0;
			//DISABLE INTERRUPT
			TIMSK &= ~((1 << TOIE2) | (1 << OCIE2));
			break;
		
		case AVR_TIMER_16BIT_TIMER1:
			//STOP CLOCK TO TIMER
			TCCR1B = 0x00;
			//CLEAR COUNT
			TCNT1 = 0;
			//DISABLE INTERRUPT
			TIMSK &= ~((1 << TOIE2) | (1 << OCIE1A) | (1 << OCIE1B));
			break;

		default:
			break;
	}
}
