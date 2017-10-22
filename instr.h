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
typedef enum {
  NUM,
  ACAO,
  VAR,
  CELULA
} Tipo;

typedef struct {
  int tipo_terreno;
  int num_cristais;
  int ocupado;
  int base;
} Celula;

typedef struct {
  Tipo tipo;
  union {
 	int n;
 	int ac;
 	int v;
  Celula cel;
  } valor;
} OPERANDO;


/* Instrução */
typedef struct {
  OpCode instr;
  OPERANDO op;
} INSTR;
