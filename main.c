#include <stdio.h>
#include <stdlib.h>

struct Node{
	int num;
	int pid;
	int prioridade;
	int cpu;
	int cpu_feitos;
	int io;
	int io_feitos;
	char nome[10];
	int situacao;
	struct Node *prox;
};
char tecla;
char bloqueado[] = "\x1b[31mBloqueado\x1b[0m";
char executando[] = "\x1b[32mExecutando...\x1b[0m";
char aguardando[] = "\x1b[33mAguardando\x1b[0m";
char finalizado[] = "\x1b[34mFinalizado\x1b[0m";
int qtprontos;

//situ=0  aguardando, 1=executando, 2=bloqueado, 3=finalizado

typedef struct Node node;
int seed_pid = 102;
int tam;
//int Escalonador_Simples(node *PRONTOS,node *EXECUTANDO,node *BLOQUEADO,node *FINALIZADOS);
int menu(void);
void opcao(node *PRONTOS,node *EXECUTANDO,node *BLOQUEADO,node *FINALIZADOS, int op);
void inicia(node *PRONTOS);
int vazia(node *PRONTOS);
node *aloca();
void insere(node *PRONTOS);
node *retira(node *PRONTOS);
void listaProcessos(node *PRONTOS,node *EXECUTANDO,node *BLOQUEADO,node *FINALIZADOS);
void libera(node *PRONTOS);
void desenha_linha1(int qtd);


int main(void){
	node *PRONTOS = (node *) malloc(sizeof(node));
	node *BLOQUEADO = (node *) malloc(sizeof(node));
	node *EXECUTANDO = (node *) malloc(sizeof(node));
	node *FINALIZADOS = (node *) malloc(sizeof(node));

	inicia(PRONTOS);
	inicia(BLOQUEADO);
	inicia(FINALIZADOS);
	inicia(EXECUTANDO);
	int opt;

	do{
		opt=menu();
		opcao(PRONTOS,EXECUTANDO,BLOQUEADO, FINALIZADOS,opt);
	}
	while(opt);

	free(PRONTOS);
	return 0;	
}

int menu(void){
	system("clear");
	int opt;
	printf("Escolha a opcao\n");
	printf("0. Sair\n");
	printf("1. Zerar PRONTOS\n");
	printf("2. Exibir PRONTOS\n");
	printf("3. Adicionar Elemento na PRONTOS\n");
	printf("4. Retirar Elemento da PRONTOS\n");
	printf("Opcao: "); scanf("%d", &opt);
	return opt;
}

void opcao(node *PRONTOS,node *EXECUTANDO, node *BLOQUEADO,node *FINALIZADOS,int op){
	node *tmp;
	switch(op){
		case 0:
			libera(PRONTOS);
			break;
		case 1:
			libera(PRONTOS);
			inicia(PRONTOS);
			break;
		case 2:
			listaProcessos(PRONTOS,EXECUTANDO, BLOQUEADO, FINALIZADOS);
			break;
		case 3:
			insere(PRONTOS);
			break;
		case 9: Escalonador_Simples(PRONTOS,EXECUTANDO, BLOQUEADO,FINALIZADOS);
			break;
		case 4:
			tmp= retira(PRONTOS);
			if(tmp != NULL){
				printf("Retirado: %3d\n\n", tmp->num);
				libera(tmp);
			}
			break;
		default:
			printf("Comando invalido\n\n");
	}
}

void inicia(node *PRONTOS){
	PRONTOS->prox = NULL;
	tam=0;
}

int vazia(node *PRONTOS) {
	if(PRONTOS->prox == NULL)
		return 1;
	else return 0;
}

node *aloca(){
	system("clear");
	node *novo=(node *) malloc(sizeof(node));
	if(!novo){
		printf("Sem memoria disponivel!\n");
		exit(1);
	}
	else{
		desenha_linha1(70);
		printf("Qtd Processos criados:  %d\n",tam);
		desenha_linha1(70);
		printf("Novo PID     -> %d\n",seed_pid);
		printf("Nome         -> "); scanf("%s", novo->nome);
		printf("Prioridade   -> "); scanf("%d", &novo->prioridade);
		printf("Ciclos IO    -> "); scanf("%d", &novo->io);
		printf("Ciclos CPU   -> "); scanf("%d", &novo->cpu);
		novo->pid = seed_pid;
		novo->situacao = 0;
		novo->io_feitos =0;
		novo->cpu_feitos=0;
		seed_pid++;		
		return novo;
	}
}

