#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void CheckVertexNumber(short n);
void CheckEdgeNumber(short n, int m);
int CheckLength(long long edge_len);
int CheckVertices(short src, short dst, short n);
void AddEdge(unsigned int* matrix, short n, short src, short dst, long long edge_len);
void RunDijkstraAlgorithm(const unsigned int* matrix, short n, short s, short f);

int main() {
    short n;
    if(scanf("%hd", &n) != 1) {
        printf("bad number of lines");
        return 0;
    }
    CheckVertexNumber(n);
    short s, f;
    if(scanf("%hd %hd", &s, &f) != 2) {
        printf("bad number of lines");
        return 0;
    }
    if(!CheckVertices(s, f, n)) {
        return 0;
    }
    int m;
    if(scanf("%d", &m) != 1) {
        printf("bad number of lines");
        return 0;
    }
    CheckEdgeNumber(n, m);

    unsigned int* matrix = calloc(n * n, sizeof(int));
    short src, dst;
    long long edge_len;
    for(int i = 0; i < m; i++) {
        if(scanf("%hd %hd %lld", &src, &dst, &edge_len) != 3) {
            printf("bad number of lines");
            free(matrix);
            return 0;
        }
        if(!CheckLength(edge_len)) {
            free(matrix);
            return 0;
        }
        if(!CheckVertices(src, dst, n)) {
            free(matrix);
            return 0;
        }
        AddEdge(matrix, n, src, dst, edge_len);
    }

    RunDijkstraAlgorithm(matrix, n, s, f);

    free(matrix);
    return 0;
}

void CheckVertexNumber(short n) {
    if(n < 0 || n > 5000) {
        printf("bad number of vertices");
        exit(0);
    }
}

void CheckEdgeNumber(short n, int m) {
    if(m < 0 || m > (int) n * (n - 1) / 2) {
        printf("bad number of edges");
        exit(0);
    }
}

int CheckLength(long long edge_len) {
    if(edge_len < 0 || edge_len > INT_MAX) {
        printf("bad length");
        return 0;
    }
    return 1;
}

int CheckVertices(short src, short dst, short n) {
    if(src < 1 || src > n || dst < 1 || dst > n) {
        printf("bad vertex");
        return 0;
    }
    return 1;
}

void AddEdge(unsigned int* matrix, short n, short src, short dst, long long edge_len) {
    matrix[(src - 1) * n + dst - 1] = (unsigned int)edge_len;
    matrix[(dst - 1) * n + src - 1] = (unsigned int)edge_len;
}

void FindShortestRoute(const unsigned int* matrix, unsigned int* minValue, short* shortest_route, short n, short minIndex) {
    char* visited = calloc(n, 1);
    for(int v = 0; v < n; v++) {
        visited[minIndex] = 1;
        unsigned int tempValue = UINT_MAX;
        short tempIndex = 0;
        
        for(short i = 0; i < n; i++) {
            if(!visited[i]) {
                if(matrix[minIndex * n + i] && minValue[i] >= (minValue[minIndex] + matrix[minIndex * n + i])) {
                    if(minValue[minIndex] + matrix[minIndex * n + i] > INT_MAX) {
                        minValue[i] = (unsigned int)INT_MAX + 1;
                    } else {
                        minValue[i] = minValue[minIndex] + matrix[minIndex * n + i];
                    }
                    shortest_route[i] = (short)(minIndex + 1);
                }
                if(tempValue > minValue[i]) {
                    tempValue = minValue[i];
                    tempIndex = i;
                }
            }
        }
        minIndex = tempIndex;
    }
    free(visited);
}

void PrintShortestRoute(short* shortest_route, short f) {
    printf("%hd ", f);
    if(shortest_route[f - 1] != 0) {
        PrintShortestRoute(shortest_route, shortest_route[f - 1]);
    }
}

void RunDijkstraAlgorithm(const unsigned int* matrix, short n, short s, short f) {
    unsigned int* minValue = malloc(n * sizeof(int));
    minValue[--s] = 0;
    for(short i = 0; i < n; i++) {
        if(i == s) {
            continue;
        }
        minValue[i] = UINT_MAX;
    }
    short* shortest_route = calloc(n + 1, sizeof(short));
    FindShortestRoute(matrix, minValue, shortest_route, n, s);

    short routesToF = 0;
    for(short i = 0; i < n; i++) {
        if(minValue[i] == UINT_MAX) {
            printf("oo ");
        } else if(minValue[i] > INT_MAX) {
            printf("INT_MAX+ ");
        } else {
            printf("%u ", minValue[i]);
            if(matrix[(f - 1) * n + i]) {
                routesToF++;
            }
        }
    }
    printf("\n");

    if(minValue[f - 1] == UINT_MAX) {
        printf("no path");
    } else if(minValue[f - 1] > INT_MAX && routesToF >= 2) {
        printf("overflow");
    } else {
        PrintShortestRoute(shortest_route, f);
    }

    free(minValue);
    free(shortest_route);
}
