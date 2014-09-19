#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "processos.h"
#include "processador.h"

int tempExe = 0;
Processo pr[100];
int cont = 0;
int contVirt =0;


typedef struct {
      int nroPag;
      int deslocamento;
      char dados;
} MemoriaFisica;

typedef struct {
        int PagFis;
        int PagLog;
        int BitPres;
} TabelaPag;

typedef struct {
        char dados;
        int nroPag;
        int deslocamento;       
} MemoriaVirtual;


typedef struct{
        MemoriaFisica *memFis;
        MemoriaVirtual *memVir;
        TabelaPag *tab;
        Processo *pr;
        int tam;
}Estrutura;


int pid = 0;
int pidA = 0;
int indA = 0;

void ajuda();

void Remover(TabelaPag *tabela ,MemoriaFisica *memo,int pid1,int tamMem, MemoriaVirtual *mv){
     int i,aux,i2,i3,i4,aux1,aux2;
     for(i = 0; i < tamMem; i++){
           if(pr[pid1].primeiraPag <= memo[i].nroPag){
                                aux2 = i;
                                for(i4 = 0; pr[pid1].tamanho >= i4;i4++){
                                       memo[aux2].dados = '-';
                                       tabela[memo[aux2].nroPag].BitPres = 0;
                                }
           }      
     }
     for(i2 = 0; i2 < (tamMem*2); i2++){
           if(pr[pid1].primeiraPag <= mv[i2].nroPag){
                                   aux1 = i2;
                                   for(i3 = 0; pr[pid1].tamanho >= i3 ; i3++){
                                          mv[aux1].dados = '-';
                                          tabela[mv[aux1].nroPag].BitPres = 0;
                                   }
           }      
     }
}

int verificaFis(MemoriaFisica *memo, int tm, int tamMem){
    int aux, cont;
    for (aux = 0; aux < tamMem; aux++){
        if (memo[aux].dados == '-'){
           cont++;                   
        }
        else{
                cont = 0;  
        }
        if (cont == tm){
                 return(1);
        }
        
    }
    return(0);
}

void carrega(MemoriaFisica *memo, MemoriaVirtual *mv, int p, Processo *prc, TabelaPag *tabela, int tamMem){
     int a, bitsPag, cont, i, aux;
     for (a = 0; a < tamMem; a++){
         if (memo[a].dados == prc[p].dado){
            memo[a].dados = '-';                  
         }   
     }
     cont = 0;
     bitsPag = bitsPag - 1;
     for(i = 0;i < tamMem; i++){
           if(prc[p].t_restante != 0){
                  if(memo[i].dados != '-'){
                                  i = i + bitsPag;
                                  continue;
                  }
                       prc[p].memoria = 0;
                       if(memo[i].dados == '-'){
                            memo[i].dados = prc[pid].dado;
                            aux = memo[i].nroPag;
                            prc[p].primeiraPag = aux;
                            tabela[aux].BitPres = 1;
                            prc[p].t_restante--;
                        }
                  }
                  prc[p].ultimaPag = memo[i].nroPag;    
          }
          for (a = 0; a < tamMem; a++){
              if (prc[p].dado == mv[a].dados){
                                 mv[a].dados = '-';
              }    
          }
}


