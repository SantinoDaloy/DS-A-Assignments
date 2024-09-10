#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#define N 8

int BacktrackRecursive(int x, int y, int siguiente, int sol[N][N],
				int posible_x[], int posible_y[], int used[64]);

int checkeo_continuar(int x, int y, int sol[N][N], int used[64], int siguiente)
{
    if (x >= 0 && y >= 0 && x < N && y < N && sol[x][y] == 0 && used[siguiente] == 0){
        return 1;
    }
    if (x >= 0 && y >= 0 && x < N && y < N && sol[x][y] == siguiente){
        return 1;
    }
    return 0;
}

int Backtrack(int sol[N][N], int startx, int starty, int used[64])
{
	int posible_x[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
	int posible_y[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };

	BacktrackRecursive(startx, starty, 2, sol, posible_x, posible_y, used);
	return 1;
}

int BacktrackRecursive(int x, int y, int siguiente, int sol[N][N],
				int posible_x[N], int posible_y[N], int used[64])
{
    int sumx=0;
    int sumy=0;
    int cx = 0;
    int cy = 0;
    for (int x = 0;x<N;x++){
        for (int y = 0; y < N;y++){
            sumx += sol[x][y];
        }
        if (sumx == 260){
            cx++;
            sumx = 0;
        }
    }
    for (int x = 0;x<N;x++){
        for (int y = 0; y < N;y++){
            sumy += sol[y][x];
        }
        if (sumy == 260){
            cy++;
            sumy = 0;
        }
    }
	if (siguiente == (N * N) + 1 && cy == 8 && cx == 8){
        return 1;
    }
	for (int k = 0; k < 8; k++) {
		int next_x = x + posible_x[k];
		int next_y = y + posible_y[k];
		if (checkeo_continuar(next_x, next_y, sol, used, siguiente)) {
			sol[next_x][next_y] = siguiente;
			if (BacktrackRecursive(next_x, next_y, siguiente + 1, sol,
							posible_x, posible_y, used)
				== 1)
				return 1;
			else
                if (used[sol[next_x][next_y]] == 0){
				    sol[next_x][next_y] = 0;
                }
		}
	}
	return 0;
}
int comparar(char *fragmento, char *consulta)
{
    if (strlen(fragmento) != strlen(consulta)) {
        return 0;
    }
    int dic1[256] = { 0 }, dic2[256] = { 0 };
    for (int k = 0; k < strlen(fragmento); k++) {
        if (dic1[(int)fragmento[k]] != dic2[(int)consulta[k]]) {
            return 0;
        }
        dic1[(int)fragmento[k]]++;
        dic2[(int)consulta[k]]++;
    }
    return 1;
}

int main(int argc, char** argv)
{
    char* option;
    // Revisamos los argumentos
    if(argc < 4) {
        printf("Modo de uso: %s <A/B> <input.txt> <output.txt>\n", argv[0]);
        return 1;
    }
    FILE *input_file = fopen(argv[2], "r");
    FILE *output_file = fopen(argv[3], "w");
    option = argv[1];

    //HASHING
    int count = 0;
    if(strcmp(option, "A") == 0){
        int length, consultas;
        fscanf(input_file, "%d", &length);
        char hebra[length];
        fscanf(input_file, "%s %d", hebra, &consultas);
        int c_size;
        for (int k = 0; k < consultas;k++){
            fscanf(input_file, "%d", &c_size);
            char c_hebra[c_size];
            fscanf(input_file, "%s", c_hebra);
            int start = 0;
            int finish = c_size;
            char subs[c_size + 1];
            int encontro = 0;
            while (finish < length){
                strncpy(subs, hebra + start, finish - start);
                subs[c_size] = '\0';
                if (comparar(subs, c_hebra)){
                    fprintf(output_file, "%i %i\n", start, finish - 1);
                    count++;
                    encontro++;
                    break;
                }
                start++;
                finish++;
            }
            if (encontro == 0){
                fprintf(output_file, "%i %i\n", encontro, encontro);
            }

        }
        
        
    }

    //BACKTRACKING

    else if(strcmp(option, "B") == 0){
        int sol[N][N];
        for (int k = 0;k<N;k++){
            fscanf(input_file, "%d %d %d %d %d %d %d %d", &sol[k][0], &sol[k][1], &sol[k][2], &sol[k][3], &sol[k][4], &sol[k][5], &sol[k][6], &sol[k][7]);
        }
        int used[65];
        int startx, starty;
        for (int k=0;k<=64;k++){
            used[k] = 0;
        }
        for (int x = 0;x<N;x++){
            for (int y = 0; y < N;y++){
                if (sol[x][y] != 0){
                    used[sol[x][y]] = 1;
                }
                if (sol[x][y] == 1){
                    startx = x;
                    starty = y;
                }
            }
        }

        Backtrack(sol, startx, starty, used);

        for (int x = 0;x<N;x++){
            for (int y = 0; y < N;y++){
                fprintf(output_file, "%i ", sol[x][y]);
            }
            fprintf(output_file, "\n");
        }
    }

    return 0;
}