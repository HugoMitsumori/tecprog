#include <stdio.h>
#include <stdlib.h>
#include "maq.h"

/* Hugo Mitsumori 8941262
   Paulo Araujo   8941112
*/
#define SOMA 0
#define SUBTRACAO 1
#define MULTIPLICACAO 2
#define DIVISAO 3

//#define DEBUG

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
  "STL",
  "RCE",
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
  "FRE",
  "ALC",
  "ATR",
  "SYS"
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
  Maquina *m = (Maquina*)calloc(1, sizeof(Maquina));
  if (!m) Fatal("Memória insuficiente",4);
  m->vida = 1;
  m->ip = 0;
  m->prog = p;
  return m;
}

void destroi_maquina(Maquina *m) {
  free(m);
}

/* Alguns macros para facilitar a leitura do código */
#define ip (m->ip)
#define pil (&m->pil)
#define exec (&m->exec)
#define prg (m->prog)
#define rbp (m->rbp)

int checaNumero(Pilha* pilha);
int checa2Numero(Pilha* pilha);
void operacao(Pilha* pilha, int operacao);

Acao exec_maquina(Maquina *m, int n) {
  int i;
  rbp = 0;
  Acao acao; /* Caso não tenha chamada de sistema, retorna Ação NULO */
  acao.tipo = NULO;
  acao.direcao = 0;
  for (i = 0; i < n; i++) {
    OpCode   opc = prg[ip].instr;
    OPERANDO arg = prg[ip].op;
    D(printf("%3d: %-4.4s %d\n     ", ip, CODES[opc], arg));
    switch (opc) {
      OPERANDO tmp;
      OPERANDO op1;
      OPERANDO op2;
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
      operacao(pil, SOMA);
      break;
    case SUB:
      operacao(pil, SUBTRACAO);
      break;
    case MUL:
      operacao(pil, MULTIPLICACAO);
      break;
    case DIV:
      operacao(pil, DIVISAO);
      break;
    case JMP:
      ip = arg.valor.n;
      continue;
    case JIT:
      if ( checaNumero(pil) && desempilha(pil).valor.n != 0) {
        ip = arg.valor.n;
        continue;
      }
      break;
    case JIF:
      if ( checaNumero(pil) && desempilha(pil).valor.n == 0) {
        ip = arg.valor.n;
        continue;
      }
      break;
    case CALL:
      op1.tipo = op2.tipo = NUM;
      op1.valor.n = ip;
      op2.valor.n = rbp;
      empilha(exec, op1);
      empilha(exec, op2);
      rbp = exec->topo - 2;
      ip = arg.valor.n;
      continue;
    case RET:
      rbp = desempilha(exec).valor.n;
      ip = desempilha(exec).valor.n;
      break;
    case EQ:
      if (checa2Numero(pil)) {
        tmp.tipo = NUM;
        if ( tmp.valor.n == desempilha(pil).valor.n)
          tmp.valor.n = TRUE;
        else
          tmp.valor.n = FALSE;
        empilha(pil, op1);
      }
      break;
    case GT:
      if (checa2Numero(pil)) {
        tmp.tipo = NUM;
        if ( tmp.valor.n < desempilha(pil).valor.n)
          tmp.valor.n = TRUE;
        else
          tmp.valor.n = FALSE;
        empilha(pil,op1);
      }
      break;
    case GE:
      if (checa2Numero(pil)) {
        tmp.tipo = NUM;
        if ( tmp.valor.n <= desempilha(pil).valor.n)
          tmp.valor.n = TRUE;
        else
          tmp.valor.n = FALSE;
        empilha(pil,op1);
      }
      break;
    case LT:
      if (checa2Numero(pil)) {
        tmp.tipo = NUM;
        if ( tmp.valor.n > desempilha(pil).valor.n)
          tmp.valor.n = TRUE;
        else
          tmp.valor.n = FALSE;
        empilha(pil,op1);
      }
      break;
    case LE:
      if (checa2Numero(pil)) {
        tmp.tipo = NUM;
        if ( tmp.valor.n >= desempilha(pil).valor.n)
          tmp.valor.n = TRUE;
        else
          tmp.valor.n = FALSE;
        empilha(pil,op1);
      }
      break;
    case NE:
      if (checa2Numero(pil)) {
        tmp.tipo = NUM;
        if ( tmp.valor.n != desempilha(pil).valor.n)
          tmp.valor.n = TRUE;
        else
          tmp.valor.n = FALSE;
        empilha(pil,op1);
      }
      break;
    case STO:
      m->Mem[arg.valor.n] = desempilha(pil);
      break;
          case RCL:
      empilha(pil,m->Mem[arg.valor.n]);
      break;
    case END:
      return acao;
    case PRN:
      printf("%d ()\n", desempilha(pil).valor.n);
      break;
    case STL:
      exec->val[rbp + arg.valor.n + 1] = desempilha(pil);
      break;
          case RCE:
      empilha(pil, exec->val[rbp + arg.valor.n + 1]);
      break;
    case ALC:
      if (checaNumero(pil))
        exec->topo += arg.valor.n;
      break;
    case FRE:
      if (checaNumero(pil))
        exec->topo -= arg.valor.n;
      break;
    case ATR:
      op1 = desempilha (pil);
      op2.tipo = op1.tipo;
      if (op2.tipo == ACAO){
        if (arg.valor.n == 0) op2.valor.n = op1.valor.acao.tipo;
        else if (arg.valor.n == 1) op2.valor.n = op1.valor.acao.direcao;
      }
      else if (op2.tipo == CELULA){
        if (arg.valor.n == 0) op2.valor.n = op1.valor.cel.tipo_terreno;
        else if (arg.valor.n == 1) op2.valor.n = op1.valor.cel.num_cristais;
        else if (arg.valor.n == 2) op2.valor.n = op1.valor.cel.ocupado;
        else if (arg.valor.n == 3) op2.valor.n = op1.valor.cel.base;
      }
      empilha (pil, op2);
      break;
    case SYS:
      if (arg.tipo == ACAO){
        return arg.valor.acao;
      }
      break;
    }
    D(puts("Pilha de dados:") );
    D(imprime(pil,5));
    D(puts("\nPilha de execucao:"));
    D(imprime(exec,5));
    D(puts("\n"));
    ip++;
  }
  return acao;
}

