/*
Mason's gain code v1.0
Inputs: Number of nodes
		From node
		To Node
		Forward Gain
		Backward Gain

Outputs:Forward paths
		Loops
		Store as lists
		creates an array
		stores 1 or 0 depending on if loops are touching or not
		finds the sum of gain product of 'k' non touching loops
*/

#include<iostream>
#include<conio.h>
#include<list>
using namespace std;

class Graph 
{ 
    int N;    // No. of vertices in graph 
    int** adjM;	//Adjacency Matrix
    list<int> *loop, *path_list, *temp_loop;
	int** touching_loops;	//2D matrix to store if two loops are touching 
    int total_loop_number;	//stores the total number of loops in the given circuit
    
    // A recursive function used by printAllPaths() 
    void printAllPathsUtil(int , int , bool [], int [], int &); 
  	void printAllLoopsUtil(int , bool [], int [], int &);
  	// A function used by areLoopsTouchingUtil
	int areLoopsTouchingUtil(int*, int, int*, int);
	void nonTouchingLoopArray(int, int, int, int&, int*);
	  
public:
	Graph(int N); 
    void addEdge(int u, int v, int w, int x); 
    void printAllPaths(int s, int d);
	void printAllLoops(); 
    void printAdjacencyMatrix();
  	void storeLoop(int [], int, int);
  	void storePath(int [], int);
  	void printLoopList();
  	void printPathList();
  	int  loopIsUnique(int [], int, int);
  	int checkLoops(int [], int, int, int, int);
	void sortLoops();
	void printTempLoopList();
	void areLoopsTouching();
	void createTouchingLoopsMatrix();
	int nonTouchingLoopGain(int*, int);
	int totalNonTouchingLoopGain(int);
	void delta();
}; 
   
Graph::Graph(int N)	// Constructor
{
	this -> N = N;
	adjM = new int*[N];				// creating adjacency matrix
	for(int i=0;i<N;i++) adjM[i] = new int[N];
	
	for(int i=0;i<N;i++)			// filling all the elements in the adjacency matrix with 0
		for(int j=0; j<N;j++)
			adjM[i][j]=0;
	
	loop = new list<int>[N+3];		// creating N+1 lists for loops and one list to store the size of the loop and one to store the gain
	path_list = new list<int>[N+1];	// creating N lists for paths one list to store the total gain
	temp_loop = new list<int>[N+1];
	
	total_loop_number = 0;
}
    

void Graph::addEdge(int u, int v, int w, int x)	// edge from node u to node v with gain w and edge from node v to node u with gain w
{
	adjM[u][v] += w;
	adjM[v][u] += x;
}

void Graph::printAllPaths(int s, int d)
{
	bool *visited = new bool[N];	//To mark if a node is visited while DFS
	int *path = new int[N];			//To store the path from the source node to the current node
	int path_index = 0;				//
	
	for(int i=0; i<N; i++) visited[i] = false;
	
	printAllPathsUtil(s, d, visited, path, path_index);	
}

void Graph::printAllPathsUtil(int u, int d, bool visited[], int path[], int &path_index)
{
	visited[u] = true;
	path[path_index] = u;
	path_index++;
	
	if(u==d)
	{
		// copied from Geeks for Geeks, we have to change this part. 
		// This part only prints the path. It would be good if we can store the path somewhere so that we can use whenever we want
		// preferrably a dynamic 2D array. Can we make it using vector?
		for(int i=0;i<path_index;i++)
		   cout << path[i] << " "; 
        cout << endl; 
        storePath(path,path_index); 
	}
	else
	{
		for(int i=0;i<N;i++)
		{
			if(adjM[u][i]!=0&&!visited[i])
			{
                printAllPathsUtil(i, d, visited, path, path_index);
			}
		}
	}
	path_index--; 
    visited[u] = false;
}

void Graph::storePath(int path[], int path_index)
{
	int temp, gain=1;
	int list_row=0;
	for(temp=0;temp<path_index;temp++)
	{
		if(temp>0) gain *= adjM[path[temp-1]][path[temp]];
		path_list[list_row].push_back(path[temp]);
		list_row++;
	}
	while(list_row<N) 
	{
		path_list[list_row].push_back(-1);
		list_row++;
	}
	path_list[N].push_back(gain);
}

