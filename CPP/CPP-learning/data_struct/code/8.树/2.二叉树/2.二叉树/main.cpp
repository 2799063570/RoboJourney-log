#include <iostream>
#include <queue>
using namespace std;

template <typename T>
struct TreeNode {

	T data;
	TreeNode* left;
	TreeNode* right;
	TreeNode() : data(0), left(NULL), right(NULL) {}
	TreeNode(T d) : data(d), left(NULL), right(NULL) {}

};

template<typename T>
class Tree
{
private:
	TreeNode<T>* nodes;
	TreeNode<T>* root;
	size_t nodeSize;

	TreeNode<T>* create(T a[], int size, int nodeID, T nullnode);
	void visit(TreeNode<T>* node);
	void preOrder(TreeNode<T>* node);
	void inOrder(TreeNode<T>* node);
	void postOrder(TreeNode<T>* node);
	void levelOrder(TreeNode<T>* node);

public:
	Tree();
	Tree(size_t num);
	~Tree();
	TreeNode<T>* getTreeNode(int nodeID);
	void assignVal(int nodeID, T val);
	//void addChild(int nodeID, int childID);
	void createTree(T a[], int size, T nullNode);
	void preOrderTraversal();
	void inOrderTraversal();
	void postOrderTraversal();
	void levelOrderTraversal();
	void setRoot(size_t id);
};

template<typename T>
Tree<T>::Tree()
{
	nodeSize = 10001;
	nodes = new TreeNode<T>[nodeSize];
	root = NULL;
}
template<typename T>
Tree<T>::Tree(size_t num)
{
	nodeSize = num;
	nodes = new TreeNode<T>[nodeSize];
	root = NULL;
}
template <typename T>
Tree<T>::~Tree()
{
	delete[] nodes;
}
template <typename T>
void Tree<T>::visit(TreeNode<T>* node)
{
	cout << node->data << endl; 
}
template<typename T>
void Tree<T>::preOrder(TreeNode<T>* node)
{
	if (node == NULL) return;
	cout << node->data << " ";
	preOrder(node->left);
	preOrder(node->right);
}
template <typename T>
void Tree<T>::inOrder(TreeNode<T>* node)
{
	if (node)
	{
		inOrder(node->left);
		cout << node->data << " ";
		inOrder(node->right);
	}
}
template <typename T>
void Tree<T>::postOrder(TreeNode<T>* node)
{
	if (node)
	{
		postOrder(node->left);
		postOrder(node->right);
		cout << node->data << " ";		
	}
}
template <typename T>
void Tree<T>::levelOrder(TreeNode<T>* node)
{
	if (!node) return;
	queue<TreeNode<T>* > q;
	q.emplace(node);
	while (!q.empty())
	{
		size_t size = q.size();
		for (int i = 0; i < size; i++)
		{
			TreeNode<T>* curr = q.front();
			q.pop();
			if (curr->left) q.emplace(curr->left);
			if (curr->right) q.emplace(curr->right);
			cout << curr->data << " ";
		}
	}
}
template <typename T>
TreeNode<T>* Tree<T>::create(T a[], int size, int nodeID, T nullnode)
{
	if (nodeID >= size || a[nodeID] == nullnode) return NULL;
	TreeNode<T>* nowNode = getTreeNode(nodeID);
	nowNode->data = a[nodeID];
	nowNode->left = create(a, size, nodeID * 2, nullnode);
	nowNode->right = create(a, size, nodeID * 2 + 1, nullnode);
	return nowNode;
}
template <typename T>
void Tree<T>::createTree(T a[], int size, T nullNode)
{
	root = create(a, size, 1, nullNode);
}
template <typename T>
TreeNode<T>* Tree<T>::getTreeNode(int nodeID)
{
	if (nodeID >= nodeSize) throw std::out_of_range("invaild index");
	return &nodes[nodeID];
}
template <typename T>
void Tree<T>::assignVal(int nodeID, T val)
{
	TreeNode<T>* node = getTreeNode(nodeID);
	if(node) node->data = val;
}
template <typename T>
void Tree<T>::preOrderTraversal()
{
	preOrder(root);
}
template <typename T>
void Tree<T>::inOrderTraversal()
{
	inOrder(root);
}
template <typename T>
void Tree<T>::postOrderTraversal()
{
	postOrder(root);
}
template <typename T>
void Tree<T>::levelOrderTraversal()
{
	levelOrder(root);
}
template <typename T>
void Tree<T>::setRoot(size_t id)
{
	if (id > nodeSize) throw std::out_of_range("invaild id");
	root = getTreeNode(id);
}

template <typename T>
void getSum(TreeNode<T>* root, int& sum, int bit)
{
	if (root == NULL) return;
	sum = sum * (int)pow(2, bit) + root->data;
	getSum(root->left, sum, bit + 1);
	getSum(root->right, sum, bit + 1);
}
template <typename T>
int sumRootToLeaf(TreeNode<T>* root) {
	int ret = 0;
	getSum(root, ret, 0);
	return ret;
}
int main()
{
	/*const char nullNode = '-';
	char a[15] = {
		nullNode, 'a', 'b', 'c', 'd',
		nullNode, 'e', 'f', 'g', 'h',
		nullNode, nullNode, nullNode, nullNode, 'i'
	};
	Tree<char> T(15);
	T.createTree(a, 15, nullNode);
	T.preOrderTraversal(); cout << endl;
	T.inOrderTraversal(); cout << endl;
	T.postOrderTraversal(); cout << endl;

	T.setRoot(2);
	T.assignVal(2, 'p');
	T.preOrderTraversal(); cout << endl;*/
	int a[8] = { 0, 1,0,1,0,1,0,1 };
	Tree<int> T(15);
	T.createTree(a, 8, '-');
	T.preOrderTraversal(); cout << endl;
	T.levelOrderTraversal();
	cout << sumRootToLeaf(T.getTreeNode(1));
	return 0;
}