#include "globais.h"

/** parser.cpp
*
* Arquivo com funções relacionadas ao parser da entrada para as estruturas de dados do programa.
*
* @date 05.07.2017
* @authors Caio Vianna, David Morosini
*/


 /** @brief Função para contar uma quantidade de caracteres especifica dada uma sequencia de caracteres.
* @param char * str: string de caracteres.
* @param char c: o caractere a ser buscado
* @return int: quantidade de caracteres obtidos naquela string
*/
int conta_caracter(char * str, char c){
	int cont = 0;
	for(; *str; str++){
		if(*str == c){
			cont++;
		}
	}
	return cont;
}

 /** @brief Função para ler da entrada os dados necessários
* @param Não possui parametros de entrada
* @return data: a estrutura de dados Data preenchida
*/
data get_data(void){
	
	char text[TAM_MAX];
	data v;
	int cont = 0;

	/**
	*	Utiliza se duas variaveis char *, sendo uma delas (cadeia_simbolos) usada para de fato 
	*   armazenar as strings lidas e a outra somente para contar a quantidade de '\t' em cada linha.
	*/
	char * cadeia_simbolos, * var_auxiliar;

	fgets(text, TAM_MAX, stdin);

	cadeia_simbolos = strtok(text, SEPARADOR_FULL);

	/**
	*  Lendo somente as informações dos semáforos
	*/
	while(cadeia_simbolos != NULL){
		v.sem.push_back(atoi(cadeia_simbolos));
		cadeia_simbolos = strtok(NULL, SEPARADOR_FULL);
	}

	/**
	*  Lendo as informações dos comandos
	*/
	while(!feof(stdin)){
		fgets(text, TAM_MAX, stdin);
		var_auxiliar = text;

		cadeia_simbolos = strtok(text, " \t");

		int qtd_tabs = conta_caracter(var_auxiliar, SEPARADOR_TAB);


		if(!strcmp(cadeia_simbolos, _THREAD)){
			thread nova;
			nova.last_index = 0;
			cadeia_simbolos = strtok(NULL, " \n\t");
			strcpy(nova.nome, cadeia_simbolos);
			v.threads.push_back(nova);
		}
		else if(!strcmp(cadeia_simbolos, _IF)){
			comando novo;
			novo.comando = IF;
			novo.tabs = qtd_tabs;
			novo.valor = -1;
			cadeia_simbolos = strtok(NULL, " ");
			/**
			*  Sabendo que o comando foi IF, a string subsequente (da mesma linha) é a condição atual do IF
			*/
			strcpy(novo.condicao, cadeia_simbolos);		
			v.threads[v.threads.size() - 1].comandos.push_back(novo);			
		}
		else if(!strcmp(cadeia_simbolos, _ELSE)){
			comando novo;
			novo.valor = -1;
			novo.comando = ELSE;
			novo.tabs = qtd_tabs;
			v.threads[v.threads.size() - 1].comandos.push_back(novo);	
		}
		else if(!strcmp(cadeia_simbolos, _P)){
			comando novo;
			novo.comando = P;
			novo.tabs = qtd_tabs;
			/**
			*  Sabendo que o comando foi P, a string subsequente (da mesma linha) é o valor do semáforo
			*/			
			cadeia_simbolos = strtok(NULL, " ");
			novo.valor = atoi(cadeia_simbolos);
			v.threads[v.threads.size() - 1].comandos.push_back(novo);	
		}
		else if(!strcmp(cadeia_simbolos, _V)){
			comando novo;
			novo.comando = V;
			novo.tabs = qtd_tabs;
			cadeia_simbolos = strtok(NULL, " ");
			/**
			*  Sabendo que o comando foi V, a string subsequente (da mesma linha) é o valor do semáforo
			*/
			novo.valor = atoi(cadeia_simbolos);
			v.threads[v.threads.size() - 1].comandos.push_back(novo);	
		}	
	}
	return v;
}
