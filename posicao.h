#ifndef __POSICAO_H__
#define __POSICAO_H__

#define NORTE 0
#define NORDESTE 1
#define SUDESTE 2
#define SUL 3
#define SUDOESTE 4
#define NOROESTE 5

typedef struct {
  int i;
  int j;
} Posicao;


Posicao vizinho(Posicao local, int direcao);

int distancia (Posicao pos1, Posicao pos2);

int distancia_ij (Posicao pos, int i, int j);

#endif
