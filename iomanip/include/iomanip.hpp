/* Custom manipulators.
 * @file
 * @date 2018-08-05
 * @author Anonymous
 */
#pragma once
#ifndef __IOMANIP_HPP__
#define __IOMANIP_HPP__

#include <iostream>

//{ endm manipulator
std::ostream &endm(std::ostream &arg_ss);
//}

//{ squares manipulator
class SQ_proxy 
{
private:
	std::ostream *stream;
public:
	SQ_proxy(std::ostream&);
	SQ_proxy(const SQ_proxy &source_proxy);
	
	friend SQ_proxy operator<<(std::ostream &stream, SQ_proxy (*manip)(std::ostream&));

	template<typename T>
	friend std::ostream& operator<<(const SQ_proxy& sq_proxy, const T &mes)
	{
		return  *(sq_proxy.stream) << "[" << mes << "]";
	}
};
SQ_proxy squares(std::ostream&);
//}

//{ add manipulator

template<typename T>
class ADD_oper_proxy;

class ADD_proxy
{
private:
	std::ostream *stream;

public:
	ADD_proxy(std::ostream&);
	ADD_proxy(const ADD_proxy &source_proxy);

	template<typename T>
	friend ADD_oper_proxy<T> operator<<(const ADD_proxy &proxy, const T &operand)
	{
		return ADD_oper_proxy<T>(proxy, operand);
	}

	template<typename T>
	friend class ADD_oper_proxy;

};

ADD_proxy add(std::ostream&);

ADD_proxy operator<<(std::ostream &stream, ADD_proxy (*manip)(std::ostream&));


template<typename T>
class ADD_oper_proxy
{
private:
	std::ostream *base_stream;
	T accumulator;
public:
	ADD_oper_proxy<T>(const ADD_proxy &source_proxy, const T &operand)
	{
		this->base_stream = source_proxy.stream;
		accumulator = operand;
	}

	friend std::ostream& operator<<(const ADD_oper_proxy<T> &oper_proxy, const T &operand)
	{
		return  *(oper_proxy.base_stream) << oper_proxy.accumulator + operand;
	}
};


//}

#endif // __IOMANIP_HPP__
