#include <iostream>
using namespace std;

template<typename T>
class Stack {

private:
	struct Node {
		T data;
		Node* next;
		Node(T d) : data(d), next(NULL) {}
	};
	Node* head;
	int size;

public:
	Stack() : head(NULL), size(0) {}
	~Stack();
	void push(T element);
	T pop();
	T top() const;
	int getSize() const;
};

template<typename T>
Stack<T>::~Stack()
{	
	while (!head)
	{
		Node* temp = head;
		delete temp;
		head = head->next;
	}
}

template<typename T> 
void Stack<T>::push(T element)
{
	Node* newNode = new Node(element);
	newNode->next = head;
	head = newNode;
	++size;
}

template<typename T>
T Stack<T>::pop()
{
	if (size == 0) throw std::underflow_error("Stack is empty");
	Node* temp = head;
	T val = head->data;
	head = head->next;
	--size;	
	delete temp;
	return val;
}

template<typename T>  
T Stack<T>::top() const
{
	if (size == 0) throw std::underflow_error("Stack is empty");
	return head->data;
}

template<typename T>
int Stack<T>::getSize() const
{
	return size;
}

int main()
{

	Stack<int> st;
	st.push(11);
	st.push(22);
	st.push(33);
	st.push(1441);
	cout << st.top() << endl;
	cout << st.pop() << endl;
	cout << st.top() << endl;
	cout << st.getSize() << endl;

	return 0;
}