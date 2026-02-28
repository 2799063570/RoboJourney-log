#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;


int main()
{
	unordered_map<string, int> hash;
	hash["Aaaa"] = 6;		// hash≤Â»Î
	hash["xxx"] = 34;
	cout << hash["Aaaa"] << endl;
	hash["Aaaa"]++;			// ∏ƒ
	cout << hash["Aaaa"] << endl;
	if (hash.find("Aaaa") != hash.end())	// hash≤È’“	
		cout << "find" << endl;
	hash.erase("Aaaa");			// hash…æ≥˝
	string str = "jejeje";
	cout << sizeof(str[0]) << endl;

	unordered_set<int> u;
	u.insert(12);u.insert(12);
	cout << u.count(12) << endl;
	cout << u.count(12) << endl;
	return 0;
}
