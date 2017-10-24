#include "globais.h"


void exibe_matriz(matriz m){
	for(int i = 0; i < m.size(); i++){
		for(int j = 0; j < m[i].size(); j++){
			cout << m[i][j] << "\t";
		}
		cout << endl;
	}
}



matriz aloca_matriz(int l, int c){
	matriz m(l, vector<t_data>(c));
	int i, j;
	for(i = 0; i < l; i++){
		for(j = 0; j < c; j++){
			m[i][j] = 0;
		}
	}
	return m;
}

matriz cria_matriz_deteccao(vector<leitor > threads, unsigned int qdt_recursos){
	//a criação da matriz conta a disposição dos recursos e processos como uma matriz de adjacências
	unsigned int tam = threads.size() + qdt_recursos;
	matriz resp = aloca_matriz(tam, tam);

	int contador = threads.size();

	int i, j;
	for(i = 0; i < threads.size(); i++){
		for(j = 0; j < (*threads[i].comandos).size(); j++){
			comando c = (*threads[i].comandos)[j];
			//marcando a solicitação do recurso
			resp[i][contador + c.valor] = 1;
			resp[contador + c.valor][i] = 1;
		}
	}
	return resp;
}

bool detecta_deadlock(matriz m, int x, int y, int qtd){
	int cont = 0;
	int i = 0, x_ref = y, y_ref = x;
	
	while(i < m[x_ref].size() &&  cont < qtd){
		//cout << m[x_ref][i] << endl;
		if(m[x_ref][i] != 0 && i != y_ref){
			//cout << "x,y : " << x_ref << i << endl;
			cont++;
			y_ref = x_ref;
			x_ref = i;
			i = 0;
			if(cont >= 4 && y_ref == x){//condicao que aparenta o menor deadlock possível
				//tem deadlock nessa porra
				return true;
			}
		} 
		i++;
	}

	//cout << cont << endl;

	cout << endl << endl;
	return false;
}
 
bool percorre_matriz(matriz m){
	int i, j;
	bool deadlock = false;
	int cont = 0, cont2 = 0;

	for(i = 0; i < m.size(); i++){
		for(j = 0; j < m[i].size(); j++){
			if(m[i][j] == 1){
				cont++;
			}
		}
	}



	for(i = 0; i < m.size(); i++){
		for(j = 0; j < m[i].size(); j++){
			if(m[i][j] == 1){
				deadlock = detecta_deadlock(m, i, j, cont);
				if(deadlock){
					return deadlock;
				}
			}

		}
	}
	return deadlock;
}



