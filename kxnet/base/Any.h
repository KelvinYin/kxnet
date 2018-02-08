#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <exception>

namespace kxnet 
{
namespace base
{

class Any
{
public:
    Any(void) 
      : typeIndex_(std::type_index(typeid(void)))
    {}

    Any(const Any& rhs)
      : ptr_(rhs.Clone()), typeIndex_(rhs.typeIndex_) 
    {}
    
    Any(Any && rhs) 
      : ptr_(std::move(rhs.ptr_)), typeIndex_(rhs.typeIndex_) 
    {}

    // 创建智能指针时，对于一般的类型，通过std::decay来移除引用和cv符，从而获取原始类型
    template<typename U, class = typename std::enable_if<!std::is_same<typename std::decay<U>::type, Any>::value, U>::type> 
    Any(U && value) 
      : ptr_(new Derived <typename std::decay<U>::type>(std::forward<U>(value))), typeIndex_(std::type_index(typeid(typename std::decay<U>::type)))
    {}

    bool IsNull() const { return !bool(ptr_); }

    template<class U> bool Is() const
    {
        return typeIndex_ == std::type_index(typeid(U));
    }

    //将Any转换为实际的类型
    template<class U>
    U* AnyCast()
    {
      if (!Is<U>())
      {
          // cout << "can not cast " << typeid(U).name() << " to " << typeIndex_.name() << endl;
          throw std::bad_cast();
      }

      auto derived = dynamic_cast<Derived<U>*> (ptr_.get());
      return &derived->m_value;
    }

    Any& operator=(const Any& a)
    {
        if (ptr_ == a.ptr_)
            return *this;

        ptr_ = a.Clone();
        typeIndex_ = a.typeIndex_;
        return *this;
    }

private:
    struct Base;
    typedef std::unique_ptr<Base> BasePtr;

    struct Base
    {
        virtual ~Base() {}
        virtual BasePtr Clone() const = 0;
    };

    template<typename T>
    struct Derived : Base
    {
      template<typename U>
      Derived(U && value) : m_value(std::forward<U>(value)) { }

      BasePtr Clone() const
      {
          return BasePtr(new Derived<T>(m_value));
      }

      T m_value;
    };

    BasePtr Clone() const
    {
      if (ptr_ != nullptr)
        return ptr_->Clone();

      return nullptr;
    }

    BasePtr ptr_;
    std::type_index typeIndex_;
};

}
}
