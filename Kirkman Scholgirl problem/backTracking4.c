#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Declaração de constantes
#define ULTIMA_POSICAO 139
#define NALUNAS 20

// Declaração de variaveis globais
int matriz[200], paresUtilizados[25][25], utilizadosNoDia[10][25], nIteracoes;

// Funcao que verifica se uma aluna pode andar em determinado dia da semana
int podeAndar(int posicao, int aluna){
	int tamanhoGrupo = posicao%4;
	int diaDaSemana = posicao/NALUNAS;
	int nGruposAnteriores = (posicao-(diaDaSemana*NALUNAS))/4;

	if(tamanhoGrupo == 0){
		if(utilizadosNoDia[diaDaSemana][aluna] == 1){
			return 0;
		}else{

			int i;
			for(i=0; i<nGruposAnteriores; i++){
				if(matriz[(4*i)+tamanhoGrupo + (diaDaSemana*NALUNAS)] >= aluna){
					return 0;
				}
			}

			if(diaDaSemana != 0){
				if(nGruposAnteriores==0 && aluna!=1){
					return 0;
				}else if(nGruposAnteriores==1 && aluna!=2){
					return 0;
				}else if(nGruposAnteriores==2 && aluna!=3){
					return 0;
				}
			}

			return 1;
		}
	}else if(tamanhoGrupo == 1){
		int a1;
		a1 = matriz[posicao-1];

		if(utilizadosNoDia[diaDaSemana][aluna] == 1){
			return 0;
		}else if(paresUtilizados[a1][aluna] == 1){
			return 0;
		}else{
			if(a1>=aluna){
				return 0;
			}

			int i;
			if(nGruposAnteriores==0){
				for(i=1; i<posicao; i+=NALUNAS){
					//printf("** %d\n", matriz[i]);
					if(matriz[i]>=aluna){
						return 0;
					}
				}
			}


			return 1;
		}
	}else if(tamanhoGrupo == 2){
		int a1, a2;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];

		if(utilizadosNoDia[diaDaSemana][aluna] == 1){
			return 0;
		}else if(paresUtilizados[a1][aluna] == 1){
			return 0;
		}else if(paresUtilizados[a2][aluna] == 1){
			return 0;
		}else{
			if(a1>=aluna){
				return 0;
			}

			int i;
			if(nGruposAnteriores==0){
				for(i=2; i<posicao; i+=NALUNAS){
					//printf("** %d\n", matriz[i]);
					if(matriz[i]>=aluna){
						return 0;
					}
				}
			}

			return 1;
		}
	}else{
		int a1, a2, a3;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];
		a3 = matriz[posicao-3];

		if(utilizadosNoDia[diaDaSemana][aluna] == 1){
			return 0;
		}else if(paresUtilizados[a1][aluna] == 1){
			return 0;
		}else if(paresUtilizados[a2][aluna] == 1){
			return 0;
		}else if(paresUtilizados[a3][aluna] == 1){
			return 0;
		}else{
			if(a1>=aluna){
				return 0;
			}

			return 1;
		}
	}
}

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
		paresUtilizados[a1][aluna] = 1;
		paresUtilizados[aluna][a1] = 1;
		matriz[posicao] = aluna;
	}else if(tamanhoGrupo == 2){
		int a1, a2;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];

		utilizadosNoDia[diaDaSemana][aluna] = 1;
		paresUtilizados[a1][aluna] = 1;
		paresUtilizados[aluna][a1] = 1;
		paresUtilizados[a2][aluna] = 1;
		paresUtilizados[aluna][a2] = 1;
		matriz[posicao] = aluna;
	}else{
		int a1, a2, a3;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];
		a3 = matriz[posicao-3];

		utilizadosNoDia[diaDaSemana][aluna] = 1;
		paresUtilizados[a1][aluna] = 1;
		paresUtilizados[aluna][a1] = 1;
		paresUtilizados[a2][aluna] = 1;
		paresUtilizados[aluna][a2] = 1;
		paresUtilizados[a3][aluna] = 1;
		paresUtilizados[aluna][a3] = 1;
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
		paresUtilizados[a1][aluna] = 0;
		paresUtilizados[aluna][a1] = 0;
		matriz[posicao] = -1;
	}else if(tamanhoGrupo == 2){
		int a1, a2;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];

		utilizadosNoDia[diaDaSemana][aluna] = 0;
		paresUtilizados[a1][aluna] = 0;
		paresUtilizados[aluna][a1] = 0;
		paresUtilizados[a2][aluna] = 0;
		paresUtilizados[aluna][a2] = 0;
		matriz[posicao] = -1;
	}else{
		int a1, a2, a3;
		a1 = matriz[posicao-1];
		a2 = matriz[posicao-2];
		a3 = matriz[posicao-3];

		utilizadosNoDia[diaDaSemana][aluna] = 0;
		paresUtilizados[a1][aluna] = 0;
		paresUtilizados[aluna][a1] = 0;
		paresUtilizados[a2][aluna] = 0;
		paresUtilizados[aluna][a2] = 0;
		paresUtilizados[a3][aluna] = 0;
		paresUtilizados[aluna][a3] = 0;
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

// Funcao que gera a arvore de solucoes usando backtracking para preencher a matriz solucao
void geraArvore(int posicao){
	//imprime();

	int i;
	for(i=1; i<=NALUNAS; i++){
		if(podeAndar(posicao, i) == 1){
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