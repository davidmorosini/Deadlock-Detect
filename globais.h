#ifndef GLOBAIS_H
#define GLOBAIS_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <stack>

#define _THREAD "thread"
#define _IF "if"
#define _ELSE "else"
#define _P "P"
#define _V "V"
#define SEPARADOR_FULL " \t"
#define SEPARADOR_TAB '\t'
#define TAM_MAX 200
#define MAX 999


using namespace std;



typedef int t_data;

typedef vector<vector <t_data> > matriz;

typedef enum {IF, ELSE, P, V}tipo;
typedef enum{EMPILHA, DESEMPILHA}acao;

typedef struct Thread thread;
typedef struct Comandos comando;
typedef struct Data data;

//thread

typedef struct leitor{
	acao a;
	int index;
	int flagV;
	vector<comando> * comandos;
}leitor;

struct Comandos{
	char condicao[20];
	tipo comando;
	//esse valor vai fazer sentido dependendo do comando
	unsigned int tabs;
	//semaforo
	unsigned char valor;
};

struct Thread{
	char nome[20];
	unsigned int last_index;
	vector<comando> comandos;
};

struct Data{
	vector<thread> threads;
	vector<int> sem;
};

#endif
