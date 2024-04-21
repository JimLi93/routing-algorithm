#include <stdio.h>
#include <stdlib.h>


//  Define 999 as infinity
int infinity = 999;

//  Number of nodes
int nodes_num = 4;

//  The distance table
//  table[i][j] reocrd the distance between node i and node j
int table[4][4];


// The structure of the node 
struct Node{

    //  Record the minimum distance from the source node to its adjacent nodes
    int dist[4];

    //  Record whether the node have been visited
    int visit[4];

}nodes[4];

//  Print the minimum distance from the source node to its adjacent nodes
//  Input arguement: (int)id is the id of the source node
void print_min_cost(int id)
{
    printf("Min cost %d : %d %d %d %d\n", id, nodes[id].dist[0], nodes[id].dist[1], nodes[id].dist[2], nodes[id].dist[3]);
}

//  Use Dijkstra's algorithm to find the minimum distance from the source node to its neighbors
//  Input arguement: (int)id is the id of the source node
void dijkstra(int id)
{
   // Hint: You should refer the distance in table[][] and use the struct Node to implement your algorithm  
   /* TODO */
   nodes[id].visit[id] = 1;
   for(int i=0;i<4;i++){
        nodes[id].dist[i] = table[id][i];
   }
   for(int iter=0;iter<nodes_num-1;iter++){
        int min = infinity * 10;
        int chosen = -1;
        for(int i=0;i<4;i++){
            if( (nodes[id].visit[i] == 0) && (nodes[id].dist[i] < min) ){
                chosen = i;
                min = nodes[id].dist[i];
            }
        }
        nodes[id].visit[chosen] = 1;
        for(int i=0;i<4;i++){
            if(nodes[id].visit[i] == 0){
                if(nodes[id].dist[i] > (nodes[id].dist[chosen]+table[chosen][i])){
                    nodes[id].dist[i] = nodes[id].dist[chosen]+table[chosen][i];
                }
            }
        }
   }
}


// Initialize 
void init()
{
    // Initialize nodes' distance to infinity and visit to 0
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            nodes[i].dist[j] = infinity;
            nodes[i].visit[j] = 0;
        }
    }

    // Initialize the distance table
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            table[i][j] = infinity;
        }
    }
}


int main(void)
{
    // Initalize node
    init();

    // Declare variables to open the file
    FILE *file;
    char *line = NULL;
    size_t len = 0;
    file = fopen("di_config.txt", "r");

    // Read the config file to get the distance table
    for(int i = 0;i < 4; i++)
    {
        getline(&line, &len, file);
        sscanf(line, "%d %d %d %d", &table[i][0], &table[i][1], &table[i][2], &table[i][3]);
    }
    

    // Implement algorithm on node 0
    dijkstra(0);
    
    // Implement algorithm on node 1
    dijkstra(1);
    
    // Implement algorithm on node 2
    dijkstra(2);
    
    // Implement algorithm on node 3
    dijkstra(3);

    // Print
    print_min_cost(0);
    print_min_cost(1);
    print_min_cost(2);
    print_min_cost(3);


    // Close the file
    fclose(file);

    return 0;
}