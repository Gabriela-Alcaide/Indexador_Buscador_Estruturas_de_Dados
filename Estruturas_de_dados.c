#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define TAMANHO 1000

                                                                   // Função strsep
char* strsep(char** stringp, const char* delim) {
	char* begin, * end;
	begin = *stringp;

	if (begin == NULL) return NULL;

	/* Find the end of the token.  */
	end = begin + strcspn(begin, delim);

	if (*end)
	{
		/* Terminate the token and set *STRINGP past NUL character.  */
		*end++ = '\0';
		*stringp = end;
	}
	else
		/* No more delimiters; this is the last token.  */
		*stringp = NULL;

	return begin;
}


                                                                   // Estrutura que armazena as linhas do texto.

// Struct de cada nó (cada linha do texto).
typedef struct _linha_ {
	char* caracteres;
	int numero_dessa_linha;
	struct _linha_* proxima_linha;
} Linha;

// Struct da estrutura em si.
typedef struct {
	Linha* primeira_linha;
	int quantidade_linhas;
} Texto;

// Criar estrutura vazia.
Texto* criar_estrutura_guarda_texto() {
	Texto* texto = (Texto*)malloc(sizeof(Texto));
	texto->primeira_linha = NULL;
	texto->quantidade_linhas = 0;
	return texto;
}

// Inserir um nó (inserir uma linha do texto na estrutura).
void inserir_linha(Texto* texto, char* infos) {
	Linha* nova_linha = (Linha*)malloc(sizeof(Linha));

	// Se essa é a primeira linha do texto.
	if (texto->primeira_linha == NULL) {
		texto->primeira_linha = nova_linha;
	}
	// Se já há linhas no texto.
	else {
		Linha* p = texto->primeira_linha;

		while (p->proxima_linha) {
			p = p->proxima_linha;
		}
		// Agora, "p" aponta para a última linha na estrutura (quero inserir a nova linha depois dela).

		p->proxima_linha = nova_linha;
	}
	// Agora a nova linha está inserida na estrutura.

	// O texto passa a ter 01 linha a mais.
	texto->quantidade_linhas++;

	// Guardar os caracteres da linha.
	nova_linha->caracteres = (char*)malloc(sizeof(char) * TAMANHO);
	strcpy(nova_linha->caracteres, infos);

	// O nó "sabe" qual linha ele representa.
	nova_linha->numero_dessa_linha = texto->quantidade_linhas;

	// Não há linha após a linha recém-inserida.
	nova_linha->proxima_linha = NULL;
}

// Buscar um nó (buscar uma linha do texto na estrutura).
Linha* buscar_linha(Texto* texto, int numero_linha) {
	int i = 1;

	Linha* p = texto->primeira_linha;

	while (p) {
		if (i == numero_linha) return p;
		p = p->proxima_linha;
		i++;
	}

	return NULL;
}


                                                                   // Estrutura que armazena as ocorrências de uma palavra.
// Struct de cada nó (linha de cada ocorrência).
typedef struct _ocorrencia_ {
	int linha_onde_ocorre;
	struct _ocorrencia_* proxima_ocorrencia;
} Ocorrencia;

// Struct da estrutura em si.
typedef struct {
	Ocorrencia* primeira_ocorrencia;
} OcorrenciasDaPalavra;

// Criar estrutura vazia.
OcorrenciasDaPalavra* criar_lista_guarda_ocorrencias() {
	OcorrenciasDaPalavra* listaDeOcorrencias = (OcorrenciasDaPalavra*)malloc(sizeof(OcorrenciasDaPalavra));
	listaDeOcorrencias->primeira_ocorrencia = NULL;
	return listaDeOcorrencias;
}

// Inserir um nó (inserir uma ocorrência da palavra na lista).
void inserir_ocorrencia(OcorrenciasDaPalavra* listaDeOcorrencias, int linha) {
	Ocorrencia* nova_ocorrencia = (Ocorrencia*)malloc(sizeof(Ocorrencia));

	// Se essa é a primeira ocorrência.
	if (listaDeOcorrencias->primeira_ocorrencia == NULL) {
		listaDeOcorrencias->primeira_ocorrencia = nova_ocorrencia;
	}
	// Se já há ocorrências na estrutura.
	else {
		Ocorrencia* p = listaDeOcorrencias->primeira_ocorrencia;

		while (p->proxima_ocorrencia) {
			p = p->proxima_ocorrencia;
		}
		// Agora "p" aponta para a última ocorrência na estrutura (quero inserir a nova ocorrência depois dela).

		p->proxima_ocorrencia = nova_ocorrencia;
	}
	// Agora a nova ocorrência está na lista.

	nova_ocorrencia->linha_onde_ocorre = linha;
}

