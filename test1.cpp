#include <iostream>
#include <string>
using namespace std;

inline void Swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main(void)
{
   string str1 = "Hello";
   string str2 = "World";
   string str3;
   int  len;
   int len2 ;
   str3 = str1;
   cout << "str3 : " << str3 << endl;
   str3 = str1 + str2;
   cout << "str1 + str2 : " << str3 << endl;
   len = str3.size();
   len2 = str3.length();
   cout << "str3.size() :  " << len << "\t" << len2 << endl;

   int m=10,n=20;
   Swap(&m,&n);
   cout << m << "\t" << n <<endl;
	return 0;
}
