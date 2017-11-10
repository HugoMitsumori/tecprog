#include "pilha.h"

#define MAXMEM 100

typedef struct {
  int id;
  int time;
  int vida;
  int dano;
  Pilha pil;
  Pilha exec;
  OPERANDO Mem[MAXMEM];
  INSTR *prog;
  int ip;
  int rbp;
  Posicao posicao;
  int num_cristais;
} Maquina;


Maquina* cria_maquina(INSTR *p);

void destroi_maquina(Maquina *m);

Acao exec_maquina(Maquina *m, int n);
