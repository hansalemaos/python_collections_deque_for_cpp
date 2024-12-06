# std::vector as collections.deque with size limit and python api for C++

```cpp 
#include "pydeque.hpp"

int main()
{
    using namespace pyindex;
    std::vector<int> testvector_int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> testvector_int2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    PyDeque mydeque{testvector_int, 10};
    std::cout << mydeque << std::endl;
    for (int i{}; i < 100; i++)
    {
        mydeque.push_back(i);
        std::cout << mydeque << std::endl;
    }
    for (int i{}; i < 100; i++)
    {
        mydeque.emplace_back(i);
        std::cout << mydeque << std::endl;
    }
    for (int i{}; i < 100; i++)
    {
        mydeque.extend(testvector_int);
        std::cout << mydeque << std::endl;
    }
    for (int i{}; i < 100; i++)
    {
        mydeque.append_left(i);
        std::cout << mydeque << std::endl;
    }
    for (int i{}; i < 100; i++)
    {
        mydeque.insert(5, 99);
        std::cout << mydeque << std::endl;
    }
    for (int i{}; i < 100; i++)
    {
        mydeque.extend_left(testvector_int2);
        std::cout << mydeque << std::endl;
    }
    mydeque.index(10);
    try
    {
        mydeque.index(11);
    }
    catch (...)
    {
        std::cout << "Value not found" << std::endl;
    }
    for (int i{}; i < 100; i++)
    {
        try
        {
            std::cout << "Popping: " << mydeque.pop() << std::endl;
        }
        catch (...)
        {
            std::cout << "Deque is empty" << std::endl;
            break;
        }
        std::cout << mydeque << std::endl;
    }

    for (int i{}; i < 10; i++)
    {
        mydeque.extend_left(testvector_int2);
        std::cout << mydeque << std::endl;
    }
    for (int i{}; i < 100; i++)
    {
        try
        {
            std::cout << "Popping: " << mydeque.pop_left() << std::endl;
            mydeque.reverse();
            mydeque.rotate(i);
        }
        catch (...)
        {
            std::cout << "Deque is empty" << std::endl;
            break;
        }
        std::cout << mydeque << std::endl;
    }
    for (int i{}; i < 10; i++)
    {
        mydeque.extend_left(testvector_int2);
        std::cout << mydeque << std::endl;
    }
    for (int i{}; i < 100; i++)
    {
        auto index_and_value = mydeque.generator_enumerate();
        std::cout << "Index: " << index_and_value.first << " Value: " << index_and_value.second << std::endl;
    }
    for (int i{}; i < 100; i++)
    {
        auto myvalue = mydeque.generator();
        std::cout << myvalue << std::endl;
    }
    mydeque.set_generator_index(0);
    for (int i{}; i < 100; i++)
    {
        auto index_and_value = mydeque.generator_enumerate();
        if (index_and_value.first < 3)
        {
            continue;
        }
        std::cout << "Index: " << index_and_value.first << " Value: " << index_and_value.second << std::endl;
    }

    auto test1 = mydeque + 100;
    auto test2 = mydeque + testvector_int2;
    std::cout << test1 << std::endl;
    std::cout << test2 << std::endl;
    test1 += testvector_int2;
    test2 += 500;
    std::cout << test1 << std::endl;
    std::cout << test2 << std::endl;

    std::cout << test2[-2] << std::endl;
    std::cout << test2[1] << std::endl;
    test2[-2] = 10000;
    test2[1] = 999999;
    std::cout << test2[-2] << std::endl;
    std::cout << test2[1] << std::endl;
    std::cout << test2 << std::endl;

    try
    {
        std::cout << test2[100];
    }
    catch (...)
    {
        std::cout << "Index out of range" << std::endl;
    }

    test2[PY_SLICE(1, 8, 2)] = 10000;
    std::cout << test2 << std::endl;
    auto resultvec = test2.get_all_indices(10000);
    std::cout << resultvec << std::endl;
    std::vector<int> myv{11111, 22222};
    test2[PY_SLICE(1, 8, 2)] = myv;
    std::cout << test2 << std::endl;
    test2.change_max_size(1000);
    test2 *= 10;
    std::cout << test2 << std::endl;
    test2.change_max_size(5);
    std::cout << test2 << std::endl;
    test2.change_max_size(1000);
    std::cout << test2 << std::endl;
}

```
