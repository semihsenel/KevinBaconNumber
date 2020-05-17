#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define LINE 3000
#define VERTEX 200000

typedef struct Qnode{
	int s;
	struct node* next;
}QNODE;
// The node of queue that is used in BFS

typedef struct queue{
	QNODE* front;
	QNODE* rear;
}QUEUE;
// The structure of queue

typedef struct node{
	int vertex;
    struct node* next;
}NODE;
// The node of linked list

typedef struct Graph {
    int numVertices;
    struct node** adjList;
}Graph;
// The structure of graph

typedef struct movies{
	char name[150];
}Movies;
// Movies list

typedef struct actors{
	char name[150];
}Actors;
// Actors list

typedef struct table{
	int number;
	int lineNum;
	char name[LINE];
	char connection[50][LINE];
}Table;
// The table to save the actors that is searched already

NODE* createNode(int); 
Graph* createGraph(int);
void addEdge(Graph*, int, int);
void fillGraph(Graph*, char*);
QNODE* createNodeQueue(int);
void enqueue(QUEUE**, int);
int dequeue(QUEUE**);
int isQueueEmpty(QUEUE*);
int BFS(QUEUE**,int, int, int);
QUEUE* createQueue(QUEUE*);
int findMovieNum(char*);
void BFSforAllActors(QUEUE**, int);
void outputFormat(QUEUE*);
void remake(QUEUE**);


Graph* graph;
int movieNum; // Number of movies in the file
int movieCounter = 0; // counter to add edge
int actorCounter; // counter to edd edge too
int frekans[50] = {0}; // Number of kevin bacon numbers
Table table[VERTEX]; // Table array
int tableCounter = 0; // counter to add to table

Movies movies[15000];
Actors actors[190000];

int main(){
	QUEUE* queue;
	char filename[15];

	graph = createGraph(200000); // Create the graph for 200 000 vertex
	queue = createQueue(queue); // Create the queue to use in BFS

	printf("Enter the file name : ");
	scanf("%s",filename);
	movieNum = findMovieNum(filename); // Find movie number in the file
	actorCounter = movieNum; // Assign movie number to actor counter
    fillGraph(graph,filename); // Fill the graph with the movies and actors
    outputFormat(queue); // Ask the user for output

	return 0;

}


void outputFormat(QUEUE* queue){
	int type, indexToStart, indexToFind, result;
	char name[50];
	indexToStart = findIndex("Bacon, Kevin"); // Find index of Kevin Bacon to start to search
	int start = 1;

	while(start){
		printf("\n\nEnter 1 to list frequency for all actors\nEnter 2 for Kevin Bacon Number of a spesific actor\nEnter 3 for exit\n");
		scanf("%d",&type);

		if(type == 1){ 
		// Find frequency for all actors
			BFSforAllActors(&queue, indexToStart);
		}
		else if(type == 2){
			printf("\nEnter an actor name : ");
	    	scanf(" %[^\n]s",name); // Take input for the actor's name
			int found = alreadyFound(name); // Check the name is it found already
			if(found < 0){ // if it has not found yet
				strcpy(table[tableCounter].name,name); // add the name into the table
		   		indexToFind = findIndex(name); // Find index of actor to send BFS
		   		result = BFS(&queue, indexToStart, indexToFind,1); // assign result of BFS to result
		   		if(result > 0){
		   			// Print Kevin Bacon Number
		   			printf("Kevin Bacon Number of %s is %d.\n",name,result);
				}
				else if(result == 0){
					// If result is 0. The actor that user searched for is Kevin Bacon
					printf("It is Kevin Bacon");
					printf("Kevin Bacon Number of Kevin Bacon is 0.\n");
				}
				else{
					// If result < 0 than. The actor's Kevin Bacon Number is infinite.
					printf("Kevin Bacon Number of %s is infinite.",name);
				}
			}
	   		remake(&queue);
		}
		else if(type == 3){
			// To end of program
			printf("End of the program\nGoodbye\nTake Care\n");
			return;
		}
	}
	
}

