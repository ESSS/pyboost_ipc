def test_has_all_expected_exported_symbols():
    import boost_ipc
    expected_symbols = {
        'anonymous_instance',
        'unique_instance',
        'create_only',
        'managed_shared_memory',
        'message_queue',
        'modes',
        'open_copy_on_write',
        'open_only',
        'open_or_create',
        'open_read_only',
        'permissions',
        'shared_memory_object',
    }
    assert expected_symbols.issubset(dir(boost_ipc))
