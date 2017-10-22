#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "posicao.h"


Posicao vizinho(Posicao local, int direcao) {
  Posicao pos;
  switch (direcao) {
    case Norte:
      pos.i = local.i - 1;
      pos.j = local.j;
      break;
    case Nordeste:
      pos.i = local.i;
      pos.j = local.j + 1;
      break;
    case Sudeste:
      pos.i = local.i + 1;
      pos.j = local.j + 1;
      break;
    case Sul:
      pos.i = local.i + 1;
      pos.j = local.j;
      break;
    case Sudoeste:
      pos.i = local.i + 1;
      pos.j = local.j - 1;
      break;
    case Noroeste:
      pos.i = local.i;
      pos.j = local.j - 1;
      break;
  }
  return pos;
}

int distancia (Posicao pos1, Posicao pos2) {
  int dist_i, dist_j;
  dist_i = abs(pos1.i - pos2.i);
  dist_j = abs(pos1.j - pos2.j);
  if (dist_i > dist_j)
    return dist_i;
  return dist_j;
}

int distancia_ij (Posicao pos, int i, int j) {
  int dist_i, dist_j;
  dist_i = abs(pos.i - i);
  dist_j = abs(pos.j - j);
  if (dist_i > dist_j)
    return dist_i;
  return dist_j;
}
