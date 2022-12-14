// Bianca Santos Pastos - RA: 211150126
// Trabalho de Implementação Individual (T1) 
// 2° semestre 2° ano Curso Bacharelado em Ciências da Computação
// Disciplina Grafos e suas Aplicações - Professor Dênis Salvadeo
// dezembro 2022

#include<stdio.h>
#include<stdlib.h>

// busca em profundidade
#define BRANCO 0	// nao visitado
#define CINZA 1		// descoberto
#define PRETO 2		// finalizado

// ----------------------- structs ----------------------------
typedef struct lista_adj{
  int vert; 					// Vértice 
  int peso;						// valor do peso da aresta em caso de grafo valorado || 0 em caso de grafo nao valorado
  struct lista_adj* prox;		// ponteiro p proximo da lista
} Lista;

typedef struct vertice_cabeca_lista{
	Lista *cabeca;
} Cabeca;

typedef struct grafo {
  int num_verts;
  int num_arestas;
  Cabeca *adj;			//vertices da estrutura
} Grafo;

typedef struct fila{
	int elem[100];
	int frente;
	int atras;
} Fila;

// -------------------- prototipos funcoes --------------------
FILE* gera_arq(FILE *arq, int num_vertices, int num_arestas, char tipo, int valorado);
FILE* escreve_arq(FILE *arq, int vi, int vj, int peso);

void criar_mat(FILE *arq, int num_vertices, int mat[num_vertices][num_vertices], int num_arestas, int tipo, int valorado);
Lista* criar_lista(int vj, int peso);

void conversao_mat_lista(int num_vertices, int mat[num_vertices][num_vertices], int valorado, int num_arestas);
void conversao_lista_mat(int num_arestas, Grafo *grafo);
void calcula_grau(int num_vertices, int mat[num_vertices][num_vertices]);

void BFS(Grafo *grafo, int vert_fonte);
Fila* criar_fila_vazia();

void DFS(Grafo *grafo);
void DFS_visita(Grafo *grafo, int u, int *cor);

void dijkstra(Grafo *grafo, int fonte);

