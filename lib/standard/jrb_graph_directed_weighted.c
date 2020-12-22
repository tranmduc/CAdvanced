#include "jrb_graph_directed_weighted.h"
#include "queue.h"
#include "jrb.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jval.h"

/** Graph is represented by adjancent lists with JRB; the key is vertex number, the value is another JRB
 * JRB:{
 *  0: {2: 1, 3: 1, 4: 2} *Note: 2:1 means node 0 connects to 2 with weight 1
 *  1: {}
 *  2: {3:1,}
 * ...
 * }
*/

Graph createGraph(){
    Graph g;
    g.vertices = make_jrb();
    g.edges = make_jrb();
    return g;
}


/** Add a vertex with a name and id to the graph */
void addVertex(Graph graph, int id, char* name){
    //Check for duplicate id
    JRB node = jrb_find_int(graph.vertices, id);

    if(node != NULL){
        //error, return
        fprintf(stderr, "ERROR: Vertex with id %d already exists. Cannot add another one.\n", id);
        return;
    }

    //key not found, create new key
    JRB new_node = jrb_insert_int(graph.vertices, id, new_jval_s(name));
    printf("Add vertex %d with name %s to graph successfully\n", id, jval_s(new_node->val));
    return;
}

/** Get name of a vertex from the graph, given the id */
char* getVertex(Graph graph, int id){
    //printf("Enter getVertex with id %d\n", id); //DEBUG
    if (graph.vertices == NULL){
        fprintf(stderr, "ERROR: The graph is empty. Cannot find vertex\n");
        return NULL;
    }

    //find
    JRB find_node = jrb_find_int(graph.vertices, id);
    if(find_node == NULL){
        printf("There's no vertex with id %d in the graph.\n", id);
        return NULL;
    }

    //found
    //printf("Found name: %s\n", jval_s(find_node->val)); //DEBUG
    return jval_s(find_node->val);
}

/** Check if the graph has vertex with id */
int hasVertex(Graph graph, int id){
    if (graph.vertices == NULL)
        return 0;
    //find
    JRB find_node = jrb_find_int(graph.vertices, id);
    if(find_node == NULL){
        return 0;
    }
    return 1;

}

/** Add a directed edge from v1 to v2. Return 1 if success and 0 if not*/
int addDirectedEdge(Graph graph, int v1, int v2, double weight){
    //printf("Enter addDirectedEdge from %d to %d\n", v1, v2); //DEBUG

    if (graph.vertices == NULL){
        fprintf(stderr, "ERROR: The graph is empty. Please add vertex first before adding edges\n");
        return 0;
    }
    //check if v1,v2 exists in graph.vertices
    if(!hasVertex(graph, v1)){
        fprintf(stderr, "ERROR: The graph has no vertex %d. Please add it first.\n", v1);
        return 0;
    }
    if(!hasVertex(graph, v2)){
        fprintf(stderr, "ERROR: The graph has no vertex %d. Please add it first.\n", v2);
        return 0;
    }

    //check if the edge already exists
    if(hasEdge(graph, v1, v2)){
        fprintf(stderr, "ERROR: Edge from %d to %d already exists.\n", v1, v2);
        return 0;
    }

    //printf("addDirectedEdge: pass all checks\n"); //DEBUG

    // ADDING: give graph has vertices, and no edge yet created

    ////JRB of vertex 1 in the graph.edges
    JRB vertex_1_jrb = jrb_find_int(graph.edges, v1);
    JRB vertex_1_tree;

    //init new JRB for vertex 1 if empty
    if(vertex_1_jrb == NULL){
        vertex_1_tree = make_jrb(); //change from jrb->tree (node -> value)
        jrb_insert_int(graph.edges, v1, new_jval_v(vertex_1_tree)); //add v1 tree to main tree
        //printf("Init %d's tree and add to main tree\n", v1); //DEBUG
    } else{ //non empty
        vertex_1_tree = (JRB) jval_v(vertex_1_jrb->val);
    }
    
    jrb_insert_int(vertex_1_tree, v2, new_jval_d(weight)); //add v2 to v1's tree
    printf("Add edge from %d to %d with weight %.2f sucessfully\n", v1,v2, weight); //DEBUG
    return 1;
    
}

/** Add a unit directed edge (weight = 1) from v1 to v2. Return 1 if success and 0 if not*/
int addUnitDirectedEdge(Graph graph, int v1, int v2){
    return addDirectedEdge(graph, v1, v2, 1);
}