// Imprimir os nós (linhas onde uma palavra ocorre).
void imprimir_ocorrencias(Texto* texto, OcorrenciasDaPalavra* listaDeOcorrencias) {
	Ocorrencia* p;
	int linha_anterior = 0;

	for (p = listaDeOcorrencias->primeira_ocorrencia; p; p = p->proxima_ocorrencia) {
		int linha_dessa_ocorrencia = p->linha_onde_ocorre;

		if (linha_dessa_ocorrencia != linha_anterior) {
			Linha* endereco_linha = buscar_linha(texto, linha_dessa_ocorrencia);
			char* conteudo_linha = endereco_linha->caracteres;
			printf("%05d: %s\n", linha_dessa_ocorrencia, conteudo_linha);
		}

		linha_anterior = linha_dessa_ocorrencia;
	}
}


                                                                   // Estrutura para indexação usando Lista Ligada.

// Struct de cada nó (cada palavra do texto).
typedef struct _palavra_ {
	char* caracteres;
	int quantas_vezes_ocorro;
	OcorrenciasDaPalavra* em_quais_linhas_ocorro;
	struct _palavra_* proxima_palavra;
} Palavra;

// Struct da estrutura em si.
typedef struct {
	Palavra* primeira_palavra;
} Lista;

// Criar estrutura vazia.
Lista* criar_lista_guarda_palavras() {
	Lista* lista = (Lista*)malloc(sizeof(Lista));
	lista->primeira_palavra = NULL;
	return lista;
}

// Inserir um nó (inserir uma palavra na estrutura).
void inserir_palavra_lista(Lista* lista, char* palavra, int linha) {
	Palavra* nova_palavra = (Palavra*)malloc(sizeof(Palavra));

	// Se essa é a primeira palavra.
	if (lista->primeira_palavra == NULL) {
		lista->primeira_palavra = nova_palavra;
	}
	// Se já há palavras na estrutura.
	else {
		Palavra* p = lista->primeira_palavra;

		while (p->proxima_palavra) {
			p = p->proxima_palavra;
		}
		// Agora "p" aponta para a última palavra na estrutura (quero inserir a nova palavra depois dela).

		p->proxima_palavra = nova_palavra;
	}
	// Agora a nova palavra está na lista.

	// Guardar os caracteres.
	nova_palavra->caracteres = (char*)malloc(sizeof(char) * 50);
	strcpy(nova_palavra->caracteres, palavra);

	// A palavra "sabe" em quais linhas está.
	nova_palavra->em_quais_linhas_ocorro = criar_lista_guarda_ocorrencias();
	inserir_ocorrencia(nova_palavra->em_quais_linhas_ocorro, linha);

	// A palavra "sabe" quantas vezes aparece no texto.
	nova_palavra->quantas_vezes_ocorro = 1;

	// Não há palavras após a palavra recém-inserida.
	nova_palavra->proxima_palavra = NULL;
}

// Buscar um nó (buscar uma palavra).
Palavra* buscar_palavra_lista(Lista* lista, char* palavra) {
	Palavra* p = lista->primeira_palavra;

	while (p) {
		if (strcmp(palavra, p->caracteres) == 0) return p;
		p = p->proxima_palavra;
	}

	return NULL;
}





                                                                    // Estrutura para indexação usando Árvore Binária de Busca.

// Struct de cada nó (cada palavra do texto).
typedef struct _palavra_arvore_ {
	char* caracteres;
	int quantas_vezes_ocorro;
	OcorrenciasDaPalavra* em_quais_linhas_ocorro;
	struct _palavra_arvore_* proxima_palavra_esquerda;
	struct _palavra_arvore_* proxima_palavra_direita;
} Palavra_Arvore;

// Struct da estrutura em si.
typedef struct {
	Palavra_Arvore* primeira_palavra;
} Arvore;

// Criar estrutura vazia.
Arvore* criar_arvore_guarda_palavras() {
	Arvore* arvore = (Arvore*)malloc(sizeof(Arvore));
	arvore->primeira_palavra = NULL;
	return arvore;
}

