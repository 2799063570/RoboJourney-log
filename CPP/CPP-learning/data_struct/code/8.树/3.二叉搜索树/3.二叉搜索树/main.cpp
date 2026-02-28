#include <iostream>

using namespace std;

template <typename T>
struct TreeNode {

	T data;
	TreeNode<T>* left;
	TreeNode<T>* right;
	TreeNode() : data(0), left(NULL), right(NULL) {}
	TreeNode(T d) : data(d), left(NULL), right(NULL) {}
};

template<typename T>
class BinarySearchTree {
private:
	TreeNode<T>* root;
	TreeNode<T>* insertNode(TreeNode<T>* node, T element);
	TreeNode<T>* removeNode(TreeNode<T>* node, T element);
	bool searchNode(TreeNode<T>* node, T element);
	void inOrder(TreeNode<T>* node);

public:
	BinarySearchTree() : root(NULL) {}
	~BinarySearchTree();
	void insert(T element)
	{
		root = insertNode(root, element);
	}
	void remove(T element)
	{
		root = removeNode(root, element);
	}
	bool search(T element)
	{
		return searchNode(root, element);
	}
	void inOrderTraversal()
	{
		inOrder(root);
		cout << endl;
	}

};
template <typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
	while (root)
	{
		remove(root->data);
	}
}
template <typename T>
TreeNode<T>* BinarySearchTree<T>::insertNode(TreeNode<T>* node, T element)
{
	if (node == NULL) node = new TreeNode<T>(element);
	else
	{
		if (element < node->data)
		{
			node->left = insertNode(node->left, element);
		}
		else if (element > node->data)
		{
			node->right = insertNode(node->right, element);
		}
	}	
	return node;
}
template <typename T>
TreeNode<T>* BinarySearchTree<T>::removeNode(TreeNode<T>* node, T element)
{
	if (node == NULL) return NULL;
	if (node->data > element) node->left = removeNode(node->left, element);
	else if (node->data < element) node->right = removeNode(node->right, element);
	else
	{
		if (node->left && node->right == NULL)
		{
			TreeNode<T>* temp = node;
			node = node->left;
			delete temp;
		}
		else if (node->left == NULL && node->right)
		{
			TreeNode<T>* temp = node;
			node = node->right;
			delete temp;
		}
		else if (node->left && node->right)
		{
			TreeNode<T>* curr = node->right;
			while (curr->left)
			{
				curr = curr->left;
			}
			node->data = curr->data;
			//delete curr;  // 没有处理上一层根节点的指向问题  容易导致内存泄漏
			node->right = removeNode(node->right, curr->data);
		}
		else
		{
			delete node;
			return NULL;
		}
		
	}
	return node;
}
template <typename T>
bool BinarySearchTree<T>::searchNode(TreeNode<T>* node, T element)
{
	if (node == NULL) return false;
	if (node->data == element) return true;
	else if (node->data > element) return searchNode(node->left, element);
	else return searchNode(node->right, element);
}
template <typename T>
void BinarySearchTree<T>::inOrder(TreeNode<T>* node)
{
	if (node == NULL) return;
	inOrder(node->left);
	cout << node->data << " ";
	inOrder(node->right);
}

int main()
{
	BinarySearchTree<int> t;
	t.insert(1);
	t.insert(2);
	t.inOrderTraversal();
	t.insert(4);
	t.insert(3);
	cout << t.search(1) << endl;
	cout << t.search(100) << endl;
	t.inOrderTraversal();
	t.remove(3);
	t.inOrderTraversal();
	t.remove(1);
	t.inOrderTraversal();
	return 0;
}