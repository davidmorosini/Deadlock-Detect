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
#define _P "p"
#define _V "v"
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
typedef struct Ponto pt;
typedef struct Leitor leitor;

struct Leitor{
	acao a;
	int index;
	int flagV;
	vector<comando> * comandos;
};

struct Comandos{
	char condicao[TAM_MAX];
	tipo comando;
	/** Quantidade de tabulações de cada comando*/
	unsigned int tabs;
	/** Qual o semaforo usado (operações P e V)*/
	unsigned char valor;
};

struct Thread{
	char nome[TAM_MAX];
	unsigned int last_index;
	vector<comando> comandos;
};

struct Data{
	vector<thread> threads;
	vector<int> sem;
};

struct Ponto{
	int x, y;
};

#endif
