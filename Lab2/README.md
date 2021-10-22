# PedroSplendore_CSW41
Project for Embedded systems course

Porque a variável ui32Loop é declarada como volatile dentro de main()?
R. uma variável volatile pode ser modificada por várias threads em que estão sendo executadas em paralelo. As variáveis volatiles são excluidas de otimizações e não existem garantias de uma ordem de execução/ordenação das instruções.