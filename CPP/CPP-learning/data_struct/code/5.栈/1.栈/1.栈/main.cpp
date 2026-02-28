#include <iostream>
using namespace std;


template<typename T>
class Stack {
public:
	Stack(): data(new T[10]), size(0), capacity(10){}
	~Stack();
	void push(T element);
	T pop();
	T top() const;
	int getSize() const;

private:
	T* data;
	int size;
	int capacity;
	void resize();
};

template<typename T>
void Stack<T>::resize()
{
	int newCapcity = capacity * 2;
	T* newData = new T[newCapcity];
	for (int i = 0; i < size; i++)
	{
		newData[i] = data[i];
	}
	delete[] data;
	data = newData;
	capacity = newCapcity;
}

template<typename T>
Stack<T>::~Stack()
{
	delete[] data;
	data = NULL;
	capacity = size = 0;
}

template<typename T>
void Stack<T>::push(T element)
{
	if (size == capacity) resize();
	data[size++] = element;
}

template<typename T>
T Stack<T>::pop()
{
	if (size <= 0) throw std::underflow_error("Stack id empty");
	return data[--size];
}

template<typename T>
T Stack<T>::top() const
{
	if (size <= 0) throw std::underflow_error("Stack id empty");
	return data[size - 1];
}

template<typename T>
int Stack<T>::getSize() const
{
	return size;
}




int main()
{
	Stack<int> st;
	st.push(12);
	st.push(13);
	cout << st.top() << endl;
	cout << st.pop() << endl;
	cout << st.top() << endl;
	cout << st.getSize() << endl;



	return 0;
}