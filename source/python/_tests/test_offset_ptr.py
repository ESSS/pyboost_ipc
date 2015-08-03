from pyboost_ipc import managed_shared_memory, open_or_create
from pyboost_ipc_tests import create_struct_with_offset_ptr_in_shared_memory


def test_exports_buffer_interface():
    import numpy as np

    shmem = managed_shared_memory(open_or_create, 'MySharedMemory', 1024)

    s = create_struct_with_offset_ptr_in_shared_memory(shmem, 'object name', 8)
    buf = s.ptr.asbuffer(8)
    arr = np.frombuffer(buf, dtype=np.float64)
    arr[0] = 42.0

    assert np.frombuffer(buf, dtype=np.float64)[0] == 42.0