void insere(node *PRONTOS) {	
	if(tam==10) {
	desenha_linha1(70);
	printf("\t\t\x1b[31m --- ERRO! ---\x1b[0m\n");
	printf("\t\x1b[33m Limite de Processos Criados (10)\x1b[0m\n\n\n");
	system("read -p \"Pressione enter para voltar\" saindo"); }
	if(tam!=10) {
	node *novo=aloca();
	novo->prox = NULL;
	if(vazia(PRONTOS))
		PRONTOS->prox=novo;
	else{   node *tmp = PRONTOS->prox;
		while(tmp->prox != NULL)
			tmp = tmp->prox;
		tmp->prox = novo;
	}
	tam++;
    }
}

node *retira(node *PRONTOS){
	if(PRONTOS->prox == NULL){
		printf("PRONTOS ja esta vazia\n");
		return NULL;
	}else{
		node *tmp = PRONTOS->prox;
		PRONTOS->prox = tmp->prox;
		tam--;
		return tmp;
	}
}

void listaProcessos(node *PRONTOS,node *EXECUTANDO, node *BLOQUEADO, node *FINALIZADOS) {
	node *tmp;	
	system("clear");
	printf("\n\t\t     PROCESSOS PRONTOS     \n");
	printf("  %s", "PID");
	printf("\t%s", "NOME");
	printf("\t\t%s", "PRIO.");
	printf("\t%s", "CPU");
	printf("\t%s", "IO");
	printf("\t%s\n", "STATUS");
	desenha_linha1(70);
	tmp = PRONTOS->prox;
	while( tmp != NULL){
		printf("  %d", tmp->pid);
		printf("\t%s", tmp->nome);
		printf("\t\t%d", tmp->prioridade);
		printf("\t%d/%d", tmp->cpu_feitos, tmp->cpu);
		printf("\t%d/%d", tmp->io_feitos, tmp->io);
	if(tmp->situacao==0) printf("\t%s\n",aguardando);
   else if(tmp->situacao==1) printf("\t%s\n",executando);
   else if(tmp->situacao==2) printf("\t%s\n",bloqueado);
   else if(tmp->situacao==3) printf("\t%s\n",finalizado);
		tmp = tmp->prox;
	}
	
	printf("\n\n\n\t\t     PROCESSOS EXECUTANDO     \n");
	printf("  %s", "PID");
	printf("\t%s", "NOME");
	printf("\t\t%s", "PRIO.");
	printf("\t%s", "CPU");
	printf("\t%s", "IO");
	printf("\t%s\n", "STATUS");
	desenha_linha1(70);
	tmp = EXECUTANDO->prox;
	while( tmp != NULL){
		printf("  %d", tmp->pid);
		printf("\t%s", tmp->nome);
		printf("\t\t%d", tmp->prioridade);
		printf("\t%d/%d", tmp->cpu_feitos, tmp->cpu);
		printf("\t%d/%d", tmp->io_feitos, tmp->io);
	if(tmp->situacao==0) printf("\t%s\n",aguardando);
   else if(tmp->situacao==1) printf("\t%s\n",executando);
   else if(tmp->situacao==2) printf("\t%s\n",bloqueado);
   else if(tmp->situacao==3) printf("\t%s\n",finalizado);
		tmp = tmp->prox;
	}	

	printf("\n\n\n\t\t     PROCESSOS BLOQUEADO     \n");
	printf("  %s", "PID");
	printf("\t%s", "NOME");
	printf("\t\t%s", "PRIO.");
	printf("\t%s", "CPU");
	printf("\t%s", "IO");
	printf("\t%s\n", "STATUS");
	desenha_linha1(70);
	tmp = BLOQUEADO->prox;
	while( tmp != NULL){
		printf("  %d", tmp->pid);
		printf("\t%s", tmp->nome);
		printf("\t\t%d", tmp->prioridade);
		printf("\t%d/%d", tmp->cpu_feitos, tmp->cpu);
		printf("\t%d/%d", tmp->io_feitos, tmp->io);
	if(tmp->situacao==0) printf("\t%s\n",aguardando);
   else if(tmp->situacao==1) printf("\t%s\n",executando);
   else if(tmp->situacao==2) printf("\t%s\n",bloqueado);
      else if(tmp->situacao==3) printf("\t%s\n",finalizado);
		tmp = tmp->prox;
	}

	printf("\n\n\n\t\t     PROCESSOS FINALIZADOS     \n");
	printf("  %s", "PID");
	printf("\t%s", "NOME");
	printf("\t\t%s", "PRIO.");
	printf("\t%s", "CPU");
	printf("\t%s", "IO");
	printf("\t%s\n", "STATUS");
	desenha_linha1(70);
	tmp = FINALIZADOS->prox;
	while( tmp != NULL){
		printf("  %d", tmp->pid);
		printf("\t%s", tmp->nome);
		printf("\t\t%d", tmp->prioridade);
		printf("\t%d/%d", tmp->cpu_feitos, tmp->cpu);
		printf("\t%d/%d", tmp->io_feitos, tmp->io);
	if(tmp->situacao==0) printf("\t%s\n",aguardando);
   else if(tmp->situacao==1) printf("\t%s\n",executando);
   else if(tmp->situacao==2) printf("\t%s\n",bloqueado);
   else if(tmp->situacao==3) printf("\t%s\n",finalizado);
		tmp = tmp->prox;
	}




	printf("\n\n\n");
	
}

