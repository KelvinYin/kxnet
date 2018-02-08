#include <kxnet/base/CircularBuffer.h>

#include <assert.h>

using namespace kxnet;
using namespace kxnet::base;
namespace tests
{
void pushing_and_popping()
{
    circular_buffer<int> cb(5);

    assert(cb.size() == 0);
    assert(cb.capacity() == 5);
    assert(cb.empty());
    assert(cb.max_size() > 0);

    assert(cb.push_back(1));
    assert(cb.size() == 1);
    assert(!cb.full());
    assert(cb.capacity() == 5);
    assert(!cb.empty());
    assert(cb.front() == 1);

    assert(cb.push_back(2));
    assert(cb.size() == 2);
    assert(!cb.full());
    assert(cb.capacity() == 5);
    assert(!cb.empty());
    assert(cb.front() == 1);

    assert(cb.push_back(3));
    assert(cb.push_back(4));
    assert(cb.push_back(5));
    assert(cb.size() == 5);
    assert(cb.full());
    assert(cb.capacity() == 5);
    assert(!cb.empty());
    assert(cb.front() == 1);

    assert(!cb.push_back(6));
    assert(cb.size() == 5);
    assert(cb.capacity() == 5);
    assert(cb.full());
    assert(!cb.empty());
    assert(cb.front() == 2);

    cb.pop_front();
    assert(cb.size() == 4);
    assert(cb.capacity() == 5);
    assert(!cb.full());
    assert(!cb.empty());
    assert(cb.front() == 3);

    cb.pop_front();
    assert(cb.size() == 3);
    assert(cb.front() == 4);

    cb.pop_front();
    assert(cb.size() == 2);
    assert(cb.front() == 5);

    cb.pop_front();
    assert(cb.size() == 1);
    assert(cb.front() == 6);

    cb.pop_front();
    assert(cb.size() == 0);
    assert(cb.capacity() == 5);
    assert(cb.empty());

    // empty again
    assert(cb.push_back(7));
    assert(cb.size() == 1);
    assert(cb.capacity() == 5);
    assert(!cb.empty());
    assert(cb.front() == 7);

    assert(cb.push_back(8));
    assert(cb.push_back(9));
    assert(cb.size() == 3);
    assert(!cb.empty());
    assert(cb.front() == 7);

    cb.clear();
    assert(cb.size() == 0);
    assert(cb.capacity() == 5);
    assert(cb.empty());
}

void indexing()
{
    circular_buffer<int> cb(5);

    // A helper - like asssert, but checks for an exception
    #define assert_throws(a,b) \
        {\
            try { a; throw "Failed to throw"; } \
            catch (const b&) { /*OK*/ } \
            catch (...) { throw "Threw wrong thing"; } \
        }

    // We loop round this test several times so our data wraps around the
    // internal m_buffer array a few times.
    for (size_t n = 0; n < 10; ++n)
    {
        assert(cb.empty());

        const circular_buffer<int> &const_cb(cb);

        assert_throws(cb.at(0), std::out_of_range);
        assert_throws(cb.at(1), std::out_of_range);
        assert_throws(const_cb.at(0), std::out_of_range);
        assert_throws(const_cb.at(0), std::out_of_range);

        // we could check that operator[] explodes, but there's little point!
        assert(cb.push_back(0));
        assert(cb.push_back(1));
        assert(cb.push_back(2));
        assert(cb.at(0) == 0); assert(const_cb.at(0) == 0);
        assert(cb.at(1) == 1); assert(const_cb.at(1) == 1);
        assert(cb.at(2) == 2); assert(const_cb.at(2) == 2);
        assert(cb[0] == 0);    assert(const_cb[0] == 0);
        assert(cb[1] == 1);    assert(const_cb[1] == 1);
        assert(cb[2] == 2);    assert(const_cb[2] == 2);

        assert(cb.front() == 0);
        cb[0] = 3;
        assert(cb.front() == 3);
        assert(cb.at(0) == 3); assert(const_cb.at(0) == 3);
        assert(cb.at(1) == 1); assert(const_cb.at(1) == 1);
        assert(cb.at(2) == 2); assert(const_cb.at(2) == 2);
        assert(cb[0] == 3);    assert(const_cb[0] == 3);
        assert(cb[1] == 1);    assert(const_cb[1] == 1);
        assert(cb[2] == 2);    assert(const_cb[2] == 2);

        cb[1] = 4;
        assert(cb.at(0) == 3); assert(const_cb.at(0) == 3);
        assert(cb.at(1) == 4); assert(const_cb.at(1) == 4);
        assert(cb.at(2) == 2); assert(const_cb.at(2) == 2);
        assert(cb[0] == 3);    assert(const_cb[0] == 3);
        assert(cb[1] == 4);    assert(const_cb[1] == 4);
        assert(cb[2] == 2);    assert(const_cb[2] == 2);

        cb.pop_front();
        assert(cb[0] == 4);    assert(const_cb[0] == 4);
        assert(cb[1] == 2);    assert(const_cb[1] == 2);

        cb.pop_front();
        assert(cb[0] == 2);    assert(const_cb[0] == 2);

        cb.pop_front();
    }
}
}

int main()
{
    tests::pushing_and_popping();
    tests::indexing();
    return 0;
}
