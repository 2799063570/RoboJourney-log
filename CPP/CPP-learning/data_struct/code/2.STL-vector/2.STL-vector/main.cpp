#include <iostream>
#include <vector>
using namespace std;


int main()
{
	vector<int> ret = {23, 22, 11, 16};
	cout << ret.size() << endl;
	int originalSize = ret.size();
	for (int i = originalSize; i < originalSize + 3; i++)
	{
		ret.push_back(i + 1);
	}

	for (int i = 0; i < ret.size(); i++)
	{
		cout << ret[i] << " ";
	}
	cout << endl;


	return 0;
}