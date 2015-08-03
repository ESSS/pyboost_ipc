#include <pyboost_ipc/api.hpp>

void register_offset_ptrs() {
    register_offset_ptr<void>();
    register_offset_ptr<double>();
    register_offset_ptr<float>();
    register_offset_ptr<long>();
    register_offset_ptr<unsigned long>();
    register_offset_ptr<int>();
    register_offset_ptr<unsigned int>();
    register_offset_ptr<short>();
    register_offset_ptr<char>();
    register_offset_ptr<unsigned char>();
}
