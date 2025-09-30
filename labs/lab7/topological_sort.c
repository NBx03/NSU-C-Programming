#include <stdio.h>
#include <stdlib.h>

#define BYTES_TO_BITS (sizeof(int) * 8)

void CheckVerticesAndEdges(short n, int m);
void CheckVertex(short vertex, short n);
void AddEdge(int *adj_matrix, short n, short v1, short v2);
void TopologicalSort(int *adj_matrix, char *color, short *sorted_vertices, short n);
void FreeArrays(int *adj_matrix, char *color, short *sorted_vertices);

int main() {
    short n;
    int m;
    if(scanf("%hd\n%d", &n, &m) != 2) {
        printf("bad number of lines");
        return EXIT_SUCCESS;
    }
    CheckVerticesAndEdges(n, m);
    int *adj_matrix = calloc((n + 1) * (n + 1) / BYTES_TO_BITS + 1, sizeof(int));
    char *color = calloc(n + 1, 1);
    short *sorted_vertices = calloc(n, sizeof(short));
    short v1, v2;
    for(int i = 0; i < m; i++) {
        if(scanf("%hd %hd", &v1, &v2) != 2) {
            printf("bad number of lines");
            FreeArrays(adj_matrix, color, sorted_vertices);
            return EXIT_SUCCESS;
        }
        CheckVertex(v1, n);
        CheckVertex(v2, n);
        AddEdge(adj_matrix, n, v1, v2);
    }
    TopologicalSort(adj_matrix, color, sorted_vertices, n);
    FreeArrays(adj_matrix, color, sorted_vertices);
    return EXIT_SUCCESS;
}

void CheckVerticesAndEdges(short n, int m) {
    if(n < 0 || n > 2000) {
        printf("bad number of vertices");
        exit(0);
    }
    if(m < 0 || m > (int) n * (n - 1) / 2) {
        printf("bad number of edges");
        exit(0);
    }
}

void CheckVertex(short vertex, short n) {
    if(vertex < 1 || vertex > n) {
        printf("bad vertex");
        exit(0);
    }
}

void AddEdge(int *adj_matrix, short n, short v1, short v2) {
    int edge_pos = v1 * n + v2;
    adj_matrix[edge_pos / BYTES_TO_BITS] |= 1 << (edge_pos % BYTES_TO_BITS);
}

int DepthFirstSearch(int *adj_matrix, char *color, short *sorted_vertices, short i, short n, short *sort_ind) {
    color[i]++;
    for(short j = 1; j <= n; j++) {
        if(adj_matrix[(i * n + j) / BYTES_TO_BITS] & (1u << (i * n + j) % BYTES_TO_BITS)) {
            if(color[j] == 1) {
                printf("impossible to sort");
                return 1;
            }
            else if(!color[j]) {
                if(DepthFirstSearch(adj_matrix, color, sorted_vertices, j, n, sort_ind))
                    return 1;
            }
        }
    }
    color[i]++;
    sorted_vertices[*sort_ind] = i;
    (*sort_ind)++;
    return 0;
}

void TopologicalSort(int *adj_matrix, char *color, short *sorted_vertices, short n) {
    short sort_ind = 0;
    for(short i = 1; i <= n; i++) {
        if(!color[i])
            if(DepthFirstSearch(adj_matrix, color, sorted_vertices, i,  n, &sort_ind))
                return;
    }
    for(short i = --n; i >= 0; i--) {
        printf("%hd ", sorted_vertices[i]);
    }
}

void FreeArrays(int *adj_matrix, char *color, short *sorted_vertices) {
    free(adj_matrix);
    free(color);
    free(sorted_vertices);
}
