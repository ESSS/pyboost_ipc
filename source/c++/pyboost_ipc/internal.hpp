#ifndef HPP_BOOST_IPC_INTERNAL
#define HPP_BOOST_IPC_INTERNAL

#include <boost/python.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/interprocess/interprocess_fwd.hpp>

typedef boost::python::class_<
    boost::interprocess::managed_shared_memory,
    boost::shared_ptr<boost::interprocess::managed_shared_memory>,
    boost::noncopyable
> bindings_managed_shared_memory_type;

#endif // HPP_BOOST_IPC_INTERNAL
