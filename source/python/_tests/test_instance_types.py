from pyboost_ipc import anonymous_instance, unique_instance
from pyboost_ipc_tests import receive_object_name_or_instance_type
import pytest


def test_anonymous_instance():
    assert receive_object_name_or_instance_type(anonymous_instance) == 'anonymous'

def test_unique_instance():
    assert receive_object_name_or_instance_type(unique_instance) == 'unique'

def test_str():
    assert receive_object_name_or_instance_type('Name') == 'Name'
    with pytest.raises(Exception) as e:
        assert receive_object_name_or_instance_type(None)
