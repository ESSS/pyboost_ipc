#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/to_python_converter.hpp>

#include <pyboost_ipc/api.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>

using namespace boost::python;

struct boost_ipc_string_converter
{
    boost_ipc_string_converter()
    {
        to_python_converter<boost_ipc::string, boost_ipc_string_converter>();
    }

    //To python
    static PyObject* convert(boost_ipc::string const& s) {
        return incref(object(s.c_str()).ptr());
    }
};

void register_string()
{
    boost_ipc_string_converter();
}
