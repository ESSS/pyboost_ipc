#include <boost/python.hpp>

#include "boost_ipc/internal.hpp"

#include <boost/interprocess/exceptions.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/interprocess/permissions.hpp>
#include <boost/interprocess/detail/os_file_functions.hpp>

#include <map>


using namespace boost::python;
using namespace boost::interprocess;

void translate_exception(interprocess_exception const& e) {
    std::string message = e.what();
    auto error_code = e.get_error_code();
    message += ". Error code: ";
    message += boost::lexical_cast<std::string>((int)error_code);

    static std::map<error_code_t, std::string> m;
    if (m.empty()) {
        m[system_error] = "system_error";
        m[other_error] = "other_error";
        m[security_error] = "security_error";
        m[read_only_error] = "read_only_error";
        m[io_error] = "io_error";
        m[path_error] = "path_error";
        m[not_found_error] = "not_found_error";
        m[busy_error] = "busy_error";
        m[already_exists_error] = "already_exists_error";
        m[not_empty_error] = "not_empty_error";
        m[is_directory_error] = "is_directory_error";
        m[out_of_space_error] = "out_of_space_error";
        m[out_of_memory_error] = "out_of_memory_error";
        m[out_of_resource_error] = "out_of_resource_error";
        m[lock_error] = "lock_error";
        m[sem_error] = "sem_error";
        m[mode_error] = "mode_error";
        m[size_error] = "size_error";
        m[corrupted_error] = "corrupted_error";
        m[not_such_file_or_directory] = "not_such_file_or_directory";
        m[invalid_argument] = "invalid_argument";
        m[timeout_when_locking_error] = "timeout_when_locking_error";
        m[timeout_when_waiting_error] = "timeout_when_waiting_error";
    }
    auto it = m.find(error_code);
    if (it != m.end()) {
        message += "(";
        message += it->second;
        message += ")";
    }

    PyErr_SetString(PyExc_RuntimeError, message.c_str());
}

void register_message_queue();
void register_instance_types();
void register_managed_shared_memory();
void register_offset_ptrs();
void register_shared_memory_object();
void register_creation_tags();
void register_containers();
void register_string();

bool g_has_loaded_boost_ipc = false;
BOOST_IPC_DECL bool has_loaded_boost_ipc() {
    return g_has_loaded_boost_ipc;
}

BOOST_PYTHON_MODULE( boost_ipc )
{
    g_has_loaded_boost_ipc = true;
    register_exception_translator<interprocess_exception>(&translate_exception);

    register_message_queue();
    register_instance_types();
    register_managed_shared_memory();
    register_offset_ptrs();
    register_shared_memory_object();
    register_creation_tags();
    register_string();
    register_containers();

    //TODO: finish "permissions"
    class_<permissions>("permissions", init<>());

    //mode_t
    enum_<mode_t>("modes")
        .value("read_only", read_only)
        .value("read_write", read_write)
        .value("copy_on_write", copy_on_write)
        .value("read_private", read_private);
}
