#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>


#include "arena.h"



/* ---------------------------   Celulas -----------------------------------*/

/* aloca memoria e define as propriedades das celulas */
Celula*** inicializaCelulas (int n, int m, int num_times, FILE* display) {
  int i, j, k, rand_num;
  Posicao* bases;
  float f;

  /* alocação de memória */
  Celula*** arena = (Celula***) calloc ( n, sizeof(Celula**) );
  for ( i = 0 ; i < n ; i++ ) {
    arena[i] = (Celula**) calloc ( m, sizeof (Celula*) );
    for ( j = 0 ; j < m ; j++ ) {
      arena[i][j] = (Celula*) calloc (1, sizeof(Celula) );
    }
  }

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


  bases = malloc ( num_times * sizeof(Posicao));

  /* inicializa bases */
  bases[0].i = n/4;
  bases[0].j = m/4;

  bases[1].i = n/4;
  bases[1].j = 3*m/4;

  bases[2].i = 3*n/4;
  bases[2].j = m/4;

  bases[3].i = 3*n/4;
  bases[3].j = 3*m/4;


  /* definir as celulas das bases (1 por time) */
  for ( k = 0; k < num_times ; k++ ){
    arena[ bases[k].i ][ bases[k].j ]->base = k + 1;
    fprintf(display, "base base.png %d %d\n", bases[k].i, bases[k].j);
  }

  /* sortear depositos de cristais  (5% das celulas) */
  for ( f = 0 ; f < n * m * 0.05 ;) {
    i = rand_num = rand() % n;
    j = rand_num = rand() % m;
    if (arena[i][j]->num_cristais != 0 || distancia_ij(bases[0], i, j) <= 2 ||
      distancia_ij(bases[1], i, j) <= 2 || distancia_ij(bases[2], i, j) <= 2
      || distancia_ij(bases[3], i, j) <= 2) continue;    
    arena[i][j]->num_cristais = (rand_num = rand() % 5) + 1;
    fprintf(display, "cristal %d %d %d\n", arena[i][j]->num_cristais, i, j);
    f++;
  }

  return arena;
}


/* alloca memoria para as maquinas do jogo */
Maquina*** inicializaMaquinas(int num_times, int maquinas_por_time, INSTR *instrucoes) {
  int i, j;
  Maquina*** maquinas = (Maquina***) calloc ( num_times, sizeof(Maquina**) );
  for ( i = 0 ; i < num_times ; i++ ) {
    maquinas[i] = (Maquina**) calloc ( maquinas_por_time, sizeof (Maquina*) );
    for ( j = 0 ; j < maquinas_por_time ; j++ ) {
      maquinas[i][j] = cria_maquina(instrucoes);
    }
  }
  return maquinas;
}

/* ---------------------------   Exercitos -----------------------------------*/

