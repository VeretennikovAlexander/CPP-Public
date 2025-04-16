
#include <vector>
#include <list>
#include <initializer_list>
#include <algorithm>
#include <iostream>

using namespace std;

int main(int argc, const char** args)
{
  vector<float> Data({0,5,1,7,2});

  list<float> List;
  
  for_each(begin(Data), end(Data), [&List](float Val){
	List.push_back(Val);
	});
	 
  for_each(begin(Data), end(Data), [&](float Val){
	List.push_back(Val);
	});

  for_each(begin(List), end(List), [](float Val){
	cout << Val;
	cout << endl;
	});

  return 0;
}

