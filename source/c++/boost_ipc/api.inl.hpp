#ifndef HPP_INL_BOOST_IPC_API
#define HPP_INL_BOOST_IPC_API

#include <boost_ipc/api.hpp>

#include <boost/python.hpp>

#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/type_traits.hpp>

#include "boost_ipc/internal.hpp"

namespace detail {

using namespace boost::python;
using namespace boost::interprocess;

template <class Type_>
struct register_offset_ptr
{
    typedef boost::interprocess::offset_ptr<Type_> value_type;
    typedef register_offset_ptr<Type_> self_t;

    register_offset_ptr() {
        using namespace boost::python;

        to_python_converter<
            value_type,
            self_t
#ifdef BOOST_PYTHON_SUPPORTS_PY_SIGNATURES
            , true
#endif
        >();
    }

    //To-python
    static PyObject* convert(value_type const& ptr)
    {
        if (ptr) {
            PyObject* py_buf = PyBuffer_FromReadWriteMemory(ptr.get(), 0);
            return incref(py_buf);
        }
        return incref(Py_None);
    }

    static const PyTypeObject* get_pytype()
    {
        return converter::registered<value_type>::converters.to_python_target_type();
    }
};

template <class T>
object asbuffer(offset_ptr<T>& ptr, size_t nbytes)
{
    PyObject* py_buf = PyBuffer_FromReadWriteMemory(ptr.get(), nbytes);
    object retval = object(handle<>(py_buf));
    return retval;
}

template <class T>
T* find_managed_shared_memory(boost::shared_ptr<managed_shared_memory> p_segment, boost::type<T> py_class, managed_shared_memory::segment_manager::char_ptr_holder_t p_name)
{
    return p_segment->find<T>(p_name).first;
}

template <class T>
typename managed_shared_memory::segment_manager::construct_proxy<T>::type
construct_managed_shared_memory(
    boost::shared_ptr<managed_shared_memory> p_segment,
    boost::type<T> py_class,
    managed_shared_memory::segment_manager::char_ptr_holder_t p_name
)
{
    return p_segment->construct<T>(p_name);
}

//! Converter from python type() to boost::type<>
template <class T>
struct register_conversion_from_python_type_to_boost_type
{
    //TODO: check if ctypes support is needed (or already supported)
    register_conversion_from_python_type_to_boost_type()
    {
        converter::registry::push_back(
            &convertible,
            &construct,
            type_id< boost::type<T> >());
    }

    static void* convertible(PyObject* py_object)
    {
        if (!PyType_Check(py_object)) {
            return NULL;
        }

        using namespace boost::python;
        const converter::registration* registration = converter::registry::query(type_id<T>());
        if (!registration) {
            return NULL;
        }

        PyTypeObject* type_object = registration->get_class_object();
        if (py_object != (void*)type_object) {
            return NULL;
        }

        return py_object;
    }

    static void construct(
        PyObject* py_object,
        converter::rvalue_from_python_stage1_data* data)
    {
        typedef converter::rvalue_from_python_storage< boost::type<T> > storage_type;
        void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

        new (storage)boost::type<T>();
        data->convertible = storage;
    }
};

template <class Type_>
void register_construct_proxy()
{
    typedef typename managed_shared_memory::segment_manager::construct_proxy<Type_>::type proxy_t;
    std::string class_name = "_proxy";
    class_name += typeid(Type_).name();
    class_<proxy_t>(class_name.c_str(), no_init);
        ////TODO [muenz]: receive more argument types
        //.def("__call__", &proxy_t::operator(), return_value_policy<reference_existing_object>())
        //;
}

} //namespace 'detail'

template <class Type_>
void register_offset_ptr()
{
    using namespace boost::python;

    typedef boost::interprocess::offset_ptr<Type_> T;
    class_<T> c(typeid(T).name());

    //TODO [muenz]: I think that I could create a custom type for offset_ptr and make it "implicitly convertible"
    //to the python-api buffer type
    c.def("asbuffer", &::detail::asbuffer<Type_>);
}

template <class T>
void register_ipc_type() {
    //TODO [muenz]: It would be nice if we could have this error checking here, but is_default_constructible
    //is not very easy to know, see: http://stackoverflow.com/questions/2733377/is-there-a-way-to-test-whether-a-c-class-has-a-default-constructor-other-than
    //BOOST_STATIC_ASSERT_MSG(
    //    boost::is_default_constructible<T>::value,
    //    "The type must be default constructible"
    //);

    if (!has_loaded_boost_ipc()) {
        throw std::runtime_error("The 'boost_ipc' bindings must be loaded before adding custom bindings");
    }

    using namespace boost::python;

    ::detail::register_conversion_from_python_type_to_boost_type<T>();
    ::detail::register_construct_proxy<T>();

    get_bindings_managed_shared_memory().def("find", &::detail::find_managed_shared_memory<T>, return_value_policy<reference_existing_object>());
    get_bindings_managed_shared_memory().def("construct", &::detail::construct_managed_shared_memory<T>);
}

#endif //HPP_INL_BOOST_API