// Inserir um nó (inserir uma palavra na estrutura).
void inserir_palavra_arvore_recursiva(Palavra_Arvore* primeira, Palavra_Arvore* nova) {

	if (strcmp(nova->caracteres, primeira->caracteres) == -1) {
		if (primeira->proxima_palavra_esquerda) inserir_palavra_arvore_recursiva(primeira->proxima_palavra_esquerda, nova);
		else primeira->proxima_palavra_esquerda = nova;
	}

	if (strcmp(nova->caracteres, primeira->caracteres) == 1) {
		if (primeira->proxima_palavra_direita) inserir_palavra_arvore_recursiva(primeira->proxima_palavra_direita, nova);
		else primeira->proxima_palavra_direita = nova;
	}

}

void inserir_palavra_arvore(Arvore* arvore, char* palavra, int linha) {
	Palavra_Arvore* nova_palavra = (Palavra_Arvore*)malloc(sizeof(Palavra_Arvore));

	// Guardar os caracteres.
	nova_palavra->caracteres = (char*)malloc(sizeof(char) * 50);
	strcpy(nova_palavra->caracteres, palavra);

	// A palavra "sabe" em quais linhas está.
	nova_palavra->em_quais_linhas_ocorro = criar_lista_guarda_ocorrencias();
	inserir_ocorrencia(nova_palavra->em_quais_linhas_ocorro, linha);

	// A palavra "sabe" quantas vezes aparece no texto.
	nova_palavra->quantas_vezes_ocorro = 1;

	// Não há palavras após a palavra recém-inserida.
	nova_palavra->proxima_palavra_esquerda = NULL;
	nova_palavra->proxima_palavra_direita = NULL;

	// Se essa é a primeira palavra.
	if (arvore->primeira_palavra == NULL) {
		arvore->primeira_palavra = nova_palavra;
	}
	// Se já há palavras na estrutura.
	else {
		inserir_palavra_arvore_recursiva(arvore->primeira_palavra, nova_palavra);
	}
}

// Buscar um nó (buscar uma palavra).
Palavra_Arvore* buscar_palavra_arvore_recursiva(Palavra_Arvore* no, char* palavra) {

	if (no) {
		if (strcmp(palavra, no->caracteres) == 0) return no;
		if (strcmp(palavra, no->caracteres) == -1) return buscar_palavra_arvore_recursiva(no->proxima_palavra_esquerda, palavra);
		return buscar_palavra_arvore_recursiva(no->proxima_palavra_direita, palavra);
	}

	return NULL;
}

Palavra_Arvore* buscar_palavra_arvore(Arvore* arvore, char* palavra) {
	return buscar_palavra_arvore_recursiva(arvore->primeira_palavra, palavra);
}

// Imprimir palavras (para teste).
void imprimir_recursiva(Palavra_Arvore* no) {
	if (no) {
		imprimir_recursiva(no->proxima_palavra_esquerda);
		printf("%s\n", no->caracteres);
		imprimir_recursiva(no->proxima_palavra_direita);
	}
}

void imprimir_arvore(Arvore* arvore) {
	imprimir_recursiva(arvore->primeira_palavra);
	printf("\n");
}





                                                                   // Programa

// Recebe 03 parâmetros por linha de comando:
	// argc: quantidade de parâmetros.
	// argv[0]: nome do programa.
	// argv[1]: nome do arquivo de texto a ser lido.
	// argv[2]: estrutura de dados para indexação das palavras.

