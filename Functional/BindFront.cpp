
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <ranges>
#include <set>
#include <functional>
#include <iostream>

/*
 * gcc, ubuntu.
 * 
 * Example output:
 * 
 * 1010
 * 1010
 * 1010
 * 
 * https://en.cppreference.com/w/cpp/utility/functional/bind_front
 * 
 **/

using namespace std;

int sum(int a, int b)
{
    return a + b;
}

int main(int argc, char** argv)
{
    auto add1000 = bind_front(sum, 1000);
    
    auto add1000lambda = bind_front([](int a, int b){
		return a+b; 
		}, 1000);
		
	auto add1000std = bind_front(plus<int>(), 1000);
    
    cout << add1000(10) << endl;
    cout << add1000lambda(10) << endl;
    cout << add1000std(10) << endl;

	return 0;
}



