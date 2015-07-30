#include <boost/python.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::interprocess;
using namespace boost::python;

void register_shared_memory_object() {
    typedef shared_memory_object T;
    class_<T, boost::shared_ptr<T>, boost::noncopyable> c("shared_memory_object", init<>());
    c.def(
        init<create_only_t, const char *, boost::interprocess::mode_t, optional<const permissions&> >(
            args("tag", "name", "mode", "permissions")
        )
    );
    c.def(
        init<open_or_create_t, const char *, boost::interprocess::mode_t, optional<const permissions&> >(
            args("tag", "name", "mode", "permissions")
        )
    );
    c.def(
        init<open_only_t, const char *, boost::interprocess::mode_t>(
            args("tag", "name", "mode")
        )
    );
    c.def("remove", &T::remove);
    c.staticmethod("remove");

    //TODO: finish "shared_memory_object"
}