int alreadyFound(char* name){
	// Check the name if it was found already
	int i,j;
	for(i=0; i<tableCounter; i++){
		// for all table
		if(!strcmp(name,table[i].name)){
			// search the name in the table
			for(j = 0; j<table[i].lineNum; j++){
				// in table's connection string for all lines
				// print the connection
				printf("%s\n",table[i].connection[j]);
			}
			printf("Kevin Bacon Number of %s is %d.\n",name,table[i].number);
		}
		return 1;
	}
	return -1;
}


void BFSforAllActors(QUEUE** queue, int s){
	// Find frequency for all actors in the file
	int i,v;
	NODE* temp;
	int visited[VERTEX];
	for(i = 0; i < VERTEX; i++){
		visited[i] = -1;
	}
	// Assign all index to unvisited
	enqueue(queue,s);
	// Add Kevin Bacon to the queue
	visited[s] = 0;
	// Assign Kevin Bacon as visited
	while(!isQueueEmpty((*queue))){
		// As long as queue is not empty
		v = dequeue(queue);
		// Take an index from the queue
		temp = graph->adjList[v];
		// Assing temp to vth List
		while(temp != NULL){
			// For all linked list
			if(visited[temp->vertex] == -1){
				// If temp was not visited
				// Add the vertex to the queue
				// Assign it as visited
				enqueue(queue,temp->vertex);
				visited[temp->vertex] = visited[v] + 1;
			}
			temp = temp->next;
			// Go to the next node
		}
	}
	
	for(i = movieNum; i < actorCounter; i++){
		// For all actors
		if(visited[i] != -1){
		// If it is visited
			frekans[visited[i]/2]++;
			// Increment the index visited[i]/2
			// Divison with 2 is necessary for ignore the movies between two actors
		}
		else
			frekans[49]++;
		// If it is not visited ýncrement 49th index
		// I use 49th index for infinite
	}
	
	for(i=0;i < 49; i++){
		// for all frequency list
		if(frekans[i]>0){
			// If greater than 0 print the frequency
			printf("There is %d people whose Kevin Bacon Number is %d.\n", frekans[i], i);
		}	
			
	}
	// Print the infinite ones
	printf("There is %d pople whose Kevin Bacon Number is infinite", frekans[49]);
	
	

}

void remake(QUEUE** queue){
	// Clear and Free the queue to use again
	while(!isQueueEmpty(*queue))
		dequeue(queue);
	*queue = createQueue(*queue);
}

int findMovieNum(char* filename){
	// Find movie number in the file to set actor counter
	FILE* fp;
	char str[LINE]; // The buffer
	fp = fopen(filename,"r");
	if(fp == NULL){
		// When error occurs while the file is openning then shutdown the program
		printf("File Openning Error");
		exit(0);
	}
	int counter = 0; // Set counter to 0
	while(fgets(str,LINE-1,fp)){
			counter++;
			// Increment counter for the each line
			// Because each line has one movie
	}
	fclose(fp); // Close the file
	return counter; // Return the counter ( Movie Number )
	
}

int findIndex(char* name){
	// Find index of a actor
	int i;
	for(i = 0; i < actorCounter-movieNum; i++){
		// For all actors
		if(!strcmp(actors[i].name, name)){
			// Search the name in the actors array
			return movieNum+i; // Return index 
		}
	}
	// If it didn't exist in the array
	// Then return -1
	return -1;
}

