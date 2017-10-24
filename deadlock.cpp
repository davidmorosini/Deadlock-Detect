#include "globais.h"

/** deadlock.cpp
*
* Arquivo com funções relacionadas a identificação dos possiveis deadlocks
*
* @date 05.07.2017
* @authors Caio Vianna, David Morosini
*/


 /** @brief Função alocar espaço de memória para um vector<vector<int> > trabalhando assim com uma matriz diâmica
* @param int l: quantidade de linhas da matriz.
* @param int c: quantidade de colunas da matriz.
* @return matriz: uma estrutura de dados matriz alocada.
*/
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


 /** @brief Função para criar a matriz de detecção do deadlock
* @param vector<leitor > threads: vector de estruturas de dados leitor (verificar em globais.h)
* @param unsigned int qdt_recursos: quantidade de recursos (semáforos) existentes.
* @param matriz * indices: forma de retorno para uma matriz que mapeia os indices dos comandos P's solicitados.
* @return matriz: uma estrutura de dados matriz alocada na forma da matriz de detecção.
*/
matriz cria_matriz_deteccao(vector<leitor > threads, unsigned int qdt_recursos, matriz * indices){
	/**
	* Criação da matriz conta a disposição dos recursos e processos como uma matriz de adjacências
	* Matriz n x m, onde:
	* n = Quantidade de Threads + Quantidade de recursos 
	* m = Quantidade de Threads + Quantidade de recursos
	*
	* A matriz de indices é criada com o mesmo tamanho, porém o conteúdo se difere, ja que é composta pelos
	* indices dos P's da matriz de adjacencias criada
	*/
	unsigned int tam = threads.size() + qdt_recursos;
	matriz resp = aloca_matriz(tam, tam);
	(*indices) = aloca_matriz(tam, tam);
	int contador = threads.size();

	bool flagIgual = false;
	int i, j;
	for(i = 0; i < threads.size(); i++){
		for(j = 0; j < (*threads[i].comandos).size(); j++){
			if((*threads[i].comandos)[j].comando == P){
				/**
				* A idéia principal por traz da matriz de detecção é mapear todos os pedidos de recurso
				* até um dado momento de forma a verificar se neste conjunto de requisições há a possibilidade
				* de deadlock, por isso mapeamos na mesma a quantidade de requisições de cada recurso que cada 
				* thread solicitou. Na matriz de indices salvamos apenas qual a ordem desta solicitação na execução 
				* da sua thread.
				*/
				comando c = (*threads[i].comandos)[j];
				resp[i][contador + c.valor] = resp[i][contador + c.valor] + 1;
				resp[contador + c.valor][i] = resp[contador + c.valor][i] + 1;

				/**
				* Indices é um ponteiro somente para retorno dos dados
				*/
				(*indices)[i][contador + c.valor] = j;
				(*indices)[contador + c.valor][i] = j;
			}
		}
	}
	return resp;
}


 /** @brief Função eliminar todas as threads e recursos que nao interferem no possível deadlock
* @param matriz m: matriz de detecção ja criada
* @param vector<int> rec: vector de recursos
* @return matriz: matriz de detecção restando apenas as threads e recursos que de fato podem influenciar no deadlock
*/
matriz elimina_ruidos(matriz m, vector<int> rec){
	int soma = 0;
	int i, j;

	int qtd_threads = m.size() - rec.size();

	/**
	* i começa em qtd_threads por causa do formato da matriz de detecção:
	* as linhas de 0 a qtd_threads representam as threads na horizontal
	* as colunas de 0 a qtd_threads representam as threads na vertical
	* as demais posições representam os recursos
	*
	*  		t0	t1 ... tn 	r0	...	rm
	*	t0
	*	t1
	*  	...
	* 	tn
	*   r0
	*   ...
	*   rm
	*
	* 	Aplicando a primira "poda", de colunas:
	*  Se a soma de requisições de um dado recurso por todas as threads
	*  for menor ou igual a qantidade disponivel, significa que aquele recurso
	*  pode ser alocado para todos, logo não participa do possivel deadlock, caso exista.
	*
	*	Eliminação de colunas -> consiste em zerar toda a coluna referente ao recurso que só possui uma requisição
	*/
	for(i = qtd_threads; i < m.size(); i++){
		soma = 0;
		for(j = 0; j < qtd_threads; j++){
			soma += m[j][i];
		}
		/**
		* Caso o somatório de requisições (coluna J, cada coluna representando um recurso) for menor ou igual
		* a quantidade de recursos disponiveis (da mesma posicao J), zeramos a coluna J da matriz de detecção
		*/
		if(soma <= rec[i - qtd_threads]){
			for(j = 0; j < qtd_threads; j++){
				m[j][i] = 0;
				m[i][j] = 0;
			}
		}
	}

	/**
	* 	Aplicando a segunda "Poda", que consiste em:
	* 	Se uma dada Thread esta requisitando apenas um recurso, existindo apenas duas possibilidades:
	* 	Cenário 1: Ela possui o recurso, mas como não esta solicitando nenhum outro e este estando em sua posse
	*   a mesma segue sua execução, terminando, libera o recurso para as demais.
	*   Cenário 2: Ela não possui o recurso, mas como não possui nenhum outro recurso em sua posse, esta espera não
	*   configura deadlock, podemos supor então que o conjunto de recursos esta alocado para as demais threads
	*   portando podemos refazer os calculos sem levar em conta esta thread, por ela não influenciar no deadlock
	*   caso exista.
	* 	
	*	Eliminação de linhas -> consiste em zerar toda a linha referente a thread que só possui uma requisição
	*/
	int qtd_somas = 0;
	for(i = 0; i < qtd_threads; i++){
		qtd_somas = 0;
		for(j = qtd_threads; j < m[i].size(); j++){
			if(m[i][j] != 0){
				qtd_somas++;
			}
		}
		/**
		* Caso o somatório de requisições (linha i, cada linha representando uma thread) for igual
		* a 1 (um), zeramos a linha i da matriz de detecção
		*/
		if(qtd_somas == 1){
			for(j = qtd_threads; j < m[i].size(); j++){
				if(m[i][j] != 0){
					m[i][j] = 0;
					m[j][i] = 0;
				}
			}
		}
	}

	/**
	* É feita a recontagem das colunas para que caso seja necessário, seja refeito o procedimento
	*/
	qtd_somas = 0;
	for(i = qtd_threads; i < m.size(); i++){
		soma = 0;
		for(j = 0; j < qtd_threads; j++){
			soma += m[j][i];
		}	
		if(soma > 0 && soma <= rec[i - qtd_threads]){	
 			qtd_somas++;
		}
	}

	/**
	*  Caso a quantidade de colunas cujo somatorio seje menor ou igual a quantidade deisponivel de recursos
	* for maior que 1, significa que eu posso realizar a "poda" novamente.
	*/
	if(qtd_somas > 0){
		elimina_ruidos(m, rec);
	}

	return m;
}

 /** @brief Função detectar o deadlock de uma matriz de detecção
* @param matriz m: matriz de detecção ja criada
* @param int x: posição x da matriz onde sera iniciada a verificação
* @param int y: posição y da matriz onde sera iniciada a verificação
* @param int qtd: quantidade de recursos requisitados no bloco atual de requisições
* @return vector<pt>: vector da estrutura de dados Ponto, contendo a possivel sequencia de deadlock
*/
vector<pt> detecta_deadlock(matriz m, int x, int y, int qtd){
	int cont = 0;
	int i = 0, y_aux = x, x_aux = y;
	pt pnt;
	vector<pt> seq_deadlock;

	/**
	* Partindo desde ponto, sera executado uma busca de ciclos em grafos
	*/
	pnt.x = x;
	pnt.y = y;
	seq_deadlock.push_back(pnt);

	while(i < m[x_aux].size() && cont < qtd - 1){
		/**
		*  Uma das condições de permanencia do While é o contador ser menor que qtd -1 pois, qtd representa 
		*  a quantidade de recursos requeridos, entao so faz sentido procurar um ciclo de no maximo este tamanho
		*/
		if(m[x_aux][i] != 0 && i != seq_deadlock.back().x){
			/**
			* Caso tenha solicitação do recurso atual, e caso ele não mapeie novamente no mesmo recurso
			* (representação da matriz de detecção, alguns pontos remapeiam)
			*/
			cont++;
			pnt.x = x_aux;
			pnt.y = i;
			seq_deadlock.push_back(pnt);
			y_aux = x_aux;
			x_aux = i;
			i = 0;
			if(x == x_aux && seq_deadlock.size() >= 4){
				/*
				* seq_deadlock.size() >= 4, o menor caso de deadlock entre duas threads, necessita de 4 arestas
				* de um grafo para se configurar um ciclo representando um deadlock
				*/
				return seq_deadlock;
			}
		}
		else if(i + 1 == m[x_aux].size() && m[x_aux][i] == 0){
			/**
			*  Condição esta que me diz que este caminho não forma um ciclo, sendo necessário voltar e tentar outro
			*  Representado por: seq_deadlock.back().y + 1, que é o comando anterior, acrescido de um para continuar
			*  a execução
			*/
			i = seq_deadlock.back().y + 1;
			seq_deadlock.pop_back();
			if(!seq_deadlock.empty()){
				x_aux = seq_deadlock.back().y;			
			}else{
				break;
			}
		}else{
			i++;
		}
	}

	/*
	* é necessário limpar a pilha, pois caso esse trecho de código seje executado, pode ser que existam subciclos
	* que não configuram deadlock
	*/
	seq_deadlock.clear();
	return seq_deadlock;
}

 
 /** @brief Função percorrer a matriz de detecção buscando possiveis ciclos a partir de cada posicção cuja
*   houver requisição de recurso
* @param vector<leitor> threads: vector da estrutura de dados Leitor
* @param vector<int> rec: vector de recursos
* @param matriz * indices: forma de retorno para uma matriz que mapeia os indices dos comandos P's solicitados.
* @return vector<pt>: vector da estrutura de dados Ponto, contendo a possivel sequencia de deadlock
*/vector<pt> percorre_matriz(vector<leitor> threads, vector<int> rec, matriz * indices){
	int i, j;
	vector<pt> deadlock;
	int cont = 0;

	/**
	*	a partir do vector de leitores, que representa o bloco de comoandos naquele dado momento
	*  cria se uma matriz de detecção, e logo após são retirados todos os ruídos pelo algoritmo
	*  de "poda"
	*/
	matriz m = cria_matriz_deteccao(threads, rec.size(), indices);
	matriz aux_m = elimina_ruidos(m, rec);


	/**
	*  É realizada a contagem de recursos solicitados
	*/
	for(i = 0; i < m.size(); i++){
		for(j = 0; j < m[i].size(); j++){
			if(m[i][j] == 1){
				cont++;
			}
		}
	}

	int qtd_threads = m.size() - rec.size();
	bool resp = false;

	/**
	*	o bloco de comandos é percorrido para se testar os possiveis comandos que geram deadlock
	*/
	for(i = 0; i < threads.size(); i++){
		for(j = 0; j < (*threads[i].comandos).size(); j++){
			int coluna = (*threads[i].comandos)[j].valor + qtd_threads;
			/**
			*  coluna representa qual recurso estou verificando no momento, e caso ele seja mapeado
			*  na matriz de indices, que por sua vez mapeia todos os recursos solicitados, rode o algoritmo
			* de detecção de deadlock, por que assim garante se que o comando existe, pode ocasionar deadlock
			* e esta mapeado na matriz de indices
			*/
			if(aux_m[i][coluna] != 0){
				deadlock = detecta_deadlock(aux_m, i, coluna, cont);
				if(!deadlock.empty()){
					resp = true;
				}
			}
			/**
			*  Caso o vector deadlock seja preenchido, significa que foi encontrado ciclo, podendo este ser de fato
			*  um deadlock
			*/
			if(resp){
				return deadlock; 
			}
		}
	}
	return deadlock;
}