void Graph::printPathList()
{
	cout<<"\nThe stored Path:\n";
	list<int>::iterator i;
	for(int temp=0; temp<N+1; temp++)
	{
		for (i = path_list[temp].begin(); i != path_list[temp].end(); ++i) 
            cout<<*i<<"  ";
        cout<<"\n";
	}
}

void Graph::printAllLoops()
{
	bool *visited = new bool[N];	//To mark if a node is visited while DFS
	int *path = new int[N];			//To store the path from the source node to the current node
	int path_index = 0;				//
	for(int i=0; i<N; i++) visited[i] = false;
	
	printAllLoopsUtil(0, visited, path, path_index);		
}
	
void Graph::printAllLoopsUtil(int u, bool visited[], int path[], int &path_index)
{
	visited[u] = true;
	path[path_index] = u;
	path_index++;
	
	for(int i=0;i<N;i++)
	{
		if(adjM[u][i]!=0)
		{
			if(!visited[i])
               printAllLoopsUtil(i, visited, path, path_index);
            else
            {
            	
            	int loop_start;
            	int j;
            	for(j=0;path[j]!=i;j++);
            	loop_start = j;
		  		for(;j<path_index;j++)
		  			cout << path[j] << " "; 
		  		cout<<i;
        		cout << endl;
        		if(loopIsUnique(path,loop_start,path_index))
        			{
        				storeLoop(path,loop_start,path_index);
						total_loop_number++;
					}
			}
		}
	}
	path_index--; 
    visited[u] = false;
}

int Graph::loopIsUnique(int path[], int loop_start, int path_index)
{
	int loop_number=0;
	list<int>::iterator i;
	for (i = loop[N+1].begin(); i != loop[N+1].end(); ++i)
	{
		if(*i == (path_index-loop_start))
		{
			for(int temp=0;temp<*i;temp++)		// temp variable to find if the loop has any node matching the node in path[loop_start]
			{
				list<int>::iterator j = loop[temp].begin();
				for(int temp1=0;temp1<loop_number;temp1++) ++j;	//temp variable to move to the loop_number'th column/element of list[temp]
				if(*j==path[loop_start])
				{
					if(checkLoops(path, loop_start, path_index, loop_number, temp)) return 0; 
				}
			}
		}
		loop_number++;
	}
	return 1;
}

int Graph::checkLoops(int path[], int loop_start, int path_index, int loop_number, int temp)
{
	list<int>::iterator j = loop[temp].begin();	//iterator to reach loop[temp][loop_number]
	for(int temp2=0;temp2<loop_number;temp2++) ++j; //*j = loop[temp][loop_number]

	for(int temp1 = 0; temp1<(path_index-loop_start); temp1++)
	{
		if(path[loop_start+temp1]!= *j) return 0;
		temp++;
		j = loop[temp].begin();	//iterator to reach loop[temp][loop_number]
		for(int temp2=0;temp2<loop_number;temp2++) ++j; //*j = loop[temp][loop_number]
		if(*j == -1)
		{
			temp = 1;
			j = loop[temp].begin();	//iterator to reach loop[temp][loop_number]
			for(int temp2=0;temp2<loop_number;temp2++) ++j; //*j = loop[temp][loop_number]
		}
	}
	return 1;
}

void Graph::storeLoop(int path[], int loop_start, int path_index)
{
	int temp, gain=1;
	int list_row=0;
	for(temp=loop_start;temp<path_index;temp++)
	{
		if(temp>loop_start) gain *= adjM[path[temp-1]][path[temp]];
		loop[list_row].push_back(path[temp]);
		list_row++;
	}
	loop[list_row].push_back(path[loop_start]);
	gain *= adjM[path[temp-1]][path[loop_start]];
	list_row++;
	while(list_row<N+1) 
	{
		loop[list_row].push_back(-1);
		list_row++;
	}
	loop[N+1].push_back(path_index-loop_start);
	loop[N+2].push_back(gain);
}

