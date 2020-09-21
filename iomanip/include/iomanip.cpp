#include<iomanip.hpp>


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
	stream = &source_stream;
}

SQ_proxy::SQ_proxy(const SQ_proxy &source_proxy)
{
	stream = source_proxy.stream;
}

SQ_proxy squares(std::ostream &source_stream)
{
	return SQ_proxy(source_stream);
}

SQ_proxy operator<<(std::ostream &stream, SQ_proxy (*manip)(std::ostream&))
{
	return manip(stream);
}


// add manipulator part
ADD_proxy::ADD_proxy(std::ostream &source_stream)
{
	stream = &source_stream;
}

ADD_proxy::ADD_proxy(const ADD_proxy &source_proxy)
{
	this->stream = source_proxy.stream;
}

ADD_proxy add(std::ostream &source_stream)
{
	return ADD_proxy(source_stream);
}

ADD_proxy operator<<(std::ostream &stream, ADD_proxy (*manip)(std::ostream&))
{
	return manip(stream);
}