/** Add two directed edge from v1 to v2 and vice versa. Return 1 if success and 0 if not*/
int addBidirectedEdge(Graph graph, int v1, int v2, double weight){
    return addDirectedEdge(graph, v1, v2, weight) * addDirectedEdge(graph, v2, v1, weight);
}

/**  Get weight from v1 to v2. return INFINITIVE_VALUE if no edge between v1 and v2*/
double getEdgeValue(Graph graph, int v1, int v2){
    if(!hasEdge(graph, v1, v2)) return INFINITY_LENGTH;

    //has edge
    JRB vertex_1_jrb = jrb_find_int(graph.edges, v1);
    JRB vertex_1_tree= (JRB) jval_v(vertex_1_jrb->val);
    JRB edge_jrb = jrb_find_int(vertex_1_tree, v2);
    double weight = jval_d(edge_jrb->val);
    return weight;
}


/** Remove edge from v1 to v2 if any*/
void removeEdge(Graph graph, int v1, int v2){
    if (graph.vertices == NULL || graph.edges == NULL){
        return;
    }

    if(!hasEdge(graph, v1, v2)){
        printf("There's no edge from %d to %d\n", v1,v2);
        return;
    }

    //non-empty graph, has edge
    JRB v1_main_tree_node = jrb_find_int(graph.edges, v1);
    JRB v1_tree = (JRB) jval_v(v1_main_tree_node->val);
    JRB v2_node_on_v1_tree = jrb_find_int(v1_tree, v2);
    jrb_delete_node(v2_node_on_v1_tree);

    //check emptiness of v1_tree. If yes, delete tree
    if(jrb_empty(v1_tree)){
        free(v1_tree);
        free(v1_main_tree_node);
    }  

    printf("Delete edge from %d to %d sucessfully\n", v1, v2);

    return;

}

/** Remove a vertex along with its edges*/
void removeVertex(Graph graph, int v){
    if (graph.vertices == NULL || graph.edges == NULL){
        return;
    }

    //del in vertices
    JRB vertex_node = jrb_find_int(graph.vertices, v);
    if(vertex_node == NULL) return;
    jrb_delete_node(vertex_node);

    //del in edges
    JRB vertex_travel;
    jrb_traverse(vertex_travel, graph.vertices){
        int vertex_travel_key = jval_i(vertex_travel->key);
        if(hasEdge(graph, v, vertex_travel_key)) removeEdge(graph, v, vertex_travel_key);
        if(hasEdge(graph, vertex_travel_key, v)) removeEdge(graph, vertex_travel_key, v);
    }

    return;
}

/** Check whether there's an edge from v1 to v2. Former: isAdjacent*/
int hasEdge(Graph graph, int v1, int v2){
    if (graph.vertices == NULL || graph.edges == NULL){
        return 0;
    }

    //non-empty graph
    JRB v1_main_tree_node = jrb_find_int(graph.edges, v1);
    if(v1_main_tree_node == NULL){ //v1 not connected to any vertex
        return 0;
    }

    JRB v1_tree = (JRB) jval_v(v1_main_tree_node->val);
    JRB v2_node_on_v1_tree = jrb_find_int(v1_tree, v2);

    if(v2_node_on_v1_tree == NULL){
        return 0;
    } else {
        //printf("%d and %d are connected with weight %d\n", v1, v2, jval_i(v2_node_on_v1_tree->val));//DEBUG
        return 1;
    }
}

/** Count all edges that point to this vertex. Store list of vertices into output[]*/
int inDegree(Graph graph, int v, int* output){
    if (graph.vertices == NULL || graph.edges == NULL){
        return 0;
    }

    //check vertex's existance
    JRB vertex_jrb = jrb_find_int(graph.vertices, v);
    if(vertex_jrb == NULL){
        return 0;
    }

    //check vertices list for which node has edge to v
    JRB vertices_traveling_node;
    int index = 0;

    jrb_traverse(vertices_traveling_node, graph.vertices){
        int node_id = jval_i(vertices_traveling_node->key);
        if(hasEdge(graph, node_id,v))
            output[index++] = node_id;
    }
    
    return index;
}