void descarrega(MemoriaFisica *memo, MemoriaVirtual *mv, Fila *f, int tamMem, TabelaPag *tabela, Processo *prc){
     int pi, a, aux, cont, i, bitsPag, i2;
     pi = ultimoFila(&(*f));
     for (a = 0; a < tamMem; a++){
              if (prc[pi].dado == memo[a].dados){
                                 memo[a].dados = '-';
              }    
     }
     cont = 0;
     bitsPag = bitsPag - 1;
     for(i = 0;i < tamMem; i++){
           if(prc[pi].t_restante != 0){
                  if(memo[i].dados != '-'){
                                  i = i + bitsPag;
                                  continue;
                  }
                                  prc[pi].memoria = 1;
                                    if(mv[contVirt].dados != '-'){
                                                    contVirt = contVirt + bitsPag;
                                                    continue;                                                    
                                    } 
                                    else if((prc[pi].primeiraPag >= 0) && (mv[contVirt].dados == '-')){
                                         if (cont == 0){
                                            memo[i].dados = prc[pi].dado;
                                            cont++;      
                                         }
                                         mv[contVirt].dados = prc[pi].dado;
                                         aux = mv[contVirt].nroPag;
                                         prc[pi].t_restante--;
                      
                                    }
                                    else if(mv[i].dados == '-'){
                                             memo[i].dados = prc[pi].dado;
                                             mv[i].dados = '-';
                                             aux = mv[contVirt].nroPag;
                                             prc[pi].primeiraPag = aux;
                                             prc[pi].t_restante--;
                                    }
                                    contVirt++;          
                  }
           }
}



void verificaProcesso(Processo *prc, Fila *f, TabelaPag *tabela, MemoriaFisica *memo, MemoriaVirtual *mv, int p, int tm){
     int a, b, c;
     if (prc->memoria == 1){
        if (verificaFis(&(*memo),prc[p].tm, tm)){
                    descarrega(&(*memo), &(*mv), &(*f), tm, &(*tabela), &(*prc));
                    carrega(&(*memo), &(*mv), p, &(*prc), &(*tabela), tm);
        }
        else{
             carrega(&(*memo), &(*mv), p, &(*prc), &(*tabela), tm);
        }                 
     }
}



void SO(Estrutura *e, Processo *prcs) {
           
           Processo p_processador,p_aux;

           
           
           short int flagPrPs = 1;
           short int control = 1, control2=1;
           int i = 0;
           int ctrlTime = 0;
           int p1, p2;
           int contexto = 0;
           p_processador.PID = 0;
           p_processador.prioridade = 10;
           while( control2 ) //--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
           {
                  
                    
                    zera(&filaAptos);
                    if( p_processador.PID <= 0 ) ESTADO_PCDR = 0; 
                    
                     if( !ESTADO_PCDR && !filaVazia( &filaAptos ) && p_processador.t_execucao == 0 )
                    {
                        trocaProcesso( &filaAptos, &p_processador, 1 ); // ----//---//---- AREA DE TROCA DE PROCESSOS ---//-------//----//
                        contexto = TROCA_CONTEXTO;
                        ESTADO_PCDR = 1;
                        ctrlTime = 0;
                    }
                     if( ctrlTime == QUANTUM )
                     {
                           if( !filaVazia(&filaAptos) )
                           {
                                 p_aux = getProcesso_topo(&filaAptos);
                                 if( p_aux.prioridade <= p_processador.prioridade ) 
                                 {
                                     trocaProcesso( &filaAptos, &p_processador, 0 ); // ----//---//---- AREA DE TROCA DE PROCESSOS ---//-------//----//
                                     contexto = TROCA_CONTEXTO;
                                 }
                           }
                           prioridade();                           
                            ctrlTime = 0;
                    }
                    if( entraProcessos( prcs, &filaAptos ) ) //Caso entrou algum processo na fila de aptos   
                    {   
                           if( !ESTADO_PCDR )
                           {  
                               trocaProcesso(&filaAptos,&p_processador, 0); // ----//---//---- AREA DE TROCA DE PROCESSOS ---//-------//----// 
                               verificaProcesso(&p_processador, &filaAptos, e->tab, e->memFis, e->memVir, p_processador.PID, e->tam);
                               contexto = TROCA_CONTEXTO;
                               ctrlTime=0;
                           } 
                           ESTADO_PCDR = 1;
                    }
                    p1 = p_processador.PID;           
                    processador(&p_processador);  
                    p2 = p_processador.PID;
                    if (p1 != p2){
                       Remover(e->tab,e->memFis,p_processador.dado,e->tam,e->memVir);
                    }
                    
                    if( p_processador.t_execucao == 0 && filaVazia(&filaAptos) ) {
                        p_processador.PID = 0;
                        p_processador.prioridade = 10;    
                        
                    }
                    
                    if( filaVazia(&filaAptos) && p_processador.t_execucao == 0 && flagPrPs == 0 && !temProcessos(prcs) ) 
                    {
                        control2 = 0;
                        
                    }
                    
                    
                    zera(&filaAptos);
                  // pri(&filaAptos);
                    //Pequeno criterio de parada                            
                    TIME += USE_TIME; //Controle de tempo geral
                    ctrlTime += USE_TIME; //Controle de tempo do Sistema operacional
                    i++;
                    sleep(USE_TIME_SLEEP); //Espera o tempo escolhido pelo usuario
                    
                      
           } //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
           
    }


