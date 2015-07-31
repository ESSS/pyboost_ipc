from pyboost_ipc import message_queue, open_or_create
import pytest


@pytest.yield_fixture
def queue():
    message_queue.remove(b'foo_queue')
    queue = message_queue(open_or_create, b"foo_queue", 1, 10)
    yield queue
    message_queue.remove(b'foo_queue')

@pytest.mark.parametrize('buff', [
    bytearray([42]),
    buffer(b'Foo')
 ])
def test_send(queue, buff):
    queue.send(buff, 0)

def test_send_invalid_buffer(queue):
    with pytest.raises(TypeError):
        queue.send(object(), 0)

def test_receive(queue):
    queue.send(buffer(b'ABC'), 0)
    buff = bytearray(10)
    rcvd_size, priority = queue.receive(buff)
    assert rcvd_size == 3
    assert priority == 0
    assert buff[0] == ord('A')
    assert buff[1] == ord('B')
    assert buff[2] == ord('C')
