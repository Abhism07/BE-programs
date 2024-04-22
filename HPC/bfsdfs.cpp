#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define MAX_NODES 100

// Structure to represent a node in the graph
typedef struct Node {
    int value;
    struct Node* next;
} Node;

// Structure to represent a graph
typedef struct Graph {
    Node* adjacency_list[MAX_NODES];
    int num_nodes;
} Graph;

// Function to add an undirected edge between two nodes in the graph
void add_edge(Graph* graph, int src, int dest) {
    // Add edge from src to dest
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = dest;
    newNode->next = graph->adjacency_list[src];
    graph->adjacency_list[src] = newNode;

    // Add edge from dest to src (since it's an undirected graph)
    newNode = (Node*)malloc(sizeof(Node));
    newNode->value = src;
    newNode->next = graph->adjacency_list[dest];
    graph->adjacency_list[dest] = newNode;
}

// Breadth First Search (BFS)
void bfs(Graph* graph, int start) {
    int visited[MAX_NODES] = {0}; // Array to track visited nodes
    int queue[MAX_NODES]; // Queue for BFS traversal
    int front = 0, rear = 0;

    queue[rear++] = start;
    visited[start] = 1;

    while (front < rear) {
        int current = queue[front++];
        printf("%d ", current);

        // Traverse adjacent nodes
        #pragma omp parallel for
        for (Node* temp = graph->adjacency_list[current]; temp != NULL; temp = temp->next) {
            int neighbor = temp->value;
            if (!visited[neighbor]) {
                #pragma omp critical
                {
                    queue[rear++] = neighbor;
                    visited[neighbor] = 1;
                }
            }
        }
    }
}

// Depth First Search (DFS) recursive utility function
void dfs_util(Graph* graph, int v, int visited[]) {
    visited[v] = 1;
    printf("%d ", v);

    // Traverse adjacent nodes
    for (Node* temp = graph->adjacency_list[v]; temp != NULL; temp = temp->next) {
        int neighbor = temp->value;
        if (!visited[neighbor]) {
            dfs_util(graph, neighbor, visited);
        }
    }
}

// Depth First Search (DFS)
void dfs(Graph* graph, int start) {
    int visited[MAX_NODES] = {0}; // Array to track visited nodes
    dfs_util(graph, start, visited);
}

int main() {
    Graph graph;
    graph.num_nodes = 6;

    // Initialize adjacency list
    for (int i = 0; i < MAX_NODES; i++) {
        graph.adjacency_list[i] = NULL;
    }

    // Add edges to create an undirected graph
    add_edge(&graph, 0, 1);
    add_edge(&graph, 0, 2);
    add_edge(&graph, 1, 3);
    add_edge(&graph, 1, 4);
    add_edge(&graph, 2, 5);

    printf("BFS Traversal: ");
    bfs(&graph, 0);
    printf("\n");

    printf("DFS Traversal: ");
    dfs(&graph, 0);
    printf("\n");

    return 0;
}
