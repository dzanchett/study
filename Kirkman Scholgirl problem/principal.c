#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Declaração de variaveis globais
int matriz[200], paresUtilizados[25][25], utilizadosNoDia[10][25], ultimaPosicao, nAlunas, alunasPorGrupo;
unsigned long long int nIteracoes;

// Funcao que verifica se uma aluna pode andar em determinado dia da semana
int podeAndar(int posicao, int aluna){
	int tamanhoGrupo = posicao%alunasPorGrupo;
	int diaDaSemana = posicao/nAlunas;
	int nGruposAnteriores = (posicao-(diaDaSemana*nAlunas))/alunasPorGrupo;

	if(tamanhoGrupo == 0){
		if(utilizadosNoDia[diaDaSemana][aluna] == 1){
			return 0;
		}else{

			int i;
			for(i=0; i<nGruposAnteriores; i++){
				if(matriz[(alunasPorGrupo*i)+tamanhoGrupo + (diaDaSemana*nAlunas)] >= aluna){
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

			if(alunasPorGrupo == 3){
				int i;
				if(nGruposAnteriores==0){
					for(i=1; i<posicao; i+=nAlunas){
						//printf("** %d\n", matriz[i]);
						if(matriz[i]>=aluna){
							return 0;
						}
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

			return 1;
		}
	}else if(tamanhoGrupo == 3){
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
	int tamanhoGrupo = posicao%alunasPorGrupo;
	int diaDaSemana = posicao/nAlunas;

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
	}else if(tamanhoGrupo == 3){
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
	int tamanhoGrupo = posicao%alunasPorGrupo;
	int diaDaSemana = posicao/nAlunas;
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
	}else if(tamanhoGrupo == 3){
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

// Funcao que verifica se determinada matriz solucao eh valida
int ehValido(){
	 int dia, posicao, i, j;

	for(dia=0; dia<7; dia++){
		for(posicao=1; posicao<=nAlunas; posicao++){
			if(utilizadosNoDia[dia][posicao] == 0){
				return 0;
			}
		}
	} 

	for(i=1; i<=nAlunas; i++){
		for(j=1; j<=nAlunas; j++){
			if(paresUtilizados[i][j] > 1){
				return 0;
			}
		}
	}

	return 1;
}

// Funcao que imprime a matriz solucao
void imprime(){
	int i, j, k;

	printf("\n");

	for(i=0; i<5; i++){
		for(j=0; j<7; j++){
			for(k=0; k<alunasPorGrupo; k++){
				if(k){
					printf(", ");
				}
				printf("%02d", matriz[(j*nAlunas)+(i*alunasPorGrupo)+k]);
			}
			printf(" | ");
		}
		printf("\n");
	}

	printf("\n\n");

	printf("Ocorreram %Ld chamadas a função recursiva\n", nIteracoes);

	printf("\n\n");

	return;
}

// Funcao que gera a arvore de solucoes usando backtracking para preencher a matriz solucao
void geraArvoreBacktracking(int posicao){
	//imprime();
	nIteracoes++;

	int i;
	for(i=1; i<=nAlunas; i++){
		if(podeAndar(posicao, i) == 1){
			posicionaAluna(posicao, i);

			if(posicao == ultimaPosicao){
				imprime();
				exit(1);
				return;
			}else{
				geraArvoreBacktracking(posicao+1);
			}

			removeAluna(posicao);
		}
	}
}

// Funcao que gera a arvore de solucoes usando forca bruta para preencher a matriz solucao
void geraArvoreForcaBruta(int posicao){
	//imprime();
	nIteracoes++;

	int i;
	for(i=1; i<=nAlunas; i++){
		posicionaAluna(posicao, i);

		if(posicao == ultimaPosicao){
			if(ehValido()){
				imprime();
				exit(1);
				return;
			}
		}else{
			geraArvoreForcaBruta(posicao+1);
		}

		removeAluna(posicao);
	}
}

// Funcao principal
int main(){
	int nTecnica, valorN, flag1, flag2;

	nIteracoes = 0;
	flag1 = 1;
	flag2 = 1;

	memset(utilizadosNoDia, 0, sizeof utilizadosNoDia);
	memset(paresUtilizados, 0, sizeof paresUtilizados);
	memset(matriz, -1, sizeof matriz);


	printf("Kirkman's  schoolgirl  problem\n\n");

	while(flag1){
		flag1 = 0;
		printf("Insira a técnica que você deseja usar:\n\t1 -> Forca Bruta\n\t2 -> Backtracking\n>>> ");
		scanf("%d", &nTecnica);

		switch(nTecnica){
			case 1:
			case 2:
				while(flag2){
					flag2 = 0;
					printf("Insira um valor para n:\n(Os valores disponíveis para n encontram-se abaixo)\n\t-> 10\n\t-> 15\n\t-> 20\n>>> ");
					scanf("%d", &valorN);
					switch(valorN){
						case 10:
							ultimaPosicao = 69;
							nAlunas = 10;
							alunasPorGrupo = 2;
							break;
						case 15:
							ultimaPosicao = 104;
							nAlunas = 15;
							alunasPorGrupo = 3;
							break;
						case 20:
							ultimaPosicao = 139;
							nAlunas = 20;
							alunasPorGrupo = 4;
							break;
						default:
							printf("\n\033[31;1mValor inválido para n!\033[0m\n\n");
							flag2 = 1;
							break;
					}
				}
				if(nTecnica == 1){
					geraArvoreForcaBruta(0);
				}else{
					geraArvoreBacktracking(0);
				}
				break;
			default:
				printf("\n\033[31;1mValor inválido para técnica!\033[0m\n\n\n");
				flag1 = 1;
				break;
		}
	}

	return 0;

}