#include <iostream>
#include <queue>
#include <string>
#include <cstring>

using namespace std;

int team[999999 + 1];
int main()
{
	int t = 0;
	int epcho = 1;
	
	while (cin >> t && t)
	{
		cout << "Scenario #" << epcho << endl; 
		epcho++;
		memset(team, 0, sizeof(team));
		int n = 0;
		for(int i = 1; i<=t; i++)
		{
			cin >> n;
			int num;
			while (n--)
			{
				cin >> num;
				team[num] = i;
			}
		}
		queue<int> q[1001];
		int r = 0, l = 0;
		string str;
		while (cin >> str)
		{
			if (str == "STOP") break;
			else if (str == "ENQUEUE")
			{
				int c_num;
				cin >> c_num;
				int i = r;
				for (; i < l; i++)
				{
					if (team[q[i].front()] == team[c_num]) break;
				}
				q[i].push(c_num);
				if (i == l) {
					l++;
					//team[c_num] = i;
				}
			}
			else {
				while (1)
				{
					if (q[r].size()) 
					{
						int val = q[r].front();
						q[r].pop();
						cout << val << endl;
						break;
					}								
					else r++;
				}
				
	
				

			}
		}
		cout << endl;
	}


	return 0;
}