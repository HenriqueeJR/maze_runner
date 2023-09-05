#include <stdio.h>
#include <stack>
#include <thread>
#include <stdlib.h>

using namespace std;

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

bool saida_encontrada = false;
bool finished = false;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;

	// Abre o arquivo para leitura (fopen)
	FILE* file = fopen(file_name, "r");

	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
	fscanf(file, "%d%d", &num_rows, &num_cols);

	// Aloca a matriz maze (malloc)
	maze = (char**)malloc(num_rows * sizeof(char*));
	for (int i = 0; i < num_rows; ++i) {
		maze[i] = (char *) malloc(num_cols * sizeof(char));			// Aloca cada linha da matriz
	}
		
	
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			fscanf(file, " %c", &(maze[i][j]));
			if (maze[i][j] == 'e') {
				initial_pos.i = i;
				initial_pos.j = j;
			}
			
		}
	}
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

void thread_aux(pos_t pos) {

	std::stack<pos_t> valid_positions_aux;
	
	while(!saida_encontrada) {

		maze[pos.i][pos.j] = 'o';
		maze[pos.i][pos.j] = '.';
		
		this_thread::sleep_for(chrono::milliseconds(50));

		if (pos.i >= 0 && pos.i < num_rows && (pos.j+1) >= 0 && (pos.j+1) < num_cols) {
			if (maze[pos.i][pos.j+1] == 'x') {
				pos_t valid_pos;
				valid_pos.i = pos.i;
				valid_pos.j = pos.j+1;
				valid_positions_aux.push(valid_pos);
			}
			else if (maze[pos.i][pos.j+1] == 's') {
				saida_encontrada = true;
			}

		}

		if (pos.i >= 0 && pos.i < num_rows && (pos.j-1) >= 0 && (pos.j-1) < num_cols) {
			if (maze[pos.i][pos.j-1] == 'x') {
				pos_t valid_pos;
				valid_pos.i = pos.i;
				valid_pos.j = pos.j-1;
				valid_positions_aux.push(valid_pos);
			}
			else if (maze[pos.i][pos.j-1] == 's') saida_encontrada = true;
			
		}

		if (pos.i+1 >= 0 && pos.i+1 < num_rows && (pos.j) >= 0 && (pos.j) < num_cols) {
			if (maze[pos.i+1][pos.j] == 'x') {
				pos_t valid_pos;
				valid_pos.i = pos.i+1;
				valid_pos.j = pos.j;
				valid_positions_aux.push(valid_pos);
			}
			else if (maze[pos.i+1][pos.j] == 's') saida_encontrada = true;
			
		}

		if (pos.i-1 >= 0 && pos.i-1 < num_rows && (pos.j) >= 0 && (pos.j) < num_cols) {
			if (maze[pos.i-1][pos.j] == 'x') {
				pos_t valid_pos;
				valid_pos.i = pos.i-1;
				valid_pos.j = pos.j;
				valid_positions_aux.push(valid_pos);
			}
			else if (maze[pos.i-1][pos.j] == 's') saida_encontrada = true;
			
		}

		if (valid_positions_aux.size() == 1) {
			pos = valid_positions_aux.top();
			valid_positions_aux.pop();	
		}
		else if (valid_positions_aux.size() == 2) {
			pos = valid_positions_aux.top();
			valid_positions_aux.pop();	
			thread t(thread_aux, valid_positions_aux.top());
			t.detach();
			valid_positions_aux.pop();
			
		}
		else if (valid_positions_aux.size() == 3) {
			pos = valid_positions_aux.top();
			valid_positions_aux.pop();	
			thread t1(thread_aux, valid_positions_aux.top());
			t1.detach();
			valid_positions_aux.pop();
			thread t2(thread_aux, valid_positions_aux.top());
			t2.detach();
			valid_positions_aux.pop();
		}
		else if (valid_positions_aux.empty()) {
			finished = true;
			break;
		}
		
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
void walk(pos_t pos) {

	while(!saida_encontrada) {

		//printf("\n\n boo \n\n");

		maze[pos.i][pos.j] = 'o';
		maze[pos.i][pos.j] = '.';

		this_thread::sleep_for(chrono::milliseconds(50));
		
		if (pos.i >= 0 && pos.i < num_rows && (pos.j+1) >= 0 && (pos.j+1) < num_cols) {
			if (maze[pos.i][pos.j+1] == 'x') {
				pos_t valid_pos;
				valid_pos.i = pos.i;
				valid_pos.j = pos.j+1;
				valid_positions.push(valid_pos);
			}
			else if (maze[pos.i][pos.j+1] == 's') saida_encontrada = true;
			

		}

		if (pos.i >= 0 && pos.i < num_rows && (pos.j-1) >= 0 && (pos.j-1) < num_cols) {
			if (maze[pos.i][pos.j-1] == 'x') {
				pos_t valid_pos;
				valid_pos.i = pos.i;
				valid_pos.j = pos.j-1;
				valid_positions.push(valid_pos);
			}
			else if (maze[pos.i][pos.j-1] == 's') saida_encontrada = true;
			
		}

		if (pos.i+1 >= 0 && pos.i+1 < num_rows && (pos.j) >= 0 && (pos.j) < num_cols) {
			if (maze[pos.i+1][pos.j] == 'x') {
				pos_t valid_pos;
				valid_pos.i = pos.i+1;
				valid_pos.j = pos.j;
				valid_positions.push(valid_pos);
			}
			else if (maze[pos.i+1][pos.j] == 's') saida_encontrada = true;
			
		}

		if (pos.i-1 >= 0 && pos.i-1 < num_rows && (pos.j) >= 0 && (pos.j) < num_cols) {
			if (maze[pos.i-1][pos.j] == 'x') {
				pos_t valid_pos;
				valid_pos.i = pos.i-1;
				valid_pos.j = pos.j;
				valid_positions.push(valid_pos);
			}
			else if (maze[pos.i-1][pos.j] == 's') saida_encontrada = true;
			
		}

		if (valid_positions.size() == 1) {
			pos = valid_positions.top();
			valid_positions.pop();	
		}
		else if (valid_positions.size() == 2) {
			pos = valid_positions.top();
			valid_positions.pop();	
			thread t(thread_aux, valid_positions.top());
			t.detach();
			valid_positions.pop();
		}
		else if (valid_positions.size() == 3) {
			pos = valid_positions.top();
			valid_positions.pop();	
			thread t1(thread_aux, valid_positions.top());
			t1.detach();
			valid_positions.pop();
			thread t2(thread_aux, valid_positions.top());
			t2.detach();
			valid_positions.pop();
		}
		else if (valid_positions.empty()) {
			
			break;
		}
	}	
}

void impressao() {
	while (!saida_encontrada) {
		system("clear");
		print_maze();
		this_thread::sleep_for(chrono::milliseconds(50));
	}

	if (saida_encontrada) printf("%s\n", "SAIDA ENCONTRADA");
	else printf("%s\n", "SAIDA NAO ENCONTRADA");

}

int main(int argc, char* argv[]) {

	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);

	// chamar a função de navegação
	thread t1(walk, initial_pos);
	t1.detach();

	impressao();
	return 0;
}