void fillGraph(Graph* graph, char* filename){
	// Fill the graph with actors and movies in the file
	FILE* fp;
	char str[LINE];
	char* token,*movie,*actor;
	int ret, firstQuarter, half, lastQuarter;
	int counter = 0;

	firstQuarter = (int)(float)movieNum*25/100; 
	half = (int)(float)movieNum*50/100;
	lastQuarter = (int)(float)movieNum*75/100;
	// The section of file to print how many percent is done

	fp = fopen(filename,"r");
	printf("%s started to be read. It may take long time\n", filename);
	if(fp == NULL){
		printf("File could not open!");
		exit(0);
	}
	while(fgets(str,LINE,fp)){
		// Read a line
		token = strtok(str,"\n");
		// Get rid of the '\n' character
		movie = strtok(token,"/");
		// First column is movie others are actor
		strcpy(movies[movieCounter].name, movie);
		// Add movie to the movies array
		actor = strtok(NULL,"/");
		// take an actor
		while(actor != NULL){
			ret = isThere(actor);
			// Check the actor exits
			if(ret == -1){
				// If it is a new actor
				strcpy(actors[actorCounter-movieNum].name,actor);
				// Add the actors list
				addEdge(graph,movieCounter,actorCounter);
				// Add an edge between movie in the line and the actor
				actorCounter++;
				// Increment the counter
			}
			else{
				// If it is not a new actor
				// Ret is the index of the actor
				// Add an edge between movie and the actor
				addEdge(graph,movieCounter,ret);
			}	
			actor = strtok(NULL,"/");
		}
		movieCounter++;
		counter++; // Counter for calculate how many percent is done
		if(counter == firstQuarter){
			printf("25%c is completed\n",'%');
		}
		else if(counter == half){
			printf("50%c is completed\n",'%');
		}
		else if(counter == lastQuarter){
			printf("75%c is completed\n",'%');
		}

	}
	fclose(fp);
	// Close the file
	printf("100%c is completed\n",'%');
	printf("Graph has been filled\n");
}

int isThere(char* actor){
	// Check the actor exits alreaady
	int i = 0;
	for(; i < actorCounter-movieNum; i++){
		// for all actors in the actors array
		if(!strcmp(actors[i].name,actor)){
			// If it was found
			// return index
			return movieNum + i;
		}
	}
	// Otherwise return -1
	return -1;
}

QNODE* createNodeQueue(int value){
	// Create a node for queue
	QNODE* newNode = (QNODE*) malloc(sizeof(QNODE));
	// Allocate for the node
	newNode->s = value;
	// Assign the value to the node's s variable
	newNode->next = NULL;
	// Assign NULL to the node's next
	
	// return Node
	return newNode;
}

QUEUE* createQueue(QUEUE* queue){
	// Create a queue
	queue = (QUEUE*) malloc(sizeof(QUEUE));
	// Allocate for the queue
	queue->front = NULL;
	queue->rear = NULL;
	// Assign null to front and rear because queue is empty
	
	return queue; // Return the queue
}

void enqueue(QUEUE** queue, int s){
	// Add an element to the queue
	QNODE* newNode = createNodeQueue(s);
	// create a node
	if((*queue)->rear == NULL){
		// if queue is empty
		(*queue)->rear = newNode;
		(*queue)->front = newNode;
		// Assign the node to rear and front
	}
	else{
		// Otherwise, add the next of rear
		// And rear will demonstrate the new node
		(*queue)->rear->next = newNode;
		(*queue)->rear = newNode;
	}

}

int dequeue(QUEUE** queue){
	// Remove an element from the queue
	if((*queue)->front == NULL){
		// If queue is empty than return -3
		return -3;
	}
	QNODE* garbage;
	int s = (*queue)->front->s;
	// Assign s to a variable
	garbage = (*queue)->front;
	// Assign the node to a variable to free
	(*queue)->front = (*queue)->front->next;
	// Assign front to the next node
	if((*queue)->front == NULL){
		// If queue is empty now
		// Assign NULL to rear
		(*queue)->rear = NULL;
	}
	
	free(garbage);
	// Free the node
	
	return s; // return the value of the element that was removed
}

