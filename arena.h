#include "maq.h"

#define PLANO 0
#define RUGOSO 1
#define TRUE 1
#define FALSE 0

typedef struct {
  int tipo_terreno;
  int num_cristais;
  int ocupado;
  int base;
} Celula;

typedef struct {	
  int tempo;
  Celula*** celulas;
  Maquina*** maquinas;
} Arena;


Arena* inicializa( int n, int m, int num_times);

Celula*** incializaCelulas (int n, int m, int num_times);
void atualiza ();
void insereExercito(Arena* arena, int time);
void removeExercito();
void sistema (int op);
