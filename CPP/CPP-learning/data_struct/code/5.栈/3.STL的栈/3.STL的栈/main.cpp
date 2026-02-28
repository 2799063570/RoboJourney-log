#include <iostream>
#include <stack>
using namespace std;

/*
	empty
	push
	pop 
	top
*/

int main()
{


	//int n = 0, m = 0;
	//stack<int> st_i;
	//while (cin >> n >> m)
	//{		
	//	int num = (n < 0) ? -n : n;
	//	while (num)
	//	{
	//		st_i.push(num%m);
	//		num /= m;			
	//	}
	//	if (n < 0) cout << "-";
	//	while (!st_i.empty())
	//	{
	//		int id = st_i.top();
	//		if (id < 10) cout << st_i.top();
	//		else {
	//			char a = 65 + id - 10;
	//			cout << a;
	//		}			
	//		st_i.pop();
	//	}
	//	cout << endl;

	//}
	int n = 0;
	while (cin >> n)
	{
		stack<int> st;
		while (n)
		{
			st.push(n % 2);
			n /= 2;
		}
		while (!st.empty())
		{
			cout << st.top();
			st.pop();
		}
		cout << endl;
	}


	return 0;
}