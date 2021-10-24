/*	Author: David Strathman
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Link To Vid:
 *
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
enum SM_States { SM_LOOP } SM_State;
unsigned int iterate[] = {0x01, 0x02, 0x04};
signed char i = 0x00;

void Tick() {
  switch(SM_State) {
    case SM_LOOP:
    SM_State = SM_LOOP;
  }

  switch (SM_State) {
    case SM_LOOP:
      PORTB = iterate[i];
      ++i;
      if (i >= 3) {
        i = 0;
      }
  }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRB = 0xFF; PORTB = 0x01;
    DDRA = 0x00; PORTA = 0xFF;
    TimerSet(1000);
    TimerOn();
    SM_State = SM_LOOP;
    /* Insert your solution below */
    while (1) {
      Tick();
      while (!TimerFlag);
      TimerFlag = 0;
    }
    return 1;
}
