#include "globais.h"

int conta_caracter(char * str, char c){
	int cont = 0;
	for(; *str; str++){
		if(*str == c){
			cont++;
		}
	}
	return cont;
}

data get_data(void){
	char text[TAM_MAX];
	
	data v;

	int cont = 0;

	char * cadeia_simbolos, * var_auxiliar;

	//fazer a leitura dos semaforos
	fgets(text, TAM_MAX, stdin);


	cadeia_simbolos = strtok(text, SEPARADOR_FULL);



	while(cadeia_simbolos != NULL){
		v.sem.push_back(atoi(cadeia_simbolos));
		cadeia_simbolos = strtok(NULL, SEPARADOR_FULL);
	}


	//leitura dos comandos
	while(!feof(stdin)){
		fgets(text, TAM_MAX, stdin);

		var_auxiliar = text;

		cadeia_simbolos = strtok(text, " \t");

		int qtd_tabs = conta_caracter(var_auxiliar, SEPARADOR_TAB);


		//funciona

		if(!strcmp(cadeia_simbolos, _THREAD)){
			thread nova;
			nova.last_index = 0;
			//captura o "nome da thread"	
			cadeia_simbolos = strtok(NULL, " ");
			strcpy(nova.nome, cadeia_simbolos);
			v.threads.push_back(nova);
		}
		else if(!strcmp(cadeia_simbolos, _IF)){
			comando novo;
			novo.comando = IF;
			novo.tabs = qtd_tabs;
			cadeia_simbolos = strtok(NULL, " ");
			strcpy(novo.condicao, cadeia_simbolos);
			v.threads[v.threads.size() - 1].comandos.push_back(novo);			
		}
		else if(!strcmp(cadeia_simbolos, _ELSE)){
			comando novo;
			novo.comando = ELSE;
			novo.tabs = qtd_tabs;
			v.threads[v.threads.size() - 1].comandos.push_back(novo);	
		}
		else if(!strcmp(cadeia_simbolos, _P)){
			comando novo;
			novo.comando = P;
			novo.tabs = qtd_tabs;
			//qual o semaforo que deu o comando
			cadeia_simbolos = strtok(NULL, " ");
			novo.valor = atoi(cadeia_simbolos);
			v.threads[v.threads.size() - 1].comandos.push_back(novo);	
		}
		else if(!strcmp(cadeia_simbolos, _V)){
			comando novo;
			novo.comando = V;
			novo.tabs = qtd_tabs;
			cadeia_simbolos = strtok(NULL, " ");
			//qual o semaforo que deu o comando
			novo.valor = atoi(cadeia_simbolos);
			v.threads[v.threads.size() - 1].comandos.push_back(novo);	
		}	


	}
	return v;
}
