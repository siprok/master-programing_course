/* Image iterator.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */

#ifndef __ITERATOR_HPP__
#define __ITERATOR_HPP__

#include <algorithm>
#include <type_traits>
#include <vector>
#include <utility>
#include <iterator>

#include <boost/iterator/iterator_adaptor.hpp>

template<class Iterator>
class image_iterator : public boost::iterator_adaptor<image_iterator<Iterator>, Iterator, boost::use_default, boost::random_access_traversal_tag>
{
public:
	image_iterator() : image_iterator::iterator_adaptor_(0) {}

	explicit image_iterator(Iterator p) : image_iterator::iterator_adaptor_(p), init_it(p), width(0), stride(0), delta(stride - width) {}

	explicit image_iterator(Iterator p, size_t width_, size_t stride_) : image_iterator::iterator_adaptor_(p), init_it(p), width(width_), stride(stride_), delta(stride_ - width_) {}
	
private:
	friend class boost::iterator_core_access;
	Iterator init_it;
	size_t width;
	size_t stride;
	size_t delta;

	auto& dereference() const
	{
		return *this->base_reference();
	}

	bool equal(image_iterator const& other) const
	{
		return this->base_reference() == other.base_reference();
	}

	void increment()
	{
		this->base_reference()++;
		bool condition_1 = (std::distance(this->base(), init_it) % stride == delta) & (std::distance(this->base(), init_it) > 0);
		bool condition_2 = (std::distance(init_it, this->base()) % stride == width) & (std::distance(init_it, this->base()) > 0);
		if (condition_1 || condition_2)
			this->base_reference() += delta;		

	}

	void decrement()
	{
		this->base_reference()--;
		bool condition_1 = (std::distance(this->base(), init_it) % stride == 1) & (std::distance(this->base(), init_it) > 0);
		bool condition_2 = (std::distance(init_it, this->base()) % stride == width) & (std::distance(init_it, this->base()) > 0);
		if (condition_1 || condition_2)
			this->base_reference() -= delta;
	}

	void advance(int n)
	{
		if (n > 0)
		{
			for (int i = 0; i < n; i++)
				increment();
		}
		else
		{
			for (int i = 0; i < -n; i++)
				decrement();
		}
	}

	template <class OtherDerived, class OtherIterator, class V, class C, class R, class D>
	ptrdiff_t distance_to(boost::iterator_adaptor<OtherDerived, OtherIterator, V, C, R, D> const& y) const
	{
		ptrdiff_t this_d, this_row, this_column, outer_d, outer_row, outer_column, between_d, rows, columns, addition;
		auto it = (image_iterator<Iterator> const&)y;
		addition = 0;
		between_d = std::distance(this->init_it, it.init_it);
		rows = std::abs(between_d) / stride;
		columns = between_d == 0 ? 0 : stride;
		this_d = std::distance(this->init_it, this->base());
		outer_d = std::distance(it.init_it, it.base());
		if (between_d != 0)
		{
			if (this_d == 0 && outer_d == 0)
				return between_d / std::abs(between_d) * rows * width;
			addition = between_d / std::abs(between_d) * rows * width;

		}
		if (this_d < 0)
		{
			this_d = std::abs(this_d);
			this_row = rows - this_d / stride;
			this_column = columns - this_d % stride;
		}
		this_row = this_d / stride;
		this_column = this_d % stride;

		if(outer_d < 0)
		{
			outer_d = std::abs(outer_d);
			outer_row = rows - outer_d / stride;
			outer_column = columns - outer_d % stride;				
		}
		outer_row = outer_d / stride;
		outer_column = outer_d % stride;
			
		return addition + (outer_row * width + outer_column) - (this_row * width + this_column);		
	}
public:

};
//}

template<class Container = std::vector<uint8_t>>
class image
{
public:
	image(size_t width, size_t height, size_t stride) :
		data(stride * height),
		width_(width),
		stride_(stride)
	{}

	auto begin()
	{
		return image_iterator(data.begin(), width_, stride_);
	}

	auto end()
	{
		return image_iterator(data.end(), width_, stride_);
	}

	auto begin() const
	{
		return image_iterator(data.begin(), width_, stride_);
	}

	auto end() const
	{
		return image_iterator(data.end(), width_, stride_);
	}

private:
	Container data; // image data including strides. Access by (x, y): data[y * stride_ + x]
	size_t width_;
	size_t stride_;
};

#endif // __ITERATOR_HPP__
