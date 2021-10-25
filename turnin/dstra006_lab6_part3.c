/*	Author: David Strathman
 *  Partner(s) Name:
 *	Lab Section:
 *	Assignment: Lab #6  Exercise #3
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
unsigned char currA = 0x00;
unsigned char holdA = 0x00;
unsigned char heldA = 0x00;
unsigned char cnt = 0x00;
unsigned char visited = 0x00;
unsigned char execute = 0x00;
enum SM_States {SM_Start, SM_Begin, SM_Check, SM_Hold, SM_Held} SM_State;

void Tick() {
  switch (SM_State) {
    case SM_Start:
      SM_State = SM_Begin;
      break;

    case (SM_Begin):
      if ((~PINA) == 0x00) {
        SM_State = SM_Begin;
      } else {
        SM_State = SM_Check;
      }
      break;

    case (SM_Check):
      SM_State = SM_Hold;
      break;

    case (SM_Held):
      if (~PINA != 0x00) {
        SM_State = SM_Hold;
      } else {
        SM_State = SM_Begin;
      }
      break;

    case (SM_Hold):
      holdA = ~PINA;
      if (cnt >= 0x0A) {
        execute = 0x01;
        SM_State = SM_Held;
        break;
      }
      if (currA == 0x03 && holdA != 0x00) {
        SM_State = SM_Hold;
      }
      else if (currA == 0x03 && holdA == 0x00) {
        SM_State = SM_Begin;
      } else {
        if (holdA == currA) {
          SM_State = SM_Hold;
        } else if (holdA != currA && ~PINA != 0x00){
          SM_State = SM_Check;
        } else {
          SM_State = SM_Begin;
        }
      }
      break;

    default:
      SM_State = SM_Begin;
      break;
  }

  switch (SM_State) {
    case SM_Begin:
      break;
    case (SM_Check):
      cnt = 0;
      currA = ~PINA;
      if (currA == 0x01 && PORTB <= 8) {
        PORTB = PORTB + 1;
        cnt++;
      }
      else if (currA == 0x02 && PORTB >= 1){
        PORTB = PORTB - 1;
        cnt++;
      }
      else if (currA == 0x03) {
        PORTB = 0x00;
        cnt++;
      }
      break;

    case SM_Hold:
        cnt = cnt + 1;
      break;

    case SM_Held:
      cnt = 0;
      visited = 0x01;
      if (holdA == 0x01 && PORTB <= 8) {
        PORTB = PORTB + 1;
      }
      else if (holdA == 0x02 && PORTB >= 1) {
        PORTB = PORTB - 1;
      }
      break;

    default:
    break;
  }
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
    DDRC = 0xFF; PORTC = 0x00; // Configure port C's 8 pins as outputs, initialize to 0s
    DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
    DDRD = 0xFF; PORTD = 0x00; // Configure port D's 8 pins as outputs, initialize to 0s
    TimerSet(100);
    TimerOn();
    SM_State = SM_Start;
    PORTB = 0x07;
    /* Insert your solution below */
    while (1) {
      Tick();
      while (!TimerFlag);
      TimerFlag = 0;
    }
    return 1;
}
