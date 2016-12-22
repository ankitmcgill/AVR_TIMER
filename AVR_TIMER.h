///////////////////////////////////////////////////////
// AVR TIMER LIBRARY
//
// ONLY IMPLEMENTS THE FOLLOWING TIMER MODES:
//	1. NORMAL
//	2. CLEAR TIMER ON COMPARE (CTC)
//
// ONLY SUPPORTS THE FOLLOWING TIMER CLOCK SOURCES:
//	1. INTERNAL (FROM IO CLOCK)
//
// DECEMBER 22, 2016
//
// ANKIT BHATNAGAR
// ANKIT.BHATNAGARINDIA@GMAIL.COM
///////////////////////////////////////////////////////

#ifndef _AVR_TIMER_H_
#define _AVR_TIMER_H_

#include <stdio.h>
#include <avr/io.h>

#define AVR_TIMER_OPMODE_NORMAL	0
#define AVR_TIMER_OPMODE_CTC	1

#define AVR_TIMER_INTERRUPT_OFF	0
#define AVR_TIMER_INTERRUPT_ON	1

#define AVR_TIMER_FLAG_OVERFLOW		0x00
#define AVR_TIMER_FLAG_OCA_MATCH	0x02
#define AVR_TIMER_FLAG_OCB_MATCH	0x08

#define AVR_TIMER_CTC_OC_MODE_DISCONNECTED	0x00
#define AVR_TIMER_CTC_OC_MODE_TOGGLE		0x01
#define AVR_TIMER_CTC_OCA_MODE_CLEAR		0x02
#define AVR_TIMER_CTC_OCA_MODE_SET			0x03

#define AVR_TIMER_8BIT_TIMER0	0
#define AVR_TIMER_16BIT_TIMER1	1
#define AVR_TIMER_8BIT_TIMER2	2

#define AVR_TIMER_CLOCK_DISABLE			0x00
#define AVR_TIMER_CLOCK_PRESCALE_NONE	0x01
#define AVR_TIMER_CLOCK_PRESCALE_8		0x02
#define AVR_TIMER_CLOCK_PRESCALE_64		0x03
#define AVR_TIMER_CLOCK_PRESCALE_256	0x04
#define AVR_TIMER_CLOCK_PRESCALE_1024	0x05


void AVR_TIMER_Enable_Mode_Normal(uint8_t timer_num, uint8_t timer_clock, uint8_t interrupt_enable);
void AVR_TIMER_Enable_Mode_Ctc(uint8_t timer_num, uint8_t timer_clock);
void AVR_TIMER_Ctc_Set_Oca_parameters(uint8_t timer_num, uint8_t oc_mode, uint8_t top_value, uint8_t interrupt_enable);
void AVR_TIMER_Ctc_Set_Ocb_parameters(uint8_t timer_num, uint8_t oc_mode, uint8_t top_value, uint8_t interrupt_enable);
uint8_t AVR_TIMER_Get_Flag_Value(uint8_t timer_num, uint8_t timer_flag); 
void AVR_TIMER_Disable(uint8_t timer_num);


#endif