void Graph::printLoopList()
{
	cout<<"\nThe stored Loops:\n";
	list<int>::iterator i;
	for(int temp=0; temp<N+3; temp++)
	{
		for (i = loop[temp].begin(); i != loop[temp].end(); ++i) 
            cout<<*i<<"  ";
        cout<<"\n";
	}
}

void Graph::printAdjacencyMatrix()
{
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
			cout<<adjM[i][j]<<" ";
		cout<<"\n";
	}
}      

void Graph::sortLoops()
{
	int loop_number=-1;
	list<int>::iterator i;
	for (i = loop[N+1].begin(); i != loop[N+1].end(); ++i)   //loop that runs as many iterations as there are loops in the circuit 
    {
		loop_number++;
    	int *loop_array = new int[*i]; 		//create an array of size equal to that of the current loop
    	for(int temp=0;temp<*i; temp++)
		{
			list<int>::iterator j = loop[temp].begin();
			for(int temp1=0; temp1<loop_number; temp1++)	// to reach the element loop[temp][loop_number] and store the element in array[temp]
			{
				++j;		
			}
			loop_array[temp] = *j;
		}
		//bubble sort loop_array`
		{
			int temp2, temp3; 
	   		bool swapped; 
   			for (temp2 = 0; temp2 < *i-1; temp2++) 
   			{ 
   	  			swapped = false; 
   	  			for (temp3 = 0; temp3 < *i-temp2-1; temp3++) 
   	  			{ 
					if (loop_array[temp3] > loop_array[temp3+1]) 
        			{		 
			 			int temp4;
			 			temp4 = loop_array[temp3];
			 			loop_array[temp3] = loop_array[temp3+1];
			 			loop_array[temp3+1] = temp4;	 
          	 			swapped = true; 
        			}		 
     			}	 
  
     			// IF no two elements were swapped by inner loop, then break 
     			if (swapped == false) 
      	  			break; 
			}	
		}
		int temp5;
		for(temp5=0;temp5<*i;temp5++)
		{
			temp_loop[temp5].push_back(loop_array[temp5]);
		}
		while(temp5<N+1) 
		{
			temp_loop[temp5].push_back(-1);
			temp5++;	
		}
	}    
}

void Graph::printTempLoopList()
{
	cout<<"\nThe sorted Loops:\n";
	list<int>::iterator i;
	for(int temp=0; temp<N+1; temp++)
	{
		for (i = temp_loop[temp].begin(); i != temp_loop[temp].end(); ++i) 
            cout<<*i<<"  ";
        cout<<"\n";
	}
}

void Graph::createTouchingLoopsMatrix()
{
	touching_loops = new int*[total_loop_number];				// creating matrix that stores if two loops are touching
	for(int i=0;i<total_loop_number;i++) touching_loops[i] = new int[total_loop_number];
	
	for(int i=0;i<total_loop_number;i++)			// filling all the elements in the matrix with 0
		for(int j=0; j<total_loop_number;j++)
			touching_loops[i][j]=0;
}

