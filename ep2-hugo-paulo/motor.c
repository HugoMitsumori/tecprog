#include <stdio.h>
#include "maq.h"

INSTR programa[] = {
  {PUSH, {NUM, 8}},
  {CALL, {NUM, 4}},
  {PRN, {NUM, 0}},
  {END, {NUM, 8}},
  {ALC, {NUM, 1}},
  {DUP, {NUM, 0}},
  {STL, {NUM, 1}},
  {PUSH, {NUM, 1}},
  {EQ, {NUM, 0}},
  {JIF, {NUM, 13}},
  {PUSH, {NUM, 1}},
  {FRE, {NUM, 1}},
  {RET, {NUM, 0}},
  {RCE, {NUM, 1}},
  {PUSH, {NUM, 1}},
  {SUB, {NUM, 0}},
  {CALL, {NUM, 4}},
  {RCE, {NUM, 1}},
  {MUL, {NUM, 0}},
  {FRE, {NUM, 1}},
  {RET, {NUM, 0}},
};

int main(int ac, char **av) {
	Maquina *maq = cria_maquina(programa);
	exec_maquina(maq, 1000);
	destroi_maquina(maq);
	return 0;
}
