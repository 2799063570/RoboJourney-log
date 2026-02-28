#define _CRT_SECURE_NO_WARNINGS  // 禁用不安全函数警告
#include <iostream>
#include <cstring>
#include <string>
using namespace std;

class Mystring
{
private:
	char* str;
	size_t length;

public:
	Mystring();
	Mystring(const char* s);
	Mystring(const Mystring& s);
	~Mystring();
	size_t getLength() const;
	char operator[] (size_t index) const;
	Mystring& operator=(const Mystring& s);
	bool operator==(const Mystring& s) const;
	bool operator!=(const Mystring& s) const;
	Mystring copy() const;
	Mystring operator+(const Mystring& s);
	friend ostream& operator<<(ostream& out, const Mystring& s);
};

Mystring::Mystring()
{
	str = new char[1];
	*str = '\0';
	length = 0;
}
Mystring::Mystring(const char* s)
{
	length = strlen(s);
	str = new char[length + 1];
	strcpy(str, s);
}
Mystring::Mystring(const Mystring& s)
{
	str = new char[s.getLength() + 1];
	strcpy(str, s.str);
	length = s.getLength();
}

Mystring::~Mystring()
{
	delete[] str;
}

size_t Mystring::getLength() const
{
	return length;
}

char Mystring::operator[] (size_t index) const
{
	if (index >= length) throw std::out_of_range("Invalid Index");
	return str[index];
}

Mystring& Mystring::operator=(const Mystring& s)
{
	if (*this != s)
	{
		length = s.getLength();
		delete[] str;
		str = new char[length + 1];
		strcpy(str, s.str);
	}
	return *this;
}

bool Mystring::operator==(const Mystring& s) const
{
	if (length != s.getLength()) return false;
	for (int i = 0; i < length; i++)
	{
		if (str[i] != s[i]) return false;
	}
	return true;
}

bool Mystring::operator!=(const Mystring& s) const
{
	if (length != s.getLength()) return true;
	for (int i = 0; i < length; i++)
	{
		if (str[i] != s[i]) return true;
	}
	return false;
}

Mystring Mystring::copy() const
{
	/*Mystring s = *this;
	return s;*/
	return *this;	
}

Mystring Mystring::operator+(const Mystring& s)
{
	char* new_data = new char[length + s.getLength()];
	strcpy(new_data, str);
	strcpy(new_data + length, s.str);
	Mystring nstr(new_data);
	return nstr;
}

ostream& operator<<(ostream& out, const Mystring& s)
{
	out << s.str;
	return out;
}

int main()
{
	/*Mystring m("12212");
	cout << m << endl;
	Mystring m1("23123");
	cout << (m + m1) << endl;

	Mystring s = m + m1;
	cout << s << endl;

	cout << (s == (m + m1)) << endl;

	Mystring n = "haha";
	cout << n << endl;

	cout << (s == "(m + m1)") << endl;

	cout << s.copy() << endl;

	Mystring total;
	string a;
	cin >> a;*/
	string s;
	while (cin >> s)
	{
		string ret = "";
		char maxc = 'a';
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] > maxc) maxc = s[i];
		}
		for (int i = 0; i < s.size(); i++)
		{
			ret = ret + s[i];
			if (s[i] == maxc) ret = ret + "(max)";
		}
		cout << ret << endl;
	}


	/*char s[500];
	while (gets_s(s))
	{
		
		int len = strlen(s);
		int cnt = 0;
		bool flag = false;
		for (int i = 0; i < len; i++)
		{
			
			if (flag || i == 0)
			{
				if(s[i] >= 'a' || s[i] <= 'z') s[i] = s[i] - ('a' - 'A');
				flag = false;
			}
			if (s[i] == ' ') flag = true;
		}
		printf("%s\n", s);

	}*/

	return 0;
}