/* comportamento basico das maquinas */
INSTR programa[] = {
  {SYS, {ACAO, {.acao = {MOVER, SUDESTE}}}},
  {SYS, {ACAO, {.acao = {MOVER, SUDOESTE}}}},
  {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
  {SYS, {ACAO, {.acao = {MOVER, NOROESTE}}}},
  {SYS, {ACAO, {.acao = {MOVER, NORDESTE}}}},
  {SYS, {ACAO, {.acao = {MOVER, LESTE}}}}
};

/* instancia uma nova maquina na posição definida */
/* TODO: receber instrucoes como parametro? */
void insereMaquina (Arena* arena, int time, Posicao pos) {
  int i;
  /* cria a maquina na primeira posicao livre da linha */
  for ( i = 0 ; i < arena->maquinas_por_time ; i++ ) {
    if ( arena->maquinas[time-1][i] == NULL ) /* aloca espaço se necessário */
      arena->maquinas[time-1][i] = cria_maquina(programa);
    if ( arena->maquinas[time-1][i]->id == 0 ) { /* senão, só setta as variaveis */
      arena->maquinas[time-1][i]->id = time * 100 + i + 1;
      arena->maquinas[time-1][i]->time = time;
      arena->maquinas[time-1][i]->posicao.i = pos.i;
      arena->maquinas[time-1][i]->posicao.j = pos.j;
      arena->celulas[pos.i][pos.j]->ocupado = 1;
      arena->celulas[pos.i][pos.j]->num_cristais = 0;
      fprintf(arena->display, "rob GILEAD_A.png %d\n", arena->maquinas[time-1][i]->id);
      return;
    }
  }
  printf("Não há mais espaço para novas máquinas nesse time!");
}

/* insere as maquinas de um novo time na arena */
void insereExercito(Arena* arena, int time) {
  int i;
  Posicao base;
  /* posição i dos times 1, 2, 3 e 4 */
  if ( time > 2 )
    base.i = 3*arena->n/4;
  else
    base.i = arena->n/4;

  /* posição j dos times 1, 2, 3 e 4 */
  if ( time % 2 == 0 )
    base.j = 3*arena->m/4;
  else
    base.j = arena->m/4;

  for ( i = 0 ; i < arena->maquinas_por_time ; i++ )
    insereMaquina(arena, time, vizinho(base, i));
}

/* remove um maquina especifica */
void removeMaquina(Arena* arena, int id){
  int pos_i, pos_j;
  /* calcula posicoes da maquina na array */
  int time = id/100;
  int num_maquina = id - time*100 - 1;
  pos_i = arena->maquinas[time-1][num_maquina]->posicao.i;
  pos_j = arena->maquinas[time-1][num_maquina]->posicao.j;
  fprintf(arena->display, "remove rob %d %d %d\n", id, pos_i, pos_j);
  arena->celulas[pos_i][pos_j]->ocupado = FALSE;
  destroi_maquina(arena->maquinas[time-1][num_maquina]);
  arena->maquinas[time-1][num_maquina] = NULL;
}

/* remove as maquinas de um time da arena */
void removeExercito(Arena* arena, int time) {
  int i, pos_i, pos_j;
  for ( i = 0 ; i < arena->maquinas_por_time ; i++ ) {
    removeMaquina(arena, time*100 + i + 1);
  }
}

/* Dada uma posicao ocupada, retorna a máquina que a ocupa */
Maquina* procuraMaquina (Arena* arena, Posicao posicao){
  int i, j;
  Maquina* maquina;
  Celula* celula = arena->celulas[posicao.i][posicao.j];
  
  if (celula->ocupado)
    //Procura a maquina na lista de maquinas
    for (i = 0; i < arena->num_times; i++)
      for (j = 0; j < arena->maquinas_por_time; j++)
        if (arena->maquinas[i][j] &&
            arena->maquinas[i][j]->posicao.i == posicao.i &&
            arena->maquinas[i][j]->posicao.j == posicao.j)
          return arena->maquinas[i][j];           

  return maquina;
}

/* retorna o vizinho na direção especificada de uma posição da arena */
Vizinho* checaVizinho ( Arena* arena, Posicao posicao, int direcao ){
    Posicao pos_vizinha = vizinho( posicao, direcao );
    Vizinho* vizinho;
    vizinho->tipo = PAREDE;
    Celula* celula;
    /* checa se o vizinho está dentro do limite da arena */
    if ( pos_vizinha.i >= 0 && pos_vizinha.j >= 0 && pos_vizinha.i < arena->n && pos_vizinha.j < arena->m ){
      celula = arena->celulas[pos_vizinha.i][pos_vizinha.j];
      if ( celula->base > 0 ){ /* é uma base */
        vizinho->tipo = BASE;
        vizinho->valor.time_base = celula->base;
      } else if ( celula->num_cristais > 0 ){ /* é um deposito de cristais */
        vizinho->tipo = CRISTAL;
        vizinho->valor.num_cristais = celula->num_cristais;
      } else if ( celula->ocupado ){
        vizinho->tipo = ROBO;
        vizinho->valor.robo = procuraMaquina(arena, pos_vizinha);
      } else {
        vizinho->tipo = VAZIO;
      }
    }    

    return vizinho;
}

/* atualiza o ponteiro dos vizinhso de todas as máquinas da arena */
void atualizaVizinhos (Arena* arena){
  int i, j;
  Maquina* maq;
  for ( i = 0 ; i < arena->num_times ; i++ ){
    for ( j = 0 ; j < arena->maquinas_por_time ; j++ ) {
      if (arena->maquinas[i][j] != NULL){
        maq = arena->maquinas[i][j];
        maq->vizinho_nordeste = checaVizinho(arena, maq->posicao, NORDESTE);
        maq->vizinho_leste = checaVizinho(arena, maq->posicao, LESTE);
        maq->vizinho_sudeste = checaVizinho(arena, maq->posicao, SUDESTE);
        maq->vizinho_sudoeste = checaVizinho(arena, maq->posicao, SUDOESTE);
        maq->vizinho_oeste = checaVizinho(arena, maq->posicao, OESTE);
        maq->vizinho_noroeste = checaVizinho(arena, maq->posicao, NOROESTE);
      }
    }
  }
}

void atacar (Arena* arena, Maquina* maquina, int direcao){
  int i, j;
  Maquina* atacada;
  Posicao pos_vizinha = vizinho(maquina->posicao, direcao);
  Celula* cel_vizinha = arena->celulas[pos_vizinha.i][pos_vizinha.j];
  printf("Máquina %d atacando na direcao %d\n", maquina->id, direcao);

  //Verifica se existe uma maquina na posicao a ser atacada  
  if (cel_vizinha->ocupado){
    //Procura a maquina vizinha, na lista de maquinas    
    atacada = procuraMaquina(arena, pos_vizinha);

    atacada->vida -= maquina->dano;

    if (atacada->vida <= 0) {
      printf("maquina %d morreu\n", atacada->id);
      removeMaquina(arena, atacada->id);
    }
  }
  else printf ("Não tem máquinas para atacar nessa direcao\n");
}

/* chamadas de sistemas da maquina para a arena */
void sistema (Arena * arena, Maquina* maquina, TipoAcao tipo, int direcao){
  int i, j;
  Posicao pos_vizinha = vizinho(maquina->posicao, direcao);
  Celula* cel_vizinha = arena->celulas[pos_vizinha.i][pos_vizinha.j];
  Celula* atual = arena->celulas[maquina->posicao.i][maquina->posicao.j];
  Maquina* maq;
  switch (tipo) {
    case NULO:
      break;
    case MOVER:      
      if (cel_vizinha != NULL && !cel_vizinha->ocupado) {
        atual->ocupado = FALSE;
        cel_vizinha->ocupado = TRUE;
        fprintf(arena->display, "%d %d %d %d %d\n", maquina->id, maquina->posicao.i, maquina->posicao.j, pos_vizinha.i, pos_vizinha.j);
        maquina->posicao.i = pos_vizinha.i;
        maquina->posicao.j = pos_vizinha.j;
      }      
      break;
    case RECOLHER:
      if (cel_vizinha->num_cristais > 0){
        cel_vizinha->num_cristais--;
        maquina->num_cristais++;
        fprintf(arena->display, "recolhe %d %d\n", pos_vizinha.i, pos_vizinha.j);
      }
      break;
    case DEPOSITAR:
      if (maquina->num_cristais > 0){        
        maquina->num_cristais--;
        cel_vizinha->num_cristais++;
        if (cel_vizinha->num_cristais >= 5) 
          printf("======\nTime %d venceu a partida!!\n======\n", maquina->time);
      }
      break;
    case ATACAR:
      atacar(arena, maquina, direcao);
      break;
  }
}

/* avança um timestep (correspondente ao numero de instrucoes) para todos os robos */
void atualiza (Arena* arena, int num_instrucoes) {
  int i, j;
  Acao acao;
  /* executa as instruções */
  for ( i = 0 ; i < arena->num_times ; i++ ){
    for ( j = 0 ; j < arena->maquinas_por_time ; j++ ) {
      if ( arena->maquinas[i][j] != NULL && arena->maquinas[i][j]->id != 0) {
        acao = exec_maquina(arena->maquinas[i][j], num_instrucoes);
        if ( acao.tipo != NULO ) /* caso tenha chamada de sistema */
          sistema(arena, arena->maquinas[i][j], acao.tipo, acao.direcao);
      }
    }
  }

  atualizaVizinhos(arena);
  arena->tempo++;
}


/* inicializa a Arena e seus atributos */
Arena* inicializa (int n, int m, int num_times) {
  Arena* arena = (Arena*) malloc (sizeof(Arena));
  arena->display = popen("./apres", "w");
  if (arena->display == NULL) {
    fprintf(stderr,"Não encontrei o programa de exibição\n");
    return NULL;
  }
  arena->tempo = 0;
  arena->maquinas_por_time = 6;
  arena->n = n;
  arena->m = m;
  arena->num_times = num_times;
  arena->bases = malloc(num_times * sizeof(Posicao));
  arena->celulas = inicializaCelulas(n, m, num_times, arena->display);
  arena->maquinas = inicializaMaquinas(num_times, arena->maquinas_por_time, programa);
  for (int time = 1; time <= num_times; time++)
    insereExercito (arena, time);
  return arena;
}

/* ========================== TESTES =========================== */


/* função teste para verificar a inicialização correta da arena */
void imprimeCelulas (Arena* arena) {
  int i, j;
  for ( i = 0 ; i < arena->n ; i++ )
    for ( j  = 0 ; j < arena->m ; j++ )
      //if (arena->celulas[i][j]->num_cristais != 0)
        printf("celula[%d][%d] - terreno: %d, cristais: %d, base: %d\n",
         i, j, arena->celulas[i][j]->tipo_terreno, arena->celulas[i][j]->num_cristais,
         arena->celulas[i][j]->base);
}

/* função teste para verificar a inicialização correta das masquinas */
void imprimeMaquinas (Arena* arena) {
  printf("Maquinas atuais:\n");
  for (int i = 0; i < arena->num_times; i++)
    for (int j = 0; j < arena->maquinas_por_time; j++){
      if (arena->maquinas[i][j] != NULL) {
        printf("Maquina %d - time %d, pos: [%d, %d]\n", arena->maquinas[i][j]->id,
          arena->maquinas[i][j]->time, arena->maquinas[i][j]->posicao.i, arena->maquinas[i][j]->posicao.j);
      }
    }
}

/* testa inclusão e removção de exércitos */
void testaExercitos(Arena* arena){
  imprimeMaquinas(arena);
  int i;
  for (i = 1 ; i < 4 ; i++){
    printf("----\nRemovendo exercito %d\n----\n", i);
    removeExercito(arena, i);
    imprimeMaquinas(arena);
  }
}

/* funções teste para verificar as chamadas de sistema */
void testaMovimento(Arena* arena){
  int i;
  Maquina* maquina;
  removeExercito(arena, 1);
  removeExercito(arena, 2);
  removeExercito(arena, 3);
  for (i = 0 ; i < 5 ; i++)
    removeMaquina(arena, 400 + i + 1);
  INSTR programa[] = {
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, SUDESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, SUDOESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, NOROESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, NORDESTE}}}},
  };
  maquina = arena->maquinas[3][5];
  maquina->prog = programa;

  for (i = 0 ; i < 6 ; i++){    
    imprimeMaquinas(arena);
    printf("Maquina %d moveu para direcao %d\n", maquina->id, maquina->prog[maquina->ip].op.valor.acao.direcao);
    atualiza(arena, 1);
  }
}

