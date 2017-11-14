#include "posicao.h"
#define TRUE 1
#define FALSE 0


/* Códigos das instruções */
typedef enum {
  PUSH,
  POP,
  DUP,
  ADD,
  SUB,
  MUL,
  DIV,
  JMP,
  JIT,
  JIF,
  CALL,
  RET,
  STL,
  RCE,
  EQ,
  GT,
  GE,
  LT,
  LE,
  NE,
  STO,
  RCL,
  END,
  PRN,
  FRE,
  ALC,
  ATR,
  SYS
} OpCode;

/* Tipos dos operandos */
/* no momento, são todos inteiros */


typedef struct {
  int tipo_terreno;
  int num_cristais;
  int ocupado;
  int base;
} Celula;

typedef enum {
  NULO,
  MOVER,
  RECOLHER,
  DEPOSITAR,
  ATACAR
} TipoAcao;

typedef struct {
  TipoAcao tipo;
  int direcao;
} Acao;

typedef enum {
  NUM,
  ACAO,
  VAR,
  CELULA
} Tipo;

typedef struct {
  Tipo tipo;
  union {
    int n;
    int v;
    Acao acao;
    Celula cel;
  } valor;
} OPERANDO;

/* Instrução */
typedef struct {
  OpCode instr;
  OPERANDO op;
} INSTR;
