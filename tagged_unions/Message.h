#pragma once

#include <string>
#include <type_traits>
#include <cassert>

namespace my_messages {

struct Payload1 {
  int a;
  double b;
};

struct Payload2 {
  double a;
  int b;
  std::string c;
};

struct Message {

  enum class Type {
    Unknown,
    Type1,
    Type2
  };

  union Data {
    Payload1 payload1;
    Payload2 payload2;

    Data() {
    }

    ~Data() {
    }
  };

  Type GetType() const {
    return _type;
  }

  template<Type U, typename = typename std::enable_if<U == Type::Type1>::type>
  const Payload1& GetData() const {
    return _data.payload1;
  }
  
  template<Type U, typename = typename std::enable_if<U == Type::Type2>::type>
  const Payload2& GetData() const {
    return _data.payload2;
  }

  Message(Payload1 data) : _type(Type::Type1) {
    new (&_data.payload1) Payload1(std::move(data));
  }

  Message(Payload2 data) : _type(Type::Type2) {
    new (&_data.payload2) Payload2(std::move(data));
  }
  
  Message(const Message&& other)
  : _type(other._type)
  {
    switch (_type)
    {
      case Type::Type1:
        new (&_data.payload1) Payload1(other._data.payload1);
        break;
      case Type::Type2:
        new (&_data.payload2) Payload2(other._data.payload2);
        break;
      default:
        assert(false);
        break;
    }
  }

  ~Message() {
    switch (_type) {
      case Type::Type1:
        _data.payload1.~Payload1();
        break;
      case Type::Type2:
        _data.payload2.~Payload2();
        break;
      default:
        assert(false);
        break;
    }
  }

private:
  const Type _type;
  Data _data;
};

}
