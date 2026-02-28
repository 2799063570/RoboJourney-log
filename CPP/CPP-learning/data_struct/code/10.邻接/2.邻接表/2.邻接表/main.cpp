#include <iostream>

using namespace std;

class Graph {
private:
	struct EdgeNode {
		int vertex;
		int weight;
		EdgeNode* next;
	};

	struct VertexNode {
		int vertex;
		EdgeNode* firstEdge;
	};
	int vertices;
	VertexNode* nodes;

public:
	Graph(int vertices);
	~Graph();
	void addEdge(int u, int v, int w);
	void printGraph();
};
Graph::Graph(int vertices)
{
	this->vertices = vertices;
	nodes = new VertexNode[vertices];
	for (int i = 0; i < vertices; i++)
	{
		nodes[i].firstEdge = NULL;
		nodes[i].vertex = i;
	}
}
Graph::~Graph()
{
	for (int i = 0; i < vertices; i++)
	{
		EdgeNode* curr = nodes[i].firstEdge;
		while (curr!= nullptr)
		{
			EdgeNode* temp = curr->next;
			delete curr;
			curr = temp;
		}
		delete[] nodes;
	}
}
void Graph::addEdge(int u, int v, int w)
{
	EdgeNode* edge = new EdgeNode;
	edge->vertex = v;
	edge->weight = w;
	edge->next = nodes[u].firstEdge;
	nodes[u].firstEdge = edge;

}
void Graph::printGraph()
{
	for (int i = 0; i < vertices; i++)
	{
		cout << "Vertex" << nodes[i].vertex << ": ";
		EdgeNode* curr = nodes[i].firstEdge;
		while (curr != nullptr)
		{
			cout << curr->vertex << "(" << curr->weight << ")" << " ";
			curr = curr->next;
		}
		cout << endl;		
	}
}


int main()
{
	Graph g(10);
	g.addEdge(0, 1, 1);
	g.addEdge(0, 2, 1);
	g.addEdge(3, 1, 1);
	g.addEdge(4, 1, 1);
	g.addEdge(6, 1, 1);
	g.addEdge(3, 2, 1);
	g.addEdge(5, 2, 1);
	g.addEdge(6, 2, 1);
	g.addEdge(0, 4, 1);
	g.addEdge(0, 5, 1);
	g.addEdge(3, 6, 1);
	g.addEdge(4, 7, 1);
	g.addEdge(6, 4, 1);
	g.addEdge(3, 7, 1);
	g.addEdge(5, 7, 1);
	g.addEdge(6, 9, 1);

	g.printGraph();

	return 0;
}