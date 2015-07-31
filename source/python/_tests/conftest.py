import pytest


@pytest.fixture(autouse=True)
def clear_shared_memory(request):
    from pyboost_ipc import shared_memory_object
    shared_memory_object.remove('MySharedMemory')
    request.addfinalizer(lambda: shared_memory_object.remove('MySharedMemory'))

@pytest.fixture
def shmem():
    from pyboost_ipc import managed_shared_memory, open_or_create
    return managed_shared_memory(open_or_create, 'MySharedMemory', 8192)
