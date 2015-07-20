def test_creation_tags():
    from boost_ipc import create_only, open_copy_on_write, open_only, open_or_create, open_read_only
    from boost_ipc_tests import receive_creation_tag
    assert receive_creation_tag(create_only) == 1
    assert receive_creation_tag(open_only) == 2
    assert receive_creation_tag(open_read_only) == 3
    assert receive_creation_tag(open_copy_on_write) == 4
    assert receive_creation_tag(open_or_create) == 5

