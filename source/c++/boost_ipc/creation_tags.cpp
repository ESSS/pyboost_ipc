#include <boost/python.hpp>
#include <boost/interprocess/creation_tags.hpp>

using namespace boost::interprocess;
using namespace boost::python;

template <class T>
void register_creation_tag(std::string p_var_name, T const& p_tag) {
    const char* class_name = typeid(T).name();
    class_<T>(class_name, no_init);
    scope().attr(p_var_name.c_str()) = p_tag;
}

void register_creation_tags() {
    register_creation_tag("create_only", create_only);
    register_creation_tag("open_only", open_only);
    register_creation_tag("open_read_only", open_read_only);
    register_creation_tag("open_copy_on_write", open_copy_on_write);
    register_creation_tag("open_or_create", open_or_create);
}