/** Count all edges that point out from this vertex. Store list of vertices into output[]*/
int outDegree(Graph graph, int v, int* output){
    if (graph.vertices == NULL || graph.edges == NULL){
        return 0;
    }

    //check vertex's existance
    JRB vertex_jrb = jrb_find_int(graph.vertices, v);
    if(vertex_jrb == NULL){
        return 0;
    }

    //printf("Ckpt 3.1\n");

    JRB edge_jrb = jrb_find_int(graph.edges, v);
    if(edge_jrb == NULL) return 0; //no outward nodes

    JRB vertex_tree = (JRB) jval_v(edge_jrb->val);
    JRB travelling_node;

    //printf("Ckpt 3.2\n");

    int index = 0;
    //printf("Ckpt 2\n");
    jrb_traverse(travelling_node, vertex_tree){
        //printf("Ckpt 3\n");
        //printf("Ckpt 3.3: Traveling from id %d to id %d\n", v, jval_i(travelling_node->key));
        output[index++] = jval_i(travelling_node->key);
        //weight = jval_i(travelling_node->val)
    }

    //printf("Ckpt 3.4\n");
    return index;
}

void dropGraph(Graph graph){
    JRB node;
    jrb_traverse(node, graph.edges){
        jrb_free_tree((JRB) jval_v(node->val));
     }
         
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
    return;
}

/** Print vertices of the graph
 * Vertices:
 * id 1: <name_1>
 * id 2: <name_2>
 * ...
*/
void printVerticesGraph(Graph graph){
    printf("Vertices:\n");
    if (graph.vertices == NULL){
        printf("Empty\n");
        return;
    }

    //non-empty
    JRB traveling_node;
    jrb_traverse(traveling_node, graph.vertices){
        printf("id %d: %s\n", jval_i(traveling_node->key), jval_s(traveling_node->val));
    }
    return;
}

/** Print edges of the graph
 * root
 * |- <name_1>: <name_2>
 * |- <name_2>: <name_3>
*/
void printEdgesGraph(Graph graph){
    printf("Edges:\n");
    if (graph.edges == NULL){
        printf("Empty\n");
        return;
    }

    //non-empty, print root and node
    JRB vertice_jrb;
    jrb_traverse(vertice_jrb, graph.edges){
        int vertice_id = jval_i(vertice_jrb->key);
        printf("|- node %s: ", getVertex(graph, vertice_id)); //print name

        //child tree
        JRB vertice_child_tree = (JRB) jval_v(vertice_jrb->val);
        JRB vertice_child_jrb;
        jrb_traverse(vertice_child_jrb, vertice_child_tree){
            int vertice_edge_id = jval_i(vertice_child_jrb->key);
            printf("%s ", getVertex(graph, vertice_edge_id)); //->val is weight
        }
        printf("\n");
    }
}

/** Given the graph, print the vertices and edges */
void printGraph(Graph graph){
    printVerticesGraph(graph);
    printEdgesGraph(graph);
}

/** Get all vertices in a graph that joins at least one edge 
 *  Copy from vertices only => abundance; Copy from edges only => lack (at leaves)
 * Need to get from both
*/
JRB getEdgefulVertices(Graph graph){
    if(graph.edges == NULL || graph.vertices == NULL) return NULL;

    //copy vertices to unvisited list. Copy from vertices => abundance; Copy from edges => lack (at leaves)
    JRB unvisited_vertices = make_jrb();
    JRB node;
    jrb_traverse(node, graph.edges){
        int node_key = jval_i(node->key);
        JRB check1 = jrb_find_int(unvisited_vertices, node_key);
        if(check1 == NULL){
            jrb_insert_int(unvisited_vertices,node_key, new_jval_i(1)); //1 is weight
            //printf("Added %d to unvisited_vertices\n", node_key); //DEBUG
        }
        
        JRB node_inner_tree = (JRB) jval_v(node->val);
        JRB node_inner;

        jrb_traverse(node_inner, node_inner_tree){
            //check if node_inner alredy in unvisited_vertices
            int node_inner_key = jval_i(node_inner->key);
            JRB check2 = jrb_find_int(unvisited_vertices, node_inner_key);
            if(check2 == NULL){//not in unvisited yet
                jrb_insert_int(unvisited_vertices, node_inner_key, new_jval_i(1));//1 is weight
                //printf("Added %d to unvisited_vertices\n", node_inner_key); //DEBUG
            }                 
        }
    }

    return unvisited_vertices;
}

/** Void function of int*/
void void_funct_int(int v){
    return;
}