void libera(node *PRONTOS){
	if(!vazia(PRONTOS)){
		node *proxNode,
			  *atual;

		atual = PRONTOS->prox;
		while(atual != NULL){
			proxNode = atual->prox;
			free(atual);
			atual = proxNode;
		}
	}
}

void desenha_linha1(int qtd){
	int i=0;
	for(i=0; i<qtd; i++) 
	printf("\u2501");	
	printf("\n");
}

int transfereEntre(node *origem, node*destino, node *pilhaOrigem) {
	node *novo=(node *) malloc(sizeof(node));
	novo->pid = origem->pid;
	strcpy(novo->nome,origem->nome);
	novo->io = origem->io;
	novo->io_feitos = origem->io_feitos;
	novo->cpu_feitos = origem->cpu_feitos;
	novo->cpu = origem->cpu;
	novo->prioridade = origem->prioridade;	
	novo->situacao = origem->situacao;	
	novo->prox = NULL;
	if(vazia(destino))
	destino->prox=novo;
	else{
		node *tmp = destino->prox;
		while(tmp->prox != NULL)
			tmp = tmp->prox;
		tmp->prox = novo;
	}
	retira(pilhaOrigem);
	return novo;
}

int Escalonador_Simples(node *PRONTOS, node *EXECUTANDO, node *BLOQUEADO, node *FINALIZADOS) {
	int i, j,k, finalizou;
	node *tmp;
	node *tmp2;
	node *novo ;
	while(qtprontos!=tam) {
		listaProcessos(PRONTOS,EXECUTANDO, BLOQUEADO, FINALIZADOS);
		tmp = PRONTOS->prox;
		while( tmp != NULL){ 
			sleep(1);
			finalizou=0;	 
			tmp->cpu_feitos++;
			tmp->situacao=1;
			novo = transfereEntre(tmp, EXECUTANDO,PRONTOS);
			listaProcessos(PRONTOS,EXECUTANDO, BLOQUEADO, FINALIZADOS);
			sleep(2);
	 	   	if(novo->io_feitos==novo->io && novo->cpu_feitos==novo->cpu){ 
				novo->situacao=3;
				novo = transfereEntre(novo, FINALIZADOS,EXECUTANDO);
				listaProcessos(PRONTOS,EXECUTANDO, BLOQUEADO, FINALIZADOS);
				sleep(2);
				qtprontos++;
				finalizou=1;
			}
			if(novo->io_feitos<novo->io && finalizou==0) {
				if(!vazia(BLOQUEADO)){	tmp2 = BLOQUEADO->prox;
					tmp2->situacao=0;
					transfereEntre(tmp2, PRONTOS,BLOQUEADO);
					listaProcessos(PRONTOS,EXECUTANDO, BLOQUEADO, FINALIZADOS);
					sleep(1);
				}
				novo->io_feitos++;
				novo->situacao=2;
				novo = transfereEntre(novo, BLOQUEADO,EXECUTANDO);
				listaProcessos(PRONTOS,EXECUTANDO, BLOQUEADO, FINALIZADOS);
				sleep(2);
			}
			else {			
				novo->situacao=0;
				transfereEntre(novo, PRONTOS,EXECUTANDO);
				listaProcessos(PRONTOS,EXECUTANDO, BLOQUEADO, FINALIZADOS);
				sleep(2);
			}			
			tmp = tmp->prox;		  
		}	
	}

return 0;
}