/* executa a operação dada utilizando os 2 valores do topo da pilha */
void operacao (Pilha* pilha, int operacao) {
  OPERANDO op1 = desempilha(pilha);
  OPERANDO op2 = desempilha(pilha);
  OPERANDO res;
  if (op1.tipo == NUM && op2.tipo == NUM) {
    res.tipo = NUM;
    switch (operacao) {
    case SOMA:
      res.valor.n = op1.valor.n + op2.valor.n; break;
    case SUBTRACAO:
      res.valor.n = op1.valor.n - op2.valor.n; break;
    case MULTIPLICACAO:
      res.valor.n = op1.valor.n*op2.valor.n; break;
    case DIVISAO:
      res.valor.n = op1.valor.n/op2.valor.n; break;
    }
    empilha(pilha, res);
  } else {
    empilha(pilha, op2);
    empilha(pilha, op1);
    printf("!!Tentativa de operação com tipo incompatível!!\n");
  }
}

/* verifica se o valor no topo da pilha é do tipo NUM */
int checaNumero(Pilha* pilha){
  if (espia(pilha).tipo == NUM)
    return TRUE;
  else
    return FALSE;
}

/* verifica se os 2 valores no topo da pilha são do tipo NUM */
int checa2Numero(Pilha* pilha){
  OPERANDO op = desempilha(pilha);
  if (espia(pilha).tipo == NUM && op.tipo == NUM){
    empilha(pilha, op);
    return TRUE;
  }
  empilha(pilha, op);
  return FALSE;
}
