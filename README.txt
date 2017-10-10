Hugo Mitsumori  8941262
Paulo Araujo    8941112


Nessa fase implementamos a Arena como uma estrutura que contém uma matriz de Celulas, um registro das máquinas presentes e algumas informações da batalha.

Optamos por armazenar as máquinas em uma matriz, em que cada linha representa as máquinas de um time diferente.

Cada maquina recebe como identificador único de valor igual a (time*100 + indice_coluna + 1)
Um id 0 pode eventualmente representar máquinas inativas/removidas do jogo

