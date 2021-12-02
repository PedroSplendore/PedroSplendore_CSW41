//*****************************************************************************
//
//PEDRO ROMANO SPLENDORE - CSW41
//
//*****************************************************************************

#include "tx_api.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

//escolha 0 -> a / 1 -> b / 2 -> c / 3 -> d / 4 -> e
#define PROGRAMA 4

#define DEMO_BYTE_POOL_SIZE     9120
#define DEMO_STACK_SIZE         1024

TX_THREAD               thread_1;
TX_THREAD               thread_2;
TX_THREAD               thread_3;
TX_BYTE_POOL            byte_pool_0;
TX_MUTEX                mutex_0;

UCHAR                   byte_pool_memory[DEMO_BYTE_POOL_SIZE];

void GPIO_Init(void){
  ///GPIO N/////////////////////////////////
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION));
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1 ); //PN1 - D1 PN0, D2
  
  ///GPIO F/////////////////////////////////
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4 ); //PF4 - D3
}

void Liga_LED(uint8_t LED){
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, LED);
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, LED);
  GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4, LED);
}

void Desliga_LED(){
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, 0);
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, 0);
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0);
}

//loop para timer - cada iteração dura 0,6 micro segundos
uint8_t Timer_Loop(uint8_t LED, uint32_t tempo){

  uint32_t contador = 0;
  
  while(contador<=tempo){
    Liga_LED(LED);
    contador++;
    Desliga_LED();
  }

  return 0;
}

void thread_1_2_3(ULONG numero_thread){
  
  if(numero_thread==1){
    while(1){

      if(PROGRAMA==0 | PROGRAMA==1 | PROGRAMA==2){ // CASO A ou B ou C
        Timer_Loop(2, 500000);
        tx_thread_sleep(1000);
      }
      else{ // CASO D ou E
        UINT status = tx_mutex_get(&mutex_0,TX_WAIT_FOREVER);
        if (status != TX_SUCCESS)
          break;
        Timer_Loop(2, 500000);
        tx_thread_sleep(1000);
      
        status = tx_mutex_put(&mutex_0);
        if (status != TX_SUCCESS)
          break;
      }
    }
  }

  else if(numero_thread==2){
    while(1){ //CASO A ou B ou C ou D ou E
      Timer_Loop(1, 833333);
      tx_thread_sleep(1500);
    }
  }

  else if(numero_thread==3){
    while(1){ 
      if(PROGRAMA==0 | PROGRAMA==1 | PROGRAMA==2){ // CASO A ou B ou C
        Timer_Loop(16, 1333333);
        tx_thread_sleep(4000);
      }
      else{// CASO D ou E
        UINT status = tx_mutex_get(&mutex_0,TX_WAIT_FOREVER);
        if (status != TX_SUCCESS)
          break;
        Timer_Loop(16, 1333333);
        tx_thread_sleep(4000);

        status = tx_mutex_put(&mutex_0);
        if (status != TX_SUCCESS)
          break;
      }
    }
  }
}


 int main(){
   
  GPIO_Init();
  
  Desliga_LED();
  
  //Timer_Loop(2, 100000000); //utilizado para realizar o calculo
  
  tx_kernel_enter();
  
}


/* Define what the initial system looks like.  */

void    tx_application_define(void *first_unused_memory)
{

CHAR    *pointer = NULL;

#ifdef TX_ENABLE_EVENT_TRACE
    tx_trace_enable(trace_buffer, sizeof(trace_buffer), 32);
#endif
    
    UINT Time_Slice[3];
    UINT Prioridade_Thread[3];
    UINT Preempcao_Thread[3];

    for(int i=0;i<3;i++){ // 0 corresponde a thread 1, 1 corresponde a thread 2 e 2 corresponde a thread 3
      
      if(PROGRAMA==0){//CASO A
        Time_Slice[i]=50;
        Prioridade_Thread[i]=0;
        Preempcao_Thread[i]=0;
      }
      else if (PROGRAMA==1){//CASO B
        Time_Slice[i]= TX_NO_TIME_SLICE; // Não tem
        Prioridade_Thread[i]=i;
        Preempcao_Thread[i]=0;
      }  
      else{//CASO C ou D ou E
        Time_Slice[i]= TX_NO_TIME_SLICE; // Não tem
        Prioridade_Thread[i]=i;
        Preempcao_Thread[i]=i;
      }
    }   
       
    if(PROGRAMA==3){ //CASO D
      tx_mutex_create(&mutex_0, "mutex 0", TX_NO_INHERIT); //sem herança
    }
    if(PROGRAMA==4){ //CASO E
      tx_mutex_create(&mutex_0, "mutex 0", TX_INHERIT); //com herança
    }

    /* Create a byte memory pool from which to allocate the thread stacks.  */
    tx_byte_pool_create(&byte_pool_0, "byte pool 0", byte_pool_memory, DEMO_BYTE_POOL_SIZE);

    /* Allocate the stack for thread 1.  */
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

    /* Create thread 1. */
    tx_thread_create(&thread_1, "thread 1", thread_1_2_3, 1,  
            pointer, DEMO_STACK_SIZE, 
            Prioridade_Thread[0], Preempcao_Thread[0], Time_Slice[0], TX_AUTO_START);

    /* Allocate the stack for thread 2.  */
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);
    
    /* Create thread 2. */
    tx_thread_create(&thread_2, "thread 2", thread_1_2_3, 2,  
            pointer, DEMO_STACK_SIZE, 
            Prioridade_Thread[1], Preempcao_Thread[1], Time_Slice[1], TX_AUTO_START);

    /* Allocate the stack for thread 3.  */
    tx_byte_allocate(&byte_pool_0, (VOID **) &pointer, DEMO_STACK_SIZE, TX_NO_WAIT);

    /* Create thread 3. */  
    tx_thread_create(&thread_3, "thread 3", thread_1_2_3, 3,  
            pointer, DEMO_STACK_SIZE, 
            Prioridade_Thread[2], Preempcao_Thread[2], Time_Slice[2], TX_AUTO_START);
}
