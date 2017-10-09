#include "pilha.h"

#define MAXMEM 100

typedef struct {
  int id;
  int time;
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog;
  int ip;
  int rbp;
  int pos_x;
  int pos_y;
  int num_cristais;
} Maquina;

Maquina* cria_maquina(INSTR *p);

void destroi_maquina(Maquina *m);

void exec_maquina(Maquina *m, int n);
