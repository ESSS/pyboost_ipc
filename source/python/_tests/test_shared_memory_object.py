from boost_ipc import create_only, modes, open_only, open_or_create, shared_memory_object
import pytest


def test_constructor_create_only():
    shared_memory_object(create_only, 'MySharedMemory', modes.read_write)
    with pytest.raises(RuntimeError):
        shared_memory_object(create_only, 'MySharedMemory', modes.read_write)

def test_constructor_open_or_create():
    shared_memory_object(open_or_create, 'MySharedMemory', modes.read_write)
    shared_memory_object(open_or_create, 'MySharedMemory', modes.read_write)

def test_constructor_open_only():
    with pytest.raises(RuntimeError):
        shared_memory_object(open_only, 'MySharedMemory', modes.read_write)

@pytest.mark.parametrize('mode', [
    modes.read_only,
    modes.read_write,
    pytest.mark.xfail(modes.copy_on_write, reason='TODO: failing'),
    pytest.mark.xfail(modes.read_private, reason='TODO: failing'),
])
def test_constructor_different_modes(mode):
    shared_memory_object(open_or_create, 'MySharedMemory', mode)

def test_remove_without_shared_memory():
    assert not shared_memory_object.remove('MySharedMemory')

def test_remove():
    shared_memory_object(open_or_create, 'MySharedMemory', modes.read_write)
    assert shared_memory_object.remove('MySharedMemory')
