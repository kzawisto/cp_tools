


#include<gtest/gtest.h>
#include<cp_tools/numeric_utility.h>


using namespace cp_tools;
TEST(test, combined_variance) {
	std::vector<double> vec,vec2,vec3;
	srand(1);
	for(unsigned i =0;i < 30; i++) {
		vec3.push_back((rand() % 10000) / 10000.0);
	}
	for(unsigned i =0;i < 20; i++) {
		vec2.push_back((rand() % 10000) / 10000.0);
	}
	vec.insert(vec.end(), vec3.begin(), vec3.end());
	vec.insert(vec.end(), vec2.begin(), vec2.end());
	auto mv2 =  get_mean_and_variance(vec2.begin(), vec2.end());
	auto mv3 =  get_mean_and_variance(vec3.begin(), vec3.end());
	auto mv = combine_variance(mv2, mv3);
	auto mv_actual = get_mean_and_variance(vec.begin(), vec.end());
	ASSERT_NEAR(mv.mean, mv_actual.mean, 1e-7);
	ASSERT_NEAR(mv.variance, mv_actual.variance, 1e-7);
};


