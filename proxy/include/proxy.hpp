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
    class proxy: private std::lock_guard<std::mutex>
    {
    public:
        proxy( T* ptr_, std::mutex &mute): lock_guard<std::mutex>(mute), pr_ptr(ptr_)
        {}

		T* operator -> () const
		{
			return pr_ptr;
		}
    private:
		T* pr_ptr;
    };

    ptr_holder<T>::proxy operator -> () const
    {
        return ptr_holder<T>::proxy(ptr_, mutex_);
    }
    //}

private:
    T* ptr_;
    mutable std::mutex mutex_;
};

#endif // __PROXY_HPP__
