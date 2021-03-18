// NonSTL.cpp : Driver for testing the NonSTL implementations of data structures
//

#include "vector.h"

#include <iostream>

int main()
{
	non_stl::vector<int> vec = { 0,1,2 };
	vec.assign({ 6,7,8,9 });

	const non_stl::vector<int> vec2(vec.begin(), vec.end());
	non_stl::vector<int> vec3(4);
	vec3.assign(vec2.begin(), vec2.end());
	auto it = vec3.end();
	--it;
	vec3.insert(it, 3);
	vec3.emplace_back(69);
	vec3.push_back(70);
	auto x = 17;
	vec3.push_back(x);
	std::cout << vec2[2] << std::endl;
	std::cout << vec3[0] << ", " << vec3[1] << ", " << vec3[2] << ", " << vec3[3] << ", " << vec3[4] << ", " << vec3[5] << ", " << vec3[6] << ", " << vec3[7] << std::endl;
	return 0;
}
