#include <iostream>
#include <vector>
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

int findElement(SequentialList* list, eleType element)
{
	for (int i = 0; i < list->size; i++)
	{
		eleType val = getElement(list, i);
		if (val == element) return i;
	}
	return -1;
}


int main()
{
	vector<int> nums = {1, 4, 4};
	int cnt = 0;
	int num[50];

	for (int i = 0; i < nums.size() - 1; i++)
	{
		for (int j = i + 1; j < nums.size(); j++)
		{
			if (nums[i] == nums[j])
			{
				num[cnt] = nums[i];
				cnt++;
			}
		}
	}
	if (cnt == 0) return 0;
	int ret = num[0];
	for (int i = 1; i < cnt; i++)
	{
		ret = ret | num[i];
	}
	cout << ret << endl; 
	cout << cnt << endl;
	cout << (10 ^ 18) << endl;

	return 0;
}