void Thread(void *estrutura){
     int aux, cont;
     int pid;
     Estrutura *e = (Estrutura *)estrutura;
     SO(e, &pr[0]);
}




void inicializaMemo(int bitsPag,int tamMem,int qtdPag,MemoriaFisica *mem){
                   int i,nroPag = 0,aux = 0;
                   for(i = 0;i < tamMem ; i++){
                         if(aux == bitsPag){
                                aux = 0;
                                nroPag++;
                         }
                         if(aux != bitsPag){
                                mem[i].nroPag = nroPag; 
                                mem[i].deslocamento = aux;
                         }
                         aux++;
                         mem[i].dados = '-';
                        
                   }                                  
}


void inicializaMemoVir(int bitsPag,int tamMemVir,int qtdPag,MemoriaVirtual *memV){
                   int i,nroPag = 0,aux = 0;
                   for(i = 0;i < tamMemVir ; i++){
                         if(aux == bitsPag){
                                aux = 0;
                                nroPag++;
                         }
                         if(aux != bitsPag){
                                memV[i].nroPag = nroPag; 
                                memV[i].deslocamento = aux;
                         }
                         aux++;
                         memV[i].dados = '-';
                        
                   }                                  
}




void mostraMemo(int tamMem,MemoriaFisica *mem){
     int i;
     printf("\n\nMemoria Fisica\n\n");
     printf("|  NP  |  DE  |  DA  |\n");
     for(i = 0;i < tamMem;i++){
        if(mem[i].deslocamento == 0){
                               printf(" --------------------\n");
        }           
        printf("|   %d  |   %d  |  %c   |\n",mem[i].nroPag,mem[i].deslocamento,mem[i].dados);
     }   
     printf(" --------------------\n");
     
     
}


void mostraMemoVir(int tamMemVir,MemoriaVirtual *memV){
     int i;
     printf("*--------------------*\n");
     printf("|  Memoria Virtual   |\n");
     printf("*--------------------*\n");
     printf("|  NP  |  DE  |  DA  |\n");
     for(i = 0;i < tamMemVir;i++){
        if(memV[i].deslocamento == 0){
                               printf("*--------------------*\n");
        }           
        printf("|   %d  |   %d  |  %c   |\n",memV[i].nroPag,memV[i].deslocamento,memV[i].dados);
     }   
     printf("*--------------------*\n");
     
     
}




void inicializaTabe_Pag(TabelaPag *tabela,int qtdPag){
     int aux = 0;
     while(aux < qtdPag){
             tabela[aux].PagFis = aux;
             tabela[aux].PagLog = aux;
             tabela[aux].BitPres = 0;
             aux++;
     }
}

void mostraTabe_Pag(TabelaPag *tabela, int qtdPag){
     int aux = 0;
     printf("\n\n\n\n\n\nTabela de Paginas \n\n");
     printf("| NPL | NPF | BP |\n");
     while(aux < qtdPag){
               printf(" -----------------\n");
               printf("|  %d  |  %d  |  %d |\n",tabela[aux].PagFis,tabela[aux].PagLog,tabela[aux].BitPres);
               aux++;
     }
     printf(" -----------------\n");
}

