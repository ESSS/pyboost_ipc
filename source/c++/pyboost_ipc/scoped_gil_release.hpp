#include <boost/python.hpp>
#include <boost/noncopyable.hpp>


class ScopedGilRelease
    : private boost::noncopyable
{
public:
    inline ScopedGilRelease()
        : m_thread_state(NULL)
    {
        release();
    }

    inline ~ScopedGilRelease()
    {
        acquire();
    }

private:
    inline void acquire() {
        if (m_thread_state) {
            PyEval_RestoreThread(m_thread_state);
            m_thread_state = NULL;
        }
    }

    inline void release() {
        if (m_thread_state == NULL) {
            m_thread_state = PyEval_SaveThread();
        }
    }
    PyThreadState * m_thread_state;
};
