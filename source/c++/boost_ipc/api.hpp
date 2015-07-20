#ifndef HPP_BOOST_IPC_API
#define HPP_BOOST_IPC_API

#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>

template <class T>
void register_offset_ptr();

template <class T>
void register_ipc_type();

namespace boost_ipc {
    namespace ipc = boost::interprocess;

    //I've created a custom allocator because when exposing methods of the vector, the boost::container::vector
    //requires that the allocator be default-constructible for "reasons". This situation should never
    //occur because I create the vectors always passing the segment_manager instance.
    template <class T>
    struct MyAllocator
        : ipc::allocator<T, ipc::managed_shared_memory::segment_manager>
    {
        typedef ipc::allocator<T, ipc::managed_shared_memory::segment_manager> super;

        MyAllocator()
            : super(NULL)
        {
        }
        MyAllocator(ipc::managed_shared_memory::segment_manager *segment_mngr)
            : super(segment_mngr)
        {}
    };

    template <class T>
    struct vector {
        typedef MyAllocator<T> allocator;
        typedef boost::container::vector<T, allocator> type;
    };
}

#include <boost_ipc/api.inl.hpp>

#endif // HPP_BOOST_IPC_API