/** Check if the graph is Directed Acyclic Graph
 * run DFS using an arbitrary vertex as the source. If the source can be visited twice, then there is a cycle in the graph.
 * Maintain a JRB tree of visited nodes to check whether all nodes have been visited
 * Return: True of False
*/
int isDAG(Graph graph){
    JRB node;
    // Broadcast from all vertices to find cycle
    jrb_traverse(node, graph.vertices){
        JRB tree = (JRB)jval_v(node->val);
        int start = jval_i(node->key);
        int dd = DFS(graph, start, -1, void_funct_int);
        if (dd == -2){
            printf("Graph is cyclic at start=%d\n", start); //DEBUG
            return 0; // DFS return -2 if cycle found
        } 
    }
    return 1;
}


/** Traverse the graph using BFS, and act some f function on the graph's node 
 * Characteristic: directed, unweighted, feasible
 * start is the first vertex to visit
 * stop is the vertex to be visited at the end, if stop = -1, all the vertices may be visited
 * func is a pointer to the function that process on the visited vertices
 * return: number of node traveled, -1 if not found
 * */
int BFS(Graph graph, int start, int stop, void (*func)(int)){
    //init visited array
    int visited[GRAPH_MAX_SIZE];
    memset(visited, 0, GRAPH_MAX_SIZE);

    int node_traveled = 0;

    //init queue, put start node
    Queue q = createQueue();
    pushQueue(q, new_jval_i(start));

    //main loop
    while(!isQueueEmpty(q)){
        int next_vertex = jval_i(popQueue(q));
        if(!visited[next_vertex]){
            //not visited, process
            node_traveled++;
            func(next_vertex);
            visited[next_vertex] = 1;

            //add adjancent vertices
            int output[GRAPH_MAX_SIZE]; 
            for (int i = 0; i < GRAPH_MAX_SIZE; i++) output[i] = -1; //-1 is fake vertices           
            
            outDegree(graph, next_vertex, output);

            for (int i = 0; i < GRAPH_MAX_SIZE; i++)
            {
                int v = output[i];
                //printf("Considering adjancent vertex %d\n", v); //DEBUG
                if(v != -1){
                    //not fake
                    if(!visited[v]){
                        //not visited, check if any of it is stop vertex
                        if(v == stop){
                            func(stop);
                            printf("A way from %d to %d has been found\n",start, stop);
                            return ++node_traveled;
                        }

                        //enQueue
                        pushQueue(q, new_jval_i(v));
                    }
                } else{ 
                    //encounter fake vertex, break
                    break;
                }
            }
            
        }
    }

    //queue empty, not find stop yet
    if(stop != -1){
        printf("Can not find a way from %d to %d\n", start, stop);
        return -1;
    } else{
        //supposed to span tree if stop = -1
        printf("Complete spanning from vertex %d using BFS\n", start);
        return node_traveled;
    }  
}


/** Traverse the graph using DFS, and act some f function on the graph's node 
 * Characteristic: directed, unweighted, feasible
 * start is the first vertex to visit
 * stop is the vertex to be visited at the end, if stop = -1, all the vertices may be visited
 * func is a pointer to the function that process on the visited vertices
 * return: number of node traveled, -1 if not found, -2 if cycle is detected
 * */
int DFS(Graph graph, int start, int stop, void (*func)(int)){
    //init visited array
    int visited[GRAPH_MAX_SIZE];
    memset(visited, 0, GRAPH_MAX_SIZE);

    int node_traveled = 0;

    //init stack, put start node
    Stack stack = createStack();
    pushStack(stack, new_jval_i(start));

    //main loop
    while(!isStackEmpty(stack)){
        int next_vertex = jval_i(popStack(stack));
        if(!visited[next_vertex]){
            //not visited, process
            node_traveled++;
            func(next_vertex);
            visited[next_vertex] = 1;

            //add adjancent vertices
            int output[GRAPH_MAX_SIZE]; 
            for (int i = 0; i < GRAPH_MAX_SIZE; i++) output[i] = -1; //-1 is fake vertices           
            
            outDegree(graph, next_vertex, output);

            for (int i = 0; i < GRAPH_MAX_SIZE; i++)
            {
                int v = output[i];                
                if(v != -1){
                    //printf("Considering adjancent vertex %d\n", v); //DEBUG
                    //not fake
                    if(!visited[v]){
                        //not visited, check if any of it is stop vertex
                        if(v == stop){
                            func(stop);
                            printf("A way from %d to %d has been found\n",start, stop);
                            return ++node_traveled;
                        }

                        //pushStack
                        pushStack(stack, new_jval_i(v));
                    } else{
                        if(v == start) return -2; //cycle found
                    }
                } else{ 
                    //encounter fake vertex, break
                    break;
                }
            }
            
        }
    }

    //queue empty, not find stop yet
    if(stop != -1){
        printf("Can not find a way from %d to %d\n", start, stop);
        return -1;
    } else{
        //supposed to span tree if stop = -1
        //printf("Complete spanning from vertex %d using DFS\n", start); //DEBUG
        return node_traveled;
    }  
}