void mostra_tabelaPag_PID(TabelaPag *tabela,MemoriaFisica *memo,int qtdPag,char dado,int tamMem, int pid){
     int i,aux,aux1,index=0;
     printf("\n\nTabela de Paginas do PID %d\n\n",pid);
     printf(" -------------\n");
     printf("|  ID  |  PG  |\n");
     for(i = 0;i < tamMem;i++){
           if(memo[i].dados == dado){
                            for(aux = 0; aux < 8;aux++){
                                if(tabela[aux].PagFis == memo[i].nroPag){
                                             printf("|  %d   |  %d   |\n",index,memo[i].nroPag);
                                             index++;
                                }                        
                            }
           }
     }
      printf(" -------------");
     
}

int impar(int i){
    if(i % 2 == 0){
         return(0);
    }
    else return (1);
        
}

void mostrar_tabela_VirFis(TabelaPag *tabela ,MemoriaFisica *memo, Processo *prc,int tamMem, int qtdPag,int bitsPag,MemoriaVirtual *mv){
     int i, aux, aux1, index=0;
     printf("\n\nTabela de Paginas do PID %d\n\n",pid);
     printf(" -------------\n");
     printf("|  Fis  |  Vir  |\n");
     for(i = 0;i < tamMem;i++){
           if((memo[i].dados != '-' )&&(memo[i].dados != '!' )){
                            for(aux = 0; aux < 8;aux++){
                                if(memo[i].dados == mv[i].dados){
                                             printf("|  %d   |  %d   |\n",memo[i].nroPag, mv[i].nroPag);
                                             index++;
                                }                        
                            }
           }
     }
}




void addDado(TabelaPag *tabela ,MemoriaFisica *memo, Processo *prc,int tamMem, int qtdPag,int bitsPag,MemoriaVirtual *mv){
     int i,aux,aux1,i2, cont;
     cont = 0;
     bitsPag = bitsPag - 1;
     for(i = 0;i < tamMem; i++){
           if(pr[pid].t_restante != 0){
                  aux1 = impar(pid);
                  if(memo[i].dados != '-'){
                                  i = i + bitsPag;
                                  continue;
                  }
                  if(aux1 == 1){
                                    pr[pid].memoria = 1;
                                    if(mv[contVirt].dados != '-'){
                                                    contVirt = contVirt + bitsPag;
                                                    continue;                                                    
                                    } 
                                    else if((pr[pid].primeiraPag >= 0) && (mv[contVirt].dados == '-')){
                                         if (cont == 0){
                                            memo[i].dados = pr[pid].dado;
                                            cont++;      
                                         }
                                         mv[contVirt].dados = pr[pid].dado;
                                         aux = mv[contVirt].nroPag;
                                         pr[pid].t_restante--;
                      
                                    }
                                    else if(mv[i2].dados == '-'){
                                             memo[i].dados = pr[pid].dado;
                                             mv[i].dados = '-';
                                             aux = mv[contVirt].nroPag;
                                             pr[pid].primeiraPag = aux;
                                             pr[pid].t_restante--;
                                    }
                                    contVirt++;
                                    
                             
                  }
                  else{
                       pr[pid].memoria = 0;
                       if(memo[i].dados == '-' && pr[pid].primeiraPag >= 0){  
                            memo[i].dados = pr[pid].dado;
                            aux = memo[i].nroPag;
                            tabela[aux].BitPres = 1;
                            pr[pid].t_restante--;     
                        }else if(memo[i].dados == '-'){
                            memo[i].dados = pr[pid].dado;
                            aux = memo[i].nroPag;
                            pr[pid].primeiraPag = aux;
                            tabela[aux].BitPres = 1;
                            pr[pid].t_restante--;
                        }
                  }
                  pr[pid].ultimaPag = memo[i].nroPag;    
          }
     }
     pr[pid].vz_processador = 2;


}