void testaAtaque(Arena* arena){
  int i;
  Maquina *maq1, *maq2;
  imprimeMaquinas(arena);
  removeExercito(arena, 1);
  removeExercito(arena, 2);
  for (i = 0 ; i < 5 ; i++){
    removeMaquina(arena, 300 + i + 1);
    removeMaquina(arena, 400 + i + 1);
  }
  maq1 = arena->maquinas[2][5];
  maq2 = arena->maquinas[3][5];
  INSTR programa1[] = {
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {ATACAR, LESTE}}}},
    {SYS, {ACAO, {.acao = {ATACAR, LESTE}}}},
    {SYS, {ACAO, {.acao = {ATACAR, LESTE}}}}
  };
  INSTR programa2[] = {
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {ATACAR, OESTE}}}},
    {SYS, {ACAO, {.acao = {ATACAR, OESTE}}}},
    {SYS, {ACAO, {.acao = {ATACAR, OESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, SUDESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, NOROESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, NOROESTE}}}},
  };
  maq1->prog = programa1;
  maq2->prog = programa2;
  for (i = 0 ; i < 12 ; i++)
    atualiza(arena, 1);
}

void testaColeta(Arena* arena){
  int i;
  Maquina *maq1, *maq2;
  imprimeMaquinas(arena);
  removeExercito(arena, 1);
  removeExercito(arena, 2);
  removeExercito(arena, 3);
  for (i = 0 ; i < 5 ; i++){
    removeMaquina(arena, 400 + i + 1);
  }
  maq1 = arena->maquinas[3][5];
  INSTR programa1[] = {
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {RECOLHER, OESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, NOROESTE}}}},
    {SYS, {ACAO, {.acao = {RECOLHER, SUDOESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {RECOLHER, SUDESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, SUDOESTE}}}},
    {SYS, {ACAO, {.acao = {RECOLHER, LESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, SUDESTE}}}},
    {SYS, {ACAO, {.acao = {RECOLHER, NORDESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {RECOLHER, NOROESTE}}}},
  };
  maq1->prog = programa1;
  if (arena->celulas[14][11]->num_cristais < 5){
    arena->celulas[14][11]->num_cristais = 5;
    fprintf(arena->display, "cristal 5 %d %d\n", 14, 11);
  }
  for (i = 0 ; i < 14 ; i++)
    atualiza(arena, 1);
}