/** Input: an directed acyclic graph (DAG)
 * Output: a linear order that respects the prec constraints
 * @output_array: store the output ids
 * @output_length: length of @output_array
 * Algo:
 * Build an “indegree table” of the DAG
*  Output a vertex v with zero indegree
*  For vertex v, the arc (v, w) is no longer useful since the task (vertex) w does not need to wait for v to finish anymore
 * */
void topologicalSort(Graph graph, int* output_array, int* output_length){
    if(graph.vertices == NULL || graph.edges == NULL){
        fprintf(stderr, "ERR: Empty graph, can not sort topologically\n");
        return;
    }

    if(!isDAG(graph)){
        fprintf(stderr, "ERR: graph is not acyclic (graph has >= 1 cycle), can not sort topologically\n");
        return;
    }

    //Init
    Queue q = createQueue();
    int output_tmp[GRAPH_MAX_SIZE];
    int indegree[GRAPH_MAX_SIZE];
    *output_length = 0;

    JRB vertices_node;

    //printf("Ckpt 1\n");

    //calculate indegree table
    jrb_traverse(vertices_node, graph.vertices){
        int v = jval_i(vertices_node->key);
        indegree[v] = inDegree(graph, v, output_tmp);
        if(indegree[v] == 0){
            pushQueue(q, new_jval_i(v));
            //printf("Ckpt 2: push %d to queue\n", v);
        }
    }

    while(!isQueueEmpty(q)){       
        int visited_node = jval_i(popQueue(q));
        //printf("Ckpt 3: Visited node %d\n", visited_node);

        //output this node & recalculate table
        output_array[(*output_length)++] =  visited_node;

        int m = outDegree(graph, visited_node, output_tmp);
        for (int i = 0; i < m; i++)
        {
            int v2 = output_tmp[i];
            indegree[v2]--;
            if(indegree[v2] == 0){
                pushQueue(q, new_jval_i(v2));
                //printf("Ckpt 4: push %d to queue\n", v2);
            }
        }
    

    };

    //printf("Ckpt 5: Queue empty\n");

    return;
}

/** Relaxing an edge (intermediate_v,current_des) means testing whether we can improve the shortest path to current_des found so far by going through intermediate_v 
 * d: array pointer of estimation shortest path from start
 * parent: use to backtrace the path
 * priorityQueue: update the PQ when relax successfully
 * Return: True if relaxed successfully, false otherwise
*/
int relax(Graph graph, int intermediate_v, int current_des, double* d, int* path, JRB priorityQueue){
    double new_weight = d[intermediate_v] + getEdgeValue(graph, intermediate_v, current_des);

    //printf("relax: inter:%d - des:%d - old weight: %.2f - new weight: %.2f\n", intermediate_v, current_des, d[current_des], new_weight); //DEBUG
    
    if(d[current_des] > new_weight){
        //it is better to go through intermediate_v to current_des
        //printf("Better\n"); //Relax
        d[current_des] = new_weight;
        path[intermediate_v] = current_des; 
        updatePriorityQueue(priorityQueue, current_des , new_weight);
        return 1;
    }
    return 0;
}

/** Update the priority queue due to the new d of vertex
 * Priority queue: to be updated
 * vertex: vertex id, used to find JRB node
 * new_d: new value of d
 * Algo: find the JRB node by vertex, then delete that node and add a new node (new_d, vertex)
 * Return: 1 if successful, 0 if vertex node not found
*/
int updatePriorityQueue(JRB priorityQueue, int vertex, double new_d){
    JRB node;
    jrb_traverse(node, priorityQueue){
        if(jval_i(node->val) == vertex){
            //found
            jrb_delete_node(node);
            jrb_insert_dbl(priorityQueue, new_d, new_jval_i(vertex));            
            return 1;
        }
    }

    //not found node
    return 0;
}


