#ifndef HPP_BOOST_IPC_INTERNAL
#define HPP_BOOST_IPC_INTERNAL

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>

#include "export.hpp"

typedef boost::python::class_<
    boost::interprocess::managed_shared_memory,
    boost::shared_ptr<boost::interprocess::managed_shared_memory>,
    boost::noncopyable
> bindings_managed_shared_memory_type;

BOOST_IPC_EXPORT bindings_managed_shared_memory_type& get_bindings_managed_shared_memory();
BOOST_IPC_EXPORT bool has_loaded_boost_ipc();

#endif // HPP_BOOST_IPC_INTERNAL
