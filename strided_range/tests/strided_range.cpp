/* Test file for strided_range task.
 * @file
 * @date 2019-08-29
 * @author Anonymous
 */

#include <boost/range/iterator_range.hpp>
#include <vector>

#include <catch2/catch.hpp>

#include <strided_range.hpp>

namespace
{
	std::vector<int> src{ 2, 4, 6, 8, 10, 12, 14, 16, 18, 20};

	template<class T>
	void check(T out, std::vector<int> ref)
	{
		REQUIRE(out == ref);
	}
}

// Two checks with different strides.
TEST_CASE("strided_range::stride")
{
	size_t width = 5;

	SECTION("width < stride")
	{
		auto out = make_strided_range(src, width, 6);
		check(out, { 2, 4, 6, 8, 10, 14, 16, 18, 20 });
	}

	SECTION("width > stride")
	{
		auto out = make_strided_range(src, width, 4);
		check(out, src);
	}
}

// Two checks with different widths.
TEST_CASE("strided_range::width")
{
	size_t stride = 5;

	SECTION("width < stride")
	{
		auto out = make_strided_range(src, 4, stride);
		check(out, { 2, 4, 6, 8, 12, 14, 16, 18 });
	}

	SECTION("width > stride")
	{
		auto out = make_strided_range(src, 6, stride);
		check(out, src);
	}
}

TEST_CASE("strided_range::stride_equal_width")
{
	SECTION("width == stride")
	{
		auto out = make_strided_range(src, 5, 5);
		check(out, src);
	}
}

// Use boost::size.
TEST_CASE("strided_range::distance")
{
	SECTION("Different sizes")
	{
		auto out1 = make_strided_range(src, 4, 4);
		REQUIRE(10 == boost::size(out1));
		auto out2 = make_strided_range(src, 4, 5);
		REQUIRE(8 == boost::size(out2));
	}
}

// Make strided_range from another strided_range.
TEST_CASE("strided_range::from_strided")
{
	SECTION("From strided range")
	{
		auto out1 = make_strided_range(src, 4, 5);
		auto out2 = make_strided_range(out1, 3, 4);
		check(out2, { 2, 4, 6, 12, 14, 16 });
	}
}

// Provide example with shifted data. It's like a window in the image. Hint: boost::make_iterator_range.
TEST_CASE("strided_range::shift")
{
	auto window = [](auto& x, size_t stride, size_t x0, size_t y0, size_t x1, size_t y1)
	{
		auto tmp = boost::make_iterator_range(x.begin() + y0 * stride + x0, x.begin() + (y1 - 1)*stride + x1);
		return make_strided_range(tmp, x1 - x0, stride);
	};

	SECTION("2*2 from 2*5")
	{
		check(window(src, 2, 0, 2, 2, 4), { 10, 12, 14, 16 });
	}

	SECTION("2*2 from 5*2")
	{
		check(window(src, 5, 2, 0, 4, 2), { 6, 8, 16, 18 });
	}
}

TEST_CASE("strided_range::corner_case1")
{
	SECTION("width == 0")
	{
		auto out = make_strided_range(src, 0, 5);
		check(out, {});
	}
}

TEST_CASE("strided_range::corner_case2")
{
	SECTION("stride == 0")
	{
		//make_strided_range(src, 5, 0); насколько понял при делении на ноль в подобных случаях возникают аппаратные исключения, которые не отлавливаются стандратными средствами отлова программных илюченй
	}

	SECTION("src.size() << width, stride")
	{
		auto out = make_strided_range(src, 100, 100);
		check(out, src);
	}
}