/** Find the shortest path from start to stop.
 * Return the total weight of the path and the path is given via path and its length.
 * path*: int array pointer of the path (v1->v3->v2)
 * num_ver_on_path*: int pointer of the path's length. Return -1 if not found
 * Return length of path. INFINITIVE_VALUE if no path is found
 * --------------------------
 * Assumption: non-negative weight edges
 * Algorithm: using Priority queue
 * For each vertex v in the graph, we maintain v.d(), the estimate of the shortest path from s, initialized to Infinity the start
 * Relaxing an edge (u,v) means testing whether we can improve the shortest path to v found so far by going through u
 * Priority queue is reorganized whenever some v.d() decreases
 *  */
double findShortestPath(Graph graph, int start, int stop, int* path, int* num_ver_on_path){
    //check trivial conditions
    if(graph.vertices == NULL || graph.edges == NULL) return INFINITY_LENGTH;
    if(jrb_find_int(graph.vertices, start) == NULL){
        //start vertex not in graph
        fprintf(stderr, "Start vertex %d not in graph\n", start);
        return INFINITY_LENGTH; 
    }
    if(jrb_find_int(graph.vertices, stop) == NULL){
        //stop vertex not in graph
        fprintf(stderr, "Stop vertex %d not in graph\n", stop);
        return INFINITY_LENGTH;
    }

    double d[GRAPH_MAX_SIZE]; //estimation from start to every vertex

    JRB priorityQueue = initPriorityQueue(graph, start, d, path);

    //printf("findShortestPath - Ckpt 1:is PQ empty: %d\n", jrb_empty(priorityQueue)); //DEBUG
    
    while(!jrb_empty(priorityQueue)){
        //extract node with smalled d[]
        JRB min_node = jrb_first(priorityQueue);
        int vertex_min_in_PQ = jval_i(min_node->val);
        jrb_delete_node(min_node);
        //printf("findShortestPath - Ckpt 2: vertex_min_in_PQ is %d with d[] = %.2f\n", vertex_min_in_PQ, jval_d(min_node->key)); //DEBUG


        //Recalculate all path from vertex_min to outDegree
        int output[GRAPH_MAX_SIZE];
        int n = outDegree(graph, vertex_min_in_PQ, output);
        int current_des;

        for(int i=0; i<n; i++){
            current_des = output[i];
            relax(graph, vertex_min_in_PQ, current_des, d, path, priorityQueue);
        }
    }    
    

    //PQ is empty, spanning from start vertex complete

    //for (size_t i = 0; i < 5; i++) printf("path[%d] = %d\n", i, path[i]); //DEBUG
    //for (size_t i = 0; i < 5; i++)printf("d[%d] = %.2f\n", i, d[i]); //DEBUG

    //can not reach destination
    if (d[stop] == INFINITY_LENGTH) return INFINITY_LENGTH;

    //can reach, find num_ver_on_path
    findNumVerOnPath(num_ver_on_path, path, start, stop);

    return d[stop];    
}


/** Given array path[], find num_ver_on_path to destination
 * Return: num_ver_on_path changes, nothing else changes
*/
void findNumVerOnPath(int* num_ver_on_path, int* path, int start, int stop){
    *num_ver_on_path = 1;
    int i = start;

    while(path[i] != stop){
        //stop: destination reached
        (*num_ver_on_path)++;
        i = path[i];
    }
}

/** Populate the PQ with data from v.d() and graph*/
JRB populatePriorityQueue(Graph graph, double* d){
    JRB pq = make_jrb(); 
    //init priority queue (key, value) = (v.d, v) ~ (double, int)
    JRB vertex_node;
    jrb_traverse(vertex_node, graph.vertices){
        int vertex = jval_i(vertex_node->key);
        jrb_insert_dbl(pq, d[vertex], new_jval_i(vertex));
        //printf("populate: d[%d] = %.2f\n", vertex, d[vertex]); //DEBUG
    }

    return pq;
}

/** Init the d,parent array and the priority queue
 * Priority queue is reorganized whenever some v.d() decreases
 * (key, value) = (v.d(), v)
 */
JRB initPriorityQueue(Graph graph, int start, double* d, int* path){
    //printf("initPriorityQueue - Ckpt 1\n"); //DEBUG

    //init d[] and parent[]
    JRB vertex_node;
    jrb_traverse(vertex_node, graph.vertices){
        int vertex = jval_i(vertex_node->key);
        d[vertex] = INFINITY_LENGTH;
        path[vertex] = -1; //set parent of vertices to null
    }

    d[start] = 0;

    // printf("initPriorityQueue - Ckpt 2\n"); //DEBUG

    //a priority queue ADT keyed by v.d(), which is re-organized whenever some d decreases (v.d() is key of tree's node, v is value)
    return populatePriorityQueue(graph, d);

}