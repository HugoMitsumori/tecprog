#ifndef __POSICAO_H__
#define __POSICAO_H__

#define Norte 0
#define Nordeste 1
#define Sudeste 2
#define Sul 3
#define Sudoeste 4
#define Noroeste 5

typedef struct {
  int i;
  int j;
} Posicao;


Posicao vizinho(Posicao local, int direcao);

int distancia (Posicao pos1, Posicao pos2);

int distancia_ij (Posicao pos, int i, int j);

#endif
