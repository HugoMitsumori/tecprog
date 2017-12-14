Hugo Mitsumori  8941262
Paulo Araujo    8941112

Fase 4

Funcionalidades corrigidas das fases anteriores:

- Testes de movimento (Fase 2)
- Testes de ataque (Fase 2)
- Testes de recolher cristal (Fase 2)
- Testes de depositar cristal na base (Fase 2)
- Correção das direções de acordo com o hexágono desenhado (Fase 3)
- Controle de visualização de cristais e removação do robô (Fase 3)


Funcionalidades novas (Fase 4):

- Integração da linguagem nova com a arena
- Implementação dos comandos mover, atacar, recolher e depositar na linguagem nova


* O robô será criado por padrão com as instruções contidas nos arquivos "programa1", "programa2", "programa3" e "progrma4",
relativo aos times (time 1 carrega programa1, e assim em diante)
Caso não existe o arquivo específico do time, irá carregar um genérico "programa"

Protocolo de comunicação entre arena e display:

rob <caminho_sprite> <id_robo> : adiciona o par (id_robo, Robo) no dicionario de robos 
base <caminho_sprite> <i> <j> : adiciona a base com a posições dadas na lista de bases
cristal <quantidade> <i> <j> : adiciona os cristais com a posições dadas na lista de cristais

remove rob <id_rob> <posicao_i> <posicao_j>: remove o robo de chave <id_robo> do dicionario de robos e desenha a celula sobre a posição

recolhe <i> <j>: recolhe um cristal da celula se possível

<id_robo> <i0> <j0> <if> <jf>: move o robo de id <id_robo> da posição (i0, j0) para a posição (if, jf)

Créditos das sprites:
- base.png: ©2010-2017 DarkDragonn, https://cardsofwars.deviantart.com/art/Pokemon-tilesheet-2-152608690
- cristal.png: Kenney - http://kenney.nl/assets/puzzle-pack







