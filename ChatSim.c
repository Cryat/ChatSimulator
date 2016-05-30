/* Queues de Mensagens*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




/*|||||||||||||||[ITEMS]||||||||||||||||*/
typedef struct message{

	int sender;
	int receiver;
	char* text;

}*item;


char *copystrings (char* string){

	/* char* ----> char* */
	/* A funcao copystrings essencialmente aloca a quantidade de memoria exacta para copiar a string passada como argumento e de seguida copia-a para
	o bloco de memoria alocado */

	char *newstr;
	int len;

	len = strlen(string);

	newstr = malloc(sizeof(char)*(len+1));

	strcpy(newstr, string);

	return newstr;
}     


item newItem(int emissor, int receptor, char* info){

	/* int X int X char* ----> item */
	/* A funcao newItem recebe dois inteiros correspondentes ao emissor e receptor, e uma string que e a mensagem enviada, criando assim um novo ponteiro
	que aponta para a estrutura message criada pela funcao*/

	item new = (item) malloc(sizeof(struct message));

	new -> sender = emissor;
	new -> receiver = receptor;
	new -> text = copystrings(info);

	return new;
}
	

void showItem(item a){

	/* item ----> void */
	/* A funcao showItem nao possui output uma vez que apenas serve para escrever no terminal o conteudo da estrutura message para qual o item a aponta */

	int receiver;
	int  sender;
	char* text;

	receiver = a -> receiver;
	sender = a -> sender;
	text = a -> text;

	printf("%d %d %s\n", receiver, sender, text);

}


int cmpItems(const void *a, const void *b){

	/* const void* X ----> int */
	/* A funcao cmpItems compara as strings de mensagem de duas struct messages e os seus senders para posterior ordenacao em qsort() */

	int cmp = strcmp((*(struct message **)a) -> text, (*(struct message **)b) -> text);
	
	if (cmp != 0)
		return cmp;
	else
		return ((*(struct message **)a) -> sender - (*(struct message **)b) -> sender);
}

void deleteItem(item a){

	/* item ----> void */
	/* A funcao deleteItem tem como funcao libertar a memoria alocada para qual o item a aponta, e além disso coloca os ponteiros utilizados com o valor NULL
	de modo a evitar problemas relacionados com as posicoes de memoria e os seus conteudos */

	free(a -> text);
	free(a);
}

/*||||||||||||||||||||||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||QUEUE|||||||||||||||||||*/
typedef struct node{

	item info;
	struct node *next;

}*link;


link newLink(item a){

	/* item ----> link */
	/* A funcao new link recebe como argumento um item e devolve um link, isto é um ponteiro que aponta para uma estrutura node que possui 2 elementos
	diferetes, o item passado como argumento da funcao e um outro link que aponta para NULL, que vira a servir para apontar para outros links */

	link new;
	new = (link) malloc(sizeof(struct node));
	new -> info = a;
	new -> next = NULL;

	return new;
}


typedef struct Q{
	link head, tail;
	int size;
}*queue;



queue newQueue(void){

	/* void ----> queue */
	/* A funcao newQueue inicializa uma estrutura Q  e cria um ponteiro queue para essa mesma estrutura */
	
	queue new;
	new = (queue) malloc(sizeof(struct Q));

	new -> head = NULL;
	new -> tail = NULL;
	new -> size = 0;

	return new; 

}


void iniQueueVec (queue v[], int size){

	/* queue* X int  ----> void */
	/*A funcao iniQueueVec inicializa uma nova fila (queue), vazia, em cada uma das entradas do vector v[] */


	int i;
	
	for (i=0; i < size; i++)
	{
		v[i]= newQueue();
	}

}

void addtoQueue(item a, queue q){

	/* item X queue ----> void */
	/* A funcao addtoQueue adiciona o item a a fila q que sao passados como argumentos da funcao */

	link nl;
	nl = newLink(a);
	if ((q -> size == 0)){
		
		q -> head = nl;
		q -> tail = nl;

	}
	else{

		q -> tail -> next = nl;
		q -> tail = nl;
	}
	q -> size +=1;
}

void remfromQueue(queue q){

	/* queue ----> void */
	/* A funcao remfromQueue remove o primero elemento da fila (head) e redireciona o link head para o elemento que estava a seguir, se a fila ja estiver vazia
	esta funcao nao tem qualquer efeito */

	link newhead;
	
	if (q -> size > 1){
		newhead = q -> head -> next;
		deleteItem(q->head->info);
		free(q -> head);
		q -> head = newhead;
		q -> size =q -> size - 1;
	}

	else if (q -> size == 1){
		newhead = q -> head -> next;
		deleteItem(q->head->info);
		free(q -> head);
		q -> head = newhead;
		q -> size =q -> size - 1;
	}

}

