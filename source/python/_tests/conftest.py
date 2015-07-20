import pytest


@pytest.fixture(autouse=True)
def clear_shared_memory(request):
    from boost_ipc import shared_memory_object
    shared_memory_object.remove('MySharedMemory')
    request.addfinalizer(lambda: shared_memory_object.remove('MySharedMemory'))
