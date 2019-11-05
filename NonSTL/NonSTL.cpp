// NonSTL.cpp : Driver for testing the NonSTL implementations of data structures
//

#include "vector.h"

#include <iostream>

int main()
{
	non_stl::vector<int> vec = { 0,1,2 };
	vec.assign({ 6,7,8,9 });
	non_stl::vector<int> vec2(vec.begin(), vec.end());
	std::cout << vec2[2] << std::endl;
	return 0;
}
