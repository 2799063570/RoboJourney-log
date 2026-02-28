#include <iostream>

using namespace std;

#define inf -1
class Graph
{
private:
	int vertices;	// 顶点个数 
	int** edges;	// 邻接矩阵 二维数组

public:
	Graph(int vertices);
	~Graph();
	void addEdge(int u, int v, int w); // 两点 权重填充的值
	void printGraph();	
};
Graph::Graph(int vertices)
{
	this->vertices = vertices;
	edges = new int* [vertices];
	for (int i = 0; i < vertices; i++)
	{
		edges[i] = new int[vertices];
		for (int j = 0; j < vertices; j++)
			edges[i][j] = inf;
	}
}
Graph::~Graph()
{
	for (int i = 0; i < vertices; i++)
	{
		delete[] edges[i];
	}
	delete[] edges;
}
void Graph::addEdge(int u, int v, int w)
{
	edges[u][v] = w;
}
void Graph::printGraph()
{
	for (int i = 0; i < vertices; i++)
	{
		for (int j = 0; j < vertices; j++)
		{
			cout << edges[i][j] << " ";
		}
		cout << endl;
	}
}
int main()
{
	Graph g(4);
	g.printGraph(); cout << endl;
	g.addEdge(1, 2, 5);
	g.addEdge(0, 1, 34);
	g.printGraph();
	return 0;
}