#include<stdio.h>
#include<stdlib.h>

#define INF 99999999


typedef struct ElementoLista {
    int vertice;
    struct ElementoLista* proximo;
} ElementoLista;

typedef struct Lista {
    int tamanho;
    ElementoLista* inicio;
} Lista;

void inicializa(Lista* lista) {
    lista->tamanho = 0;
    lista->inicio = NULL;
}

void insere(Lista* lista, int vertice) {
    ElementoLista* novo = (ElementoLista*)malloc(sizeof(ElementoLista));
    novo->vertice = vertice;
    novo->proximo = lista->inicio;
    lista->tamanho++;
    lista->inicio = novo;
}

void esvazia(Lista* lista) {
    ElementoLista* atual = lista->inicio;
    while (atual != NULL) {
        ElementoLista* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    lista->tamanho = 0;
    lista->inicio = NULL;
}

Lista *grafo ;
int **capacidades ;
int **fluxos ;
int *pais ;
int *fluxosMins ;
int *fila ; 

void print_grafo( Lista *g, int nv ){
	int i, j ;
	ElementoLista *aux ;
	for ( i = 0 ; i < nv ; i++ ){
		printf("[%d]: ", i);
		aux = g[i].inicio ;
		for ( j = 0 ; j < g[i].tamanho ; j++){
			
			printf("%d ", aux->vertice);
			aux = aux->proximo;
		}
		printf("\n");
	}
}

void imprimi_caminho( int s, int t){
	if ( t == s){
		printf("caminho aumentante: %d", t);
	}else{
		imprimi_caminho(s,pais[t]);
		printf(" %d", t);
	}
}

int min ( int a, int b){
	if ( a < b) return a ;
	else return b ;
}

int bfs(int nv, int s, int t ){
	int i ;
		
	for ( i = 0 ; i < nv; i++){
		pais[i] = -1 ;
		fluxosMins[i] = INF ;
	}
	
	int inicio = 0, fim = 0;  
	fila[fim++] = s ; 
	ElementoLista *aux ;
	
	while ( inicio != fim ){
		int u = fila[inicio]; 
		aux = grafo[u].inicio ;
		for ( i = 0 ; i < grafo[u].tamanho ; i++ ){	
			if ( capacidades[u][aux->vertice] - fluxos[u][aux->vertice] > 0) {		
				if ( pais[ aux->vertice ] == -1 ){
					pais[ aux->vertice ] = u ;
					fila[fim++] = aux->vertice ;
					fluxosMins[ aux->vertice ] = min ( fluxosMins[u], capacidades[u][aux->vertice]-fluxos[u][aux->vertice]);
				   	if ( aux->vertice == t )
				   		return fluxosMins[t] ;
				}
			}
			aux = aux->proximo ;
		}
		inicio++;
	}
	return 0 ;
}

int edmonds_karps_impl(int s, int t, int nv ){
	int fluxoMaximo = 0, incremento, verticeAux;
	
	
	while(1){
		incremento = bfs (nv, s, t) ;
		printf("capacidade residual: %d\n",incremento );
		
		if ( incremento == 0 )
			break ;
			
		imprimi_caminho(s,t);
		printf("\n");
		
		fluxoMaximo += incremento ;
		verticeAux = t ;
		
		while ( s != verticeAux ){
			
			fluxos[pais[verticeAux]][verticeAux] += incremento;
            fluxos[verticeAux][pais[verticeAux]] -= incremento;
			verticeAux = pais[verticeAux];
		}
	}
	return fluxoMaximo ;
}

int main(){

	int nv, ne, i,j, s, t, from, to, capacidade ;
	
	/*leitura do nro de vertices, arestas e qual eh a fonte e terminal*/	
	scanf("%d %d", &nv, &ne );
	scanf("%d %d", &s, &t);

	/*Alocando Memoria*/
	grafo = malloc( nv*sizeof(Lista));
	capacidades = malloc( nv * sizeof(int*) );
	fluxos = malloc( nv * sizeof(int*) );
	pais = malloc( nv*sizeof(int));
	fluxosMins = malloc( nv*sizeof(int));
	fila = malloc( nv*sizeof(int)); 
	for ( i = 0 ; i < nv ; i++){
		capacidades[i] = malloc( nv*sizeof(int));
		fluxos[i] = malloc( nv*sizeof(int));
	}
		
	/*Inicializações*/
	for ( i = 0 ; i < nv ; i++){
		inicializa(&grafo[i]);
		for ( j = 0 ; j < nv ; j++)	{
			fluxos[i][j] = 0;	
			capacidades[i][j] = 0;
		}
	}
	
	/*leitura e construcao do grafo */
	for ( i = 0 ; i < ne ; i++){
		scanf("%d %d %d", &from, &to, &capacidade );
		insere(&grafo[from], to);
		insere(&grafo[to], from);
		capacidades[from][to] = capacidade ;
	}
	
	printf("Grafo:\n");
	print_grafo( grafo, nv );
	

	int fluxoMax = edmonds_karps_impl(s, t, nv ) ;
	printf("Fluxo Maximo = %d \n", fluxoMax );
	
	return 0;
}
