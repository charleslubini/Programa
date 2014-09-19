#ifndef PROCESSO_H_INCLUDED
#define PROCESSO_H_INCLUDED


#include <stdlib.h>
#include <stdio.h>


int Pid = 0;

/*
hu3hu3hu3hu3hu3hu3hu3hu3hu3hu3hu3hu3hu3
*/

typedef struct {
    int PID;
    char dado;
    int prioridade;
    int t_chegada;
    int t_chegada_A;
    int t_inicio;
    int t_execucao;
    int t_execucao2;
    int t_saida;
    int t_restante;
    int t_servicoE;
    char t_contexto;
    int tm;
    int vz_processador;
    int tamanho;
    int primeiraPag;
    int ultimaPag;
    int memoria;
}Processo;

void inicializarPID() {
     Pid = 1;
}

int pegarPID() {
    return Pid;
}


void atualizarPID(){
     Pid++;
}

void ver_processo(Processo *ps) {
      printf("\n %d |     %d      |    %d      |    %d      |    %d    ", ps->PID, ps->prioridade, ps->t_chegada, ps->t_chegada_A, ps->t_execucao);
}


#endif // PROCESSO_H_INCLUDED

