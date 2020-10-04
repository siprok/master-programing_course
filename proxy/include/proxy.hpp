/* Proxy object.
 * @file
 * @date 2018-08-07
 * @author Anonymous
 */

#ifndef __PROXY_HPP__
#define __PROXY_HPP__

#include <mutex>


template<class T>
class ptr_holder
{
public:
    ptr_holder(T* ptr): ptr_(ptr) {}

    //{ describe proxy object
    class proxy: private T
    {
    public:
        proxy(const T &ob): T(const &ob){}

		T* operator -> () const
		{
			return &(this->proxy_ob);
		}
    private:
		T proxy_ob;

    };

	ptr_holder<T>::proxy operator -> () const
    {
        return ptr_holder<T>::proxy(ptr_);
    }
    //}

private:
    T* ptr_;
    mutable std::mutex mutex_;
};

#endif // __PROXY_HPP__