void showQueueH(queue q){

	/* queue ----> void */
	/* A funcao showQueueH serve para enviar para o terminal, o conteudo da estrutura message apontada pelo link que se encontra na head da estrutura Q */

	if (q -> size == 0)
		printf("NULL\n");
	else
		showItem(q -> head -> info);
}


void showAll (queue q){

	/* queue ----> void */
	/* A funcao showAll envia para o terminal todos os items de uma dada fila*/

	int i;
	
	link newHead = q->head;
	
	if (newHead == NULL)
		printf("NULL\n");
		
	else
	{
		for (i=0; i < (q->size); i++)
		{
			showItem(newHead->info);
			newHead = newHead->next;
		}	
	}
}




void deleteQueue(queue q){

	/* queue ----> void */
	/* A funcao deleteQueue apaga todos os elementos da estrutura Q deixando essa mesma estrutura como se tivesse sido inicializada */

	while (q -> size > 0){
		remfromQueue(q);
	}
}



void deleteAllQ(int N, queue q[]){

	/* int X queue ----> void */
	/* A funcao deleteAllQ apaga todas as filas existentes no vector q[]*/

	int i;
	
	for (i=0; i < N; i++){
		deleteQueue(q[i]);
		free(q[i]);	
	}
}



item* morfArray(queue q, item *i){

	/* queue X char** X int* X int* ----> void */
	/* A funcao morfArray e uma funcao de manipulacao das estruturas Q para que o conteudo das suas estruturas message seja colocada em arrays de modo a 
	que seja mais facil o tratamento da informacao, por exemplo para aplicar algoritmos de ordenacao */

	link newhead, auxhead = q -> head;
	int n;

	for (n = 0; n < q -> size; n++){
		i[n] = q -> head -> info;
		newhead = q -> head -> next;
		q -> head = newhead;
	}

	q -> head = auxhead;

	return i;
}





void  sortlist(queue q){

	/* queue ----> void*/
	/* A funcao sortlist organiza em ordem alfabetica as mensagens e os seus senders da fila que lhe e dada*/
	
	item *i;
	int n;

	i = malloc(sizeof(item)*(q->size));
	
	if (q -> size == 0)
		printf("%s\n", "NULL");
		
	else if(q -> size > 0){	
		i = morfArray(q, i);
		
		qsort(i, q -> size, sizeof(struct message *), cmpItems);
		
		for (n = 0; n < q -> size; n++){
		showItem(i[n]);
		}
		
	}
}


void sendMessage(queue q[]){

	/* queue* ----> void */
	/* A funcao sendMessage coloca um item numa dada fila do vector q, apanhando quem e o sender e o receiver, */
	/* bem com a mensagem a enviar, atraves de um item criado utilizando os valores apanhados anteriormente */
	
	int r, s;
	char *inform =(char*) malloc(sizeof(char)*500);
	char *resized;
	item newI;
	
	scanf("%d %d",&s,&r);
	getchar();
	gets(inform);
	resized = copystrings(inform);
	newI = newItem(s, r, resized);
	addtoQueue(newI,q[r]);
	free (resized);
	free(inform);

}



/*|||||||||||||||||||||||||||||||||||||||||||||||*/


int main (){
	
	int u, N;
	char *command = malloc(sizeof(char)*11);
	queue *Qvector = malloc(sizeof(struct Q) * N);
	
	
	scanf("%d", &N);
	
	iniQueueVec(Qvector,N);	
	scanf("%s", command);
	while ( strcmp(command, "quit") != 0 )
	{
		
		if (strcmp("send",command) == 0)
		{
			
			sendMessage(Qvector);
		}
		
		else if (strcmp("process",command) == 0)
		{
			scanf("%d",&u);
			showQueueH(Qvector[u]);
			remfromQueue(Qvector[u]);
		}
		
		else if (strcmp("list",command) == 0)
		{
			scanf("%d",&u);
			showAll(Qvector[u]);
		}
		
		else if (strcmp("listsorted",command) == 0)
		{
			scanf("%d",&u);
			sortlist(Qvector[u]);

		}
		
		else if (strcmp("kill",command) == 0)
		{
			scanf("%d",&u);
			deleteQueue(Qvector[u]);
		}
		
		else
			printf("Comando Desconhecido\n");
	scanf("%s", command);
	}
	deleteAllQ(N, Qvector);
	free(Qvector);
	free(command);
	return 0;
} 
