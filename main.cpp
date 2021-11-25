#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <forward_list>
#include <vector>
#include <stack>
#include <ostream>

using namespace std;

/*
    generate an adjacency matrix for conceptually describing a fabricated directed
    or undirected graph, isSimple set true means no self-loops and no multiple ed-
    ges between nodes; isSimple set false means it could have self-loops but still
    no multiple edges between nodes. maxWeighted means the max weight of each path
    in the graph, mode specifies directed or undirected graph.
*/
int** generateAdjacencyMatrix(const int& nodeNum, bool isSimple = true, int maxWeighted = 2, char mode = 'u')
{
    int** matrix = new int*[nodeNum];
    for_each(matrix, matrix + nodeNum, [=](int*& ptr){ptr = new int[nodeNum];});
    srand((unsigned int)time(NULL));
    for(int i = 0; i < nodeNum; i++)
    {
        for(int j = 0; j < nodeNum; j++)
        {
            if(i < j || (i == j && !isSimple))
            {
                matrix[i][j] = rand() % maxWeighted;
            }
            else if(isSimple)
            {
                matrix[i][j] = 0;
            }
        }
    }

    for(int i = 0; i < nodeNum; i++)
    {
        for(int j = 0; j < nodeNum; j++)
        {
            if(i > j) 
            {
                mode == 'u' || mode == 'U' ? matrix[i][j] = matrix[j][i]: matrix[i][j] = rand() % maxWeighted;
            }
            else
            {
                break;
            }
        }
    }
    return matrix;
}

/*
    print m by n matrix consisting of elements of type T that can be inserted into
    cout. traversal time is O(mn)
*/
template<class T>
void printMatrix(T**& matrix, int m, int n)
{
    cout << "The adjacency matrix is:" << endl << endl;;

    cout << setw(4) << ' ';
    for(int i = 0; i < n; i++)
    {
        cout << setw(4) << i;
    }

    cout << endl << "     ";
    for(int i = 0; i < n; i++){cout << "----";}
    cout << endl;

    for(int i = 0; i < m; i++)
    {
        cout << i << "  |";
        for(int j = 0; j < n; j++)
        {
            cout << setw(4) << matrix[i][j];
        }
        if(i != m - 1)
        {
            cout << endl << "   |" << endl;
        }
        else
        {
            cout << endl << endl;
        }
    }
}


/*
    convert an adjacency matrix to whether an in-going or an out-going adjacency 
    list, each node has a pair of information (node, weight) 
*/
vector<forward_list<pair<int, int>*>*>* adjacencyMatrixToList(int**& matrix, int nodeNum, char mode = 'o')
{
    vector<forward_list<pair<int, int>*>*>* nodeList = new vector<forward_list<pair<int, int>*>*>;

    if(mode == 'o' || mode == 'O')
    {
        for(int i = 0; i < nodeNum; i++)
        {
            forward_list<pair<int, int>*>* list;
            nodeList->push_back(list);
            nodeList->at(i) = new forward_list<pair<int, int>*>;
            for(int j = 0; j < nodeNum; j++)
            {
                if(matrix[i][j] > 0)
                {
                    pair<int, int>* p = new pair<int, int>; 
                    p->first = j;
                    p->second = matrix[i][j];
                    nodeList->at(i)->emplace_front(p);
                }
            }
        }
    }

    else
    {
        for(int i = 0; i < nodeNum; i++)
        {
            forward_list<pair<int, int>*>* list;
            nodeList->push_back(list);
            nodeList->at(i) = new forward_list<pair<int, int>*>;
            for(int j = 0; j < nodeNum; j++)
            {
                if(matrix[j][i] > 0)
                {
                    pair<int, int>* p = new pair<int, int>; 
                    p->first = j;
                    p->second = matrix[j][i];
                    nodeList->at(i)->emplace_front(p);
                }
            }
        }
    }
    return nodeList;
}