void Graph::areLoopsTouching()
{
	if(total_loop_number==1) 
	{
		touching_loops[0][0] = 0;
	}
	else
	{
		list<int>::iterator i,j;
		int size1=0, size2=0;
		for(int temp=0;temp<total_loop_number-1;temp++)	//to get the first loop, and so on till total_loop_number-1'th loop
		{
			i = loop[N+1].begin();
			for(int temp1=0;temp1<temp;temp1++) ++i;
			size1 = *i;									// to store the size of the loop
			int *arr1 = new int[size1];					// create an array of size equal to that of the loop
			for(int temp1=0;temp1<size1;temp1++)		// To store the elements of the loop in arr1 
			{
				i = temp_loop[temp1].begin();
				for(int temp4=0;temp4<temp;temp4++) ++i;
				arr1[temp1] = *i;			
			}

			for(int temp2=temp+1;temp2<total_loop_number;temp2++)
			{
				j = loop[N+1].begin();
				for(int temp3=0;temp3<temp2;temp3++) ++j;
				size2 = *j;								// to store the size of the loop
				int *arr2 = new int[size2];				// to create an array of size = size2
				for(int temp3=0;temp3<size2;temp3++)	// To store the elements of the loop in arr2 
				{	
					j = temp_loop[temp3].begin();
					for(int temp5=0;temp5<temp2;temp5++) ++j;
					arr2[temp3] = *j;			
				}
				
				for(int temp6=0;temp6<size2;temp6++) cout<<" "<<arr2[temp6];
				cout<<endl;
				
				touching_loops[temp][temp2] = areLoopsTouchingUtil(arr1, size1, arr2, size2);
				touching_loops[temp2][temp] = touching_loops[temp][temp2];		
			}
		}
		
	}
	for(int temp1=0; temp1<total_loop_number;temp1++)
	{
		for(int temp2 = 0; temp2<total_loop_number; temp2++)
		cout<<touching_loops[temp1][temp2]<<" ";
		cout<<endl;
	}
}

int Graph::areLoopsTouchingUtil(int* arr1,int size1, int* arr2, int size2)
{
	for(int temp1=0, temp2=0;temp1<size1&&temp2<size2;)
	{
		if(arr1[temp1]==arr2[temp2]) return 1;
		else if(arr1[temp1]<arr2[temp2])temp1++;
		else temp2++;
	}
	return 0;
}

int Graph::nonTouchingLoopGain(int* arr, int size)
{
	int loopGain=1;
	for(int i=0; i<size-1; i++)
	{
		for(int j=i+1;j<size;j++)
		if(touching_loops[arr[i]][arr[j]]==1) return 0;
	}
	list<int>::iterator i;
	for(int temp=0;temp<size;temp++)
	{
		i = loop[N+2].begin();
		for(int temp1=0;temp1<temp;temp1++) ++i;
		loopGain *= *i;							
	}
	return loopGain;
}

int Graph::totalNonTouchingLoopGain(int size)
{
	int *arr = new int[size];
	for(int i=0;i<size;i++) arr[i]=0;
	int index=0, sum=0;
	for(int i=0;i<total_loop_number+1-size;i++)
	{
		nonTouchingLoopArray(index, size, i, sum, arr);
	}
	return sum;
}

void Graph::nonTouchingLoopArray(int index, int size, int current_loop_number,int& sum, int* arr)
{
	arr[index++] = current_loop_number;
	if(index<size)
	{
		for(int j=current_loop_number+1;j<total_loop_number+1+index-size;j++)
		{
			nonTouchingLoopArray(index, size, j, sum, arr);
		}
	}
	if(index==size)
	{
		sum += nonTouchingLoopGain(arr,size);
	}
}

void Graph::delta()
{
	int delta = 1;
	for(int i=1;i<=total_loop_number;i++)
	{
		if(i%2==0) delta += totalNonTouchingLoopGain(i);
		else delta -= totalNonTouchingLoopGain(i);
	}
}

int main()
{
	char choice = 'y';
	int u,v,fwd,bckwd;
	int N;
	int s=0,d; d=N-1;
	cout<<"Enter N:";cin>> N;
	Graph g(N);
	do
	{
		cout<<"\nFrom node:";cin>>u;
		cout<<"\nTo node:";cin>>v;
		cout<<"\nForward Gain:";cin>>fwd;
		cout<<"\nBackward Gain:";cin>>bckwd;
		cout<<"\nChoice:";cin>>choice;
		g.addEdge(u,v,fwd,bckwd);
	}while(choice == 'y'||choice == 'Y');	
	g.printAdjacencyMatrix();
	cout<<"Enter the starting node"; cin>> s;
	cout<<"Enter the ending node"; cin>> d;
	g.printAllLoops(); 
	g.printPathList();
	g.printLoopList();
	g.createTouchingLoopsMatrix();
	g.sortLoops();
	g.printTempLoopList();
	g.areLoopsTouching();
	g.delta();
	getch();
	return 0;
} 
