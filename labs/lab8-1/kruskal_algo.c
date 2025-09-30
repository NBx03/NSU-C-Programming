#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct Edge {
    short src;
    short dst;
    int edge_len;
} Edge;

typedef struct VertexPair {
    short src;
    short dst;
} VertexPair;

void CheckVerticesAndEdges(short n, int m);
int CheckLength(long long edge_len);
int CheckVertices(short src, short dst, short n);
void AddEdge(Edge* edge, int i, short src, short dst, long long edge_len);
void RunKruskalAlgorithm(Edge* edge, short n, int m);

int main() {
    short n;
    int m;
    if(scanf("%hd\n%d", &n, &m) != 2) {
        printf("bad number of lines");
        return 0;
    }
    CheckVerticesAndEdges(n, m);
    struct Edge* edge = malloc(m * sizeof(Edge));
    short src, dst;
    long long edge_len;
    for(int i = 0; i < m; i++) {
        if(scanf("%hd %hd %lld", &src, &dst, &edge_len) != 3) {
            printf("bad number of lines");
            free(edge);
            return 0;
        }
        if(!CheckLength(edge_len)) {
            free(edge);
            return 0;
        }
        if(!CheckVertices(src, dst, n)) {
            free(edge);
            return 0;
        }
        AddEdge(edge, i, src, dst, edge_len);
    }
    RunKruskalAlgorithm(edge, n, m);
    free(edge);
    return 0;
}

void CheckVerticesAndEdges(short n, int m) {
    if(n < 0 || n > 5000) {
        printf("bad number of vertices");
        exit(0);
    }
    if(m < 0 || m > (int) n * (n - 1) / 2) {
        printf("bad number of edges");
        exit(0);
    }
    if(n == 0 || (n > 1 && m == 0)) {
        printf("no spanning tree");
        exit(0);
    }
    if(m == 0)
        exit(0);
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

void AddEdge(Edge* edge, int i, short src, short dst, long long edge_len) {
    edge[i].src = --src;
    edge[i].dst = --dst;
    edge[i].edge_len = (int)edge_len;
}

short SearchSet(short *set, short vertex) {
    if (vertex == set[vertex])
        return vertex;
    return set[vertex] = SearchSet(set, set[vertex]);
}

void UniteSets(short *set, short src, short dst) {
    src = SearchSet(set, src);
    dst = SearchSet(set, dst);
    if (src != dst)
        set[dst] = src;
}

int CheckSpanningTreeAvailability(Edge *edge, VertexPair *ans_edges, short n, int m) {
    short* set = malloc(n * sizeof(short));
    for(short i = 0; i < n; i++){
        set[i] = i;
    }

    short index = 0;
    for(int i = 0; i < m; i++) {
        if (SearchSet(set, edge[i].src) != SearchSet(set, edge[i].dst)) {
            UniteSets(set, edge[i].src, edge[i].dst);
            ans_edges[index].src = ++edge[i].src;
            ans_edges[index].dst = ++edge[i].dst;
            index++;
        }
    }

    free(set);
    if (index < n - 1)
        return 0;
    return 1;
}

int Compare(const Edge* edge1, const Edge* edge2) {
    return edge1->edge_len - edge2->edge_len;
}

void RunKruskalAlgorithm(Edge* edge, short n, int m) {
    VertexPair* ans_edges = malloc((n - 1) * sizeof(VertexPair));

    qsort(edge, m, sizeof(Edge), (int (*)(const void *, const void *)) Compare);

    if(CheckSpanningTreeAvailability(edge, ans_edges, n, m))
        for(int i = 0; i < n - 1; i++)
            printf("%hd %hd\n", ans_edges[i].src, ans_edges[i].dst);
    else
        printf("no spanning tree");

    free(ans_edges);
}