void KILL_ALL(TabelaPag *tabela ,MemoriaFisica *memo,int tamMem){
     int i,aux;
     
     for(i = 0; i < tamMem; i++){
           memo[i].dados = '-';
           aux = memo[i].nroPag;
           tabela[aux].BitPres = 0;
     }   
}



char *substring (char *str, int inicio, int fim) {
    if (fim > strlen (str) || fim < inicio)
        return NULL;
        char *aux = strdup (str);
        int i;
        for (i = inicio; i <= fim; i++) {
            aux [i - inicio] = str[i];
    }
    aux[fim+1] = '\0';
    return aux;
}


void mostrarProc(Estrutura e){
     int cont;
     for (cont = indA; cont < pid; cont++){
         printf ("\n\n\nProcesso: %d \nvalor: %c  \ntempo de entrada: %d  \ntempo de inicio: %d \ntempo de execucao: %d \ntempo de saida: %d \nTempo Restante: %d\n",e.pr[cont].PID, e.pr[cont].dado, e.pr[cont].t_chegada,e.pr[cont].t_inicio,e.pr[cont].t_execucao2, e.pr[cont].t_saida,e.pr[cont].t_execucao);
     }

}


void mostra_proc_PID(Estrutura e, char dado, int pid){
     int cont;
     for (cont = 0;cont < 10; cont++){
         if(e.pr[cont].dado == dado){
               printf ("\n\n\nProcesso: %d \nvalor: %c  \ntempo de entrada: %d  \ntempo de execucao: %d \ntempo de saida: %d \nTempo Restante: %d\nPRIORIDADE: %d",pid, e.pr[cont].dado, e.pr[cont].t_chegada,e.pr[cont].t_execucao2, e.pr[cont].t_saida,e.pr[cont].t_execucao, e.pr[cont].prioridade);
               break;                         
         }
     }

}



