
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <ranges>
#include <set>

/*
 * Requires gcc 14, C++ 23.
 * 
 * Example command line:
 * g++-14 -std=c++23 -Wall -o "Ranges" "Ranges.cpp"
 * (Ubunty);
 * 
 * Note:
 * https://dev.to/marcosplusplus/how-to-install-gcc-14-and-use-c23-41od
 * 
 * keywords:
 * 
 * C++ ranges
 *
 * Example book:
 * 
 * https://www.amazon.com/C-20-Details-Rainer-Grimm/dp/B09328NKXK
 * Rainer Grimm, C++20: Get the Details, 
 * Section: The Ranges Library
 * 
 * 
 * https://en.cppreference.com/w/cpp/algorithm/ranges/sort
 * 
 **/
 
using namespace std;

int main(int argc, char** argv)
{
    vector<int> numbers = {1, 2, 3, 4, 5, 10, 12, 6, 4};

	/*
     * filter, transform to double, sort (using ranges::sort), transform to vector of strings, join elements
     */
     
    auto res = numbers | views::filter([](int val){ return val % 2 == 0; }) 
		| views::transform([](int val) -> double { return (double)val; }) 
		| ranges::to<vector>();
		
    ranges::sort(res);
    
    auto res1 = res | views::transform([](double i){return to_string(i);})
        | views::join_with(',')
        | ranges::to<string>();
    
    printf("%s\n", res1.c_str());
    
    /*
     * The same result.
     * filter, transform to double, convert multiset (for sorting), transform to vector of strings, join elements
     */
     
    auto res2 = numbers | views::filter([](int val){ return val % 2 == 0; }) 
		| views::transform([](int val) -> double { return (double)val; }) 
		| ranges::to<multiset>()
		| views::transform([](double i){return to_string(i);})
        | views::join_with(',')
        | ranges::to<string>();
        
    printf("%s\n", res2.c_str());

	/*
	 * 	for_each example.
	 */            
    ranges::for_each(res, 
		[](double val){printf("%2.3f\n", val); } );
		

    /*
     * Sort with projection example.
    */
    auto res3 = numbers | views::filter([](int val){ return val % 2 == 0; }) 
    	| views::transform([](int val) -> pair<double,double> { return make_pair(0, val); }) 
    	| ranges::to<vector>();

    //Sort with projection (by a specified field of the structure).
    ranges::sort(res3, {}, &pair<double,double>::second);	
	
    //Use this to sort in the reverse order.
    //ranges::sort(res3, ranges::greater(), &pair<double,double>::second);	
	
    auto res4 = res3 | views::transform([](pair<double,double> i){return to_string(i.second);})
        | views::join_with(',')
        | ranges::to<string>();
   
    printf("%s\n", res4.c_str());
    
}


