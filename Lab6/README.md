LAB 6

Pedro Romano Splendore

Para realizar o calculo do tempo de loop para ligar e desligar os leds, foi repetido o loop 100.000.000 (cem milhões de vezes) e o tempo que o programa ficou no loop foi de aproxidamente 60 a 62 segundos, resultando em um valor de 0,0000006 ou 0,6 micro segundos. Esse valor foi utilizado no calculo dos valores dos loops de cada thread, dependendo do requisito feito no slide.

loops para a thread 1 - 500.000
loops para a thread 2 - 833.333
loops para a thread 3 - 1.333.333

a) Podemos ver nesse exemplo que, como utilizamos tarefas de mesma prioridade, sem preempção e com um time slice fixo de 50ms para todas as thread, no início do funcionamento, os 4 LEDs piscam sequencialmente até que terminem o seu tempo previsto de execução. Após isso, cada um executará a medida do seu período.

b) Nesse caso, podemos ver que, como as tarefas não tem um time slice e também não são preemptivas, quando elas iniciam a thread, ficam naquela thread até terminar a execução. Essa execução é feita basenado-se na prioridade das thread. Nesse caso elas não interrompem a execução uma da outra.

c) Aqui podemos verificar que o perído da tarefa 1 é contante, pois como existe preempção, ela faz com que a execução das outras thread parem quando ela necessita executar. Já a thread 2, é interrompida somente pela thread 1, e por fim a thread 3 pode ser interrompida tanto pela thread 1 como 2, fazendo com que sua execução dependa de ambas outras thread não estarem sendo executadas naquele momento. Caso a thread 1 precise executar enquanto a thread 2 ou 3 estão executando, a thread 1 ganha o processador.

d) Utilizando o mutex, podemos perceber que não existe preempção entre a thread 1 e 3. Caso a thread 1 esteja bloqueada ou sendo executada, a thread 2 não consegue executar e precisa esperar. Porém, caso a thread 2 esteja esperando, pode ser que a thread 3 tenha que esperar por conta disso.


e) A diferença desse caso e do acima é a presença da herança de prioridade, que faz com que a thread 2 ganhe prioridade para que a thread 3 possa executar posteriormente.