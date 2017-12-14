#include "maq.h"
#include "posicao.h"

#define PLANO 0
#define RUGOSO 1


typedef struct {	  
  Celula*** celulas;
  Maquina*** maquinas;
  int tempo;
  int num_times;
  int maquinas_por_time;
  int n; 
  int m;
  Posicao* bases;
  FILE* display;
} Arena;


Arena* inicializa( int n, int m, int num_times);

//Celula*** incializaCelulas (int n, int m, int num_times, FILE* display);
void atualiza ();
void insereExercito(Arena* arena, int time);
void removeExercito(Arena* arena, int time);
void sistema (Arena* arena, Maquina* maquina, TipoAcao tipo, int direcao);
