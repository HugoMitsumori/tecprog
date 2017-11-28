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
    fprintf(display, "cristal %dc.png %d %d\n", arena[i][j]->num_cristais, i, j);
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
  {SYS, {ACAO, {.acao = {MOVER, NOROESTE}}}},
  {SYS, {ACAO, {.acao = {MOVER, SUL}}}},
  {SYS, {ACAO, {.acao = {MOVER, NOROESTE}}}},
  {SYS, {ACAO, {.acao = {MOVER, NORDESTE}}}},
  {SYS, {ACAO, {.acao = {MOVER, SUL}}}},
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


/* remove as maquinas de um time da arena */
void removeExercito(Arena* arena, int time) {
  int i, pos_i, pos_j;
  for ( i = 0 ; i < arena->maquinas_por_time ; i++ ) {
    pos_i = arena->maquinas[time-1][i]->posicao.i;
    pos_j = arena->maquinas[time-1][i]->posicao.j;
    arena->celulas[pos_i][pos_j]->ocupado = FALSE;
    destroi_maquina(arena->maquinas[time-1][i]);
    arena->maquinas[time-1][i] = NULL;
  }

}

Vizinho* checaVizinho(Arena* arena, Posicao posicao, int direcao){
    Posicao pos_vizinha = vizinho(posicao, direcao);
    Vizinho* vizinho;
    vizinho->tipo = PAREDE;
    if (pos_vizinha.i >= 0 && pos_vizinha.j >= 0 && pos_vizinha.i < arena->n && pos_vizinha.j < arena->m){
        if (arena->celulas[pos_vizinha.i][pos_vizinha.j]->base > 0)
          vizinho->tipo = BASE;
          vizinho->valor.time_base = arena->celulas[pos_vizinha.i][pos_vizinha.j]->base;
        }    

    return vizinho;
}

void atualizaVizinhos(Arena* arena){
  int i, j;
  Maquina* maq;
  for ( i = 0 ; i < arena->num_times ; i++ ){
    for ( j = 0 ; j < arena->maquinas_por_time ; j++ ) {
      maq = arena->maquinas[i][j];
      maq->vizinho_norte = checaVizinho(arena, maq->posicao, NORTE);
      maq->vizinho_nordeste = checaVizinho(arena, maq->posicao, NORDESTE);
      maq->vizinho_sudeste = checaVizinho(arena, maq->posicao, SUDESTE);
      maq->vizinho_sul = checaVizinho(arena, maq->posicao, SUL);
      maq->vizinho_sudoeste = checaVizinho(arena, maq->posicao, SUDOESTE);
      maq->vizinho_noroeste = checaVizinho(arena, maq->posicao, NOROESTE);
    }
  }
}

/* chamadas de sistemas da maquina para a arena */
void sistema (Arena * arena, Maquina* maquina, TipoAcao tipo, int direcao){
  int i, j;
  Posicao pos_vizinha = vizinho(maquina->posicao, direcao);
  Celula* cel_vizinha = arena->celulas[pos_vizinha.i][pos_vizinha.j];
  Celula* atual = arena->celulas[maquina->posicao.i][maquina->posicao.j];
  switch (tipo) {
    case NULO:
      break;
    case MOVER:
      if (!cel_vizinha->ocupado) {
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
      }
      break;
    case DEPOSITAR:
      if (maquina->num_cristais > 0){
        maquina->num_cristais--;
        cel_vizinha->num_cristais++;
      }
      break;
    case ATACAR:
      //Verifica se existe uma maquina na posicao a ser atacada
      if (cel_vizinha->ocupado){
      //Procura a maquina vizinha, na lista de maquinas
        for (i = 0; i < arena->num_times; i++)
          for (j = 0; j < arena->maquinas_por_time; j++)
            if (arena->maquinas[i][j] &&
                arena->maquinas[i][j]->posicao.i == pos_vizinha.i &&
                arena->maquinas[i][j]->posicao.j == pos_vizinha.j){
              //Ataca maquina vizinha
              arena->maquinas[i][j]->vida--;
              arena->maquinas[i][j]->dano++;
              //Se maquina vizinha morreu, remove-a da lista e da celula vizinha
              if (arena->maquinas[i][j]->vida == 0){
                printf("maquina %d morreu\n", arena->maquinas[i][j]->id);
                fprintf(arena->display, "remove rob %d\n", arena->maquinas[i][j]->id);
                arena->maquinas[i][j] = NULL;
                cel_vizinha->ocupado = 0;
              }
            }
      }
      else puts ("Nao tem ninguem ali\n");
      break;
  }
}

/* avança um timestep (correspondente ao numero de instrucoes) para todos os robos */
void atualiza(Arena* arena, int num_instrucoes) {
  int i, j;
  Acao acao;
  /* executa as instruções */
  for ( i = 0 ; i < arena->num_times ; i++ ){
    for ( j = 0 ; j < arena->maquinas_por_time ; j++ ) {
      if ( arena->maquinas[i][j] != NULL && arena->maquinas[i][j]->id != 0) {
        acao = exec_maquina(arena->maquinas[i][j], num_instrucoes);
        //printf("Executando instrucao %d\n", acao.tipo);
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
void imprimeCelulas(Arena* arena, int n, int m ) {
  int i, j;
  for ( i = 0 ; i < n ; i++ )
    for ( j  = 0 ; j < m ; j++ )
      if (arena->celulas[i][j]->num_cristais != 0)
        printf("celula[%d][%d] - terreno: %d, cristais: %d, base: %d\n",
         i, j, arena->celulas[i][j]->tipo_terreno, arena->celulas[i][j]->num_cristais,
         arena->celulas[i][j]->base);
}

/* função teste para verificar a inicialização correta das masquinas */
void imprimeMaquinas (Arena* arena) {
  for (int i = 0; i < arena->num_times; i++)
    for (int j = 0; j < arena->maquinas_por_time; j++){
      if (arena->maquinas[i][j] != NULL) {
        printf("Maquina %d - time %d, pos: [%d, %d]\n", arena->maquinas[i][j]->id,
          arena->maquinas[i][j]->time, arena->maquinas[i][j]->posicao.i, arena->maquinas[i][j]->posicao.j);
      }
    }
}

int main () {
  int n, m, times, i, a, b;
  n = 20;
  m = 20;
  times = 4;

  Arena* arena = inicializa(n, m, times);

  imprimeCelulas(arena, n, m);
  //removeExercito(arena, 3);

  //removeExercito(arena, 1);
  //imprimeMaquinas(arena);  


  for (i = 0 ; i < 4 ; i ++){    
    atualiza(arena, 1);
  }
  pclose(arena->display);
  return 0;
}
