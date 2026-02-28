#include <iostream>
#include <stack>

using namespace std;

template <typename T>
class Queue {

private:
	stack<T> s1;
	stack<T> s2;

public:
	void enQueue(T element);
	T deQueue();
	T Front();
	bool Empty();
};

template<typename T>
void Queue<T>::enQueue(T element)
{
	s1.push(element);
}

template<typename T>
T Queue<T>::deQueue()
{

	if (s1.empty() && s2.empty()) throw std::underflow_error("Queue is Empty");
	if (s2.empty()) {
		while (!s1.empty())
		{
			s2.push(s1.top());
			s1.pop();
		}
		T num = s2.top();
		s2.pop();
		return num;		
	}
	else
	{
		T num = s2.top();
		s2.pop();
		return num;		
	}
}

template<typename T>
T Queue<T>::Front()
{

	if (s1.empty() && s2.empty()) throw std::underflow_error("Queue is Empty");
	if (s2.empty()) {
		while (!s1.empty())
		{
			s2.push(s1.top());
			s1.pop();
		}
		return s2.top();
	}
	else return s2.top();

}

template<typename T>
bool Queue<T>::Empty()
{
	return s1.empty() && s2.empty();
}


int main()
{
	Queue<int> q;
	q.enQueue(1);
	q.enQueue(233);
	q.enQueue(3);
	cout << q.Front() << endl;
	cout << q.deQueue() << endl;
	cout << q.Front() << endl;


	return 0;
}