void testaDeposita(Arena* arena){
  int i;
  removeExercito(arena, 1);
  removeExercito(arena, 2);
  removeExercito(arena, 3);
  for (i = 0 ; i < 5 ; i++){
    removeMaquina(arena, 400 + i + 1);
  }
  Maquina* maquina = arena->maquinas[3][5];
  maquina->num_cristais = 5;
  INSTR programa[] = {
    {SYS, {ACAO, {.acao = {MOVER, LESTE}}}},
    {SYS, {ACAO, {.acao = {DEPOSITAR, SUDOESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, SUDESTE}}}},
    {SYS, {ACAO, {.acao = {DEPOSITAR, OESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, SUDOESTE}}}},
    {SYS, {ACAO, {.acao = {DEPOSITAR, NOROESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, OESTE}}}},
    {SYS, {ACAO, {.acao = {DEPOSITAR, NORDESTE}}}},
    {SYS, {ACAO, {.acao = {MOVER, NOROESTE}}}},
    {SYS, {ACAO, {.acao = {DEPOSITAR, LESTE}}}}    
  };
  maquina->prog = programa;
  for (i = 0 ; i < 10 ; i++){
    atualiza(arena, 1);
    printf("maquina %d, numero de cristais: %d\n", maquina->id, maquina->num_cristais);
    printf("base 4, numero de cristais: %d\n", arena->celulas[15][15]->num_cristais);
  }
}

int main () {
  int n, m, times, i, a, b;
  n = 20;
  m = 20;
  times = 4;

  Arena* arena = inicializa(n, m, times);
  
  //testaMovimento(arena);
  //testaAtaque(arena);
  //testaColeta(arena);
  testaDeposita(arena);
  pclose(arena->display);
  return 0;
}
