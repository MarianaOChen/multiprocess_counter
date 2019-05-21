/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

#include <stdio.h>
#include <stdlib.h> /* exit() */
#include <sys/types.h> /* define pid_t */
#include <unistd.h> /* fork() */
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
	
#define MAX_PROCESSOS 4 //maximo de 4 processos
#define MAX_ENTRADAS 100

int ehprimo(int numero);

int main() {
	
	/*Leitura da entrada*/
	int numero;
	numero = '\0';
	int entradas[MAX_ENTRADAS];
	int contador = 0;
	while(scanf("%d", &numero) != EOF){
		entradas[contador] = numero;
		//printf("Recebi: %d\n", entradas[contador]);		
		contador++; //conta quantos numeros serao tratados 
   	}
	
	/*Processos*/	
	pid_t processos[MAX_PROCESSOS];
		
	/*Definir flags de proteção e visibilidade de memoria*/
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANON; 

	/*Area de memoria compartilhada - salvar resultados de verificacao*/
	int *memoria;
	memoria = (int*) mmap(NULL,sizeof(int)*MAX_ENTRADAS,protection, visibility, 0, 0);
	for(int i=0;i<MAX_ENTRADAS;i++){ //inicializacao do vetor, todos processos livres
		memoria[i] = 0;
	}

	int *disponivel; // vai mostrar qual processo esta disponivel 
	disponivel = (int*) mmap(NULL,sizeof(int)*MAX_PROCESSOS, protection, visibility, 0, 0);
	for(int i=0;i<MAX_PROCESSOS;i++){ //inicializacao do vetor, todos processos livres
		disponivel[i] = 0;
	}

	/*Processamento das entradas*/
	int num_processos = 0;
	int aux = 0;
	while(aux<contador){ 
		for(int i = 0;i < MAX_PROCESSOS; i++){
			if(disponivel[i] == 0){ //processo disponivel
				processos[i] = fork(); //novo processo
				disponivel[i] = 1;
						
			
				if(processos[i] == 0){
					//executado no processo filho			
					memoria[aux] = ehprimo(entradas[aux]);
				//	printf("Processo %d verificou numero %d, resultdo = %d\n", i, entradas[aux], memoria[aux]);
					disponivel[i] = 0;
					exit(0);
				}
				aux++; // pode verificar o proximo numero	
				break;	
			}
			
		}
	}		
	
	/*Verificando se todos os filhos terminaram de processar*/
	for (int i=0; i<MAX_PROCESSOS; i++) {
		 waitpid(processos[i], NULL, 0);
  	}

	/*Computar resultados e imprimir na tela*/
	int resultado;
	resultado = 0;
	for(int i=0;i<contador;i++){
		resultado = resultado + memoria[i];
	}

	printf("%d\n",resultado);
	//printf("%d\n",contador_processos);
	return 0;
}


int ehprimo(int numero){
/*Função que testa se o numero eh primo - um numero primo somente eh divisivel por 1 e ele mesmo
Como fazer o teste: Dado um numero, fazer divisoes sucessivas (a partir de 2)verificando o modulo, ate a metade do numero. 
Exemplo: 15. 15/2 = 7 %1 , 15/3 = 5 %0 (portanto nao eh primo)... 7: 7/2 = 3 %1, 7/3 = 2 %1 (portanto eh primo)*/

	int n;
	int modulo;
	int verificado = 1;
	int aux;

	n = numero;

	if(n <= 1){
	verificado = 0	;
	}
	else if (n == 2){
		verificado = 1;
	}
	else{
		for(aux = 2; aux <= n/2;aux++){
			//printf("%d mod %d\n", n,aux);
			modulo = numero%aux;
			if(modulo == 0){ //nao primo
				verificado = 0;
				break;
			}
		}
	}
	return verificado;

/*
	// Teste funcao ehprimo
	while(1){
	int numero;
	int resultado;	
	scanf("%d", &numero);
	
	printf("peguei numero\n");
	resultado = ehprimo(numero);

	if (resultado == 1) {printf("NÃO é primo\n");}
	else {printf("É primo\n");}
	}
	*/

}
	