/*
    print the adjacency list, traversal time is O(number of edges)
*/
void printList(vector<forward_list<pair<int, int>*>*>*& adjList)
{
    cout << "The adjacency list is (node, weight):" << endl << endl;
    for(int i = 0; i < adjList->size(); i++)
    {
        cout << "node " << i << " -> ";
        for(auto iter = adjList->at(i)->begin(); iter != adjList->at(i)->end(); ++iter)
        {
            cout << "(" << (*iter)->first << ", " << (*iter)->second << ") -> ";
        }
        cout << '0' << endl;
    }
    cout << endl;
}

/*
    implementation of BFS, namely, breadth-first-search, the input graph is man-
    aged by the adjacency list, find the connected component which contains the
    node at startNode, return a set of breadth-based level layers
*/
vector<vector<int>*>* BFS(vector<forward_list<pair<int, int>*>*>*& adjList, int startNode)
{
    int nodeNum = adjList->size();
    vector<vector<int>*>* layers = new vector<vector<int>*>;
    vector<int>* layer0 = new vector<int>;
    layer0->push_back(startNode);
    layers->push_back(layer0);
    int layerIndex = 0;
    bool explored[nodeNum];
    fill(explored, explored + nodeNum, false);
    explored[startNode] = true;

    while(!layers->at(layerIndex)->empty())
    {
        vector<int>* nextLayer = new vector<int>;
        layers->push_back(nextLayer);

        for(int node: *(layers->at(layerIndex)))
        {
            for(auto iter = adjList->at(node)->begin(); iter != adjList->at(node)->end(); ++iter)
            {
                if(!explored[(*iter)->first])
                {
                    explored[(*iter)->first] = true;
                    layers->at(layerIndex + 1)->push_back((*iter)->first);
                }
            }
        }

        layerIndex++;
    }

    layers->pop_back();
    return layers;
}

/*
    print the result of BFS algorithm layer by layer
*/
void printBFSLayers(vector<vector<int>*>*& layers)
{
    cout << "All layers derived from BFS:" << endl << endl;
    int count = 0;

    for(auto layer: *layers)
    {
        cout << "L[" << count++ << "] = { "; 

        for (auto node: *layer)
        {
            cout << node << ", ";
        }
        cout << " }" << endl;
    }

    cout << endl;
}

/*
    implementation of DFS, namely, depth-first-search, the input graph is man-
    aged by the adjacency list, find the connected component which contains the
    node at startNode, return a set of reachable nodes referring to the node at
    startNode
*/
vector<int>* DFS(vector<forward_list<pair<int, int>*>*>*& adjList, int startNode)
{
    int nodeNum = adjList->size();
    bool explored[nodeNum];
    fill(explored, explored + nodeNum, false);
    vector<int>* reachableNodes = new vector<int>;
    stack<int> pipe;
    pipe.push(startNode);

    while(!pipe.empty())
    {
        int processingNode = pipe.top();
        pipe.pop();
        if(!explored[processingNode])
        {
            explored[processingNode] = true;
            for(auto node: *(adjList->at(processingNode)))
            {
                pipe.push(node->first);
            }
        }
    }

    for(int i = 0; i < nodeNum; i++)
    {
        if(explored[i])
        {
            reachableNodes->push_back(i);
        }
    }

    return reachableNodes;
}

/*
    print the result of DFS algorithm 
*/
void printDFSNodes(vector<int>*& reachableNodes)
{
    cout << "All reachable nodes derived from DFS:" << endl << endl << "{ ";
    for(auto node: *reachableNodes)
    {
        cout << node << ' ';
    }
    cout << "}" << endl << endl;
}

int main()
{
    // generate an adjacency matrix to illustrate a supposed graph
    int nodeNum = 6;
    int** adjacencyMatrix = generateAdjacencyMatrix(nodeNum, true, 2, 'd');
    printMatrix<int>(adjacencyMatrix, nodeNum, nodeNum);

    // convert the adjacency matrix to an adjacency list that can manage the graph
    vector<forward_list<pair<int, int>*>*>* adjList = adjacencyMatrixToList(adjacencyMatrix, nodeNum, 'o');
    printList(adjList);

    // invoke BFS and print the result  
    vector<vector<int>*>* resultBFS = BFS(adjList, 0);
    printBFSLayers(resultBFS);

    // invoke DFS and print the result  
    vector<int>* resultDFS = DFS(adjList, 0);
    printDFSNodes(resultDFS);

    system("pause");
}