#include <boost/python.hpp>

#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <pyboost_ipc/api.hpp>

#include <boost/range/algorithm/replace.hpp>

using namespace boost::interprocess;
using namespace boost::python;

template <class Type_, bool UsingCustomConverter = false>
struct register_vector
{
    register_vector(std::string type_name) {
        typedef typename boost_ipc::vector<Type_>::type Vector_;

        //TODO [muenz]: I particularly would prefer that the user could do in python:
        //
        //      vector_type(ctypes.int)
        //
        // instead of:
        //
        //      vector_int
        std::string class_name = "vector_";
        boost::replace(type_name, ' ', '_');
        class_name += type_name;

        class_<Vector_>(class_name.c_str(), no_init)
            .def(vector_indexing_suite<Vector_, UsingCustomConverter>());

        register_ipc_type<Vector_>();
    }
};

void register_containers()
{
#define REGISTER_VECTOR(_Type_) register_vector<_Type_>(#_Type_)
    REGISTER_VECTOR(double);
    REGISTER_VECTOR(float);
    REGISTER_VECTOR(long);
    REGISTER_VECTOR(unsigned long);
    REGISTER_VECTOR(int);
    REGISTER_VECTOR(unsigned int);
    REGISTER_VECTOR(short);
    REGISTER_VECTOR(char);
    REGISTER_VECTOR(unsigned char);
    register_vector<boost_ipc::string, true>("string");
}
