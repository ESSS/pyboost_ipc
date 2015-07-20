#ifndef HPP_BOOST_IPC_INTERNAL
#define HPP_BOOST_IPC_INTERNAL

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>

#ifdef _WINDOWS
#   ifdef BOOST_IPC_EXPORT
#       define BOOST_IPC_DECL __declspec(dllexport)
#   else
#       define BOOST_IPC_DECL __declspec(dllimport)
#   endif
#else
#   define BOOST_IPC_DECL
#endif

typedef boost::python::class_<
    boost::interprocess::managed_shared_memory,
    boost::shared_ptr<boost::interprocess::managed_shared_memory>,
    boost::noncopyable
> bindings_managed_shared_memory_type;

BOOST_IPC_DECL bindings_managed_shared_memory_type& get_bindings_managed_shared_memory();

#endif HPP_BOOST_IPC_INTERNAL
