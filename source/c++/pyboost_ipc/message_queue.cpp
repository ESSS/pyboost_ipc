#include <boost/python.hpp>

#include <pyboost_ipc/scoped_gil_release.hpp>

#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::interprocess;
using namespace boost::python;

Py_buffer get_buffer(object obj, int flags = PyBUF_SIMPLE)
{
    PyObject* pobj = obj.ptr();
    Py_buffer pybuf;
    if (PyObject_GetBuffer(pobj, &pybuf, flags) == -1) {
        PyErr_SetString(PyExc_TypeError, "buffer argument does not implement the buffer protocol");
        throw_error_already_set();
    }
    return pybuf;
}

void wrap_send(boost::shared_ptr<message_queue> p_self, object buffer, unsigned int priority)
{
    Py_buffer pybuf = get_buffer(buffer);
    p_self->send(pybuf.buf, pybuf.len, priority);
}

bool wrap_try_send(boost::shared_ptr<message_queue> p_self, object buffer, unsigned int priority)
{
    Py_buffer pybuf = get_buffer(buffer);
    return p_self->try_send(pybuf.buf, pybuf.len, priority);
}

tuple wrap_receive(boost::shared_ptr<message_queue> p_self, object buffer)
{
    message_queue::size_type recvd_size;
    unsigned int priority;
    Py_buffer pybuf = get_buffer(buffer, PyBUF_WRITABLE);
    {
        ScopedGilRelease release_gil;
        p_self->receive(pybuf.buf, pybuf.len, recvd_size, priority);
    }
    return make_tuple(recvd_size, priority);
}

tuple wrap_try_receive(boost::shared_ptr<message_queue> p_self, object buffer)
{
    message_queue::size_type recvd_size;
    unsigned int priority;
    Py_buffer pybuf = get_buffer(buffer, PyBUF_WRITABLE);
    bool has_received = p_self->try_receive(pybuf.buf, pybuf.len, recvd_size, priority);
    return make_tuple(has_received, recvd_size, priority);
}

void register_message_queue()
{
    //TODO [muenz]: put in 'ipc' python module
    typedef message_queue T;
    class_<T, boost::shared_ptr<T>, boost::noncopyable> c(
        "message_queue",
        init<create_only_t, const char *, T::size_type, T::size_type, optional<const permissions&> >(
            args("tag", "name", "max_num_msg", "max_msg_size", "perm")
        )
    );

    c.def(
        init<open_or_create_t, const char *, T::size_type, T::size_type, optional<const permissions&> >(
            args("tag", "name", "max_num_msg", "max_msg_size", "perm")
        )
    );
    c.def(
        init<open_only_t, const char *>(
            args("tag", "name")
        )
    );

    c.def("send", &wrap_send, args("buffer", "priority"));
    c.def("try_send", &wrap_try_send, args("buffer", "priority"));
    //http://sysmagazine.com/posts/168827/ has bindings for datetime to boost::datetime
    //c.def("timed_send", &T::timed_send, args("buffer", "buffer_size", "priority")); //TODO [muenz]: need bindings for boost::posix_time::ptime

    c.def("receive", &wrap_receive, args("buffer"));
    c.def("try_receive", &wrap_try_receive, args("buffer"));
    //c.def("timed_receive", &T::timed_receive, args("buffer", "buffer_size", "priority")); //TODO [muenz]: need bindings for boost::posix_time::ptime

    c.add_property("num_msg", &T::get_num_msg);
    c.add_property("max_msg_size", &T::get_max_msg_size);
    c.add_property("max_msg", &T::get_max_msg);

    c.def("remove", &T::remove);
    c.staticmethod("remove");

    //TODO: finish "message_queue"
}