int isQueueEmpty(QUEUE* queue){
	// Check the queue is empty
	if(queue->front == NULL){
		// If it is empty return 1
		return 1;
	}
	else{
		// Otherwise, return 0
		return 0;
	}
}

NODE* createNode(int v){
	// Create a node for linked list
    NODE* newNode;
    newNode = (NODE*) malloc(sizeof(NODE)); // Allocate
    newNode->vertex = v;
    // Assign the value
    newNode->next = NULL;
    // Assign NULL to next
    return newNode;
    // Return the node
}

Graph* createGraph(int nVertex){
	// Create a graph
    int i;
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    // Allocate
    graph->numVertices = nVertex;
    // Assign number of vertices
    graph->adjList = (NODE**) malloc(nVertex*sizeof(NODE*));
    // Assign null to all linked lists
    for (i = 0; i < nVertex ;i++) {
        graph->adjList[i] = NULL;
    }
    // Return the graph
    return graph;
}

void addEdge(Graph* graph, int src, int dest){
	// Add edge between to nodes in the graph
	NODE* newNode = createNode(dest);
	// Create a node
	newNode->next = graph->adjList[src];
	// Add node beginning of the linked list ( srcth list )
	graph->adjList[src] = newNode;
	newNode = createNode(src);
	// Create a node
	newNode->next = graph->adjList[dest];
	// Add node beginning of the linked list ( destth list )
	graph->adjList[dest] = newNode;
}

int BFS(QUEUE** queue, int s, int index, int type){
	if(s == index){
		// If Kevin Bacon is searched return 0
		return 0;
	}
	int i,v;
	static char* name;
	NODE* temp;
	static counter = 0;
	static int visited[VERTEX];
	for(i = 0; i < VERTEX; i++){
		visited[i] = -1;
	}
	// Static variables are using to find path
	// Because I find the path recursively
	enqueue(queue,s);
	// Add Kevin Bacon to the queue
	visited[s] = 0;
	// Assign Kevin Bacon as visited
	while(!isQueueEmpty((*queue))){
		// For all elements of queue
		v = dequeue(queue);
		// Take an index from the queue
		temp = graph->adjList[v];
		// Assign vth list to temp
		while(temp != NULL){
			// For all elements of the list
			if(visited[temp->vertex] == -1){
				// If it was not visited
				enqueue(queue,temp->vertex);
				// Add the vertex to the queue
				visited[temp->vertex] = visited[v] + 1;
				// Assign vertex as visited
				if(temp->vertex == index){
					// If vertex equals to the index we search for
					int x = visited[temp->vertex]/2;
					if(type == 2){ // Type 2 for to return only the number, 1 for to return path
						// If type equls 2 return visited/2
						// Because I increment visited array in each step
						// So it equals 2 times kevin bacon number
						// Because it contains the movies
						// To ignore the movies I divided by 2
						return x;
					}
					printf("%s\n",v>=movieNum?actors[v-movieNum].name:movies[v].name);
					// print the name
					// If v greater or equals than movie number it is an actor
					// Otherwise, it is a movie
					remake(queue);
					// Clear the queue
					strcpy(table[tableCounter].connection[counter], v>=movieNum?actors[v-movieNum].name:movies[v].name);
					// Add the path to the table
					counter++;
					if(v != s){
						// V != Kevin Bacon 
						// Call BFS for v
						// Because v is the step before the index we search
						// So every time I call BFS for v it will go back
						return BFS(queue,s,v,1);
					}
					else{
						// If v equals KEvin Bacon
						table[tableCounter].number = counter/2;
						// Assign Kevin Bacon Number as number
						// I had explained the divison by 2 above
						table[tableCounter++].lineNum = counter;
						// Assign line number of connection string as lineNum
						// It equals counter because the path contains movies and actors together
						// Return Kevin Bacon Number
						return counter/2;
					}
				}
			}
			temp = temp->next;
		}
	}
	// If there is no path
	// return -1
	return -1;

}
