#include <iostream>

using namespace std;


template<typename T>
struct ListNode {

	T data;
	ListNode* next;
	ListNode(T d) : data(d), next(NULL) {}
};

template<typename T>
struct TreeNode {

	T data;
	ListNode<TreeNode<T>* >* childrenHead;
	TreeNode() : data(T()), childrenHead(NULL) {}		// 防止出现野指针
	//TreeNode(T d) : data(d), childrenHead(NULL) {}
	void addChild(TreeNode<T>* node)
	{
		ListNode<TreeNode<T>*>* newNode = new ListNode<TreeNode<T>*>(node);
		if (childrenHead == NULL) childrenHead = newNode;
		// 头插法
		else
		{
			newNode->next = childrenHead;
			childrenHead = newNode;
		}
	}
};

template <typename T>
class Tree
{
private:
	TreeNode<T>* nodes;
	TreeNode<T>* root;

public:
	Tree();
	Tree(int maxNodes);
	~Tree();
	TreeNode<T>* getTreeNode(int id);
	void setRoot(int rootId);
	void addChild(int parentId, int sonId);
	void assignDate(int nodeId, T data);
	void print(TreeNode<T>* node = NULL);

};

template<typename T>
Tree<T>::Tree()
{
	nodes = new TreeNode<T>[1001];
	root = NULL;
}
template<typename T>
Tree<T>::Tree(int maxNodes)
{
	nodes = new TreeNode<T>[maxNodes];
	root = NULL;
}
template<typename T>
Tree<T>::~Tree()
{
	delete[] nodes;
}
template<typename T>
TreeNode<T>* Tree<T>::getTreeNode(int id)
{
	return &nodes[id];
}
template<typename T>
void Tree<T>::setRoot(int rootId)
{
	root = getTreeNode(rootId);
}
template<typename T>
void Tree<T>::addChild(int parentId, int sonId)
{
	getTreeNode(parentId)->addChild(getTreeNode(sonId));
}
template<typename T>
void Tree<T>::assignDate(int nodeId, T data)
{
	getTreeNode(nodeId)->data = data;
}
template<typename T>
void Tree<T>::print(TreeNode<T>* node)
{
	if (node == NULL)
	{
		node = root;
	}
	cout << node->data << " ";
	ListNode<TreeNode<T>*>* curr = node->childrenHead;		// 子节点的链表头 链表存储的节点地址
	while (curr)
	{
		print(curr->data);			// 基于递归的思想 先逐步打印子节点 当子节点为空的时候 回溯节点 
		//cout << curr->data << " ";
		curr = curr->next;
	}
	cout << endl;

}


int main()
{
	Tree<char> T(9);
	T.setRoot(0);
	T.assignDate(0, 'a');
	T.assignDate(1, 'b');
	T.assignDate(2, 'c');
	T.assignDate(3, 'd');
	T.assignDate(4, 'e');
	T.assignDate(5, 'f');
	T.assignDate(6, 'g');
	T.assignDate(7, 'h');
	T.assignDate(8, 'i');
	T.addChild(0, 1);
	T.addChild(0, 2);
	T.addChild(1, 3);
	T.addChild(2, 4);
	T.addChild(2, 5);
	T.addChild(3, 6);
	T.addChild(3, 7);
	T.addChild(3, 8);
	
	T.print();

	return 0;
}