int TAM;
#include <stdlib.h>
#include "processos.h"
#include "boolean.h"

typedef struct {
		int topo;
		Processo *memo;
} Fila;

//-------------------------------------------------------------------
void inicializaFila(Fila *p_rec, int TM) 
{
		p_rec->topo=-1;
		p_rec->memo = (Processo *)malloc(sizeof(Processo) * TM);
		TAM = TM;
		int i = 0;
		while( i < TM )
		{ 
               p_rec->memo[i].PID = -1;
               i++;
        }
}




//-------------------------------------------------------------------
int ordenarFila(Fila *p_rec) {
    bool control = true;
    int i, a;
    for (a = 0; a < TAM; a++){
        if(p_rec->memo[a].t_execucao <= 0){
              p_rec->memo[a].PID = -1;
              p_rec->topo--;                       
        }    
    }
    if(p_rec->topo < 1) { // Para ver se há somente um ou nenhum processo na fila
              control = true;
    }
    if( control ) {
        int i_menor, i_maior;
        int p_maior, p_menor;
        int t_chda,mPid;

        Processo p_aux;
     
        int d=0;
        while( d < (p_rec->topo) )
        {
            i = (p_rec->topo) - d;
            i_menor = i;
            p_menor = p_rec->memo[i].prioridade;
            t_chda = p_rec->memo[i].t_chegada;
            mPid = p_rec->memo[i].PID;
            while( i > -1) {
                 if( ( p_rec->memo[i].prioridade <= p_menor )&& (p_rec->memo[i].PID != -1) ) {
                     if( ( p_rec->memo[i].prioridade < p_menor ) || ( p_rec->memo[i].t_chegada < t_chda ) || ( p_rec->memo[i].PID < mPid )  ) {
                         i_menor = i;
                         p_menor = p_rec->memo[i].prioridade;
                         t_chda = p_rec->memo[i].t_chegada;
                         mPid = p_rec->memo[i].PID;
                     }
                 }
                 i--;      
            }
            
                                       p_aux  =  p_rec->memo[ (p_rec->topo) - d ];
              p_rec->memo[ p_rec->topo - d ]  =  p_rec->memo[ i_menor ];
                      p_rec->memo[ i_menor ]  =  p_aux;
            //------------------------------------------------------------------------
            d++;
        }
    }
}

//-------------------------------------------------------------------
void organizaFila( Fila *p_rec )
{
     int i;
     for(i=1;i<TAM;i++)
     {
         p_rec->memo[i-1] = p_rec->memo[i];
     }
     ordenarFila(p_rec);
}

//-------------------------------------------------------------------
int filaCheia(Fila *p_rec) 
{
	if(p_rec->topo==(TAM-1)) {
 	    return (1);
	} else {
		 return (0);
	 }
 }
 
//-------------------------------------------------------------------
void mostraFila(Fila *p_rec) 
{
	int i;
	//printf("\n\n TOPO: %d \n", p_rec->topo);
	printf("\n\n---------------------------- FILA DE APTOS -------------------------------");
	printf("\nPID | PRIORIDADE |     TC      |    TCA       |     TE    ");
    printf("\n    |            |             |              |           ");
	for(i=0;i<(p_rec->topo+1);i++)
	{
		ver_processo( &p_rec->memo[i] );
	}
	printf("\n--------------------------------------------------------------------------");
   
   // ver_processo( &p_rec->memo[p_rec->topo] );		
}

//------------------------------------------------------------------- 
 int filaVazia(Fila *p_rec) 
 {
     int a, flag;
	if(p_rec->topo==-1) {
 	    return (1);
	} else {
         for(a = 0; a < TAM; a++){
               if (p_rec->memo[a].PID != -1)
                  flag = 1;
         } 
         if (flag) 
		    return (0);
          else     
                   return (1); 
	 }
 }
 void zera(Fila *p_rec){
      int a;
      for(a = 0; a < TAM; a++){
               if (p_rec->memo[a].PID == 0)
                  p_rec->memo[a].PID = -1;
      }
 } 
 
  void pri(Fila *p_rec){
      int a;
      printf("\n\n");
      for(a = 0; a < TAM; a++){
               printf(" %d -",p_rec->memo[a].PID);
      }
     
 } 
 
 int ultimoFila(Fila *p_rec){
     int a, p;
     for(a = 0; a < TAM; a++){
               if (p_rec->memo[a].PID > 0){
                  p = p_rec->memo[a].PID;                        
               }
      }
      return(p);
 }
 
//-------------------------------------------------------------- 	
void Emfileirar(Fila *p_rec, Processo e) 
{
          
     if(emfileiraTesta(&p_rec))
     {
         if((e.PID != -1) && (e.PID != 0)){
                  p_rec->topo++;
	              p_rec->memo[p_rec->topo]=e;
         }
     }
     ordenarFila(p_rec);
}

//-------------------------------------------------------------------
void Desemfileirar(Fila *p_rec) 
{
     if(!filaVazia(p_rec)) p_rec->topo--;
 
     
	//organizaFila(p_rec);
}

//-------------------------------------------------------------------
int emfileiraTesta(Fila *p_rec)
{
    int a, flag;
	if(filaCheia(p_rec)) { 
                         return 0; 
    }
	else { return (1); }
}

//-------------------------------------------------------------------
int desemfileiraTesta(Fila *p_rec)
{
	if(filaVazia(p_rec)) return 0;
	else {
		Desemfileirar(p_rec);
		 return (1);
		}
}
//-------------------------------------------------------------------
int destruirFila(Fila *p_rec) {
    free(p_rec->memo);
}
//------------------------------------------------------------------- 
Processo getProcesso_topo(Fila *p_rec) {
         return p_rec->memo[p_rec->topo];
}


void aumentaPrioridade(Fila *p_rec){
     int a;
     for (a = 0; a < TAM; a++){
         if (p_rec->memo[a].prioridade > 1){
                  p_rec->memo[a].prioridade--;                 
         }    
     }
}
