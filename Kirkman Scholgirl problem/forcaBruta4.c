#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Declaração de constantes
#define ULTIMA_POSICAO 139
#define NALUNAS 20

// Declaração de variaveis globais
int matriz[200], paresUtilizados[25][25], utilizadosNoDia[10][25], nIteracoes;

// Funcao que posiciona uma aluna na matriz solução
void posicionaAluna(int posicao, int aluna){
	int tamanhoGrupo = posicao%4;
	int diaDaSemana = posicao/NALUNAS;

	if(tamanhoGrupo == 0){
		utilizadosNoDia[diaDaSemana][aluna] = 1;
		matriz[posicao] = aluna;
	}else if(tamanhoGrupo == 1){
		int a1;
		a1 = matriz[posicao-1];

		utilizadosNoDia[diaDaSemana][aluna] = 1;
		paresUtilizados[a1][aluna]++;
		paresUtilizados[aluna][a1]++;
		matriz[posicao] = aluna;
	}else if(tamanhoGrupo == 2){
		int a1, a2;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];

		utilizadosNoDia[diaDaSemana][aluna] = 1;
		paresUtilizados[a1][aluna]++;
		paresUtilizados[aluna][a1]++;
		paresUtilizados[a2][aluna]++;
		paresUtilizados[aluna][a2]++;
		matriz[posicao] = aluna;
	}else{
		int a1, a2, a3;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];
		a3 = matriz[posicao-3];

		utilizadosNoDia[diaDaSemana][aluna] = 1;
		paresUtilizados[a1][aluna]++;
		paresUtilizados[aluna][a1]++;
		paresUtilizados[a2][aluna]++;
		paresUtilizados[aluna][a2]++;
		paresUtilizados[a3][aluna]++;
		paresUtilizados[aluna][a3]++;
		matriz[posicao] = aluna;
	}
}

// Funcao que remove uma aluna da matriz solucao
void removeAluna(int posicao){
	int tamanhoGrupo = posicao%4;
	int diaDaSemana = posicao/NALUNAS;
	int aluna = matriz[posicao];

	if(tamanhoGrupo == 0){
		utilizadosNoDia[diaDaSemana][aluna] = 0;
		matriz[posicao] = -1;
	}else if(tamanhoGrupo == 1){
		int a1;
		a1 = matriz[posicao-1];

		utilizadosNoDia[diaDaSemana][aluna] = 0;
		paresUtilizados[a1][aluna]--;
		paresUtilizados[aluna][a1]--;
		matriz[posicao] = -1;
	}else if(tamanhoGrupo == 2){
		int a1, a2;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];

		utilizadosNoDia[diaDaSemana][aluna] = 0;
		paresUtilizados[a1][aluna]--;
		paresUtilizados[aluna][a1]--;
		paresUtilizados[a2][aluna]--;
		paresUtilizados[aluna][a2]--;
		matriz[posicao] = -1;
	}else{
		int a1, a2, a3;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];
		a3 = matriz[posicao-3];

		utilizadosNoDia[diaDaSemana][aluna] = 0;
		paresUtilizados[a1][aluna]--;
		paresUtilizados[aluna][a1]--;
		paresUtilizados[a2][aluna]--;
		paresUtilizados[aluna][a2]--;
		paresUtilizados[a3][aluna]--;
		paresUtilizados[aluna][a3]--;
		matriz[posicao] = -1;
	}
}

// Funcao que imprime a matriz solucao
void imprime(){
	int i, j, k;

	for(i=0; i<5; i++){
		for(j=0; j<7; j++){
			for(k=0; k<4; k++){
				printf("%02d, ", matriz[(j*NALUNAS)+(i*4)+k]);
			}
			printf(" | ");
		}
		printf("\n");
	}

	printf("\n\n");

	return;
}

// Funcao que verifica se determinada matriz solucao eh valida
int ehValido(){
	 int dia, posicao, i, j;

	for(dia=0; dia<7; dia++){
		for(posicao=1; posicao<=NALUNAS; posicao++){
			if(utilizadosNoDia[dia][posicao] == 0){
				return 0;
			}
		}
	} 

	for(i=1; i<=NALUNAS; i++){
		for(j=1; j<=NALUNAS; j++){
			if(paresUtilizados[i][j] > 1){
				return 0;
			}
		}
	}

	return 1;
}

// Funcao que gera a arvore de solucoes usando forca bruta para preencher a matriz solucao
void geraArvore(int posicao){
	//imprime();

	int i;
	for(i=1; i<=NALUNAS; i++){
		posicionaAluna(posicao, i);

		if(posicao == ULTIMA_POSICAO){
			if(ehValido()){
				imprime();
				exit(1);
				return;
			}
		}else{
			geraArvore(posicao+1);
		}

		removeAluna(posicao);
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