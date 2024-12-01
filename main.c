#include "graph.h"

/*
 * A basic menu based structure has been provided
 */

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int choice;
    // int startVertex = 1;
    Graph *graph = readGraph(argv[1]);
    if (graph == NULL) {
        fprintf(stderr, "Error: Unable to read the graph from the file.\n");
        return 1;
    }

    do
    {

        // Display the options
        prompt();

        // Get user input
        printf("Enter a Choice: ");
        scanf("%d", &choice);

        /*
         * You will need to add all the cases here.
         * Call the relevant functions in each case.
         * Make sure to handle invalid inputs or edge cases.
         */

        switch (choice) {
            case 1:
                displayAdjacencyList(graph);
                break;
            case 2:
                bfs(graph, 0);  // 0-based index, so starting from vertex 1 -> index 0
                break;
            case 3:
                dfs(graph, 0);  // 0-based index, so starting from vertex 1 -> index 0
                break;
            case 4:
                dijkstra(graph, 0);  // 0-based index, so starting from vertex 1 -> index 0
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 5);

    freeGraph(graph);
    
    return 0;
}
