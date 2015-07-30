from boost_ipc import (create_only, managed_shared_memory, open_only, open_or_create,
    unique_instance, vector_double, vector_int)
from boost_ipc_tests import (create_struct_with_double_in_shared_memory,
    create_struct_with_offset_ptr_in_shared_memory, create_vector_in_shared_memory,
    struct_with_double, struct_with_offset_ptr)
import numpy as np
import pytest


def test_constructor_create_only():
    managed_shared_memory(create_only, 'MySharedMemory', 1024)
    with pytest.raises(RuntimeError):
        managed_shared_memory(create_only, 'MySharedMemory', 1024)

def test_constructor_open_or_create():
    managed_shared_memory(open_or_create, 'MySharedMemory', 1024)
    managed_shared_memory(open_or_create, 'MySharedMemory', 1024)

def test_constructor_open_only():
    with pytest.raises(RuntimeError):
        managed_shared_memory(open_only, 'MySharedMemory')

    managed_shared_memory(create_only, 'MySharedMemory', 1024)
    managed_shared_memory(open_only, 'MySharedMemory')

def test_constructor_with_permissions():
    pytest.xfail('Implement')

def test_allocate(shmem):
    buf = shmem.allocate(8)
    assert len(buf) == 8

def test_allocate_and_numpy(shmem):
    buf = shmem.allocate(8)

    import numpy as np
    arr = np.frombuffer(buf, dtype=np.float64)
    assert arr.shape == (1,)
    arr.fill(-1.0)
    assert arr[0] == -1.0

    assert np.frombuffer(buf, dtype=np.float64)[0] == -1.0

def test_find_with_struct_using_pointer(shmem):
    original_struct = create_struct_with_offset_ptr_in_shared_memory(shmem, unique_instance, 10)
    original_arr = np.frombuffer(original_struct.ptr.asbuffer(8), dtype=np.float64)
    original_arr.fill(-1.0)

    find_struct = shmem.find(struct_with_offset_ptr, unique_instance)
    find_arr = np.frombuffer(find_struct.ptr.asbuffer(8), dtype=np.float64)

    assert find_arr[0] == original_arr[0] == -1.0

def test_find_with_struct_using_double(shmem):
    shmem = managed_shared_memory(open_or_create, 'MySharedMemory', 1024)

    original_struct = create_struct_with_double_in_shared_memory(shmem, unique_instance)
    original_struct.value = 1.5

    find_struct = shmem.find(struct_with_double, unique_instance)
    assert find_struct.value == 1.5

    original_struct.value = 2.5
    assert find_struct.value == 2.5

def test_find_different_struct_types(shmem):
    create_struct_with_offset_ptr_in_shared_memory(shmem, unique_instance, 10)
    create_struct_with_double_in_shared_memory(shmem, unique_instance)

    f1 = shmem.find(struct_with_offset_ptr, unique_instance)
    f2 = shmem.find(struct_with_double, unique_instance)

    assert type(f2) == struct_with_double
    assert type(f1) == struct_with_offset_ptr

def test_find_with_different_object_names(shmem):
    obj1 = create_struct_with_double_in_shared_memory(shmem, 'obj1')
    obj1.value = 1.5
    obj2 = create_struct_with_double_in_shared_memory(shmem, 'obj2')
    obj2.value = 2.5

    find_obj1 = shmem.find(struct_with_double, 'obj1')
    find_obj2 = shmem.find(struct_with_double, 'obj2')

    assert find_obj1.value == 1.5
    assert find_obj2.value == 2.5

def test_find_vector(shmem):
    create_vector_in_shared_memory(shmem, 'my vector', [1.5, 2.5])

    find = shmem.find(vector_double, 'my vector')
    assert list(find) == [1.5, 2.5]

def test_construct(shmem):
    constructed = shmem.construct(struct_with_double, unique_instance)()
    constructed.value = 1.5

    find = shmem.find(struct_with_double, unique_instance)
    assert find.value == 1.5

@pytest.mark.xfail(reason='Not implemented yet')
def test_construct_class_with_args(shmem):
    from boost_ipc_tests import class_receiving_double
    constructed = shmem.construct(class_receiving_double, unique_instance)(1.5)
    with pytest.raises(Exception):
        shmem.construct(class_receiving_double, unique_instance)()
    assert constructed.value == 1.5

@pytest.mark.xfail(reason='Not implemented yet')
def test_construct_vector(shmem):
    constructed = shmem.construct(vector_double, 'obj1')()
    constructed.push_back(1.5)

    find = shmem.find(vector_double, 'obj1')
    assert find[0] == 1.5
