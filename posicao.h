#ifndef __POSICAO_H__
#define __POSICAO_H__

#define NORDESTE 0
#define LESTE 1
#define SUDESTE 2
#define SUDOESTE 3
#define OESTE 4
#define NOROESTE 5

typedef struct {
  int i;
  int j;
} Posicao;


Posicao vizinho(Posicao local, int direcao);

int distancia (Posicao pos1, Posicao pos2);

int distancia_ij (Posicao pos, int i, int j);

#endif
