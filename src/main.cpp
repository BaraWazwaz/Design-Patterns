#include <iostream>
#include "singleton.hpp"

class MyClass
{
public:
    ~MyClass()
    {
        std::cout << "Object got deleted" << std::endl;
    }
    MyClass(const MyClass&) = delete;
    MyClass& operator=(const MyClass&) = delete;

    int& getData() { return data; }
    void setData(int _data) { data = _data; }
    operator int() { return data; }

private:
    int data;
    MyClass()
    : data(0) 
    {
        std::cout << "Object got constructed" << std::endl;
    }
    friend nitron::Singleton<MyClass>;
};

int main() 
{
    std::cout << "Hello World" << std::endl;
    typedef nitron::Singleton<MyClass> SingletonType;
    MyClass& x = SingletonType::get();
    std::cout << "Singleton value: " << x.getData() << std::endl;
    x.setData(10);
    std::cout << "Singleton value: " << x.getData() << std::endl;
    return 0;
}