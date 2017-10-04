#define ESTRADA 1
#define MONTANHA 2
#define RIO 3
#define TRUE 1
#define FALSE 0

typedef struct {
  int tipo_terreno;
  int num_cristais;
  int ocupado;
  int base;
} Celula;

void incializa ();
void atualiza ();
void insereExercito();
void removeExercito();
void sistema (int op);
