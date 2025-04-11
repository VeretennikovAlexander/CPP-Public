
/**
 * Examples:
 * 
 * sort, for_each, copy, stringstream
 * 
 * decltype позволяет взять тип у переменной.
 * 
 */

#include <vector>
#include <initializer_list>
#include <algorithm>

using namespace std;

#include <string>
#include <sstream>
#include <iterator>
#include <iostream>

template <class V>
string ToString(const V& v)
{
    stringstream ss;
    copy(v.begin(), v.end(), ostream_iterator<typename V::value_type>(ss, ", "));
    
    string result = ss.str();
    char* Str = const_cast<char*>(result.c_str());
    result.size() > 2 ? Str[result.size() - 2] = 0 : 0;
    return result;
};

template <class T>
bool Compare(T a, T b)
{
	return a < b;
}

int main(int argc, const char** args)
{
 vector<float> Data({0,5,1,7,2});

 /*
  * Сортировка с помощью лямбда функции.
  */ 
 sort(Data.begin(), Data.end(), [](const decltype(Data)::value_type a, const decltype(Data)::value_type b) {
    return a < b;
 });
 
 printf("%s\n", ToString(Data).c_str());

 for_each(Data.begin(), Data.end(), [](decltype(Data)::value_type Val){
	 cout << Val << endl;
	 });
	 
	 
	 
 /*
  * Сортировка с помощью вспомогательного класса-функции.
  */	  
	 
	 
 struct CSortOperator
 {
	 bool operator()(const decltype(Data)::value_type a, const decltype(Data)::value_type b)
	 {
		 return a < b;
	 }
 };
 	 
 sort(Data.begin(), Data.end(), CSortOperator());
 
 /*
  * Сортировка с помощью обычной функции.
  */ 
 sort(Data.begin(), Data.end(), Compare<decltype(Data)::value_type>); 
	 
	 
 return 0;
}
