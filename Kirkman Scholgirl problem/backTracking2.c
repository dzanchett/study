#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Declaração de constantes
#define ULTIMA_POSICAO 69

// Declaração de variaveis globais
int matriz[200], paresUtilizados[20][20], utilizadosNoDia[10][20], nIteracoes;

// Funcao que verifica se uma aluna pode andar em determinado dia da semana
int podeAndar(int posicao, int aluna){
	int tamanhoGrupo = posicao%2;
	int diaDaSemana = posicao/10;

	if(tamanhoGrupo == 0){
		if(utilizadosNoDia[diaDaSemana][aluna] == 1){
			return 0;
		}else{
			return 1;
		}
	}else{
		int a1;
		a1 = matriz[posicao-1];

		if(utilizadosNoDia[diaDaSemana][aluna] == 1){
			return 0;
		}else if(paresUtilizados[a1][aluna] == 1){
			return 0;
		}else{
			return 1;
		}
	}
}

// Funcao que posiciona uma aluna na matriz solução
void posicionaAluna(int posicao, int aluna){
	int tamanhoGrupo = posicao%2;
	int diaDaSemana = posicao/10;

	if(tamanhoGrupo == 0){
		utilizadosNoDia[diaDaSemana][aluna] = 1;
		matriz[posicao] = aluna;
	}else{
		int a1;
		a1 = matriz[posicao-1];

		utilizadosNoDia[diaDaSemana][aluna] = 1;
		paresUtilizados[a1][aluna] = 1;
		paresUtilizados[aluna][a1] = 1;
		matriz[posicao] = aluna;
	}
}

// Funcao que remove uma aluna da matriz solucao
void removeAluna(int posicao){
	int tamanhoGrupo = posicao%2;
	int diaDaSemana = posicao/10;
	int aluna = matriz[posicao];

	if(tamanhoGrupo == 0){
		utilizadosNoDia[diaDaSemana][aluna] = 0;
		matriz[posicao] = -1;
	}else{
		int a1;
		a1 = matriz[posicao-1];

		utilizadosNoDia[diaDaSemana][aluna] = 0;
		paresUtilizados[a1][aluna] = 0;
		paresUtilizados[aluna][a1] = 0;
		matriz[posicao] = -1;
	}
}

// Funcao que imprime a matriz solucao
void imprime(){
	int i, j, k;
	
	for(i=0; i<5; i++){
		for(j=0; j<7; j++){
			for(k=0; k<2; k++){
				printf("%02d, ", matriz[(j*10)+(i*2)+k]);
			}
			printf(" | ");
		}
		printf("\n");
	}

	printf("\n\n");

	return;
}

// Funcao que gera a arvore de solucoes usando backtracking para preencher a matriz solucao
void geraArvore(int posicao){
	//nIteracoes++;
	//printf("%09d\n", nIteracoes);
	//printf("** %d\n", posicao);
	//imprime();

	int i;
	for(i=1; i<=10; i++){
		if(podeAndar(posicao, i)==1){
			posicionaAluna(posicao, i);

			if(posicao == ULTIMA_POSICAO){
				imprime();
				exit(1);
				return;
			}else{
				geraArvore(posicao+1);
			}

			removeAluna(posicao);
		}
	}
}

// Funcao principal
int main(){

	nIteracoes = 0;

	memset(utilizadosNoDia, 0, sizeof utilizadosNoDia);
	memset(paresUtilizados, 0, sizeof paresUtilizados);
	memset(matriz, -1, sizeof matriz);

	geraArvore(0);

	return 0;
}