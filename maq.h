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
  struct vizinho* vizinho_nordeste;
  struct vizinho* vizinho_leste;
  struct vizinho* vizinho_sudeste;
  struct vizinho* vizinho_sudoeste;
  struct vizinho* vizinho_oeste;
  struct vizinho* vizinho_noroeste;
} Maquina;

typedef enum {
  PAREDE,
  VAZIO,
  BASE,
  CRISTAL,
  ROBO
} TipoVizinho;

typedef struct vizinho{
  TipoVizinho tipo;
  union {
    int time_base;
    int num_cristais;
    Maquina* robo;
  } valor;
} Vizinho;

Maquina* cria_maquina(INSTR *p);

void destroi_maquina(Maquina *m);

Acao exec_maquina(Maquina *m, int n);
