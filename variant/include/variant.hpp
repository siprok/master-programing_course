/* Variant classes.
 * @file
 * @date 2018-08-12
 * @author Anonymous
 */

#ifndef __VARIANT_HPP__
#define __VARIANT_HPP__

#include <map>
#include <memory>
#include <variant>
#include <vector>

#include <boost/variant.hpp>

//{ number
using number = std::variant<int, float>;
//}

//{ array
using array = std::vector<number>;
//}

//{ recursive_array
struct recursive_array : public std::vector<std::variant<number, std::shared_ptr<recursive_array>>>
{
	using std::vector<std::variant<number, std::shared_ptr<recursive_array>>>::vector;
};
//}

//{ recursive_array2
struct recursive_array2 : public std::vector<std::variant<number, boost::recursive_wrapper<recursive_array2>>>
{
	recursive_array2(std::initializer_list<std::variant<number, boost::recursive_wrapper<recursive_array2>>> init)
	{
		for (const std::variant<number, boost::recursive_wrapper<recursive_array2>>& elem : init)
			this->push_back(elem);
	}
};
//}

//{ variant_decorator
template<typename ... Types>
struct variant_decorator : public std::variant<Types...>
{
	template<class T>
	variant_decorator(T value) : std::variant<Types...>(value) {}
	variant_decorator() : std::variant<Types...>() {}
	template<typename T>
	T& as()
	{
		if constexpr (std::disjunction<std::is_same<Types, T>...>::value)
			return std::get<T>(*this);
		else
			return std::get<boost::recursive_wrapper<T>>(*this).get();
	}

	template<typename T>
	T as() const
	{
		return std::get<T>(*this);
	}
};

//}

//{ recursive_map

struct recursive_map : public std::map<std::string, variant_decorator<int, std::string, bool, boost::recursive_wrapper<recursive_map>>>
{
	recursive_map(std::initializer_list<value_type> init) : std::map<std::string, variant_decorator<int, std::string, bool, boost::recursive_wrapper<recursive_map>>>(init) {}
};

//}

#endif 
// __VARIANT_HPP__
