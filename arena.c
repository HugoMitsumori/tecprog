#include <stdio.h>
#include <stdlib.h>
#include "arena.h"
#include <math.h>
#include <time.h>
#include "maq.h"

Celula*** inicializaCelulas (int n, int m, int num_times) {
  int i, j, k, rand_num;
  float f;
  /* alocação de memória */
  
  Celula*** arena = (Celula***) calloc ( n, sizeof(Celula**) );
  for ( i = 0 ; i < n ; i++ ) {
    arena[i] = (Celula**) calloc ( m, sizeof (Celula**) );
    for ( j = 0 ; j < m ; j++ ) {
      arena[i][j] = (Celula*) calloc (1, sizeof(Celula*) ); 
    }
  }


  /* inicializa tudos como plano OK */

  /* cada celulas tem 20% de chance de ser rugosa*/
  srand(time(NULL));
  rand_num = rand();
  for ( i = 0 ; i < n ; i++ ) {
    for ( j = 0 ; j < m ; j++ ) {
      rand_num = rand() % 1000;
      if ( rand_num < 200 )
        arena[i][j]->tipo_terreno = RUGOSO;
    }
  }

  /* sortear tdepositos de cristais  (5% das celulas) */
  for ( f = 0 ; f < n * m * 0.05 ;) {
    i = rand_num = rand() % n;
    j = rand_num = rand() % m;
    if (arena[i][j]->num_cristais != 0) continue;
    arena[i][j]->num_cristais = (rand_num = rand() % 5) + 1;
    f++;
  }

  /* sortear as celulas das bases (1 por time) */
  for ( k = 0; k < num_times ;) {
    i = rand_num = rand() % n;
    j = rand_num = rand() % m;
    if (arena[i][j]->num_cristais != 0) continue;
    arena[i][j]->base =  1;
    k++;
  }
  
  return arena;
}


Arena* inicializa (int n, int m, int num_times) {
  Arena* arena = (Arena*) malloc (sizeof(Arena*));
  arena->celulas = inicializaCelulas(n, m, num_times);



  return arena;
}

void insereExercito(Arena* arena) {
  arena->maquinas[indiceLivre(arena)] = cria_maquina(NULL);
}


int main () {
  Arena* arena = inicializa(10, 10, 3);
  return 0;
}