int main(){
    int endeLog,nroPag,qtd=0,tempo=0,i = 0,bitsPag,tamMem,qtdPag,quantum, priori,tamMemVir;
    char dados;
    menu();
    //inicializa2();
 /*   while( 1 )
    {
           printf("Informe o numero de Bits para o endereco logico: (2 - 10): ");
           scanf("%d", &endeLog);
           if( endeLog > 1 && endeLog <= 10  ) break;
           else {
                system("cls");
                printf("Digite um numero valido...\n\n");
           }
    }        

    
    while( 1 )
    {
           printf("Informe o numero de Bits para o nro de paginas: (1 - %d): ", endeLog);
           scanf("%d", &nroPag);
           if( nroPag > 0 && nroPag < endeLog ) break;
           else {
                system("cls");
                printf("Digite um numero valido...\n\n");
           }
    }
*/ 
   endeLog = 5;
   nroPag =  4; 
    system("cls");
    bitsPag =  pow(2,(endeLog - nroPag));
    tamMem = pow(2,endeLog);
    qtdPag = pow(2,nroPag);
    
    tamMemVir = (tamMem * 2);
    MemoriaFisica mem[tamMem];
    MemoriaVirtual memV[tamMemVir];
    TabelaPag tabela[qtdPag];
    ajuda();
    
Estrutura est;
    est.pr = &(pr[0]);
    est.tab = tabela;
    est.memFis = mem;
    est.tam = tamMem;
    est.memVir = memV;
    
    
    
    
    _beginthread( Thread, 0, (void *)&est);
    
    inicializaMemo(bitsPag,tamMem,qtdPag,mem);  
    inicializaMemoVir(bitsPag,tamMemVir,qtdPag,memV);                                 
    inicializaTabe_Pag(tabela,qtdPag);    
    //-------------------------------------------------------------------------------
    int i2 = 0;
    while ( i2 == 0 ){
          
          int flag = 0, inicio, fim;
          i = 0;
          char comando[10];
          printf("\n\n");
          printf("$ ");
          fflush(stdin);
          scanf("%[^\n]",&comando);
          int Tam = strlen(comando);
    
          int res = strcmp(comando,"kill all");                                       // COMPARA KILL ALL
         if(res==0){
                    
                    mostraMemo(tamMem,mem);
                    KILL_ALL(tabela, mem,tamMem);
                    matarTodosProcessos();
                    
         }else{
               res = strcmp(comando,"help");
               if(res==0){  
                            ajuda();
               
               
         }else{
               res = strcmp(comando,"pr");                                       // COMPARA EXIT
                    if(res==0){ 
                                printf("\n\n\n---------------------------- PROCESSADOR ---------------------------------\n");
                                printf("\nPID | PRIORIDADE |     TC      |    TCA       |     TE    ");
                                printf("\n    |            |             |              |           ");
                                ver_processo(process);
                                printf("\n\n");
                    
               }else{
                  res = strcmp(comando,"fila");                                       // COMPARA EXIT
                      if(res==0){
                               printf("\nTOPO FILA: %d   |   TEMPO: %d   |   QUANTUM TIME: %d  | NP: %d  | NPN: %d ", filaAptos.topo, TIME, ctrlTime, N_PROCESSOS, N_PROCESSOS_NOW);
                               mostraFila( &filaAptos );
                               printf("\n\n");
               
                 }else{
                    res = strcmp(comando,"exit");                                       // COMPARA EXIT
                    if(res==0){
                               i2 = 1;
                    }else{
                               res = strcmp(comando,"mem");                                  // COMPARA MEM
                               if(res==0){
                                          mostraMemo(tamMem,mem);
                               }else{
                                     res = strcmp(comando,"tp");                           // COMPARA TP
                                     if(res==0){
                                     mostraTabe_Pag(tabela,qtdPag);
                               }else{
                                     res = strcmp(comando,"ps");                   // COMPARA PS
                                     if(res==0){
                                            mostrarProc(est);
                                            mostraFila(&filaAptos);
                    }else{          
                               res = strcmp(comando,"mv");                                  // COMPARA MEMV
                               if(res==0){
                                          mostraMemoVir(tamMemVir,memV);
                    
                                     }else{
                                           while(i < Tam) {                        // QUEBRA STRING PRA COMPARA KILL <PID>
                                                   if(flag == 0){ 
                                                           if(comando[i] != ' '){
                                                                         inicio = i;
                                                                         flag = 1;
                                                           }
                                                   }else{
                                                         if(comando[i] == ' ' ){
                                                                 fim = i-1;
                                                                 i++;
                                                                 break;
                                                         }
                                                   }
                                           i++;
                                           }
                                           char *subs = substring(comando,inicio,fim);
                                           char *subpid = substring(comando,fim+1,Tam);
                                           res = strcmp(subs,"kill");                                         // COMPARA KILL <PID>
                                           if(res==0){
                                                   int pid = atoi(subpid);
                                                   char dado;
                                                   dado = matarProcesso(pid);
                                                   Remover(tabela,mem,pid,tamMem,memV);
                                                   
                                                   pr[pid].t_saida = tempExe;
                                                   indA++;
                                                   pidA = pr[indA].PID;
                                                   
                                           }else{
                                                 res = strcmp(subs,"ps");                                // COMPARA PS <PID>
                                                 if(res==0){
                                                        int pid = atoi(subpid);
                                                        char dado;
                                                        //char dado = subpid[1];
                                                        dado = caracter(pid);
                                                        if (dado == '-')
                                                           continue;
                                                        mostra_proc_PID(est,dado, pid);
                                                        mostra_tabelaPag_PID(tabela,mem,qtdPag,dado,tamMem, pid);
                                                   }else{                                                   // SE NÂO DER NADA FAZ UM PROCESSO
                                                         res = strcmp(subs,"create");                        // CRIA UM PROCESSO
                                                         if(res==0){
                                                             inicio = 0, fim = 0, flag = 0;
                                                             i = 0;
                                                         while(i < Tam) {
                                                                     if(flag == 0){ 
                                                                           if(comando[i] != ' '){
                                                                                  inicio = i;
                                                                                  flag = 1;
                                                                           }
                                                                     }else{
                                                                           if(comando[i] == ' ' ){
                                                                                  fim = i;
                                                                                  break;
                                                                           }
                                                                     }
                                                                     i++;
                                                             }
                                                             char *subc = substring(comando,inicio,fim);
                                                        
                                                                
                                                        i++;
                                                             char dado = comando[i];
                                                             
                                                             i=i+2;   
                                                             flag = 0;
                                                             while(i < Tam) {
                                                                   if(flag == 0){ 
                                                                         if(comando[i] != ' '){
                                                                               inicio = i;
                                                                               flag = 1;
                                                                         }
                                                                   }else{
                                                                         if(comando[i] == ' ' ){
                                                                               fim = i;
                                                                               break;
                                                                         }
                                                                   }
                                                                   i++;
                                                             }
                                                             char *subq = substring(comando,inicio,fim);
                                                        
                                                             i++;
                                                             flag = 0;
                                                             while(i < Tam){
                                                                   if(flag == 0) { 
                                                                         if(comando[i] != ' '){
                                                                              inicio = i;
                                                                              break;
                                                                         }
                                                                   } 
                                                                   i++;
                                                             }
                                                             char *subt = substring(comando,inicio,Tam);

                                                             int qtd = atoi(subq);
                                                             int tempo = atoi(subt);  
                                                             pr[pid].dado = dado;
                                                             pr[pid].t_inicio = -1;
                                                             pr[pid].t_execucao = tempo;
                                                             pr[pid].t_execucao2 = tempo;  
                                                             pr[pid].t_restante = qtd; 
                                                             addDado(tabela,mem,&(pr[pid]),tamMem,qtdPag,bitsPag,memV);
                                                             novo_processo(pr, pid);
                                                        
                                                         if (indA == pid){
                                                                  pidA = pr[pid].PID;
                                                         }
                                                         pid++;
                                                   }
                                        }
                                        }

                                  }
                          }
                  }
                  }
                  }
                  }
                  }
          }
    }
}
   
    mostraMemo(tamMem,mem);
  
}


