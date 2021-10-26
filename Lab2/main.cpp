//*****************************************************************************
//
//PEDRO ROMANO SPLENDORE - CSW41
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

int timer =0;
void SysTickIntHandler(void)
{
  if (timer == 0){
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1); // Acende ou apaga LED D1
  }
  timer ++;
}

void GPIOInicialization(void){
   ///GPIO N/////////////////////////////////
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); // Habilita GPIO N (LED D1 = PN1)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)); // Aguarda final da habilitação
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1); // LED D1
   ///GPIO J//////////////////////////////
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ); // Habilita GPIO J (push-button SW1 = PJ0)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOJ)); // Aguarda final da habilitação
  GPIOPinTypeGPIOInput(GPIO_PORTJ_BASE, GPIO_PIN_0); // push-buttons SW1  
  GPIOPadConfigSet(GPIO_PORTJ_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}


int main(void)
{
    volatile uint32_t ui32Loop;
    GPIOInicialization();
    IntMasterEnable();
    SysTickIntRegister(SysTickIntHandler);
    SysTickPeriodSet(10000000); // para clock = 120MHz, logo 12 interrupções do systick = 1 seg
    SysTickIntEnable();
    SysTickEnable();
    int flag = 0;
    //
    // Loop.
    //
   while(1)
   {
     if (timer >= 37)
     {
      GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
      cout <<"\nVocê perdeu";
      flag =1;
     }  
     if ((GPIOPinRead(GPIO_PORTJ_BASE, GPIO_PIN_0) == 0)) //testa se o SW1 foi pressionado
     { 
      GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0x0);
      cout << "\n número de clocks foi:"<<timer*10000000<<"\n número de segundos foi:"<<timer/12.0;
      flag = 1;
     }
     if (flag == 1){
        flag = 0;

        SysCtlDelay(120000000);
        GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_PIN_1);
        timer = 0;
     }  
   }
}
