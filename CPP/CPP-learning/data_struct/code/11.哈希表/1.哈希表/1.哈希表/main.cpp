#include <iostream>

using namespace std;

// 哈希表实现 哈希函数采用取模 冲突采用链地址法
template <typename keyType, typename valueType>
class HashNode {
public:
	keyType key;
	valueType value;
	HashNode* next;

	HashNode(const keyType& key, const valueType& val)
	{
		this->key = key;
		this->value = val;
		next = NULL;
	}
};
template <typename keyType, typename valueType>
class HashTable {
private:
	int size;
	HashNode<keyType, valueType>** table;

	int hash(const keyType& key) const
	{
		int hashkey = key % size;
		if (hashkey < 0) hashkey += size;
		return hashkey;
	}
public:
	HashTable(int size = 256);
	~HashTable();
	void insert(const keyType& key, const valueType& val);
	void remove(const keyType& key);
	bool find(const keyType& key, valueType& val);
};
template <typename keyType, typename valueType>
HashTable<keyType, valueType>::HashTable(int size)
{
	this->size = size;
	this->table = new HashNode<keyType, valueType>* [size];
	for (int i = 0; i < size; i++) this->table[i] = nullptr;
}
template <typename keyType, typename valueType>
HashTable<keyType, valueType>::~HashTable()
{
	for (int i = 0; i < size; i++)
	{
		HashNode<keyType, valueType>* curr = table[i];
		while (curr)
		{
			HashNode<keyType, valueType>* temp = curr->next;
			delete curr;
			curr = temp;
		}
	}
	delete[] table;
}
template <typename keyType, typename valueType>
void HashTable<keyType, valueType>::insert(const keyType& key, const valueType& val)
{
	int hashKey = hash(key);
	HashNode<keyType, valueType>* newNode = new HashNode<keyType, valueType>(key, val);
	if (table[hashKey] == nullptr) table[hashKey] = newNode;
	else
	{
		newNode->next = table[hashKey];
		table[hashKey] = newNode;
	}
}
template <typename keyType, typename valueType>
void HashTable<keyType, valueType>::remove(const keyType& key)
{
	int hashKey = hash(key);
	if (table[hashKey] == NULL) return;
	else {
		if (table[hashKey]->key == key)
		{
			HashTable<keyType, valueType>* next = table[hashKey]->next;
			delete table[hashKey];
			table[hashKey] = next;
		}
		else {
			HashTable<keyType, valueType>* curr = table[hashKey];
			while (curr->next && curr->next->key != key)
			{
				curr = curr->next;
			}
			if (curr->next)
			{
				HashTable<keyType, valueType>* next = curr->next->next;
				delete curr->next;
				curr->next = next;
			}
		}
	}
}
template <typename keyType, typename valueType>
bool HashTable<keyType, valueType>::find(const keyType& key, valueType& val)
{
	int hashKey = hash(key);
	if (table[hashKey] == NULL) return false;
	else {
		if (table[hashKey]->key == key)
		{
			val = table[hashKey]->value;
			return true;
		}
		else
		{
			HashNode<keyType, valueType>* curr = table[hashKey];
			while (curr->next && curr->next->key != key)
			{
				curr = curr->next;
			}
			if (curr->next)
			{
				val = curr->next->value;
				return true;
			}
		}
		return false;
	}
}


template <typename keyType>
class HashCounter {
	int counterSize;
	int counterIndex;
	int* counter;
	HashTable<keyType, int>* hash;
public:
	HashCounter(int size = 256);
	~HashCounter();
	void reset();
	int add(keyType key);
	int sub(keyType key);
	int get(keyType key);
};
template <typename keyType>
HashCounter<keyType>::HashCounter(int size)
{
	counterSize = size;
	counterIndex = 0;
	counter = new int[counterSize];
	reset();
}
template <typename keyType>
HashCounter<keyType>::~HashCounter()
{
	delete[] counter;
	if (hash)
	{
		delete hash;
		hash = nullptr;
	}
}
template <typename keyType>
void HashCounter<keyType>::reset()
{
	if (hash)
	{
		delete hash;
		hash = nullptr;
	}
	hash = new HashTable<keyType, int>(counterSize);
	counterIndex = 0;
	for (int i = 0; i < counterSize; i++)
		counter[i] = 0;
}
template <typename keyType>
int HashCounter<keyType>::add(keyType key)
{
	int index;
	if (!hash->find(key, index))
	{
		index = counterIndex++;
		hash->insert(key, index);
	}
	return ++counter[index];
}
template <typename keyType>
int HashCounter<keyType>::sub(keyType key)
{
	int index;
	if (hash->find(key, index))
	{
		return --counter[index];
	}
	return 0;
}
template <typename keyType>
int HashCounter<keyType>::get(keyType key)
{
	int index;
	if (hash->find(key, index))
	{
		return counter[index];
	}
	return 0;

}

int main()
{
	HashTable<int, char> h(10000);
	h.insert(1, 'a');
	h.insert(2, 'b');
	h.insert(3, 'c');
	h.insert(41012012, 'd');

	char val;
	if (!h.find(43, val)) {
		cout << "43 not found!" << endl;
	}
	if (h.find(41012012, val)) {
		cout << "41012012 found, value is " << val << endl;
	}

	HashCounter<int> qw(1000);
	qw.add(10);
	qw.add(10);
	qw.add(10);
	qw.sub(10);
	qw.add(10);
	cout << qw.get(10) << endl;

	return 0;
}