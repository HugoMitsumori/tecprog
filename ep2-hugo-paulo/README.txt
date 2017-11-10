Hugo Mitsumori  8941262
Paulo Araujo    8941112


Nessa fase implementamos a Arena como uma estrutura que contém uma matriz de Celulas, um registro das máquinas presentes e algumas informações da batalha.

Optamos por armazenar as máquinas em uma matriz, em que cada linha representa as máquinas de um time diferente.

Cada maquina recebe como identificador único de valor igual a (time*100 + indice_coluna + 1)
Um id 0 pode eventualmente representar máquinas inativas/removidas do jogo

Incluimos Operadores compostos (Tipo e Valor) nas maquinas e no montador

As chamadas de sistema implementadas foram MOVER, RECOLHER e DEPOSITAR

Como não achamos muito legal a maquina ter acesso à arena, a forma que encontramos de fazer a chamada de sistema foi pelo retorno do exec_maquina.
A arena executa um time step e para cada instrução, verifica se o tipo de Ação retornada é diferente de NULO

As funções de teste incluídas nessa fase foram para testar a inicialização das células e das máquinas.

O ATR foi implementado para os tipos NUM, CELULA e ACAO
