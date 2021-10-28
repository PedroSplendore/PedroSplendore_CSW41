 
 
THUMB

  PUBLIC EightBitHistogram
  SECTION .text: CODE
       
preencheHistograma

  MOV R3, R5 ;Pega a posição inicial do vetor que esta em R9
  LDRB R8, [R2], #1 ; le o valor atual da posicao zero da imagem e desloca em 1 pixel o ponteiro
  ADD R8, R8 ;para ir de 16 em 16
  

  LDR R10, [R3, R8] ;le o valor da posicao atual do contador de bits
  ADD R10, #1 ;adiciona um
  

  ADD R3, R8 ; adiciona na posicao do contador de bits correspondente ao pixel lido da imagem
  STR R10, [R3] ;guarda o valor atualizado
  

  ADD R11, #1 ; contador para ver se chegou no final da imagem ou nao
  CMP R11, R4
  BNE preencheHistograma
  
  BX LR

zeraBitsHistograma
  STR R10, [R3], #2 ; para realizar a colocação de zero nas posições de memória do contador de bits
  ADD R11, #1 ;soma 1 na flag
  CMP R11, #256 ; se a flag R7 = 256, termina
  BNE zeraBitsHistograma ; se nao chegou, executa novamente a funcao

  BX LR ; retorna para onde a função estava
 
 
EightBitHistogram

    MUL R4, R0, R1 ;R4 = TAMANHO DA IMAGEM
    MOV R5, R3 ;R5 = PONTEIRO PARA O INICIO DO CONTADOR DE BITS
    
    CMP R4, #65536 ;COMPARA O TAMANHO MAXIMO DA IMAGEM
    ITT GE ;SE FOR MAIOR OU IGUAL, ENTRA NO IF E RETORNA ZERO
      MOVGE R0, #0
      BXGE LR
    
    MOV R10, #0 ; zera registradores que serao utilizados na funcao
    MOV R11, #0 
    PUSH {LR}
    BL zeraBitsHistograma ;FUNÇÃO PARA ZERAR OS REGISTROS DE BITS DO HISTOGRAMA
    POP {LR}
    

    MOV R11, #0 ;Zera o R11 que sera o contador na funcao
    PUSH {LR}
    BL preencheHistograma
    POP {LR}
    

    MOV R0, R4 ;coloca o total de pixels lidos em R0 para realizar o retorno da funcao e retorna
    BX LR
	
  END