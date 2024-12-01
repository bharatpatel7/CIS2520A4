#include "graph.h"

#define MAX_VERTICES 100

int weights[MAX_VERTICES][MAX_VERTICES]; 

/* function will display the primary user interface
    This is already done for you*/
void prompt(void)
{

    printf("\nMenu:\n");
    printf("1. Display Adjacency List\n");
    printf("2. Perform Breadth-First Search (BFS)\n");
    printf("3. Perform Depth-First Search (DFS)\n");
    printf("4. Find Shortest Path using Dijkstra's Algorithm\n");
    printf("5. Exit\n");
}

/**
 * Reads a graph from a file and constructs the graph structure.
 * @param filename The name of the file containing the adjacency matrix.
 * @return Pointer to the created Graph structure, or NULL if an error occurs.
 */
Graph *readGraph(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    
    Graph *graph = malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(file);
        return NULL;
    }

    memset(graph->adjMatrix, 0, sizeof(graph->adjMatrix));
    
    for (int i = 0; i < MAX_VERTICES; i++) {
        graph->adjList[i] = NULL;
    }

    graph->numVertices = 0;
    char line[1000];
    
    while (fgets(line, sizeof(line), file) && graph->numVertices < MAX_VERTICES) {
        int j = 0;
        char *token = strtok(line, " \t\n");
        
        while (token != NULL && j < MAX_VERTICES) {
            graph->adjMatrix[graph->numVertices][j] = atoi(token);
            token = strtok(NULL, " \t\n");
            j++;
        }
        
        graph->numVertices++;
    }

    fclose(file);

    createAdjacencyList(graph);

    return graph;
}

/**
 * Creates a new node for the adjacency list.
 * @param vertex The vertex number that this node will represent.
 * @return Pointer to the newly created Node structure.
 */
Node *createNode(int vertex)
{
    Node *newNode = malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation error.\n");
        return NULL;
    }
    newNode->vertex = vertex;
    newNode->next = NULL;
    return newNode;
}

/**
 * Displays the adjacency list of the graph.
 * @param graph Pointer to the Graph structure.
 */
void displayAdjacencyList(Graph *graph)
{
    printf("Adjacency List:\n");
    for (int i = 0; i < graph->numVertices; i++) {
        printf("Vertex %d: ", i + 1);
        Node *temp = graph->adjList[i];
        // Reverse the order of printing to match the desired output
        if (temp == NULL) {
            printf("NULL\n");
        } else {
            // Create a temporary array to reverse the order
            Node* nodes[MAX_VERTICES];
            int count = 0;
            
            // Collect nodes
            while (temp) {
                nodes[count++] = temp;
                temp = temp->next;
            }
            
            // Print in reverse order
            for (int j = count - 1; j >= 0; j--) {
                printf("-> %d (%d) ", nodes[j]->vertex + 1, weights[i][nodes[j]->vertex]);
            }
            printf("NULL\n");
        }
    }
}

/**
 * Converts the adjacency matrix of the graph to an adjacency list.
 * @param graph Pointer to the Graph structure.
 */
void createAdjacencyList(Graph *graph)
{
    for (int i = 0; i < graph->numVertices; i++) {
        Node *current = graph->adjList[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp);  // Free each existing node
        }
        graph->adjList[i] = NULL;  // Reset the list pointer
    }

    memset(weights, 0, sizeof(weights));

    for (int i = 0; i < graph->numVertices; i++) {
        for (int j = 0; j < graph->numVertices; j++) {
            if (graph->adjMatrix[i][j] != 0) {  
                Node *node = createNode(j);     
                node->next = graph->adjList[i]; 
                graph->adjList[i] = node;       
                weights[i][j] = graph->adjMatrix[i][j];
            }
        }
    }
}

/**
 * Performs Breadth-First Search (BFS) starting from a given vertex.
 * @param graph Pointer to the Graph structure.
 * @param startVertex The vertex from which BFS starts (0-based index).
 */
void bfs(Graph *graph, int startVertex)
{
    bool visited[MAX_VERTICES] = {false};
    int queue[MAX_VERTICES];
    int front = 0, rear = 0;

    visited[startVertex] = true;
    queue[rear++] = startVertex;

    printf("Final BFS Order: ");
    while (front < rear) {
        int vertex = queue[front++];
        printf("%d ", vertex + 1);
        
        for (int i = 0; i < graph->numVertices; i++) {
            if (graph->adjMatrix[vertex][i] != 0 && !visited[i]) {
                visited[i] = true;
                queue[rear++] = i;
            }
        }
    }
    printf("\n");
}

/**
 * Performs Depth-First Search (DFS) starting from a given vertex.
 * @param graph Pointer to the Graph structure.
 * @param startVertex The vertex from which DFS starts (0-based index).
 */
void dfs(Graph *graph, int startVertex)
{
    bool visited[MAX_VERTICES] = {false};

    printf("DFS traversal: ");
    dfsHelper(graph, startVertex, visited);
    printf("\n");
}

void dfsHelper(Graph *graph, int vertex, bool visited[]) {
    visited[vertex] = true;
    printf("%d ", vertex + 1);

    for (int i = 0; i < graph->numVertices; i++) {
        if (graph->adjMatrix[vertex][i] != 0 && !visited[i]) {
            dfsHelper(graph, i, visited);
        }
    }
}

/**
 * Finds the shortest path from the start vertex to all other vertices using Dijkstra's algorithm.
 * @param graph Pointer to the Graph structure.
 * @param startVertex The vertex from which to start the algorithm (0-based index).
 */
void dijkstra(Graph *graph, int startVertex)
{
    int dist[MAX_VERTICES];
    bool visited[MAX_VERTICES] = {false};
    
    for (int i = 0; i < graph->numVertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[startVertex] = 0;

    for (int count = 0; count < graph->numVertices; count++) {
        
        int u = -1;
        for (int j = 0; j < graph->numVertices; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        visited[u] = true;

        for (int v = 0; v < graph->numVertices; v++) {
            if (!visited[v] && graph->adjMatrix[u][v] != 0 && 
                dist[u] != INT_MAX && 
                dist[u] + graph->adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->adjMatrix[u][v];
            }
        }
    }

    printf("\n");
    for (int i = 0; i < graph->numVertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("Shortest distance from vertex %d to vertex %d: INF\n", startVertex + 1, i + 1);
        } else {
            printf("Shortest distance from vertex %d to vertex %d: %d\n", startVertex + 1, i + 1, dist[i]);
        }
    }
}

/**
 * Frees the memory allocated for the graph, including the adjacency list.
 * @param graph Pointer to the Graph structure to be freed.
 */
void freeGraph(Graph *graph)
{
    // Implement the function logic here
    for (int i = 0; i < graph->numVertices; i++) {
        Node *temp = graph->adjList[i];
        while (temp) {
            Node *toFree = temp;
            temp = temp->next;
            free(toFree);
        }
    }
    free(graph);
}