int main(){
	
	int num_vertices, num_arestas;
	
	char tipo;			// G - grafo nao diridigo || D - grafo dirigido
	int valorado; 		// 0 - grafo nao dirigido tipo G || grafo dirigido tipo D mas nao ponderado e 1 - grafo dirigido ponderado
	int peso;			// 1 - grafo nao dirigido ou grafo dirigido mas nao valorado || peso correspondente a cada aresta para grafo dirigido valorado
	
	FILE *arq;
	
	int opcao, i, j;
	int vi, vj;

// MENU REFERENTE A CRIACAO DO GRAFO
	do{
		printf("\nMENU INICIAL:\n1 - Grafo nao dirigido \n2 - Grafo Dirigido \n\n3 - Menu de Operacoes\n0-Sair\nOpcao: ");
		scanf("%d", &opcao);
		
		switch(opcao){
			case 1: {
				tipo = 'G';
				valorado = 0;
				peso = 1;
				
				printf("\nDigite o numero de vertices no grafo: ");
				scanf("%d", &num_vertices);
				
				printf("Digite o numero de arestas no grafo: ");
				scanf("%d", &num_arestas);
				
				arq = gera_arq(arq, num_vertices, num_arestas, tipo, valorado);
				
				printf("\nMontando o grafo... \n Digite os vertices que possuem uma aresta os conectando: \n");
				printf(" Exemplo: vertice 1 tem uma aresta o conectando com o vertice 2\n");

				for(i=0; i<num_arestas; i++){
					printf("\nVertice: ");
					scanf("%d", &vi);
					
					printf("tem uma aresta o conectando com o vertice: ");
					scanf("%d", &vj);
					
					arq = escreve_arq(arq, vi, vj, peso);
				}
				break;
			}
			case 2: {
				tipo = 'D';
				int opc;
				
				printf("\nDigite o numero de vertices no grafo: ");
				scanf("%d", &num_vertices);
				
				printf("Digite o numero de arestas no grafo: ");
				scanf("%d", &num_arestas);
				
				printf("\nQual o tipo deste grafo? \n1-Valorado \n2-Nao valorado \nOpcao: ");
				scanf("%d", &opc);
				
				if(opc == 2){
					valorado = 0;
					peso = 1;
					
					arq = gera_arq(arq, num_vertices, num_arestas, tipo, valorado);
				
					printf("\nMontando o grafo... \n Digite os vertices que possuem uma aresta os conectando: \n");
					printf(" Exemplo: vertice 1 tem uma aresta apontando para o vertice 2\n");
					
					for(i=0; i<num_arestas; i++){
						printf("\nVertice: ");
						scanf("%d", &vi);
						
						printf("tem uma aresta apontando para o vertice: ");
						scanf("%d", &vj);
						
						arq = escreve_arq(arq, vi, vj, peso);
					}
					
				}else if(opc == 1){
					valorado = 1;
					
					arq = gera_arq(arq, num_vertices, num_arestas, tipo, valorado);
				
					printf("\nMontando o grafo... \n Digite os vertices que possuem uma aresta os conectando: \n");
					printf(" Exemplo: vertice 1 tem uma aresta apontando para o vertice 2 de peso 35 \n");
					
					for(i=0; i<num_arestas; i++){
						printf("\nVertice: ");
						scanf("%d", &vi);
						
						printf("tem uma aresta apontando para o vertice: ");
						scanf("%d", &vj);
						
						printf("de peso: ");
						scanf("%d", &peso);
						
						arq = escreve_arq(arq, vi, vj, peso);
					}
				}
				break;
			}
			case 3: {
				printf("\n\nEscolha a operacao que deseja realizar com o grafo que acabou de criar: ");
				break;
			}
			case 0: {
				printf("Programa Encerrado!");
				break;
			}
			default: {
				printf("Opcao Invalida! Digite novamente!\n ");
				break;
			}
		}
		
	}while(opcao != 3);
	
// LEITURA DOS DADOS DO ARQ CRIADO e inicialização da matriz adjacencias e da lista de adjacencias-------------------------------------------------------------------------------------
	
	rewind(arq);
	fscanf(arq, "%d %d %c %d\n", &num_vertices, &num_arestas, &tipo, &valorado);
	
	// MATRIZ DE ADJACENCIAS
	int mat[num_vertices][num_vertices];
	criar_mat(arq, num_vertices, mat, num_arestas, tipo, valorado);
	
	printf("\n\nMatriz de adjacencias: \n");
	for(i=0; i<num_vertices; i++){
		for(j=0; j<num_vertices; j++){
			
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
	
	// LISTA DE ADJACENCIAS
	rewind(arq);
	fscanf(arq, "%d %d %c %d\n", &num_vertices, &num_arestas, &tipo, &valorado);
	
	// cria grafo
	Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
	grafo->num_verts = num_vertices;
	grafo->num_arestas = num_arestas;
	grafo->adj = (Cabeca *)malloc(num_vertices*sizeof(Cabeca));
	
	for(i=0; i<num_vertices; i++){
		grafo->adj[i].cabeca = NULL;
	}
	
	for(i=0; i<num_arestas; i++){
		
		fscanf(arq, "%d %d %d\n", &vi, &vj, &peso);
		Lista *nova = criar_lista(vj, peso);		// cria lista adjacencias com vertice e peso
		
		nova->prox = grafo->adj[vi].cabeca;			// proximo campo recebe a cabeca da lista
		grafo->adj[vi].cabeca = nova;
		grafo->num_arestas++;
	}
	
	printf("\nLista de adjacencias: \n");
	for(i=0; i<num_vertices; i++){
		printf("vertice %d: ", i);
		Lista *cabeca = grafo->adj[i].cabeca;
		while(cabeca){
			printf(" vertice %d peso %d --> ", cabeca->vert, cabeca->peso);
			cabeca = cabeca->prox;
		}
		printf("\n");
	}
	
// MENU REFERENTE AS OPERACOES A SEREM FEITAS COM O GRAFO CRIADO NO ARQUIVO	------------------------------------------------
	do{
		printf("\n");
		printf("1 - Conversao Matriz Adjacencias --> Lista Adjacencias \n");
		printf("2 - Conversao Lista Adjacencias --> Matriz Adjacencias \n");
		printf("3 - Calcular Grau de cada vertice \n");
		printf("4 - Realizar Busca em Largura \n");
		printf("5 - Realizar Busca em Profundidade \n");
		printf("0 - Sair\nOpcao: ");
		scanf("%d", &opcao);
		
		switch(opcao){
			
			case 1: {
				conversao_mat_lista(num_vertices, mat, valorado, num_arestas);
				break;
			}
			case 2: {
				conversao_lista_mat(num_vertices, grafo);
				break;
			}
			case 3: {
				calcula_grau(num_vertices, mat);
				break;
			}
			case 4: {
				int vert_fonte;
				printf("Digite o vertice fonte: ");
				scanf("%d", &vert_fonte);
				
				BFS(grafo, vert_fonte);
				break;
			}
			case 5: {
				DFS(grafo);
				break;
			}
			case 0: {
				printf("Programa Encerrado!");
				break;
			}
			default: {
				printf("Opcao invalida, digite novamente: ");
				break;
			}
			
		}
		
	}while(opcao != 0);

	fclose(arq);
}


FILE* gera_arq(FILE *arq, int num_vertices, int num_arestas, char tipo, int valorado){

	arq = fopen("grafo.txt", "w+");
	
	fprintf(arq, "%d %d %c %d\n", num_vertices, num_arestas, tipo, valorado);
	
	fclose(arq);
	arq = fopen("grafo.txt", "r+");
	return arq;
}

FILE* escreve_arq(FILE *arq, int vi, int vj, int peso){
	
	fseek(arq, 0, SEEK_END);
	
	fprintf(arq, "%d %d %d\n", vi, vj, peso);
	
	fclose(arq);
	arq = fopen("grafo.txt", "r+");
	return arq;
}


void criar_mat(FILE *arq, int num_vertices, int mat[num_vertices][num_vertices], int num_arestas, int tipo, int valorado){
	
	int i, j, k;
	int vi, vj, peso;
	
	// inicializa matriz com 0
	for(i=0; i<num_vertices; i++){
		for(j=0; j<num_vertices; j++){
			mat[i][j] = 0;
		}
	}
	
	rewind(arq);
	fscanf(arq, "%d %d %c %d\n", &num_vertices, &num_arestas, &tipo, &valorado);		// le novamente para pular leitura no arq
	
	for(k=0; k<num_arestas; k++){
		fscanf(arq, "%d %d %d\n", &vi, &vj, &peso);
		
		// se for grafo nao dirigido matriz simetrica
		if(tipo == 'G'){
			mat[vi][vj] = 1;
			mat[vj][vi] = 1;
		}
		
		// se for grafo dirigido
		if(tipo == 'D'){
			
			// se for com peso, matriz vai receber o peso
			if(valorado == 1){
				mat[vi][vj] = peso;
				
			}else if(valorado == 0){		// se for sem peso, recebe valor 1
				mat[vi][vj] = 1;
			}
		}
	}
	
}

Lista* criar_lista(int vj, int peso){
	
	// setando lista
	Lista *list;
	list = (Lista *)malloc(sizeof(Lista));		// aloca espaco para um no
	list->vert = vj;
	list->peso = peso;
	list->prox = NULL;

	return list;
}


void conversao_mat_lista(int num_vertices, int mat[num_vertices][num_vertices], int valorado, int num_arestas){
	
	int i, j;

	// cria grafo
	Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
	grafo->num_verts = num_vertices;
	grafo->num_arestas = num_arestas;
	grafo->adj = (Cabeca *)malloc(num_vertices*sizeof(Cabeca));
	
	for(i=0; i<num_vertices; i++){
		grafo->adj[i].cabeca = NULL;
	}
	
	// cria lista
	for(i=0; i<num_vertices; i++){
		for(j=0; j<num_vertices; j++){
			
			if(mat[i][j] != 0){
				
				Lista *nova = criar_lista(j, mat[i][j]);
				nova->prox = grafo->adj[i].cabeca;			// proximo campo recebe a cabeca da lista
				grafo->adj[i].cabeca = nova;
				grafo->num_arestas++;
			}
			
		}
	}
	
	printf("\nConversao da Matriz de Ajdacencias para a Lista de adjacencias: \nMatriz:\n");
	for(i=0; i<num_vertices; i++){
		for(j=0; j<num_vertices; j++){
			
			printf("%d ", mat[i][j]);
			
		}
		printf("\n");
	}
	
	printf("\nLista: \n");
	for(i=0; i<num_vertices; i++){
		printf("vertice %d: ", i);
		Lista *cabeca = grafo->adj[i].cabeca;
		while(cabeca){
			printf(" vertice %d peso %d --> ", cabeca->vert, cabeca->peso);
			cabeca = cabeca->prox;
		}
		printf("\n");
	}
}

void conversao_lista_mat(int num_vertices, Grafo *grafo){
	
	int mat[num_vertices][num_vertices], i, j;
	
	// inicializa matriz com 0
	for(i=0; i<num_vertices; i++){
		for(j=0; j<num_vertices; j++){
			mat[i][j] = 0;
		}
	}
	
	printf("\nConversao de Lista para Matriz: ");
	
	printf("\nLista de adjacencias: \n");
	for(i=0; i<num_vertices; i++){
		printf("vertice %d: ", i);
		Lista *cabeca = grafo->adj[i].cabeca;
		while(cabeca){
			printf(" vertice %d peso %d --> ", cabeca->vert, cabeca->peso);
			cabeca = cabeca->prox;
		}
		printf("\n");
	}
	
	for(i=0; i<num_vertices; i++){
		Lista *list = grafo->adj[i].cabeca;
		
		while(list){
			mat[i][list->vert] = list->peso;		// se for grafo valorado recebe peso senao recebe 1
			list = list->prox;
		}
		printf("\n");
	}
	
	printf("Matriz: \n");
	for(i=0; i<num_vertices; i++){
		for(j=0; j<num_vertices; j++){
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
	
}

void calcula_grau(int num_vertices, int mat[num_vertices][num_vertices]){
	
	int i, j, grau=0;
	
	// de acordo com a matriz de adjacencias
	printf("Graus de cada vertice do grafo: \n");
	
	for(i=0; i<num_vertices; i++){
		for(j=0; j<num_vertices; j++){
			
			if(mat[i][j] != 0){					// pode ser 1 ou o peso da aresta
				grau = grau+1;
			}
		}
		printf("Vertice %d possui grau %d \n", i, grau);
		grau=0;
	}
	
}

// BUSCA EM LARGURA (BFS)
Fila* criar_fila_vazia(){
	
	Fila *fila = malloc(sizeof(Fila));
	fila->frente = -1;
	fila->atras = -1;
	
	return fila;
}

void BFS(Grafo *grafo, int vert_fonte){
	
	Fila *fila;
	
	// cria vetor para atribuir uma cor a cada vertice do grafo e vetor de distancias
	int cor[grafo->num_verts];
	int dist[grafo->num_verts];
	
	int i, u, atual;
	
	// inicializa todos os vertices com a cor branca e distancia 100 (representando infinito)
	for(i=0; i<grafo->num_verts; i++){
		cor[i] = BRANCO;
		dist[i] = 100;
	}
	
	// inicializa posicao do vertice fonte
	cor[vert_fonte] = CINZA;
	dist[vert_fonte] = 0;
	
	fila = criar_fila_vazia();		// aloca e seta fila frente e tras com -1
	//add vertice fonte na fila
	if(fila->frente == -1){
		fila->frente = 0;
	}
	fila->atras++;
	fila->elem[fila->atras] = vert_fonte;
	
	// enquanto fila nao for vazia
	while( fila->atras != -1){		
		u = fila->elem[fila->frente];				// pega elemento da fila
		fila->frente++;
		
		if(fila->frente > fila->atras){
			fila->frente = -1;
			fila->atras = -1;
		}
		
		Lista *list = grafo->adj[u].cabeca;
		
		while(list){
			
			atual = list->vert;
			if(cor[atual] == BRANCO){
				cor[atual] = CINZA;
				dist[atual] = dist[u] + 1;
				
				//add na fila
				if(fila->frente == -1){
					fila->frente = 0;
				}
				fila->atras++;
				fila->elem[fila->atras] = atual;
			}
			list = list->prox;
		}
		cor[u] = PRETO;
		
	}
	
	printf("\nResultado BFS:");
	for(i=0; i<grafo->num_verts; i++){
		
		if(vert_fonte != i){
			if(dist[i] == 100){
				printf("\nNao ha caminho entre o vertice fonte %d e o vertice %d", vert_fonte, i);
			}else{
				printf("\nVertice fonte %d tem distancia %d ao vertice %d", vert_fonte, dist[i], i);
			}
		}
		
	}
	printf("\n");
	
}


// BUSCA EM PROFUNDIDADE (DFS)

void DFS(Grafo *grafo){
	
	// cria vetor para atribuir uma cor a cada vertice do grafo
	int cor[grafo->num_verts];
	
	int i;
	// inicializa todos os vertices com a cor branca
	for(i=0; i<grafo->num_verts; i++){
		cor[i] = BRANCO;
	}
	
	printf("\nResultado DFS: ");
	// se for branco, vertice ainda nao foi explorado, entao visita fazendo a busca em profundidade
	for(i=0; i<grafo->num_verts; i++){
		
		if(cor[i] == BRANCO){
			DFS_visita(grafo, i, cor);
		}
		printf("\n");
	}
	
}

void DFS_visita(Grafo *grafo, int u, int *cor){

	cor[u] = CINZA;
	Lista *vertices = grafo->adj[u].cabeca;		// visita vertice adjacente, enquanto nao acabar lista

	while(vertices){
		
		if(cor[vertices->vert] == BRANCO){
			
			DFS_visita(grafo, vertices->vert, cor);
			
		}
		vertices = vertices->prox;
	}
	
	cor[u] = PRETO;
	printf("%d <-- ", u);
}
