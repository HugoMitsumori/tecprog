#include <stdio.h>
#include <stdlib.h>
#include "maq.h"

/* #define DEBUG */

#ifdef DEBUG
#  define D(X) X
char *CODES[] = {
  "PUSH",
  "POP",
  "DUP",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "JMP",
  "JIT",
  "JIF",
  "CALL",
  "RET",
  "STS",
  "RCS",
  "EQ",
  "GT",
  "GE",
  "LT",
  "LE",
  "NE",
  "STO",
  "RCL",
  "END",
  "PRN",
  "ATR",
  "SIS",
  "ENTRY",
  "LEAVE"
};
#else
#  define D(X)
#endif

static void Erro(char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void Fatal(char *msg, int cod) {
  Erro(msg);
  exit(cod);
}

Maquina *cria_maquina(INSTR *p) {
  Maquina *m = (Maquina*)malloc(sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  m->ip = 0;
  m->prog = p;
  m->pil.topo = 0;
  m->ib = 0;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

int new_frame(Maquina *m, int n) {
  int ibc = m->ib;
  if (ibc < MAXFRM-1) {
	m->bp[++m->ib] = n+ibc;
	return m->ib;
  }
  return -1;
}

int del_frame(Maquina *m) {
  if (m->ib > 0) return --m->ib;
  return -1;
}

/* Alguns macros para facilitar a leitura do código */
#define ip (m->ip)
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)

void exec_maquina(Maquina *m, int n) {
  int i;

  for (i = 0; i < n; i++) {
	OpCode   opc = prg[ip].instr;
	OPERANDO arg = prg[ip].op;

	D(printf("%3d: %-5.5s %d\n", ip, CODES[opc], arg.val.n));

	switch (opc) {
	  OPERANDO tmp;
	  OPERANDO op1;
	  OPERANDO op2;
	  OPERANDO res;
	  
	case PUSH:
	  empilha(pil, arg);
	  break;
	case POP:
	  desempilha(pil);
	  break;
	case DUP:
	  tmp = desempilha(pil);
	  empilha(pil, tmp);
	  empilha(pil, tmp);
	  break;
	case ADD:
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op1.val.n  + op2.val.n;
		empilha(pil, res);
	  }
	  break;
	case SUB:
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op2.val.n  - op1.val.n;
		empilha(pil, res);
	  }

	  break;
	case MUL:
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op1.val.n  * op2.val.n;
		empilha(pil, res);
	  }
	  break;
	case DIV:
	  op1 = desempilha(pil);
	  op2 = desempilha(pil);

	  if (op1.t == NUM && op2.t == NUM) {
		res.t = NUM;
		res.val.n = op1.val.n  / op2.val.n;
		empilha(pil, res);
	  }
	  break;
	case JMP:
	  ip = arg.val.n;
	  continue;

	case JIT:
	  if (desempilha(pil).val.n != 0) {
		ip = arg.val.n;
		continue;
	  }
	  break;
	case JIF:
	  if (desempilha(pil).val.n == 0) {
		ip = arg.val.n;
		continue;
	  }
	  break;
	case CALL:
	  op1.t = NUM;
	  op1.val.n = ip;
	  empilha(exec, op1);
	  ip = arg.val.n;
	  continue;
	case RET:
	  ip = desempilha(exec).val.n;
	  break;

	case EQ:
	  if (desempilha(pil).val.n == desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case GT:
	  if (desempilha(pil).val.n < desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case GE:
	  if (desempilha(pil).val.n <= desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case LT:
	  if (desempilha(pil).val.n > desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case LE:
	  if (desempilha(pil).val.n >= desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case NE:
	  if (desempilha(pil).val.n != desempilha(pil).val.n)
		empilha(pil, (OPERANDO) {NUM,{1}});
	  else
		empilha(pil, (OPERANDO) {NUM,{0}});
	  break;

	case STO:
	  m->Mem[arg.val.n+m->bp[m->ib]] = desempilha(pil);
	  break;
	  
	case RCL:
	  empilha(pil,m->Mem[arg.val.n+m->bp[m->ib]]);
	  break;
	  
	case END:
	  pil->topo = 0;
	  return;
	  
	case PRN:
	  printf("%d\n", desempilha(pil).val.n);
	  break;
	  
	case ENTRY:
	  new_frame(m, arg.val.n);
	  break;
	  
	case LEAVE:
	  del_frame(m);
	  break;
	}

	D(imprime(pil,5));
	D(puts("\n"));

	ip++;
  }
}
