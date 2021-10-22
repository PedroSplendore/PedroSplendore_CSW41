//*****************************************************************************
//
// blinky.c - Simple example to blink the on-board LED.
//
// Copyright (c) 2013-2020 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.2.0.295 of the EK-TM4C1294XL Firmware Package.
//
//*****************************************************************************

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"

using namespace std;

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Blinky (blinky)</h1>
//!
//! A very simple example that blinks the on-board LED using direct register
//! access.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

//*****************************************************************************
//
// Blink the on-board LED.
//
//*****************************************************************************

uint8_t LED_D1 = 0;
int timer =0;
void SysTickIntHandler(void)
{
  //cout << "teste";
  //LED_D1 ^= GPIO_PIN_0; // Troca estado do LED D1
  //GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, LED_D1); // Acende ou apaga LED D1}
  timer ++;
}

void GPIOInicialization(void){
   ///GPIO N/////////////////////////////////
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); // Habilita GPIO N (LED D1 = PN1, LED D2 = PN0)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)); // Aguarda final da habilitação
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0); // LEDs D1 e D2 como saída
   ///GPIO J//////////////////////////////
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ); // Habilita GPIO J (push-button SW1 = PJ0, push-button SW2 = PJ1)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)); // Aguarda final da habilitação
  GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0); // push-buttons SW1 e SW2 como entrada  
}


int main(void)
{
    int Previous_State;
    volatile uint32_t ui32Loop;
    GPIOInicialization();
    IntMasterEnable();
    SysTickIntRegister(SysTickIntHandler);
    SysTickPeriodSet(12000000); // f = 1Hz para clock = 24MHz
    SysTickIntEnable();
    SysTickEnable();
    cout << "teste";
    cout << LED_D1;
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_PIN_0);
    //
    // Loop forever.
    //
    
   Previous_State = ((GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0))); //le o estado inicial dos botoes
   while(1)
   {
     if (!Previous_State)//ve se precisa ler novamente ou nao (caso SW1 esteja solto, nao le novamente)
        Previous_State = ((GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0)));
     if(Previous_State)
     {  
       if ((GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == 0)) //testa se o SW1 foi pressionado
       { 
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0x0);
        
       }
       
     }
    
   }
}