int main(int argc, char** argv) {

	// Arquivo de texto.
	FILE* in;

	// Array que armazena, temporariamente, cada linha do texto.
	char* linha;

	// Auxiliar para iterar pelas palavras.
	char* copia_ponteiro_linha;

	// Endereço da quebra de linha.
	char* quebra_de_linha;

	// Armazena, temporariamente, cada palavra da linha que está sendo lida.
	char* palavra;

	// Em qual linha do texto estou.
	int contador_linha;

	// Se recebi os argumentos necessários, executo o programa.
	if (argc == 3) {

		// Verificar quando se inicia a carga inicial do arquivo e a construção do índice.
		int inicioCargaArquivoEIndices = GetTickCount();

		// Carregar o arquivo.
		in = fopen(argv[1], "r");

		// Começo com 0 linhas lidas.
		contador_linha = 0;

		// Alocar array que armazena, temporariamente, cada linha do texto.
		linha = (char*)malloc((TAMANHO + 1) * sizeof(char));

		// Alocar estrutura que armazena as linhas do texto.
		Texto* texto = criar_estrutura_guarda_texto();

		// Aloca estrutura de Lista que armazena as palavras do texto.
		Lista* lista = criar_lista_guarda_palavras();

		// Aloca estrutura de árvore que armazena as palavras do texto.
		Arvore* arvore = criar_arvore_guarda_palavras();
	
		// Pega cada linha do texto e coloca em "linha".
		while (in && fgets(linha, TAMANHO, in)) {

				if ((quebra_de_linha = strrchr(linha, '\n'))) *quebra_de_linha = 0;

				// Inserir a linha na estrutura.
				inserir_linha(texto, linha);

				// fazemos uma copia do endereço que corresponde ao array de chars 
				// usado para armazenar cada linha lida do arquivo pois a função 'strsep' 
				// modifica o endereço do ponteiro a cada chamada feita a esta função (e 
				// não queremos que 'linha' deixe de apontar para o inicio do array).
				copia_ponteiro_linha = linha;

				// Pega cada palavra da linha.
				while ((palavra = strsep(&copia_ponteiro_linha, " .,/-"))) {

					if (strlen(palavra) != 0) {
						// antes de guardar a palavra em algum tipo de estrutura usada
						// para implementar o índice, será necessário fazer uma copia
						// da mesma, uma vez que o ponteiro 'palavra' aponta para uma 
						// substring dentro da string 'linha', e a cada nova linha lida
						// o conteúdo da linha anterior é sobreescrito.

						// Se a estrutura a ser usada é a lista.
						if (strcmp(argv[2], "lista") == 0) {
							Palavra* palavra_encontrada;

							// Guarda a palavra na estrutura.
							// Se a palavra já existe na estrutura.
							if ((palavra_encontrada = buscar_palavra_lista(lista, strlwr(palavra)))) {
								inserir_ocorrencia(palavra_encontrada->em_quais_linhas_ocorro, contador_linha + 1);
							}
							// Se a palavra ainda não existe na estrutura.
							else {
								inserir_palavra_lista(lista, strlwr(palavra), contador_linha + 1);
							}
						}
						// Se a estrutura a ser usada é a árvore;
						else {
							Palavra_Arvore* palavra_encontrada;

							// Guarda a palavra na estrutura.
							// Se a palavra já existe na estrutura.
							if ((palavra_encontrada = buscar_palavra_arvore(arvore, strlwr(palavra)))) {
								inserir_ocorrencia(palavra_encontrada->em_quais_linhas_ocorro, contador_linha + 1);
							}
							// Se a palavra ainda não existe na estrutura.
							else {
								inserir_palavra_arvore(arvore, strlwr(palavra), contador_linha + 1);
							}
						}


					}
				}

				contador_linha++;

		}

		// Agora todas as linhas e todas as palavras estão armazenadas em suas estruturas.

		// Verificar quando termina a carga inicial do arquivo e a construção do índice.
		int fimCargaArquivoEIndices = GetTickCount();

		printf("Tipo de indice: '%s'\n", argv[2]);
		printf("Arquivo texto: '%s'\n", argv[1]);
		printf("Numero de linhas no arquivo: %d\n", contador_linha);
		printf("Tempo para carregar o arquivo e construir o indice: %d ms\n", fimCargaArquivoEIndices - inicioCargaArquivoEIndices);

		char* palavra_buscar = (char*)malloc(sizeof(char) * 10);
		char* comando = (char*)malloc(sizeof(char) * 10);

		// Se a estrutura de indexação é a Lista.
		if (strcmp(argv[2], "lista") == 0) {
			printf("> ");
			scanf("%s", comando);

			while (strcmp(comando, "fim") != 0) {
				scanf("%s", palavra_buscar);

				// Se o comando é "busca"
				if (strcmp(comando, "busca") == 0) {

					// Verificar quando se inicia a busca da palavra.
					int inicioBuscaLista = GetTickCount();
					
					Palavra* endereco_palavra_encontrada = buscar_palavra_lista(lista, strlwr(palavra_buscar));

					// Verificar quando termina a busca da palavra.
					int fimBuscaLista = GetTickCount();

					// Se a palavra existe no texto.
					if (endereco_palavra_encontrada) {
						printf("Existem %d ocorrencias da palavra '%s' na(s) seguinte(s) linha(s):\n", endereco_palavra_encontrada->quantas_vezes_ocorro, endereco_palavra_encontrada->caracteres);

						imprimir_ocorrencias(texto, endereco_palavra_encontrada->em_quais_linhas_ocorro);
					}
					// Se a palavra não existe no texto.
					else {
						printf("Palavra '%s' nao encontrada.\n", palavra_buscar);
					}

					printf("Tempo de busca: %d ms\n", fimBuscaLista - inicioBuscaLista);
				}
				// Se o comando não é "fim" nem "busca"
				else {
					printf("Opcao invalida!\n");
				}

				printf("> ");
				scanf("%s", comando);
			}		
		}
		// Se a estrutura de indexação é a Árvore.
		else {
			printf("> ");
			scanf("%s", comando);

			while (strcmp(comando, "fim") != 0) {
				scanf("%s", palavra_buscar);

				// Se o comando é "busca"
				if (strcmp(comando, "busca") == 0) {

					// Verificar quando se inicia a busca da palavra.
					int inicioBuscaArvore = GetTickCount();

					Palavra_Arvore* endereco_palavra_encontrada = buscar_palavra_arvore(arvore, strlwr(palavra_buscar));

					// Verificar quando termina a busca da palavra.
					int fimBuscaArvore = GetTickCount();

					// Se a palavra existe no texto.
					if (endereco_palavra_encontrada) {
						printf("Existem %d ocorrencias da palavra '%s' na(s) seguinte(s) linha(s):\n", endereco_palavra_encontrada->quantas_vezes_ocorro, endereco_palavra_encontrada->caracteres);

						imprimir_ocorrencias(texto, endereco_palavra_encontrada->em_quais_linhas_ocorro);

					}
					// Se a palavra não existe no texto.
					else {
						printf("Palavra '%s' nao encontrada.\n", palavra_buscar);
					}

					printf("Tempo de busca: %d ms\n", fimBuscaArvore - inicioBuscaArvore);
				}
				// Se o comando não é "fim" nem "busca".
				else {
					printf("Opcao invalida!\n");
				}

				printf("> ");
				scanf("%s", comando);
			}

		}


		// FUNCIONA: AS LINHAS ESTÃO SENDO GUARDADAS NA ESTRUTURA.
/*
		Linha* ponteiro_linha;
		for (ponteiro_linha = texto->primeira_linha; ponteiro_linha; ponteiro_linha = ponteiro_linha->proxima_linha) {
			printf("Numero da linha: %d\n", ponteiro_linha->numero_dessa_linha);
			printf("Linha: %s\n", ponteiro_linha->caracteres);
		}
*/



		// FUNCIONA: AS LINHAS ESTÃO SENDO BUSCADAS NA ESTRUTURA DE ACORDO COM SEU NÚMERO.
/*
		Linha* linha = buscar_linha(texto, 2);
		printf("Numero da linha: %d\n", linha->numero_dessa_linha);
		printf("Linha: %s\n", linha->caracteres);
*/

		// FUNCIONA: AS PALAVRAS ESTÃO SENDO GUARDADAS NA LISTA - COM REPETIÇÃO.
/*
		Palavra* ponteiro_palavra;
		for (ponteiro_palavra = lista->primeira_palavra; ponteiro_palavra; ponteiro_palavra = ponteiro_palavra->proxima_palavra) {
			printf("Em qual linha esta: %d\n", ponteiro_palavra->em_quais_linhas_estou[ponteiro_palavra->i]);
			printf("Palavra: %s\n", ponteiro_palavra->caracteres);
			printf("Quantas vezes: %d\n", ponteiro_palavra->quantas_vezes_ocorro);
		}
*/		

		// FUNCIONA: AS PALAVRAS ESTÃO SENDO GUARDADAS NA LISTA - SEM REPETIÇÃO.
/*
		Palavra* ponteiro_palavra;
		for (ponteiro_palavra = lista->primeira_palavra; ponteiro_palavra; ponteiro_palavra = ponteiro_palavra->proxima_palavra) {
			printf("Em qual linha esta: %d\n", ponteiro_palavra->em_quais_linhas_estou[ponteiro_palavra->i]);
			printf("Palavra: %s\n", ponteiro_palavra->caracteres);
			printf("Quantas vezes: %d\n", ponteiro_palavra->quantas_vezes_ocorro);
		}
*/


		// FUNCIONA: AS PALAVRAS ESTÃO SENDO GUARDADAS NA ÁRVORE.
/*
		imprimir_arvore(arvore);
*/


		return 0;
	}

	// Se não recebi os argumentos necessários, não executo o programa.
	return 1;
}