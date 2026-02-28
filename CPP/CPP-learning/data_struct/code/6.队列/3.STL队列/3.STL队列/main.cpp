#include <iostream>
#include <queue>
using namespace std;

/*
	push
	pop
	empty
	front
*/

int main()
{
	/*queue<int> q;
	q.push(12);
	q.push(22);
	q.push(312);
	q.push(452);
	while (!q.empty())
	{
		cout << q.front() << endl;
		q.pop();
	}*/

	queue<int> q1, q2;
	int n = 0;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		int num = 0;
		cin >> num;
		for (int i = 1; i < num + 1; i++) q1.push(i);
		while (1)
		{
			int cnt = 1;
			if (q1.size() <= 3)
			{
				while(!q1.empty())
				{ 
					cout << q1.front() << " ";
					q1.pop();
				}
				cout << endl;
				break;
			}
			else {
				while (q1.size())
				{
					if (cnt % 2 == 1)
						q2.push(q1.front());
					q1.pop();
					++cnt;
				}
			}
			cnt = 1;
			if (q2.size() <= 3)
			{
				while (!q2.empty())
				{
					cout << q2.front() << " ";
					q2.pop();
				}
				cout << endl;
				break;
			}
			else
			{
				while (!q2.empty())
				{
					if (cnt % 3 == 1 || cnt%3 == 2)
						q1.push(q2.front());
					q2.pop();
					++cnt;
				}
			}
		}

	}

	return 0;
}