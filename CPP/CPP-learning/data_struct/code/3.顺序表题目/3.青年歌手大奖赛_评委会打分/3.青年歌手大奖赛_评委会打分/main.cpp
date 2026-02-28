#include <iostream>
using namespace std;

#define eleType double


struct SequentialList {

	eleType* element;
	int size;
	int capacity;
};

void initializeList(SequentialList* list, int size)
{
	list->element = new eleType[size]();
	list->size = 0;
	list->capacity = size;
}

void deleteList(SequentialList* list)
{
	delete[] list->element;
	list->element = NULL;
}
bool isEmpty(SequentialList* list)
{
	return list->size == 0;
}
int size(SequentialList* list)
{
	return list->size;
}

void insert(SequentialList* list, int index, eleType element)
{
	if (index <  0 || index > list->size) throw std::invalid_argument("Invalid index");
	if (list->size == list->capacity)
	{
		int newCapacity = list->capacity * 2;
		eleType* newElement = new eleType[newCapacity]();
		for (int i = 0; i < newCapacity / 2; i++)
		{
			newElement[i] = list->element[i];
		}
		delete[] list->element;
		list->element = newElement;
		list->capacity = newCapacity;
	}
	list->size++;
	//cout << list->size << endl;
	if (list->size <= list->capacity)
	{
		for (int i = list->size - 1; i > index; i--)
		{
			list->element[i] = list->element[i - 1];
		}
		list->element[index] = element;
		//list->size++;
	}
}

void deleteList(SequentialList* list, int index)
{
	if (index < 0 || index >= list->size) throw std::invalid_argument("Invalid index");
	for (int i = index; i < list->size - 1; i++)
	{
		list->element[i] = list->element[i + 1];
	}
	list->size--;
}

void updataList(SequentialList* list, int index, eleType element)
{
	if (index < 0 || index >= list->size) throw std::invalid_argument("Invalid index");
	list->element[index] = element;
}

eleType getElement(SequentialList* list, int index)
{
	if (index < 0 || index >= list->size) throw std::invalid_argument("Invalid index");
	return list->element[index];
}


int main()
{
	int n;
	while (cin >> n)
	{
		SequentialList mylist;
		double min = 1000000, max = -100000, sum = 0;
		initializeList(&mylist, n);
		for (int i = 0; i < n; i++)
		{
			double num;
			cin >> num;
			insert(&mylist, i, num);
		}
		for (int i = 0; i < mylist.size; i++)
		{
			double val = getElement(&mylist, i);
			if (min > val)	min = val;
			if (max < val)	max = val;
			sum += val;
		}
		sum = sum - min - max;
		sum /= (n - 2);
		printf("%.2f\n", sum);

	}
	return 0;
}