void menu(){
    int qtd=0,tempo=0,i = 0, quantum, priori;
    char dados,contexto,aging;
    bool c, a;
    while( 1 )
    {
           printf("Informe o quantum em segundos:(0 - 5): ");
           scanf("%d", &quantum);
           if( quantum >= 0 && quantum <= 5 ) break;
           else {
                system("cls");
                printf("Digite um numero valido...\n\n");
           }
    }        

    
    while( 1 )
    {
           printf("Prioridade do Processo: (1 - 4): ");
           scanf("%d", &priori);
           if( priori >= 1 && priori <= 4 ) break;
           else {
                system("cls");
                printf("Digite um numero valido...\n\n");
           }
    }
   c = false;
   a = false;    
    inicializa( quantum, a, c, priori);
}
    
    
void ajuda(){
     
                              printf("\n\n\n Comandos:\n");
                              printf("\n create <dado> <tam> <temp> -  Exemplo: create a 5 10");
                              printf("\n kill <pid>                -  Exemplo: kill 1 ");
                              printf("\n ps <pid>                  -  Exemplo: ps 1");
                              printf("\n kill all                  -  Mata todos os processos");
                              printf("\n ps                        -  Mostra tabela atual");
                              printf("\n pr                        -  Mostra o que o processdor esta fazendo");
                              printf("\n fila                      -  Mostra a fila para a entrada do processdor");
                              printf("\n mem                       -  Mostra memoria fisica");
                              printf("\n tp                        -  Mostra tabela de paginas");
                              printf("\n help                      -  Lista de comandos do sistema"); 
                              printf("\n exit                      -  Da um kill all e termina o programa");
}    

 

