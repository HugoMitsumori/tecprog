#include <stdio.h>
#include <stdlib.h>
#include "arena.h"
#include <math.h>
#include <time.h>

Celula*** inicializaCelulas (int n, int m, int num_times) {
  int i, j, k, rand_num;
  float f;

  /* alocação de memória */  
  Celula*** arena = (Celula***) calloc ( n, sizeof(Celula**) );
  for ( i = 0 ; i < n ; i++ ) {
    arena[i] = (Celula**) calloc ( m, sizeof (Celula*) );
    for ( j = 0 ; j < m ; j++ ) {
      arena[i][j] = (Celula*) calloc (1, sizeof(Celula) ); 
    }
  }


  /* inicializa tudo como plano OK */

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

  /* sortear depositos de cristais  (5% das celulas) */
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


Maquina*** inicializaMaquinas(int num_times, int maquinas_por_time) {  
  int i, j;
  Maquina*** maquinas = (Maquina***) calloc ( num_times, sizeof(Maquina**) );
  for ( i = 0 ; i < num_times ; i++ ) {
    maquinas[i] = (Maquina**) calloc ( maquinas_por_time, sizeof (Maquina*) );
    for ( j = 0 ; j < maquinas_por_time ; j++ ) {
      maquinas[i][j] = (Maquina*) calloc (1, sizeof(Maquina) ); 
    }
  }
  return maquinas;
}


Arena* inicializa (int n, int m, int num_times) {
  Arena* arena = (Arena*) malloc (sizeof(Arena*));
  arena->celulas = inicializaCelulas(n, m, num_times);
  arena->maquinas = inicializaMaquinas(num_times, n * m * 0.1 / num_times);
  return arena;
}

void insereExercito(Arena* arena, int time) {
  //arena->maquinas[indiceLivre(arena)] = cria_maquina(NULL);
}


void imprimeCelulas(Arena* arena, int n, int m ) {
  int i, j;
  for ( i = 0 ; i < n ; i++ )
    for ( j  = 0 ; j < m ; j++ )
      printf("celula[%d][%d] - terreno: %d, cristais: %d, base: %d \n", i, j, arena->celulas[i][j]->tipo_terreno, arena->celulas[i][j]->num_cristais, arena->celulas[i][j]->base);
}

int main () {
  int n, m, times;
  n = 10;
  m = 10;
  times = 3;

  Arena* arena = inicializa(n, m, times);

  imprimeCelulas(arena, n, m);
  
  return 0;
}
