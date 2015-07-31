from pyboost_ipc import vector_double, vector_string
from boost_ipc_tests import (create_vector_double_in_shared_memory,
    create_vector_string_in_shared_memory)


def test_string(shmem):
    v = create_vector_string_in_shared_memory(shmem, 'My vector', [b'Foo', b'Bar'])
    assert len(v) == 2
    assert type(v[0]) == bytes
    assert v[0] == b'Foo'
    assert type(v[1]) == bytes
    assert v[1] == b'Bar'
    assert list(v) == [b'Foo', b'Bar']

    found = shmem.find(vector_string, 'My vector')
    assert list(found) == [b'Foo', b'Bar']


def test_double(shmem):
    v = create_vector_double_in_shared_memory(shmem, 'My vector', [1.0, 2.0])
    assert len(v) == 2
    assert type(v[0]) == float
    assert v[0] == 1.0
    assert type(v[1]) == float
    assert v[1] == 2.0
    assert list(v) == [1.0, 2.0]

    found = shmem.find(vector_double, 'My vector')
    assert list(found) == [1.0, 2.0]
