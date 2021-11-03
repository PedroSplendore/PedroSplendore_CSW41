//*****************************************************************************
//
// Configure the UART and its pins.  This must be called before UARTprintf().
//
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <iostream>
#include <stdio.h>
#include "utils/uartstdio.h"
//#include "system_TM4C1294.h"
#include "inc/hw_memmap.h"
// includes da biblioteca driverlib
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"

using namespace std;

//****************************************************************************
//
// System clock rate in Hz.
//
//****************************************************************************
uint32_t SysClock;
uint32_t dataUART[16] = {};


void UARTInit(void){
  // Enable the GPIO Peripheral used by the UART.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

  // Enable UART0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
  //
  // Enable processor interrupts.
  //
  IntMasterEnable();
  // Configure GPIO Pins for UART mode.
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  cout <<"a";
  UARTConfigSetExpClk(UART0_BASE, SysClock, 9600,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
  cout <<"b";
  UARTEnable(UART0_BASE);
  IntEnable(INT_UART0);
  UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
  
  cout <<"a";
  UARTCharPut(UART0_BASE, 'a');
  cout <<"a";
  
  
} // UARTInit

void UARTSend(void)
{
  for(uint32_t n=0; n==(sizeof(dataUART)/sizeof(uint32_t));n++){
    UARTCharPut(UART0_BASE, dataUART[n]);
  }
}

void UARTIntHandler(void){
   uint32_t n=0;
   
   uint32_t ui32Status;
   ui32Status = UARTIntStatus(UART0_BASE, true);
   UARTIntClear(UART0_BASE, ui32Status);
   
   while(UARTCharsAvail(UART0_BASE))
    {
      dataUART[n] = UARTCharGetNonBlocking(UART0_BASE);
      n++;
      SysCtlDelay(SysClock / (1000 * 3));
    }
}

int main(){
  //clock a 120MHz
  SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_240), 120000000);
  UARTInit();
  
    //
  // Prompt for text to be entered.
  //
  UARTSend();

  //
  // Loop forever echoing data through the UART.
  //
  while(1)
  {
    //UARTReceive();
    UARTSend();
  }

}