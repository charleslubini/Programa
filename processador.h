
    #include <stdlib.h>
    #include <stdio.h>
    #include <time.h>
    
  
    #include "filaProcessos.h"
    #include "processos.h"
    #include "boolean.h"

    
    /* VARIÁVEIS GLOBAIS DE AMBIENTE =================================================================================================== */
    int QUANTUM;
    int TROCA_CONTEXTO; //Tempo de troca de contexto
    int T_EXECUCAO = 100; //Tempo de execu�ao
    int PRIORIDADE; //Prioridade mais baixa
    bool AGIN; 
    int TCA=0; //
    short int ESTADO_PCDR = 0; //Estado do processador
    int N_PROCESSOS = 10;
    
    int TIME = 0; //Controlador de tempo
    int USE_TIME = 1; //Tempo usado por ciclo
    int USE_TIME_SLEEP = 1000; //Tempo de espera por ciclo (Sleep function)
    short int FILA_VAZIA = 1;
    int N_PROCESSOS_NOW = 0;
    Fila filaAptos;
    Processo *process;
    
    /*===================================================================================================================================*/
    
    /* DECLARAÇÃO DE FUNÇÕES =========================================================================================================== */
    void menu(); //
    void novo_processo(); //Funcaoo que cria/estancia um processo
    void processador(Processo *ps);
    short int estanciarProcessos2(Fila *aptos);
    short int estanciarProcessos(Processo *prcs);
    short int entraProcessos(Processo *prcs, Fila *aptos);
    void verProcesso(Processo ps);
    void verTabelaProcessos(Processo *prcs);
    short int trocaProcesso(Fila *aptos, Processo *processador, short int terminou);
    int verificarProcessos(Processo *prcs);
    short int calcularEstatisticas(Processo *prcs);
    short int temProcessos(Processo *prcs);
    short int primeiroProcesso(Fila *aptos);
    char matarProcesso(int d);
    void matarTodosProcessos();
    char caracter(int p);
     void prioridade();
    /*===================================================================================================================================*/
    
    
    void inicializa2(){
    inicializarPID();
         inicializaFila(&filaAptos, N_PROCESSOS);
  }
    
       void inicializa(int q, bool a, bool t, int p){
         inicializarPID();
         inicializaFila(&filaAptos, N_PROCESSOS);
         QUANTUM = q;
         PRIORIDADE = p;
         AGIN = a;
         if (t == true){
               TROCA_CONTEXTO = rand()%2;      
         }
         else{
                TROCA_CONTEXTO = 0;   
         }
    }  
         
  
    Processo p_processador,p_aux;
    Processo *prcs;
    short int flagPrPs = 1;
    short int control = 1;
    int i = 0;
    int ctrlTime = 0;
    
    
    void setSO(Processo *p) {
         prcs = p;     
         flagPrPs = 1;
         control = 1;
         ctrlTime = 0;
         i = 0;
    }
    
    void processador(Processo *ps) {
         short int control = 1;
         process = ps;
         if(ps->PID > 0)
         {        
                  ps->t_execucao -= USE_TIME;
                  if(ps->t_execucao == 0)
                  {                 
                               ps->PID = 0;
                  }
                                    
         } 
    }
    
    
    void novo_processo(Processo *ps, int cont) { //Função que cria o processo                   
             ps[cont].prioridade = rand()%PRIORIDADE + 1; //Prioridade
             ps[cont].t_chegada = TIME; //Tempo de Chegada na fila
             ps[cont].t_chegada_A = TIME; //Tempo de Chegada Anterior
             TCA = ps[cont].t_chegada_A; 
             ps[cont].t_saida = ps[cont].t_execucao + ps[cont].t_chegada;
             ps[cont]. t_servicoE = ps[cont].t_saida + TROCA_CONTEXTO;
             ps[cont].PID = pegarPID();
             atualizarPID();
    }
   
    
   
    void verTabelaProcessos(Processo *prcs) {
        int i=0;
        printf("PID | PRIORIDADE |     TC      |    TCA       |     TE    ");
        printf("\n    |            |             |              |           ");
        while(i < N_PROCESSOS) { //Mostrar todos os processos
                printf("\n");
                ver_processo(&prcs[i]);
                i++;
        }
    }
    
    short int primeiroProcesso(Fila *aptos)
    {
         if( aptos->memo[1].PID == -1 && aptos->memo[0].PID != -1 ) return 1;
         else return 0;
    }
    
    short int trocaProcesso(Fila *aptos, Processo *processador, short int terminou)
    {
         Processo aux;
         processador->t_saida = TIME;
         aux = *processador;
         *processador = getProcesso_topo( aptos );
         processador->t_chegada = TIME;
         Desemfileirar( aptos );
         
        
         if( !terminou && aux.PID != 0) Emfileirar( aptos, aux );
         if( filaVazia(aptos) ) return (1); else return (0);
         
    }
    
    int verificarProcessos(Processo *prcs) {
         int i=0;
         while( i < N_PROCESSOS ) {
                if(prcs[i].t_chegada == TIME) return i;
                i++;
         }
         return (0);       
    }
    
    short int temProcessos(Processo *prcs)
    {
          if( N_PROCESSOS_NOW == N_PROCESSOS ) return (0); else return (1);
    }
    
    char matarProcesso(int d)
    {
         char cara;
         if (process->PID == d){
                printf("\nProcesso morto: %d  - PROCESSANDO",  process->PID);
                cara = process->dado;
                process->PID = -1;       
         }
         int i;
         for (i = 0; i < N_PROCESSOS; i++){
             //printf("\n bosta");
             if (filaAptos.memo[i].PID == d){
                     printf("\nProcesso morto: %d", filaAptos.memo[i].PID);
                     cara = filaAptos.memo[i].dado;
                     filaAptos.memo[i].prioridade = 0;
                     ordenarFila(&filaAptos);
                     Desemfileirar(&filaAptos);
                     break;         
             }    
         }
         return (cara);       
    }
    
    void matarTodosProcessos()
    {
         
         if (process->PID != -1){
                          printf("\nProcesso morto: %d  - PROCESSANDO",  process->PID);
         }
         process->PID = -1;
        
         for (i = 0; i <10; i++)
         {
             if (filaAptos.memo[i].PID != -1){
                printf("\nProcesso morto: %d", filaAptos.memo[i].PID);
                Desemfileirar(&filaAptos);
                //filaAptos.memo[i].PID = -1;
             }
         }  
    }
    
    
    short int entraProcessos(Processo *prcs, Fila *aptos) {
         int i=0,l=0;
         while( i < N_PROCESSOS ) 
         {
                if(prcs[i].t_chegada == TIME) {
                      Emfileirar(aptos,prcs[i]);
                      prcs[i].PID = 0;
                      //printf("\n  - ENTROU %d", prcs[i].PID);
                      l++;
                }
                i++;
         }
         N_PROCESSOS_NOW += l;
        if( l != 0 ) return 1; else return 0;         
    }
    
    char caracter(int p){
         
         if (process->PID == p){
                          return(process->dado);
         }
        
         for (i = 0; i <10; i++)
         {
             if (filaAptos.memo[i].PID == p){
                                       return(filaAptos.memo[i].dado);
             }
         }
         return('-');  
    }
    
    
   void prioridade(){
        if (AGIN == true){
           aumentaPrioridade(&filaAptos);         
        }
   }
