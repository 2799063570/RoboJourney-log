#include <iostream>
using namespace std;

#define eleType int

struct SequentialList {

	eleType* elements;
	int size;
	int capacity;
};

void initializeList(SequentialList* list, int capacity)
{
	list->elements = new eleType[capacity];
	list->size = 0;
	list->capacity = capacity;
}

void destroyList(SequentialList* list)
{
	delete[] list->elements;
	list->elements = NULL;
}

int size(SequentialList* list)
{
	return list->size;
}
bool isEmpty(SequentialList* list)
{
	return list->size;
}

void insert(SequentialList* list, int index, eleType element)
{
	if (index > list->size || index < 0)			// 判断index是否合法
		throw std::invalid_argument("Invalid index");
	if (list->size == list->capacity)				// 扩容
	{
		int newCapacity = list->capacity * 2;
		eleType* newElements = new eleType[newCapacity];
		for (int i = 0; i < list->size; i++)
		{
			newElements[i] = (list->elements)[i];
		}
		delete[] list->elements;
		list->elements = newElements;
		newElements = NULL;
		list->capacity = newCapacity;
	}
	if (list->size < list->capacity)
	{
		for (int i = list->size; i > index; i--)
		{
			(list->elements)[i] = (list->elements)[i - 1];
		}
		list->elements[index] = element;// 编译器 不知道size和capacity之间的关系
		list->size++;
	}

}

void deleteElement(SequentialList* list, int index)
{
	if (index < 0 || index >= list->size)
	{
		throw std::invalid_argument("Index invalid");
	}
	for (int i = index; i < list->size - 1; i++)
	{
		list->elements[i] = list->elements[i + 1];
	}
	list->size--;
}

int findElement(SequentialList* list, eleType element)
{
	for (int i = 0; i < list->size; i++)
	{
		if (list->elements[i] == element)
			return i;
	}
	return -1;
}

eleType getElement(SequentialList* list, int index)
{
	if (index < 0 || index >= list->size) throw std::invalid_argument("Invalid index");
	return list->elements[index];
}

void updateElement(SequentialList* list, int index, eleType element)
{
	if (index < 0 || index >= list->size) throw std::invalid_argument("Invalid index");
	list->elements[index] = element;
}

void printSequentialList(SequentialList* list)
{
	// 遍历输出
	for (int i = 0; i < size(list); i++)
	{
		cout << getElement(list, i) << " ";
	}
	cout << endl;
}

int main()
{
	SequentialList myList;

	initializeList(&myList, 10);
	for (int i = 0; i < 10; i++)
	{
		insert(&myList, i, i * 10);
	}
	cout << "size: " << size(&myList) << endl;
	cout << "Is Empty: " << isEmpty(&myList) << endl;

	// 遍历输出
	printSequentialList(&myList);

	deleteElement(&myList, 2);
	cout << "After delete: ";
	printSequentialList(&myList);

	updateElement(&myList, 2, 999);
	cout << "After Update: ";
	printSequentialList(&myList);

	cout << "999 in " << findElement(&myList, 999) << endl;

	destroyList(&myList);

	return 0;
}