#include <stdio.h>
#include <stack>
#include <stdlib.h>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

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


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {

	maze[pos.i][pos.j] = '.';
	system("clear");
	print_maze();
	
	if (pos.i >= 0 && pos.i < num_rows && (pos.j+1) >= 0 && (pos.j+1) < num_cols) {
		if (maze[pos.i][pos.j+1] == 'x') {
			pos_t valid_pos;
			valid_pos.i = pos.i;
			valid_pos.j = pos.j+1;
			valid_positions.push(valid_pos);
		}
		else if (maze[pos.i][pos.j+1] == 's') return true;
		
	}

	if (pos.i >= 0 && pos.i < num_rows && (pos.j-1) >= 0 && (pos.j-1) < num_cols) {
		if (maze[pos.i][pos.j-1] == 'x') {
			pos_t valid_pos;
			valid_pos.i = pos.i;
			valid_pos.j = pos.j-1;
			valid_positions.push(valid_pos);
		}
		else if (maze[pos.i][pos.j-1] == 's') return true;
		
	}

	if (pos.i+1 >= 0 && pos.i+1 < num_rows && (pos.j) >= 0 && (pos.j) < num_cols) {
		if (maze[pos.i+1][pos.j] == 'x') {
			pos_t valid_pos;
			valid_pos.i = pos.i+1;
			valid_pos.j = pos.j;
			valid_positions.push(valid_pos);
		}
		else if (maze[pos.i+1][pos.j] == 's') return true;
		
	}

	if (pos.i-1 >= 0 && pos.i-1 < num_rows && (pos.j) >= 0 && (pos.j) < num_cols) {
		if (maze[pos.i-1][pos.j] == 'x') {
			pos_t valid_pos;
			valid_pos.i = pos.i-1;
			valid_pos.j = pos.j;
			valid_positions.push(valid_pos);
		}
		else if (maze[pos.i-1][pos.j] == 's') return true;
		
	}


	if (!valid_positions.empty()) {
		pos_t next_position = valid_positions.top();
		valid_positions.pop();
		walk(next_position);
	}
	else return false;
}

int main(int argc, char* argv[]) {

	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);

	// chamar a função de navegação
	bool exit_found = walk(initial_pos);

	if (exit_found) printf("%s\n", "SAIDA ENCONTRADA");
	else printf("%s\n", "SAIDA NAO ENCONTRADA");
	
	
	
	return 0;
}
