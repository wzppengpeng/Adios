#include <boost/python.hpp>
#include "aiolos.hpp"

using namespace boost::python;
using namespace aiolos;

BOOST_PYTHON_MODULE(aiolos)  //python模块
{
void    (Aiolos::*create_data1)(const string&)      = &Aiolos::create_data;
void    (Aiolos::*create_data2)(const string&, const string&)   = &Aiolos::create_data;
class_<Aiolos>("Aiolos")
.def("create_data", create_data1)
.def("create_data", create_data2)
.def("train", &Aiolos::train)
.def("get_data", &Aiolos::get_data)
.def("predict", &Aiolos::predict)
.def("dump_model", &Aiolos::dump_model)
.def("restore_model", &Aiolos::restore_model)
;
}