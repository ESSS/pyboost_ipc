#include <boost/python.hpp>

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/creation_tags.hpp>

#include <boost/shared_ptr.hpp>

#include <pyboost_ipc/api.hpp>

using namespace boost::python;
using namespace boost::interprocess;

//////////////////////////////////////////////////////////////////////////

int receive_creation_tag(create_only_t) {
    return 1;
}
int receive_creation_tag(open_only_t) {
    return 2;
}
int receive_creation_tag(open_read_only_t) {
    return 3;
}
int receive_creation_tag(open_copy_on_write_t) {
    return 4;
}
int receive_creation_tag(open_or_create_t) {
    return 5;
}

//////////////////////////////////////////////////////////////////////////

struct struct_with_offset_ptr {
    offset_ptr<double> ptr;
};

struct_with_offset_ptr* create_struct_with_offset_ptr_in_shared_memory(managed_shared_memory& shared_memory, ipcdetail::char_ptr_holder<char> name, managed_shared_memory::size_type nbytes) {
    struct_with_offset_ptr* s = shared_memory.construct<struct_with_offset_ptr>(name)();
    s->ptr = static_cast<double*>(shared_memory.allocate(nbytes));
    return s;
}

struct struct_with_double {
    double value;
};

struct_with_double* create_struct_with_double_in_shared_memory(managed_shared_memory& shared_memory, ipcdetail::char_ptr_holder<char> name) {
    struct_with_double* s = shared_memory.construct<struct_with_double>(name)();
    return s;
}

std::string receive_object_name_or_instance_type(ipcdetail::char_ptr_holder<char> name) {
    if (name == ipcdetail::char_ptr_holder<char>(anonymous_instance)) {
        return "anonymous";
    }
    else if (name == ipcdetail::char_ptr_holder<char>(unique_instance)) {
        return "unique";
    }
    else {
        const char* inner_name = name;
        return inner_name;
    }
}

class class_receiving_double {
public:
    class_receiving_double(double value_)
        : value(value_)
    {}
    class_receiving_double()
        : value(-1)
    {}

    double value;
};

boost_ipc::vector<double>::type* create_vector_double_in_shared_memory(managed_shared_memory& shared_memory, ipcdetail::char_ptr_holder<char> name, list items) {
    auto* ret = shared_memory.construct<boost_ipc::vector<double>::type>(name)(shared_memory.get_segment_manager());
    for (size_t i = 0; i < len(items); ++i) {
        double value = extract<double>(items[i]);
        ret->push_back(value);
    }
    return ret;
}

typedef boost_ipc::vector<boost_ipc::string>::type vector_string;
vector_string* create_vector_string_in_shared_memory(managed_shared_memory& shared_memory, ipcdetail::char_ptr_holder<char> name, list items) {
    auto* ret = shared_memory.construct<vector_string>(name)(shared_memory.get_segment_manager());
    for (size_t i = 0; i < len(items); ++i) {
        const char* value = extract<const char*>(items[i]);
        boost_ipc::string ipc_string(shared_memory.get_segment_manager());
        ipc_string = value;
        ret->push_back(ipc_string);
    }
    return ret;
}

BOOST_PYTHON_MODULE( boost_ipc_tests )
{
    import("boost_ipc");

    def("receive_creation_tag", static_cast<int(*)(create_only_t)>(&receive_creation_tag));
    def("receive_creation_tag", static_cast<int(*)(open_only_t)>(&receive_creation_tag));
    def("receive_creation_tag", static_cast<int(*)(open_read_only_t)>(&receive_creation_tag));
    def("receive_creation_tag", static_cast<int(*)(open_copy_on_write_t)>(&receive_creation_tag));
    def("receive_creation_tag", static_cast<int(*)(open_or_create_t)>(&receive_creation_tag));

    def("receive_object_name_or_instance_type", &receive_object_name_or_instance_type);

    class_<struct_with_offset_ptr>("struct_with_offset_ptr")
        .add_property("ptr", make_getter(&struct_with_offset_ptr::ptr));
    def("create_struct_with_offset_ptr_in_shared_memory", &create_struct_with_offset_ptr_in_shared_memory, return_value_policy<reference_existing_object>());
    register_ipc_type<struct_with_offset_ptr>();

    class_<struct_with_double>("struct_with_double")
        .def_readwrite("value", &struct_with_double::value);
    def("create_struct_with_double_in_shared_memory", &create_struct_with_double_in_shared_memory, return_value_policy<reference_existing_object>());
    register_ipc_type<struct_with_double>();

    class_<class_receiving_double>("class_receiving_double", init<double>())
        .def_readonly("value", &class_receiving_double::value);
    register_ipc_type<class_receiving_double>();

    def("create_vector_double_in_shared_memory", &create_vector_double_in_shared_memory, return_value_policy<reference_existing_object>());
    def("create_vector_string_in_shared_memory", &create_vector_string_in_shared_memory, return_value_policy<reference_existing_object>());
}
