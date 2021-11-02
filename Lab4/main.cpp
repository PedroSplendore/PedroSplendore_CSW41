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

using namespace std;

void UARTInit(void){
  // Enable the GPIO Peripheral used by the UART.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

  // Enable UART0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));

  // Configure GPIO Pins for UART mode.
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  cout <<"a";
  UARTConfigSetExpClk(UART0_BASE, 40000000, 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
  cout <<"b";
  UARTEnable(UART0_BASE);
  cout <<"a";
  UARTCharPut(UART0_BASE, 'a');
  cout <<"a";
} // UARTInit

int main(){
  cout <<"a";
 // SysCtlDelay(120000000);
  uint32_t teste1 = UARTClockSourceGet(UART0_BASE);
  uint32_t systemClock = SysCtlClockFreqSet(SYSCTL_OSC_INT | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_320,40000000);
  UARTInit();
}