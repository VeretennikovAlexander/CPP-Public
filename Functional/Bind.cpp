
#include <algorithm>
#include <functional>
#include <iostream>

/*
 * gcc, ubuntu.
 * 
 * Example output:
 * 
 * 10
 * 15
 * 
 * https://www.geeksforgeeks.org/bind-function-placeholders-c/
 * 
 * https://en.cppreference.com/w/cpp/utility/functional/bind
 * 
 * See also: BindFront.cpp
 * 
 * bind_front and bind_back are simple versions of bind.
 * 
 **/

using namespace std;

int myminmax(int val, int minValue, int maxValue)
{
    val = max(val, minValue);
    val = min(val, maxValue);
    return val;
}

int main(int argc, char** argv)
{
    using namespace std::placeholders;  // for _1, _2, _3...
	
    /*
    * Первый аргумент так и останется аргументом, второй и третий фиксируем.
    */
    auto min10max15 = bind(myminmax, _1, 10, 15);
   
    cout << min10max15(2) << endl;
    cout << min10max15(20) << endl;

    return 0;
}



