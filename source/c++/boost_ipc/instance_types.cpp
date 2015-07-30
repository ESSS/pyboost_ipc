#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/interprocess/detail/segment_manager_helper.hpp>

using namespace boost::interprocess;
using namespace boost::python;

template <instance_type EnumInstanceType>
struct wrapped_instance_t
{
    typedef ipcdetail::instance_t<EnumInstanceType> const* boost_ipc_internal_type;

    wrapped_instance_t(boost_ipc_internal_type p_tag)
        : m_tag(p_tag)
    {}
    boost_ipc_internal_type m_tag;

    operator ipcdetail::char_ptr_holder<char>() const {
        return m_tag;
    }
};

template <instance_type T>
void register_instance_type(std::string p_var_name, ipcdetail::instance_t<T> const* p_tag)
{
    typedef wrapped_instance_t<T> class_type;
    const char* class_name = typeid(T).name();
    class_<class_type>(class_name, no_init);
    implicitly_convertible<class_type, ipcdetail::char_ptr_holder<char> >();
    scope().attr(p_var_name.c_str()) = class_type(p_tag);
}

struct char_ptr_holder_converter
{
    typedef ipcdetail::char_ptr_holder<char> value_type;

    char_ptr_holder_converter()
    {
        boost::python::converter::registry::push_back(
            &convertible,
            &construct,
            boost::python::type_id<value_type>());
    }

    //template <instance_type T>
    static void* convertible(PyObject* obj_ptr)
    {
        if (!PyString_Check(obj_ptr)) {
            return NULL;
        }
        return obj_ptr;
    }

    static void construct(PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        const char* value = PyString_AsString(obj_ptr);
        if (value == NULL) {
            PyErr_SetString(PyExc_ValueError, "Cant accept None values for char_ptr_holder");
            boost::python::throw_error_already_set();
        }

        typedef converter::rvalue_from_python_storage<value_type> storage_type;
        void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;
        new (storage) value_type(value);
        data->convertible = storage;
    }
};

void register_instance_types() {
    register_instance_type("unique_instance", unique_instance);
    register_instance_type("anonymous_instance", anonymous_instance);
    char_ptr_holder_converter();
}
