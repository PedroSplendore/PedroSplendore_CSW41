//*****************************************************************************
//
//PEDRO ROMANO SPLENDORE - CSW41
//
//*****************************************************************************

#include <stdint.h>
#include <iostream>
#include "utils/uartstdio.h"
#include "inc/hw_memmap.h"
#include <inttypes.h>
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"

using namespace std;

//****************************************************************************
// System clock rate 120Hz.
//****************************************************************************
uint32_t SysClock;

uint8_t vetor_caracteres[32];
uint8_t vetor_caracteres_original[32];
uint8_t posicao_vetor;
uint8_t ultima_posicao;
uint8_t envia = 0;

void UARTIntHandler(void){
  uint8_t le_caracter = (uint8_t)UARTCharGetNonBlocking(UART0_BASE); //le e salva o caracter digitado
  if(le_caracter == 13) //se for enter ativa uma flag
    envia = 1;
  else{//se nao for, salva o valor nos vetores
    vetor_caracteres[posicao_vetor] = le_caracter;
    vetor_caracteres_original[posicao_vetor] = le_caracter;
    posicao_vetor++;
  }
  UARTIntClear(UART0_BASE,UARTIntStatus(UART0_BASE,true));//limpa a interrupção
}

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

  //configure the parity and the speed in bps
  UARTConfigSetExpClk(UART0_BASE, SysClock, 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

} // UARTInit


int main(){
  //clock a 120MHz
  SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_240), 120000000);  
  //chama a funcao criada para inicializar a uart
  UARTInit();
  //diplay visual
  const char escrever []= "\033[2JEnter text: ";
  for(uint32_t i =0; i<16;i++)
  {
      UARTCharPutNonBlocking(UART0_BASE, escrever[i]);
  }
  //habilita e registra interrupções da uart
  UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
  UARTIntRegister(UART0_BASE,UARTIntHandler);
  //inicializa variaveis
  posicao_vetor = 0;
  ultima_posicao = 0;
  uint8_t transforma_caracter;

  while(1){

    if(ultima_posicao!=posicao_vetor){//verifica se houveram interrupções
      transforma_caracter = vetor_caracteres[ultima_posicao];
      if(((transforma_caracter)>=(uint8_t)65)&&((transforma_caracter)<=(uint8_t)90)){//verifica se é maiusculo, se for, transforma em minusculo
        transforma_caracter = transforma_caracter+32; 
      }
      vetor_caracteres[ultima_posicao] = transforma_caracter;//atualiza de maiusculo para minusculo
      ultima_posicao++;
    }
      if(envia){//se enter for pressionado, printa na tela a mensagem original e a mensagem sem maiusculos
        for(uint32_t i =0; i<posicao_vetor;i++){
          UARTCharPutNonBlocking(UART0_BASE,vetor_caracteres_original[i]);
        }
        UARTCharPutNonBlocking(UART0_BASE, '-');
        UARTCharPutNonBlocking(UART0_BASE, '>');
         for(uint32_t i =0; i<posicao_vetor;i++){
          UARTCharPutNonBlocking(UART0_BASE,vetor_caracteres[i]);
        }
        envia = 0; 
        posicao_vetor = 0;
        ultima_posicao = 0;
        vetor_caracteres[31] = {};
        vetor_caracteres_original[31] = {};
        UARTCharPutNonBlocking(UART0_BASE, '|');
      }
  }
}