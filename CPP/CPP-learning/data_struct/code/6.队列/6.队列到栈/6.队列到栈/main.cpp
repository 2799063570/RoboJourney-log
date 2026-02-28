#include <iostream>
#include <queue>

using namespace std;


template<typename T>
class Stack {

private:
	queue<T> q1;
	queue<T> q2;
public:
	void enQueue(T element);
	T deQueue();
	T front();
	bool empty();
};

template<typename T>
void Stack<T>::enQueue(T element)
{
	q1.push(element);
}
template<typename T>
T Stack<T>::deQueue()
{
	if (empty()) throw std::underflow_error("Stack is Empty");
	T ele = 0;
	while (q1.size() > 1)
	{
		q2.push(q1.front());
		q1.pop();
	}
	/*while (!q1.empty())
	{
		ele = q1.front();
		q1.pop();
		if (q1.empty()) break;
		q2.push(ele);
	}*/
	ele = q1.front();
	q1.pop();
	while (!q2.empty())
	{
		q1.push(q2.front());
		q2.pop();
	}
	return ele;
}

template<typename T>
T Stack<T>::front()
{
	if (empty()) throw std::underflow_error("Stack is Empty");
	T ele = 0;
	while (!q1.empty())
	{	
		ele = q1.front();
		q2.push(ele);
		q1.pop();
		if (q1.empty()) break;		
	}
	while (!q2.empty())
	{
		q1.push(q2.front());
		q2.pop();
	}
	return ele;
}

template<typename T>
bool Stack<T>::empty()
{
	return q1.empty();
}

int main()
{
	Stack<int> s;
	s.enQueue(1);
	s.enQueue(23);
	s.enQueue(3);
	s.enQueue(5);
	cout << s.front() << endl;
	cout << s.deQueue() << endl;
	cout << s.deQueue() << endl;
	cout << s.deQueue() << endl;
	cout << s.front() << endl;

	return 0;
}