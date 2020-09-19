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
	SQ_proxy *head;
	std::ostream *stream;
public:
	SQ_proxy(std::ostream&);
	SQ_proxy(SQ_proxy &source_proxy);
	SQ_proxy(const SQ_proxy &source_proxy);
	
	friend SQ_proxy& operator<<(std::ostream &stream, SQ_proxy& (*manip)(std::ostream&));

	template<typename T>
	friend std::ostream& operator<<(SQ_proxy& sq_proxy, const T &mes)
	{
		std::ostream *stream_buf = sq_proxy.stream;
		delete sq_proxy.head;
		return  *stream_buf << "[" << mes << "]";
	}

	template<typename T>
	friend std::ostream& operator<<(SQ_proxy& sq_proxy, T &mes)
	{
		std::ostream *stream_buf = sq_proxy.stream;
		delete sq_proxy.head;
		return  *stream_buf << "[" << mes << "]";
	}
};
SQ_proxy &squares(std::ostream&);
//}

//{ add manipulator

template<typename T>
class ADD_oper_proxy;

class ADD_proxy
{
private:
	ADD_proxy *head;
	std::ostream *stream;

public:
	ADD_proxy(std::ostream&);
	ADD_proxy(ADD_proxy &source_proxy);
	ADD_proxy(const ADD_proxy &source_proxy);

	ADD_proxy *get_head();
	std::ostream &get_stream();
	std::ostream *get_stream_ptr();

	template<typename T>
	friend ADD_oper_proxy<T>& operator<<(ADD_proxy &proxy, const T &operand)
	{
		return *(new ADD_oper_proxy<T>(proxy, operand));
	}

	template<typename T>
	friend ADD_oper_proxy<T>& operator<<(ADD_proxy &proxy, T &operand)
	{
		return *(new ADD_oper_proxy<T>(proxy, operand));
	}
};

ADD_proxy *add(std::ostream&);

ADD_proxy& operator<<(std::ostream &stream, ADD_proxy* (*manip)(std::ostream&));


template<typename T>
class ADD_oper_proxy
{
private:
	ADD_oper_proxy<T> *head;
	ADD_proxy *base_proxy;
	T accumulator;
public:
	ADD_oper_proxy<T>(ADD_proxy &source_proxy, const T &operand)
	{
		this->head = this;
		this->base_proxy = source_proxy.get_head();
		accumulator = operand;
	}
	
	ADD_oper_proxy<T>(ADD_proxy &source_proxy, T &operand)
	{
		this->head = source_proxy.head;
		this->base_proxy = source_proxy.get_head();
		accumulator = operand;
	}

	ADD_oper_proxy<T> *get_head()
	{
		return this->head;
	}

	ADD_proxy *get_base_proxy_ptr()
	{
		return this->base_proxy;
	}

	friend std::ostream& operator<<(ADD_oper_proxy<T> &oper_proxy, const T &operand)
	{
		T buf = oper_proxy.accumulator + operand;
		std::ostream *stream_buf = oper_proxy.get_base_proxy_ptr()->get_stream_ptr();
		delete oper_proxy.get_base_proxy_ptr()->get_head();
		delete oper_proxy.get_head();
		return *stream_buf << buf;
	}

	friend std::ostream& operator<<(ADD_oper_proxy<T> &oper_proxy, T &operand)
	{
		T buf = oper_proxy.accumulator + operand;
		std::ostream *stream_buf = oper_proxy.get_base_proxy_ptr()->get_stream_ptr();
		delete oper_proxy.get_base_proxy_ptr()->get_head();
		delete oper_proxy.get_head();
		return *stream_buf << buf;
	}
};


//}

#endif // __IOMANIP_HPP__
