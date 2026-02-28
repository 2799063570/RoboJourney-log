#include <iostream>
using namespace std;

template<typename T>
class Queue {
public:
	Queue() : data(new T[10]), front(0), rear(0), capacity(10) {}
	~Queue();
	void enqueue(T element);
	T dequeue();
	T getFront() const;
	int getSize() const;

private:
	T* data;
	int front;
	int rear;
	int capacity;
	void resize();
};

template<typename T>
Queue<T>::~Queue()
{
	delete[] data;
}
template<typename T>
void Queue<T>::resize()
{
	int newCapacity = capacity * 2;
	T* newData = new T[newCapacity];
	for (int i = 0; i < (rear - front); i++)
	{
		newData[i] = data[i + front];
	}
	rear -= front;
	front = 0;
	capacity = newCapacity;
	delete[] data;
	data = newData;
}

template<typename T>
void Queue<T>::enqueue(T element)
{
	if (rear == capacity) resize();
	data[rear++] = element;
}

template<typename T>
T Queue<T>::dequeue()
{
	if (front == rear) throw std::underflow_error("queue is empty");
	return data[front++];
}

template<typename T> 
T Queue<T>::getFront() const
{
	if (front == rear) throw std::underflow_error("queue is empty");
	return data[front];
}

template<typename T>
int Queue<T>::getSize() const
{
	return rear - front;
}



int main()
{
	Queue<int> q;
	q.enqueue(12);
	q.enqueue(1214);
	q.enqueue(124);
	cout << q.getFront() << endl;
	cout << q.dequeue() << endl;
	cout << q.getFront() << endl;
	cout << q.getSize() << endl;

	return 0;
}