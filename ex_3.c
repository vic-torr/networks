/*
Exercicio 34:
Modifique o programa baseado em sockets denominado socket simplex-talk de modo que ele
use UDP como protocolo de transporte em vez de TCP. Você terá que substituir o parâmetro
SOCK_STREAM pelo parâmetro SOCK_DGRAM no cliente e no servidor. Depois, no servidor,
remova as chamadas a listen() e accept() e substitua os dois laços aninhados no final por um
único laço que invoca recv() com o socket s. Finalmente, veja o que acontece quando dois desses
clientes UDP se conectam simultaneamente ao mesmo servidor UDP e compare isso com o
comportamento do TCP.


Anteriores:
33:
Obtenha e construa o programa de exemplo usando sockets denominado simplex-talk, mostrado
no texto. Inicie um servidor e um cliente, em janelas separadas. Enquanto o primeiro cliente
estiver sendo executado, inicie 10 outros clientes que se conectam ao mesmo servidor; esses
outros clientes provavelmente deverão ser iniciados em segundo plano, com sua entrada
redirecionada para um arquivo. O que acontece com esses 10 clientes? Seus connect()s falham,
esgotam o tempo limite ou têm sucesso? Alguma outra chamada é bloqueada? Agora encerre
o primeiro cliente. O que acontece? Experimente isso também com o valor MAX_PENDING
do servidor definido como 1.

32:
Modifique o programa baseado em sockets denominado simplex-talk de modo que, toda vez que
o cliente enviar uma linha ao servidor, este enviará a linha de volta ao cliente. O cliente
(e o servidor) agora terá que fazer chamadas alternadas a recv() e send().

*/

