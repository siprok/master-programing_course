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

	explicit image_iterator(Iterator p) : image_iterator::iterator_adaptor_(p), width_(0), stride_(0), index_(0), delta_(stride_ - width_) {}

	explicit image_iterator(Iterator p, size_t width, size_t stride, size_t index) : image_iterator::iterator_adaptor_(p), width_(width), stride_(stride), index_(index), delta_(stride_ - width_) {}
	//image_iterator(Iterator it, size_t width, size_t stride) : boost::iterator_adaptor<Iterator, uint8_t>(it), width_(width), stride_(stride) {}
private:
	friend class boost::iterator_core_access;
	size_t width_;
	size_t stride_;
	size_t index_;
	size_t delta_;

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
		index_++;
		if (index_ % stride_ == width_)
		{
			index_ += delta_;
			this->base_reference() += delta_;
		}
	}

	void decrement()
	{
		this->base_reference()--;
		if (index_ % stride_ == 0)
		{
			index_ -= delta_;
			this->base_reference() -= delta_;
		}
		index_--;
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
		auto it = (image_iterator<Iterator> const&)y;
		ptrdiff_t row1 = index_ / stride_;
		ptrdiff_t column1 = index_ % stride_;
		ptrdiff_t row2 = it.index_ / stride_;
		ptrdiff_t column2 = it.index_ % stride_;
		return (row2 * width_ + column2) - (row1 * width_ + column1);
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
		return image_iterator(data.begin(), width_, stride_, 0);
	}

	auto end()
	{
		return image_iterator(data.end(), width_, stride_, data.size());
	}

	auto begin() const
	{
		return image_iterator(data.begin(), width_, stride_, 0);
	}

	auto end() const
	{
		return image_iterator(data.end(), width_, stride_, data.size());
	}

private:
	Container data; // image data including strides. Access by (x, y): data[y * stride_ + x]
	size_t width_;
	size_t stride_;
};

#endif // __ITERATOR_HPP__
