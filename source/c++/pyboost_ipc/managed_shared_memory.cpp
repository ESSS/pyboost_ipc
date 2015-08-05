#include <boost/python.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/shared_ptr.hpp>

#include "pyboost_ipc/internal.hpp"

using namespace boost::interprocess;
using namespace boost::python;

bindings_managed_shared_memory_type& get_bindings_managed_shared_memory() {
    typedef managed_shared_memory T;
    static class_<T, boost::shared_ptr<T>, boost::noncopyable> c(
        "managed_shared_memory",
        init<create_only_t, const char *, T::size_type, optional<const void*, const permissions&> >(
            args("tag", "name", "size", "addr", "perm")
        )
    );
    return c;
}

object wrap_allocate(boost::shared_ptr<managed_shared_memory> self, managed_shared_memory::size_type nbytes) {
    void* buffer = self->allocate(nbytes);
    PyObject* py_buf = PyBuffer_FromReadWriteMemory(buffer, nbytes);
    object retval = object(handle<>(py_buf));
    return retval;
}

void* void_ptr_get_bindings_managed_shared_memory() {
    bindings_managed_shared_memory_type& ptr = get_bindings_managed_shared_memory();
    return &ptr;
}

void register_managed_shared_memory() {
    typedef managed_shared_memory T;

    def("_void_ptr_get_bindings_managed_shared_memory", &void_ptr_get_bindings_managed_shared_memory, return_value_policy<return_opaque_pointer>());

    bindings_managed_shared_memory_type& c = *(bindings_managed_shared_memory_type*) void_ptr_get_bindings_managed_shared_memory();
    c.def(
        init<open_or_create_t, const char *, T::size_type, optional<const void*, const permissions&> >(
            args("tag", "name", "size", "addr", "perm")
        )
    );
    c.def(
        init<open_copy_on_write_t, const char *, optional<const void*> >(
            args("tag", "name", "addr")
        )
    );
    c.def(
        init<open_read_only_t, const char *, optional<const void*> >(
            args("tag", "name", "addr")
        )
    );
    c.def(
        init<open_only_t, const char *, optional<const void*> >(
            args("tag", "name", "addr")
        )
    );

    //TODO: finish "managed_shared_memory"

    c.add_property("size", &T::get_size);
    c.def("allocate", &wrap_allocate, args("nbytes"));
    //c.add_property("segment_manager", &T::get_segment_manager, return_value_policy<return_opaque_pointer>());
}
