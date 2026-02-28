#include <iostream>
using namespace std;

template<typename T>
class Queue {
private:
	struct Node {
		T data;
		Node* next;
		Node(T d) : data(d), next(NULL) {}
	};
	Node* front;
	Node* rear;
	int size;

public:
	Queue() : front(NULL), rear(NULL), size(0) {}
	~Queue();
	void enqueue(T element);
	T dequeue();
	T getFront() const;
	int getSize() const;

};

template<typename T>
Queue<T>::~Queue()
{
	while (front)
	{
		Node* temp = front;
		front = front->next;
		delete temp;
	}
}

template<typename T>
void Queue<T>::enqueue(T element)
{
	Node* newNode = new Node(element);

	if (front == NULL) front = newNode;
	if (rear == NULL) rear = newNode;
	else
	{
		rear->next = newNode;
		rear = newNode;
	}
	size++;
}

template<typename T>
T Queue<T>::dequeue()
{
	if (front == NULL)	throw std::underflow_error("Queue is Empty");

	T val = front->data;
	Node* temp = front->next;
	delete front;
	front = temp;
	size--;
	if (size == 0) rear = NULL;		// ±‹√‚“∞÷∏’Î
	return val;
}

template<typename T>
T Queue<T>::getFront() const
{
	if (front == NULL)	throw std::underflow_error("Queue is Empty");
	return front->data;
}

template<typename T>
int Queue<T>::getSize() const
{
	return size;
}

int main()
{
	Queue<int> q;
	q.enqueue(1);
	q.enqueue(23);
	q.enqueue(89);
	cout << q.getFront() << endl;
	cout << q.dequeue() << endl;
	cout << q.getFront() << endl;
	cout << q.getSize() << endl;


	return 0;
}