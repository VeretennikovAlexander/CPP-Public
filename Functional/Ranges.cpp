
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
 * (Ubuntu);
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
 * https://en.cppreference.com/w/cpp/ranges
 * https://en.cppreference.com/w/cpp/algorithm/ranges/sort
 * 
 **/
 
using namespace std;

void example1(vector<int>& numbers)
{
	

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
    
    printf("Example1: %s\n", res1.c_str());
    
    printf("for_each example:\n");
    
   	/*
	 * 	for_each example.
	 */            
    ranges::for_each(res, 
		[](double val){printf("%2.3f\n", val); } );

}

void example2(vector<int>& numbers)
{	

	/*
     * The same result.
     * filter, transform to double, convert multiset (for sorting), transform to vector of strings, join elements
     */
     
    auto res = numbers | views::filter([](int val){ return val % 2 == 0; }) 
		| views::transform([](int val) -> double { return (double)val; }) 
		| ranges::to<multiset>()
		| views::transform([](double i){return to_string(i);})
        | views::join_with(',')
        | ranges::to<string>();
        
    printf("Example2: %s\n", res.c_str());

}

void example3(vector<int>& numbers)
{

	/*
     * Sort with projection example.
    */
    auto res = numbers | views::filter([](int val){ return val % 2 == 0; }) 
    	| views::transform([](int val) -> pair<double,double> { return make_pair(0, val); }) 
    	| ranges::to<vector>();

    //Sort with projection (by a specified field of the structure).
    ranges::sort(res, {}, &pair<double,double>::second);	
	
    //Use this to sort in the reverse order.
    //ranges::sort(res3, ranges::greater(), &pair<double,double>::second);	
	
    auto res1 = res | views::transform([](pair<double,double> i){return to_string(i.second);})
        | views::join_with(',')
        | ranges::to<string>();
   
    printf("Example3: %s\n", res1.c_str());
}

int main(int argc, char** argv)
{
    vector<int> numbers = {1, 2, 3, 4, 5, 10, 12, 6, 4};
    example1(numbers);
    example2(numbers);
    example3(numbers);   
    
}


