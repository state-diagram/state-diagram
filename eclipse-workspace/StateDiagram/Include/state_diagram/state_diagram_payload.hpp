/*
   (c) Copyright 2019-2021 State Diagram Contributors

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef STATE_DIAGRAM_PAYLOAD_HPP_
#define STATE_DIAGRAM_PAYLOAD_HPP_

#include "state_diagram_error.h"

namespace state_diagram
{

template<typename T>
class PayloadBase
{
public:
#ifndef STATE_DIAGRAM_STRINGLESS
  class Error
  :
    public state_diagram::Error
  {
    // This space intentionally left empty
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  class IndexationError
  :
    public Error
  {
  public:
    string
    specific()
    const
    {
      return string() + "Payload indexation error.";
    }
  };
#else
  static int constexpr indexationError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  class TypeMismatchError
  :
    public Error
  {
  public:
    string
    specific()
    const
    {
      return string() + "Payload type mismatch error.";
    }
  };
#else
  static int constexpr typeMismatchError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

protected:
  PayloadBase() = default;

  PayloadBase(T const & _firstItem)
  :
    firstItem{_firstItem}
  {
    // This space intentionally left empty
  }

  PayloadBase(T && _firstItem)
  :
    firstItem{forward<T>(_firstItem)}
  {
    // This space intentionally left empty
  }

  T firstItem;
};

template<typename T, typename... Ts>
class Payload
:
  public PayloadBase<T>
{
public:
  Payload()
  :
    PayloadBase<T>{}
  , remainingItems{}
  {
    // This space intentionally left empty
  }

  Payload(T const & _firstItem, Ts const &... _remainingItems)
  :
    PayloadBase<T>{_firstItem}
  , remainingItems{_remainingItems...}
  {
    // This space intentionally left empty
  }

  Payload(T && _firstItem, Ts &&... _remainingItems)
  :
    PayloadBase<T>{forward<T>(_firstItem)}
  , remainingItems{forward<Ts>(_remainingItems)...}
  {
    // This space intentionally left empty
  }

  Payload(tuple<Ts...> const & items)
  :
    Payload(tie(items))
  {
    // This space intentionally left empty
  }

  Payload(tuple<Ts...> && items)
  :
    Payload(tie(forward<Ts...>(items)))
  {
    // This space intentionally left empty
  }

  Payload(Payload const & other) = default;

  Payload(Payload && other) = default;

  Payload & operator=(Payload const & other) = default;

  Payload & operator=(Payload && other) = default;

  template<typename U>
  U
  get(size_t const index)
  const
  {
    return get(index, U());
  }

  template<typename U>
  U
  get(size_t const index, U const & typeHint)
  const
  {
    if (std::is_same<U, T>::value)
    {
      if (index == 0)
      {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
        return (U) PayloadBase<T>::firstItem;
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
      }
    }
    else if (index == 0)
    {
#ifndef STATE_DIAGRAM_STRINGLESS
      throw typename PayloadBase<T>::TypeMismatchError();
#else
      STATE_DIAGRAM_HANDLE_ERROR(PayloadBase<T>::typeMismatchError);
#endif // STATE_DIAGRAM_STRINGLESS
    }
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
    return (U) remainingItems.get(index - 1, typeHint);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
  }

  template<size_t index, typename U, typename... Us>
  struct TypeIndexation
  {
    using Target = typename TypeIndexation<index - 1, Us...>::Target;
  };

  template<typename U, typename... Us>
  struct TypeIndexation<0, U, Us...>
  {
  public:
    using Target = U;
  };

  template<size_t componentIndex>
  typename TypeIndexation<componentIndex, T, Ts...>::Target
  get()
  const
  {
    return get(componentIndex, typename TypeIndexation<componentIndex, T, Ts...>::Target());
  }

  template<typename U, size_t componentIndex>
  U
  get()
  const
  {
    return get(componentIndex, U());
  }

private:
  Payload<Ts...> remainingItems;
};

template<class T>
class Payload<T>
:
  public PayloadBase<T>
{
public:
  Payload()
  :
    PayloadBase<T>{}
  {
    // This space intentionally left empty
  }

  Payload(T const & item)
  :
    PayloadBase<T>{item}
  {
    // This space intentionally left empty
  }

  Payload(T && item)
  :
    PayloadBase<T>{forward<T>(item)}
  {
    // This space intentionally left empty
  }

  Payload(Payload const & other) = default;

  Payload(Payload && other) = default;

  Payload & operator=(Payload const & other) = default;

  Payload & operator=(Payload && other) = default;

  T
  get(size_t const index)
  const
  {
    if (index == 0)
    {
      return PayloadBase<T>::firstItem;
    }
#ifndef STATE_DIAGRAM_STRINGLESS
    throw typename PayloadBase<T>::IndexationError();
#else
    STATE_DIAGRAM_HANDLE_ERROR(PayloadBase<T>::indexationError);
#endif // STATE_DIAGRAM_STRINGLESS
  }

  template<typename U>
  U
  get(size_t const index, U const)
  const
  {
    if (std::is_same<U, T>::value)
    {
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
      return (U) get(index);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    }
#ifndef STATE_DIAGRAM_STRINGLESS
    throw typename PayloadBase<T>::TypeMismatchError();
#else
    STATE_DIAGRAM_HANDLE_ERROR(PayloadBase<T>::typeMismatchError);
#endif // STATE_DIAGRAM_STRINGLESS
  }

  template<size_t componentIndex>
  T
  get()
  const
  {
    return get(componentIndex);
  }
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_PAYLOAD_HPP_
