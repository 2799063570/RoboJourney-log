#include <iostream>
using namespace std;

#define eleType int

struct LinkNode {
	eleType data;
	LinkNode* next;
	LinkNode() : data(0), next(NULL) {}
	LinkNode(eleType value) : data(value), next(NULL) {}
};

class LinkedList {
private:
	LinkNode* head;
	int size;
public:
	LinkedList() : head(NULL), size(0) {}
	~LinkedList();
	void insert(int index, eleType value);
	void remove(int index);
	LinkNode* find(eleType value);
	eleType getValue(int index);
	LinkNode* getArr(int index);
	void updata(int index, eleType value);
	void print();
	void clear();
	void appead(eleType value);
	void ascInsert(eleType value);
};

LinkedList::~LinkedList()
{
	LinkNode* curr = head;
	while (curr != NULL)
	{
		LinkNode* temp = curr;
		curr = curr->next;
		delete temp;
	}
}
LinkNode* LinkedList::getArr(int index)
{
	if (index<0 || index>=size) throw(std::out_of_range("Invalid Index"));
	LinkNode* curr = head;
	// for (int i = 0; i<index; i++) curr = curr->next;
	while (index)
	{
		curr = curr->next;
		index--;
	}
	return curr;
}
eleType LinkedList::getValue(int index)
{
	if (index < 0 || index >=size) throw(std::out_of_range("Invalid Index"));
	return getArr(index)->data;
}

void LinkedList::insert(int index, eleType value)
{
	if (index < 0 || index >size) throw std::out_of_range("Invalid Index");
	LinkNode* newNode = new LinkNode;
	newNode->data = value;
	if (index == 0)
	{
		newNode->next = head;
		head = newNode;
	}
	else if (index > 0 && index < size)
	{
		LinkNode* temp = getArr(index - 1);
		newNode->next = temp->next;
		temp->next = newNode;

	}
	else if (index == size)
	{
		getArr(index - 1)->next = newNode;
		newNode->next = NULL;
	}

	size++;
}
void LinkedList::remove(int index)
{
	if (index < 0 || index >= size) throw(std::out_of_range("Invalid Index"));
	if (index == 0)
	{
		LinkNode* temp = head;
		head = head->next;
		delete temp;
	}
	else if (index > 0 && index < size - 1)
	{
		LinkNode* temp = getArr(index);
		getArr(index - 1)->next = temp->next;
		delete temp;
	}
	else
	{
		delete getArr(index);
		getArr(index - 1)->next = NULL;
	}
	size--;
}

void LinkedList::print()
{
	for (int i = 0; i < size; i++)
	{
		cout << getValue(i) << " ";
	}
	cout << endl;
}

LinkNode* LinkedList::find(eleType value)
{
	LinkNode* arr = head;
	//while(arr && arr->data != value) arr = arr->next;
	for (int i = 0; i < size; i++)
	{
		if (arr->data == value) return arr;
		arr = arr->next;
	}
	return NULL;
}
void LinkedList::updata(int index, eleType value)
{
	getArr(index)->data = value;
}

void LinkedList::clear()
{
	LinkNode* curr = head;
	while (curr != NULL)
	{
		LinkNode* temp = curr;
		curr = curr->next;
		delete temp;
	}
	head = NULL;
	size = 0;
}
void LinkedList::appead(eleType value)
{
	LinkNode* newNode = new LinkNode(value);
	newNode->next = NULL;
	if (size > 0)	getArr(size - 1)->next = newNode;
	else head = newNode;
	size++;
}

void LinkedList::ascInsert(eleType value)
{
	if (size == 0) insert(0, value);
	else
	{
		LinkNode* arr = head;
		for (int i = 0; i < size; i++)
		{
			if (value <= arr->data)
			{
				insert(i, value);
				return;
			}
			arr = arr->next;
		}
		insert(size, value);
	}
}

int main()
{
	LinkedList list;
	int n = 0;
	while (cin >> n && n)
	{
		list.appead(1);
		list.appead(1);
		list.appead(1);
		for (int i = 3; i <= 40; i++)
		{
			list.appead(list.getValue(i - 1) + list.getValue(i - 2));
		}
		//list.print();
		for (int i = 0; i < n; i++)
		{
			int temp = 0;
			cin >> temp;
			cout << list.getValue(temp) << endl;
		}
		list.clear();
	}


	return 0;
}