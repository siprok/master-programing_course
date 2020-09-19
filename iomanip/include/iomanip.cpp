#include<iomanip.hpp>

#include<string>

// endm manipulator part
std::ostream &endm(std::ostream &arg_ss)
{
	arg_ss << "[eol]\n";;
	arg_ss.flush();
	return arg_ss;
}


// squares manipulator part
SQ_proxy::SQ_proxy(std::ostream &source_stream)
{
	this->head = this;
	stream = &source_stream;
}

SQ_proxy::SQ_proxy(SQ_proxy &source_proxy)
{
	this->head = source_proxy.head;
	stream = source_proxy.stream;
}

SQ_proxy::SQ_proxy(const SQ_proxy &source_proxy)
{
	this->head = source_proxy.head;
	stream = source_proxy.stream;
}

SQ_proxy &squares(std::ostream &source_stream)
{
	return *(new SQ_proxy(source_stream));
}

SQ_proxy& operator<<(std::ostream &stream, SQ_proxy& (*manip)(std::ostream&))
{
	return manip(stream);
}


// add manipulator part
ADD_proxy::ADD_proxy(std::ostream &source_stream)
{
	head = this;
	stream = &source_stream;
}

ADD_proxy::ADD_proxy(ADD_proxy &source_proxy)
{
	this->head = source_proxy.head;
	this->stream = source_proxy.stream;
}

ADD_proxy* ADD_proxy::get_head()
{
	return this->head;
}

std::ostream& ADD_proxy::get_stream()
{
	return *(this->stream);
}

std::ostream* ADD_proxy::get_stream_ptr()
{
	return this->stream;
}

ADD_proxy *add(std::ostream &source_stream)
{
	return new ADD_proxy(source_stream);
}

ADD_proxy& operator<<(std::ostream &stream, ADD_proxy* (*manip)(std::ostream&))
{
	return *(manip(stream));
}

