#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <forward_list>
#include <vector>
#include <stack>
#include <ostream>
#include <map>
#include <queue>

using namespace std;

namespace graph
{
    typedef vector<forward_list<pair<int, int>*>*> AdjList;
    class DijkstraPriorityQueueCompare
    {
        public:
        bool operator() (const pair<int, int>& p1, const pair<int, int>& p2) const
        {
            return p1.second >= p2.second;
        }
    };
    typedef priority_queue<pair<int, int>, vector<pair<int, int>>, DijkstraPriorityQueueCompare> DijkQueue;
}


/*
    generate an adjacency matrix for conceptually describing a fabricated directed
    or undirected graph, isSimple set true means no self-loops and no multiple ed-
    ges between nodes; isSimple set false means it could have self-loops but still
    no multiple edges between nodes. maxWeighted means the max weight of each path
    in the graph, mode specifies directed or undirected graph. prob0 means the pr-
    obability of occurrence of 0
*/
int** generateAdjacencyMatrix(const int& nodeNum, bool isSimple = true, int maxWeighted = 2, char mode = 'u', float prob0 = 0.6)
{
    int** matrix = new int*[nodeNum];
    int occurrence0[100];
    fill(occurrence0, occurrence0 + 100, 1);
    fill(occurrence0, occurrence0 + (int)(prob0 * 100), 0);
    for_each(matrix, matrix + nodeNum, [=](int*& ptr){ptr = new int[nodeNum];});
    srand((unsigned int)time(NULL));
    for(int i = 0; i < nodeNum; i++)
    {
        for(int j = 0; j < nodeNum; j++)
        {
            if(i < j || (i == j && !isSimple))
            {
                matrix[i][j] = occurrence0[rand() % 100] * (rand() % maxWeighted);
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
                mode == 'u' || mode == 'U' ? matrix[i][j] = matrix[j][i]: matrix[i][j] = occurrence0[rand() % 100] * (rand() % maxWeighted);
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
graph::AdjList* adjacencyMatrixToList(int**& matrix, int nodeNum, char mode = 'o')
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
void printList(graph::AdjList*& adjList)
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
vector<vector<int>*>* BFS(graph::AdjList*& adjList, int startNode)
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
vector<int>* DFS(graph::AdjList*& adjList, int startNode)
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

/*
    find a topological order if it exists
*/
vector<int>* topologicalOrder(graph::AdjList*& inGoingList, graph::AdjList*& outGoingList)
{
    int nodeNum = inGoingList->size();
    int indeg[nodeNum];
    fill(indeg, indeg + nodeNum, 0);
    stack<int> nodeWithNoInGoing;
    vector<int>* ordering = new vector<int>;

    for(int i = 0; i < nodeNum; i++)
    {
        for(auto node: *(inGoingList->at(i)))
        {
            indeg[i]++;
        }
    }

    for(int i = 0; i < nodeNum; i++)
    {
        if(indeg[i] == 0)
        {
            nodeWithNoInGoing.push(i);
        }
    }

    while(!nodeWithNoInGoing.empty())
    {
        int precedingNode = nodeWithNoInGoing.top();
        ordering->push_back(precedingNode);
        nodeWithNoInGoing.pop();

        for(auto node: *(outGoingList->at(precedingNode)))
        {
            if((--indeg[node->first]) == 0)
            {
                nodeWithNoInGoing.push(node->first);
            }
        }
    }

    if(ordering->size() != nodeNum)
    {
        throw "This instance is not a DAG.";
    }

    return ordering;
}

/*
    print the ordering of DAG
*/
void printTopologicalOrdering(vector<int>*& ordering)
{
    cout << "There exists a topological order, hence, this graph is a DAG, the ordered array is:";
    cout << endl << endl << "{ ";
    for(auto node: *ordering)
    {
        cout << node << ", ";
    }
    cout << "}" << endl << endl;
}

/*
    implementation of Dijkstra's algorithm for single source shortest path pro-
    blem, which tends to find the shortest path from a given single source, th-
    e input graph is managed by the out-going adjacency list, note that this a-
    lgorithm only deals with nonnegative weighted path, return a map from nodes
    onto distances 
*/
map<int, int>* DijkstraSSSP(int**& adjMatrix, int nodeNum, int source)
{
    // nodes coupled with their current distances, node with shorter distance has higher priority
    graph::DijkQueue S;
    map<int, int>* result = new map<int, int>;
    stack<pair<int, int>> tempContainer;

    for(int i = 0; i < nodeNum; i++)
    {
        i == source ? S.push(pair<int, int>(i, 0)): S.push(pair<int, int>(i, INT_MAX));
    }

    while(!S.empty())
    {
        pair<int, int> curPair = S.top();
        result->insert(curPair);
        S.pop();

        while(!S.empty())
        {
            pair<int, int> targetPair = S.top();
            if(curPair.second != INT_MAX)
            {
                if(adjMatrix[curPair.first][targetPair.first] > 0)
                {
                    if(targetPair.second > adjMatrix[curPair.first][targetPair.first] + curPair.second)
                    targetPair.second = adjMatrix[curPair.first][targetPair.first] + curPair.second;
                }
            }

            tempContainer.push(targetPair);
            S.pop();
        }

        while(!tempContainer.empty())
        {
            S.push(tempContainer.top());
            tempContainer.pop();
        }
        
    }

    return result;
}

void printDijkstraSSSP(map<int, int>*& result, int nodeNum, int source)
{
    cout << "The distances of the shortest paths from source node " << source << " to the others are:" << endl << endl;
    cout << "  node     distance" << endl << "  __________________" << endl << endl;

    for(int i = 0; i < nodeNum; i++)
    {
        if((*result)[i] == INT_MAX) cout << setw(6) << i << "     " << setw(8) << "inf" << endl << endl;
        else cout << setw(6) << i << "     " << setw(8) << (*result)[i] << endl << endl;
    }
}

int main()
{
    // generate an adjacency matrix to illustrate a supposed graph
    int nodeNum = 5;
    int** adjacencyMatrix = generateAdjacencyMatrix(nodeNum, true, 5, 'd', 0.5);
    printMatrix<int>(adjacencyMatrix, nodeNum, nodeNum);

    // convert the adjacency matrix to an adjacency list that can manage the graph
    graph::AdjList* adjList = adjacencyMatrixToList(adjacencyMatrix, nodeNum, 'o');
    printList(adjList);

    // invoke BFS and print the result  
    vector<vector<int>*>* resultBFS = BFS(adjList, 0);
    printBFSLayers(resultBFS);

    // invoke DFS and print the result  
    vector<int>* resultDFS = DFS(adjList, 0);
    printDFSNodes(resultDFS);

    // invoke topologicalOrder and print the result
    graph::AdjList* inGoingList = adjacencyMatrixToList(adjacencyMatrix, nodeNum, 'i');
    graph::AdjList* outGoingList = adjacencyMatrixToList(adjacencyMatrix, nodeNum, 'o');
    vector<int>* resultTO = topologicalOrder(inGoingList, outGoingList);
    printTopologicalOrdering(resultTO);

    // invoke DijkstraSSSP and print the result
    int source = 0;
    map<int, int>* resultDijkstra = DijkstraSSSP(adjacencyMatrix, nodeNum, source);
    printDijkstraSSSP(resultDijkstra, nodeNum, source);


    system("pause");
}