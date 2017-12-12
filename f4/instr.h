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
  STS,
  RCS,
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
  ATR,
  SIS,							/* chamada de sistema */
  ENTRY,
  LEAVE
} OpCode;

/* Tipos dos operandos */
typedef enum {
  NUM,
  ACAO,
  VAR,
  CELULA
} Tipo;

/* Tipo de terreno */
typedef enum {
  ESTRADA,
  MONTANHA,
  RIO,
  BASE
} Terreno;


/* Célula */
typedef struct {
  Terreno terreno;
  short int cristais;
  short int ocupado;
} Celula;

/* Alternativamente, Celula pode ser implementada como um vetor de ints
   ou short ints, isso torna a implementação de ATR mais fácil */

/* No caso da ação, o tipo do operando é um inteiro indicando a direção,
   mas fique livre para fazer de forma diferente */

/* Operando */
typedef struct {
  Tipo t;
  union {
	int n;
	int ac;
	double v;
	Celula cel;
  } val;
} OPERANDO;

/* Instrução */
typedef struct {
  OpCode instr;
  OPERANDO op;
} INSTR;
