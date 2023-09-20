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

//! \file "state_diagram.h" To use State Diagram, #include this header file.

#ifndef STATE_DIAGRAM_H_
#define STATE_DIAGRAM_H_

#include <array>
#include <cassert>
#include <functional>

#include "state_diagram_internal.h"
#include "state_diagram_payload.hpp"

//! Namespace for all State Diagram entities.
namespace state_diagram
{

//! Base class for entities that have a name and a path.
/*!
 * Base class for everything that has a name
 * and, by implication, also a path.
 * The path of a top state,
 * external signal or external variable
 * is equal to the name of that top state, signal, or variable.
 * Every other
 * name-carrying component occurs somewhere beneath some top state.
 * There exists always a direct, mostly user-defined attachment to
 * some enclosing state or region - This
 * state or region is called the component's (immediate) parent.
 * The components's path traces the names of states and regions starting
 * from the top state
 * down to the immediate parent, post-fixed
 * by the component's own name. Names have to be
 * unique per type of component within the respective immediate parent.
 * This requirement entails that paths of such components are
 * unique per type within the entire state machine they belong to.
 */
class NamePath
:
  private PImplUpcast<NamePathImpl>
{
protected:
  NamePath(NamePathImpl * const impl);

#ifndef STATE_DIAGRAM_STRINGLESS

public:
  //! Return a reference to the component's name.
  string const & name() const;

  //! Write the component's path to an ostream.
  /*!
   * \param to the stream to be written to.
   */
  void path(ostream & to) const;

  //! Return the component's path as a string.
  /*!
   * This function may be inefficient. It's mainly intended to be used
   * for generating feedback to the user in case an error has occurred.
   *
   * \return the component's path as a string.
   */
  string path() const;

#endif // STATE_DIAGRAM_STRINGLESS

};

#ifndef STATE_DIAGRAM_STRINGLESS

//! string infix used to separate component names from each other whenever they appear in paths.
string const pathComponentSeparator{"::"};

string const outsideOfStatemachineIndicator{"<outside of statemachine>"};

#endif // STATE_DIAGRAM_STRINGLESS

template<typename Data> class Signal;

class SignalDelegate
:
  private PImplUpcast<SignalDelegateImpl>
, public NamePath
{
  template<typename Delegate, typename Data> friend class ConcreteSignal;
  friend class TriggeredTransitionImpl;
  friend class TriggerlessTransitionImpl;

protected:
  SignalDelegate(SignalDelegateImpl * const impl);

  SignalDelegateImpl * implUpcast() const;

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  //! Base class for errors thrown on setting or retrieving a signal's data value.
  class Error
  :
    public state_diagram::Error
  {
  protected:
    Error(string const & path, string const & scopePath);

  public:
    //! The path of the signal.
    string const path;

    //! The path of the scope in which the error occurs.
    string const scopePath;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a data value is set on a void signal.
  class SetOnVoidError
  :
    public Error
  {
    template<typename Data> friend class Signal;

  private:
    SetOnVoidError(string const & path, string const & scopePath);

    string specific() const override;
  };
#else
  static int constexpr setOnVoidError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a data value is retrieved from a void signal.
  class TypeMismatchOnGetError
  :
    public Error
  {
    friend class Event;

  private:
    TypeMismatchOnGetError(string const & path, string const & scopePath);

    string specific() const override;
  };
#else
  static int constexpr typeMismatchOnGetError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error n when a data value is retrieved from a void signal.
  class TypeMismatchOnSetError
  :
    public Error
  {
    friend class Event;

  private:
    TypeMismatchOnSetError(string const & path, string const & scopePath);

    string specific() const override;
  };
#else
  static int constexpr typeMismatchOnSetError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a data value is retrieved from a void signal.
  class GetOnVoidError
  :
    public Error
  {
    template<typename Data> friend class Signal;

  private:
    GetOnVoidError(string const & path, string const & scopePath);

    string specific() const override;
  };
#else
  static int constexpr getOnVoidError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a signal's data value is set after it has been activated.
  class SetOnActivatedError
  :
    public Error
  {
    template<typename Data, class Delegate> friend class ConcreteSignal;

  private:
    SetOnActivatedError(string const & path, string const & scopePath);

    string specific() const override;
  };
#else
  static int constexpr setOnActivatedError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a signal's data value is set again after is has already been set.
  class SetOnAlreadySetError
  :
    public Error
  {
    template<typename Data, class Delegate> friend class ConcreteSignal;

  private:
    SetOnAlreadySetError(string const & path, string const & scopePath);

    string specific() const override;
  };
#else
  static int constexpr setOnAlreadySetError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a signal's data value is to be retrieved when it has not been set yet.
  class GetOnNotSetError
  :
    public Error
  {
    template<typename Data, class Delegate> friend class ConcreteSignal;

  private:
    GetOnNotSetError(string const & path, string const & scopePath);

    string specific() const override;
  };
#else
  static int constexpr getOnNotSetError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever a local signal is used out of scope.
  class ScopeError
  :
    public Error
  {
    template<typename Data, class Delegate> friend class ConcreteSignal;

  private:
    ScopeError(string const & path, string const & scopePath);

    string specific() const override;
  };
#else
  static int constexpr scopeError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

private:
  bool isUnderExecution() const;

  virtual bool isInCurLocalScope() const = 0;
#ifndef STATE_DIAGRAM_STRINGLESS
  string curLocalScopePath() const;
#endif // STATE_DIAGRAM_STRINGLESS

  bool isActive() const;
  bool isSet() const;
  void markAsSet() const;

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

class Top;

class ExternalSignalDelegate
:
  private virtual PImpl<ExternalSignalDelegateImpl>
, public SignalDelegate
{
  template<class Delegate, typename Data> friend class ConcreteSignal;
  template<typename Data> friend class UnifiedExternalSignal;
  friend class Top;

public:
  using Impl = ExternalSignalDelegateImpl;

  ExternalSignalDelegate(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent);

  ~ExternalSignalDelegate();

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever the name of an external signal is not unique.
  class NameClashError
  :
    public state_diagram::Error
  {
    friend class TopStateImpl;

  private:
    NameClashError(string const & signalName);

  public:
    //! The name of an external signal that already exists in that scope.
    string const signalName;

  private:
    string specific() const override;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

private:
  bool isInCurLocalScope() const override;

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

class CompoundState;
class Region;

class LocalSignalDelegate
:
  private virtual PImpl<LocalSignalDelegateImpl>
, public SignalDelegate
{
  friend class TriggerlessTransitionImpl;
  friend class TriggeredTransitionImpl;
  friend class AutoSpecImpl;
  template<class Delegate, typename Data> friend class ConcreteSignal;
  template<typename Data> friend class UnifiedLocalSignal;

public:
  using Impl = LocalSignalDelegateImpl;

  LocalSignalDelegate(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundState const & scope);
  LocalSignalDelegate(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Region const & scope);

  ~LocalSignalDelegate();

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Base class of errors thrown whenever the name of a local signal is not unique.
  class NameClashError
  :
    public state_diagram::Error
  {
  protected:
    NameClashError(string const & scopePath, string const & signalName);

  public:
    //! The path of the scope in which the name clash error occurs.
    string const scopePath;

    //! The name of a local signal that already exists in that scope.
    string const signalName;

  private:
    string specific() const override;

  protected:
    virtual string scopeTypeIndicator() const = 0;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

private:
  bool isInCurLocalScope() const override;

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

/* Base class for events.
 *
 * Events are used on transitions to reference signals that may trigger a transition.
 * Unlike signals, events are declared without associating any data type (or void) with them.
 * In place of that, member function set and get are function templates. Any
 * actual type argument to these functions
 * is supposed to be the data type associated with the signal that the event is used
 * to refer to.
 */
class Event
{
  friend class TriggeredTransitionImpl;

public:
  //! Destruct event.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  virtual ~Event();


  //! Return the event's path as a string.
  /*!
   * This function may be inefficient. It's mainly intended to be used
   * for generating feedback to the user in case an error has occurred.
   *
   * \return the event's path as a string.
   */
#ifndef STATE_DIAGRAM_STRINGLESS
  virtual string path() const = 0;
#endif // STATE_DIAGRAM_STRINGLESS

  //! Set data payload.
  /*!
   * \param Data the data type.
   * \param data the data value as an r-value reference.
   */
  template<typename Data>
  void set(Data && data);

  //! Set data payload.
  /*!
   * \param Data the data type.
   * \param data the data value as an l-value reference.
   */
  template<typename Data>
  void set(Data const & data);

  //! Retrieve data payload.
  /*!
   * \param Data the data type.
   */
  template<typename Data>
  Data get() const;

  //! Retrieve data payload from tuple.
  /*!
   * \param Data1 the first data type.
   * \param Data2 the second data type.
   * \param RemainingData... the remaining data types.
   */
  template<typename Data1, typename Data2, typename... RemainingData>
  Payload<Data1, Data2, RemainingData...> get() const;

  //! Retrieve data payload from tuple.
  /*!
   * \param Data the target data type.
   * \param componentIndex component index.
   */
  template<typename Data, size_t componentIndex>
  Data get() const;

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

protected:
  virtual bool isUnderExecution() const = 0;
#ifndef STATE_DIAGRAM_STRINGLESS
  virtual string curLocalScopePath() const = 0;
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

private:
  virtual SignalDelegateImpl * implUpcast() const = 0;

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  template<typename Data>
  void handleNullPtrError(Signal<Data> const * const signal) const;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

template<typename Data>
class Signal
:
  public virtual Event
{
public:
  virtual void set(Data && data) = 0;
  virtual void set(Data const & data) = 0;
  virtual Data get() const = 0;
};

template<>
class Signal<void>
:
  public virtual Event
{
public:
  template<typename Data>
  void
  set(Data &&)
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

#ifndef STATE_DIAGRAM_STRINGLESS
    throwError<SignalDelegate::SetOnVoidError>();
#else
    STATE_DIAGRAM_HANDLE_ERROR(SignalDelegate::setOnVoidError);
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

  template<typename Data>
  void
  set(Data const &)
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

#ifndef STATE_DIAGRAM_STRINGLESS
    throwError<SignalDelegate::SetOnVoidError>();
#else
    STATE_DIAGRAM_HANDLE_ERROR(SignalDelegate::setOnVoidError);
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

  void
  get()
  const
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

#ifndef STATE_DIAGRAM_STRINGLESS
    throwError<SignalDelegate::GetOnVoidError>();
#else
    STATE_DIAGRAM_HANDLE_ERROR(SignalDelegate::getOnVoidError);
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

  void
  get(size_t const)
  const
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

#ifndef STATE_DIAGRAM_STRINGLESS
    throwError<SignalDelegate::GetOnVoidError>();
#else
    STATE_DIAGRAM_HANDLE_ERROR(SignalDelegate::getOnVoidError);
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

protected:
#ifndef STATE_DIAGRAM_STRINGLESS
  virtual string curLocalScopePath() const = 0;
#endif // STATE_DIAGRAM_STRINGLESS

private:
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

#ifndef STATE_DIAGRAM_STRINGLESS
  template<class Error>
  void
  throwError()
  const
  {
    if (isUnderExecution())
    {
      throw Error(path(), curLocalScopePath());
    }
    else
    {
      throw Error(path(), outsideOfStatemachineIndicator);
    }
  }
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
template<typename Data>
void
Event
::handleNullPtrError(Signal<Data> const * const signal)
const
{
  if (signal == nullptr)
  {
    if (isUnderExecution())
    {
#ifndef STATE_DIAGRAM_STRINGLESS
      throw SignalDelegate::TypeMismatchOnSetError(path(), curLocalScopePath());
#else
      STATE_DIAGRAM_HANDLE_ERROR(SignalDelegate::typeMismatchOnSetError);
#endif // STATE_DIAGRAM_STRINGLESS
    }
    else
    {
#ifndef STATE_DIAGRAM_STRINGLESS
      throw SignalDelegate::TypeMismatchOnSetError(path(), outsideOfStatemachineIndicator);
#else
      STATE_DIAGRAM_HANDLE_ERROR(SignalDelegate::typeMismatchOnSetError);
#endif // STATE_DIAGRAM_STRINGLESS
    }
  }
}
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

template<typename Data>
void
Event
::set(Data && data)
{
  Signal<Data> * const signal{dynamic_cast<Signal<Data> *>(this)};
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  handleNullPtrError<Data>(signal);
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  signal->set(forward<Data>(data));
}

template<typename Data>
void
Event
::set(Data const & data)
{
  Signal<Data> * const signal{dynamic_cast<Signal<Data> *>(this)};
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  handleNullPtrError<Data>(signal);
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  signal->set(data);
}

template<typename Data>
Data
Event
::get()
const
{
  Signal<Data> const * const signal{dynamic_cast<Signal<Data> const *>(this)};
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  handleNullPtrError<Data>(signal);
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  return signal->get();
}

template<typename Data1, typename Data2, typename... RemainingData>
Payload<Data1, Data2, RemainingData...>
Event::get()
const
{
  return get<Payload<Data1, Data2, RemainingData...>>();
}

template<typename Delegate, typename Data>
class ConcreteSignal
:
  public Signal<Data>
{
  template<typename _Data> friend class UnifiedExternalSignal;
  template<typename _Data> friend class UnifiedLocalSignal;

public:
  template<class Parent>
  ConcreteSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent)
  :
    m_delegate{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent}
  , m_data{}
  {
    // This space intentionally left empty
  }

  void
  set(Data && data)
  override
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    if (m_delegate.isUnderExecution())
    {
      if (!m_delegate.isInCurLocalScope())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::ScopeError(m_delegate.path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::scopeError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.isActive())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetOnActivatedError(m_delegate.path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setOnActivatedError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.isSet())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetOnAlreadySetError(m_delegate.path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setOnAlreadySetError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
    }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_data = forward<Data>(data);
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_delegate.markAsSet();
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

  void
  set(Data const & data)
  override
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    if (m_delegate.isUnderExecution())
    {
      if (!m_delegate.isInCurLocalScope())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::ScopeError(m_delegate.path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::scopeError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.isActive())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetOnActivatedError(m_delegate.path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setOnActivatedError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.isSet())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetOnAlreadySetError(m_delegate.path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setOnAlreadySetError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
    }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_data = data;
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_delegate.markAsSet();
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

  Data
  get()
  const override
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    if (m_delegate.isUnderExecution())
    {
      if (!m_delegate.isInCurLocalScope())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::ScopeError(m_delegate.path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::scopeError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (!m_delegate.isSet())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::GetOnNotSetError(m_delegate.path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::getOnNotSetError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
    }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    return m_data;
  }

private:
  SignalDelegateImpl *
  implUpcast()
  const override
  {
    return m_delegate.implUpcast();
  }

#ifndef STATE_DIAGRAM_STRINGLESS
  string
  path()
  const override
  {
    return m_delegate.path();
  }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  bool
  isUnderExecution()
  const override
  {
    return m_delegate.isUnderExecution();
  }

#ifndef STATE_DIAGRAM_STRINGLESS
  string
  curLocalScopePath()
  const override
  {
    return m_delegate.curLocalScopePath();
  }
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  Delegate m_delegate;
  Data m_data;
};

template<typename Delegate>
class ConcreteSignal<Delegate, void>
:
  public Signal<void>
{
  template<typename Data> friend class UnifiedExternalSignal;
  template<typename Data> friend class UnifiedLocalSignal;

public:
  template<class Parent>
  ConcreteSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent)
  :
    m_delegate{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent}
  {
    // This space intentionally left empty
  }

#ifndef STATE_DIAGRAM_STRINGLESS
  string
  path()
  const override
  {
    return m_delegate.path();
  }
#endif // STATE_DIAGRAM_STRINGLESS

private:
  SignalDelegateImpl *
  implUpcast()
  const override
  {
    return m_delegate.implUpcast();
  }

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  bool
  isUnderExecution()
  const override
  {
    return m_delegate.isUnderExecution();
  }

#ifndef STATE_DIAGRAM_STRINGLESS
  string
  curLocalScopePath()
  const override
  {
    return m_delegate.curLocalScopePath();
  }
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  Delegate const m_delegate;
};

/* External events.
 *
 * External events are used to reference external signals that may trigger a transition.
 * They are the same as general events with respect to data payloads.
 */
class ExternalEvent
:
  public virtual Event
{
  friend class TriggerlessTransitionImpl;
  friend class Top;

protected:
  virtual ExternalSignalDelegate::Impl * impl() const = 0;
};

template<typename Data>
class UnifiedExternalSignal
:
  public ConcreteSignal<ExternalSignalDelegate, Data>
, public ExternalEvent
{
public:
  UnifiedExternalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent)
  :
    ConcreteSignal<ExternalSignalDelegate, Data>(STATE_DIAGRAM_STRING_ARG_COMMA(name) parent)
  {
    // This space intentionally left empty
  }

private:
  ExternalSignalDelegate::Impl *
  impl()
  const override
  {
    return ConcreteSignal<ExternalSignalDelegate, Data>::m_delegate.m_impl;
  }
};

//! External signals that carry more than one item as data payload.
/*!
 * External signals are used to trigger transitions from outside a statemachine.
 * An external signal stays activated over the entire
 * duration of a macro-step. It may, thus, trigger an arbitrary number
 * of transitions over the course of the macro-step - it is not consumed and
 * cannot be explicitly deactivated either. It gets deactivated implicitly
 * at the end of the macro-step, that is to say, once no transition is enabled anymore.
 *
 * \param Data the type of data to be carried as payload.
 * \param RemainingData the types of remaining data to be carried as payload.
 */
template<typename Data, typename... RemainingData>
class ExternalSignal
:
  public UnifiedExternalSignal<Payload<Data, RemainingData...>>
{
public:
  //! Construct an external signal.
  /*!
   * \param name the name of the external signal.
   * \param parent the parent of the external signal.
   */
  ExternalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent)
  :
    UnifiedExternalSignal<Payload<Data, RemainingData...>>(STATE_DIAGRAM_STRING_ARG_COMMA(name) parent)
  {
    // This space intentionally left empty
  }

  //! Set the data value of an external signal from l-value references, returning a reference to the signal itself.
  /*!
   * \param data the first l-value reference.
   * \param remainingData the remaining l-value references.
   */
  ExternalSignal const &
  operator()(Data const & data, RemainingData const &... remainingData)
  {
    this->set({data, remainingData...});
    return *this;
  }

  //! Set the data value of an external signal from r-value references, returning a reference to the signal itself.
  /*!
   * \param data the first r-value reference.
   * \param remainingData the remaining r-value references.
   */
  ExternalSignal const &
  operator()(Data && data, RemainingData &&... remainingData)
  {
    this->set({forward<Data>(data), forward<RemainingData>(remainingData)...});
    return *this;
  }
};

//! External signals that carry exactly one item as data payload.
template<typename Data>
class ExternalSignal<Data>
:
  public UnifiedExternalSignal<Data>
{
public:
  //! Construct an external signal.
  /*!
   * \param name the name of the external signal.
   * \param parent the parent of the external signal.
   */
  ExternalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent)
  :
    UnifiedExternalSignal<Data>(STATE_DIAGRAM_STRING_ARG_COMMA(name) parent)
  {
    // This space intentionally left empty
  }

  //! Set the data value of an external signal from an l-value reference, returning a reference to the signal itself.
  /*!
   * \param data the l-value reference.
   */
  ExternalSignal const &
  operator()(Data const & data)
  {
    this->set(data);
    return *this;
  }

  //! Set the data value of an external signal from an r-value reference, returning a reference to the signal itself.
  /*!
   * \param data the r-value reference.
   */
  ExternalSignal const &
  operator()(Data && data)
  {
    this->set(data);
    return *this;
  }
};

//! External signals that do not carry any data payload.
template<>
class ExternalSignal<void>
:
  public UnifiedExternalSignal<void>
{
public:
  //! Construct an external signal that does not carry any data payload.
  /*!
   * \param name the name of the external signal.
   * \param parent the parent of the external signal.
   */
  ExternalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent)
  :
    UnifiedExternalSignal<void>(STATE_DIAGRAM_STRING_ARG_COMMA(name) parent)
  {
    // This space intentionally left empty
  }
};

/* Local events.
 *
 * Local events are used to reference local signals that may trigger a transition.
 * They are the same as general events with respect to data payloads.
 */
class LocalEvent
:
  public virtual Event
{
  friend class Top;
  friend class TriggeredTransitionImpl;
  friend class TriggerlessTransitionImpl;

protected:
  virtual LocalSignalDelegate::Impl * impl() const = 0;
};

template<typename Data>
class UnifiedLocalSignal
:
  public ConcreteSignal<LocalSignalDelegate, Data>
, public LocalEvent
{
public:
  template<class Parent>
  UnifiedLocalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent)
  :
    ConcreteSignal<LocalSignalDelegate, Data>(STATE_DIAGRAM_STRING_ARG_COMMA(name) parent)
  {
    // This space intentionally left empty
  }

private:
  LocalSignalDelegate::Impl *
  impl()
  const override
  {
    return ConcreteSignal<LocalSignalDelegate, Data>::m_delegate.m_impl;
  }
};

//! Local signals.
/*!
 * Besides dependencies established by transition topology,
 * local signals can be used as an additional mechanism for establishing dependencies
 * within a state machine. They serve a means of
 * communication between components that run concurrently to each other, and also
 * as a means of establishing dependencies on sequential execution paths. In all cases, dependencies
 * via local signals
 * can only occur within macro-steps - see the second-to-next paragraph.
 *
 * Local signals have as their scope
 * their respective parent. They may not be activated or used as a trigger outside this scope.
 *
 * Analogously to external signals, a local signal, once activated, stays activated over
 * the entire
 * duration of the macro-step it's activated in. It may, thus, trigger an
 * arbitrary number
 * of transitions over the remainder of the macro-step - it is not consumed and cannot be
 * explicitly deactivated either. It gets deactivated implicitly at the end of the macro step, that is to say,
 * once no transition is enabled anymore.
 *
 * \param Data the type of data to be carried as payload.
 * \param RemainingData the types of remaining data to be carried as payload.
 */
template<typename Data, typename... RemainingData>
class LocalSignal
:
  public UnifiedLocalSignal<Payload<Data, RemainingData...>>
{
public:
  //! Construct a local signal.
  /*!
   * \param name the name of the local signal.
   * \param parent the parent of the local signal.
   */
  template<class Parent>
  LocalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent)
  :
    UnifiedLocalSignal<Payload<Data, RemainingData...>>(STATE_DIAGRAM_STRING_ARG_COMMA(name) parent)
  {
    // This space intentionally left empty
  }

  //! Set the data value of a local signal from l-value references, returning a reference to the signal itself.
  /*!
   * \param data the first l-value reference.
   * \param remainingData the remaining l-value references.
   */
  LocalSignal const &
  operator()(Data const & data, RemainingData const &... remainingData)
  {
    this->set({data, remainingData...});
    return *this;
  }

  //! Set the data value of a local signal from r-value references, returning a reference to the signal itself.
  /*!
   * \param data the r-value reference.
   * \param remainingData the remaining r-value references.
   */
  LocalSignal const &
  operator()(Data && data, RemainingData &&... remainingData)
  {
    this->set({forward<Data>(data), forward<RemainingData>(remainingData)...});
    return *this;
  }
};

//! Local signals that carry exactly one item as data payload.
template<typename Data>
class LocalSignal<Data>
:
  public UnifiedLocalSignal<Data>
{
public:
  //! Construct a local signal.
  /*!
   * \param name the name of the local signal.
   * \param parent the parent of the local signal.
   */
  template<class Parent>
  LocalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent)
  :
    UnifiedLocalSignal<Data>(STATE_DIAGRAM_STRING_ARG_COMMA(name) parent)
  {
    // This space intentionally left empty
  }

  //! Set the data value of a local signal from an l-value reference, returning a reference to the signal itself.
  /*!
   * \param data the l-value reference.
   */
  LocalSignal const &
  operator()(Data const & data)
  {
    this->set(data);
    return *this;
  }

  //! Set the data value of a local signal from an r-value reference, returning a reference to the signal itself.
  /*!
   * \param data the r-value reference.
   */
  LocalSignal const &
  operator()(Data && data)
  {
    this->set(data);
    return *this;
  }
};

//! Local signals that do not carry any data payload.
template<>
class LocalSignal<void>
:
  public UnifiedLocalSignal<void>
{
public:
  //! Construct a local signal that does not carry any data payload.
  /*!
   * \param name the name of the local signal.
   * \param parent the parent of the local signal.
   */
  template<class Parent>
  LocalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent)
  :
    UnifiedLocalSignal<void>(STATE_DIAGRAM_STRING_ARG_COMMA(name) parent)
  {
    // This space intentionally left empty
  }
};

class VarDelegate
:
  private PImplUpcast<VarDelegateImpl>
, public NamePath
{
  template<typename Data, size_t size> friend class ExternalArray;
  template<typename Data, size_t size> friend class LocalArray;
  friend class VarDelegateImpl;
  template<class Delegate, typename Data> friend class Var;

protected:
  class Delegator
  {
  protected:
    virtual ~Delegator();

  public:
    virtual void makeNxtCur() = 0;
  };

  VarDelegate(VarDelegateImpl * const impl, Delegator * const delegator);
  VarDelegate(VarDelegate const &) = delete;

  void operator=(VarDelegate const &) = delete;

  VarDelegateImpl * implUpcast() const;

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  //! Base class for errors thrown on setting or retrieving a variable's data value.
  class Error
  :
    public state_diagram::Error
  {
  protected:
    Error(string const & path);

  public:
    //! The path of the variable.
    string const path;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when the type parameter supplied with a get does not match the actual type of the variable.
  class TypeError
  :
    public Error
  {
    template<class Delegate, typename Data> friend class Var;

  private:
    TypeError(string const & path, string const & scopePath);

  public:
    //! The path of the scope in which the name clash error occurs.
    string const scopePath;

  private:
    string specific() const override;
  };
#else
  static int constexpr typeError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a variable's data value is retrieved without it being valid.
  class GetOnNotValidError
  :
    public Error
  {
    template<class Delegate, typename Data> friend class Var;

  private:
    GetOnNotValidError(string const & path);

    string specific() const override;
  };
#else
  static int constexpr getOnNotValidError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a variable's data value is set after it has already been retrieved during a macro step.
  class SetAfterGetError
  :
    public Error
  {
    template<class Delegate, typename Data> friend class Var;

  private:
    SetAfterGetError(string const & path);

    string specific() const override;
  };
#else
  static int constexpr setAfterGetError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a variable's data value is set again after it has already been set during a macro step.
  class SetOnAlreadySetError
  :
    public Error
  {
    template<class Delegate, typename Data> friend class Var;

  private:
    SetOnAlreadySetError(string const & path);

    string specific() const override;
  };
#else
  static int constexpr setOnAlreadySetError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when a variable's next data value is set again after it has already been set during a macro step.
  class SetNxtOnAlreadySetNxtError
  :
    public Error
  {
    template<class Delegate, typename Data> friend class Var;

  private:
    SetNxtOnAlreadySetNxtError(string const & path);

    string specific() const override;
  };
#else
  static int constexpr setNxtOnAlreadySetNxtError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever a local variable is used out of scope.
  class ScopeError
  :
    public Error
  {
    template<class Delegate, typename Data> friend class Var;

  private:
    ScopeError(string const & varPath, string const & scopePath);

  public:
    //! The path of the scope in which the name clash error occurs.
    string const scopePath;

  private:
    string specific() const override;
  };
#else
  static int constexpr scopeError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

protected:
  template<typename Data>
  void forceSet(Data && data);

private:
  void makeNxtCur() const;

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  bool isValid() const;
  void markAsValid() const;

  bool isSet() const;
  void markAsSet() const;

  bool isSetNxt() const;

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  void markAsSetNxt() const;

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  bool hasBeenRetrieved() const;
  void markAsRetrieved() const;

  bool isUnderExecution() const;

  virtual bool isInCurLocalScope() const = 0;
#ifndef STATE_DIAGRAM_STRINGLESS
  string curLocalScopePath() const;
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  Delegator * const m_delegator;
};

class ExternalVarDelegate
:
  private virtual PImpl<ExternalVarDelegateImpl>
, public VarDelegate
{
  template<class Delegate, typename Data> friend class Var;
  friend class Top;

public:
  ExternalVarDelegate(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent, Delegator * const delegator);

  ~ExternalVarDelegate();

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever the name of an external variable is not unique.
  class NameClashError
  :
    public state_diagram::Error
  {
    friend class TopStateImpl;

  private:
    NameClashError(string const & varName);

  public:
    //! The name of an external variable that already exists in that scope.
    string const varName;

  private:
    string specific() const override;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

private:
  bool isInCurLocalScope() const override;

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

class LocalVarDelegate
:
  private virtual PImpl<LocalVarDelegateImpl>
, public VarDelegate
{
  template<class Delegate, typename Data> friend class Var;

public:
  LocalVarDelegate(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundState const & scope, Delegator * const delegator);
  LocalVarDelegate(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Region const & scope, Delegator * const delegator);

  ~LocalVarDelegate();

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Base class of errors thrown whenever the name of a local variable is not unique.
  class NameClashError
  :
    public state_diagram::Error
  {
  protected:
    NameClashError(string const & scopePath, string const & varName);

  public:
    //! The path of the scope in which the name clash error occurs.
    string const scopePath;

    //! The name of a local variable that already exists in that scope.
    string const varName;

  private:
    string specific() const override;

  protected:
    virtual string scopeTypeIndicator() const = 0;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

private:
  bool isInCurLocalScope() const override;

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

template<typename Delegate, typename Data>
class Var
:
  public VarDelegate::Delegator
{
  template<typename _Data> friend class ExternalVar;
  template<typename _Data> friend class LocalVar;
  template<typename _Data, size_t size> friend class ExternalArray;
  template<typename _Data, size_t size> friend class LocalArray;

protected:
  template<class Parent>
  Var(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent)
  :
    m_delegate{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent, this}
  , m_data{}
  , m_dataNxt{}
  , nxt{*this}
  {
    // This space intentionally left empty
  }

  template<class Parent>
  Var(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent, Data && x)
  :
    Var{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent}
  {
    set(forward<Data>(x));
  }

  template<class Parent>
  Var(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent, Data const & x)
  :
    Var{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent}
  {
    set(x);
  }

public:
  //! Return the variable's path as a string.
  /*!
   * This function may be inefficient. It's mainly intended to be used
   * for generating feedback to the user in case an error has occurred.
   *
   * \return the variable's path as a string.
   */
#ifndef STATE_DIAGRAM_STRINGLESS
  string
  path()
  const
  {
    return m_delegate.path();
  }
#endif // STATE_DIAGRAM_STRINGLESS

  //! Set the variable's data value.
  /*!
   * \param data the data value.
   */
  void
  set(Data && data)
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    if (m_delegate.isUnderExecution())
    {
      if (!m_delegate.isInCurLocalScope())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::ScopeError(path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::scopeError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.isSet())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetOnAlreadySetError(path());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setOnAlreadySetError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.hasBeenRetrieved())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetAfterGetError(path());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setAfterGetError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
    }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_data = forward<Data>(data);
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_delegate.markAsValid();
    m_delegate.markAsSet();
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

  //! Set the variable's data value.
  /*!
   * \param data the data value.
   */
  void
  operator<<(Data && data)
  {
    set(forward<Data>(data));
  }

  //! Set the variable's data value.
  /*!
   * \param data the data value.
   */
  Var
  operator=(Data && data)
  {
    set(forward<Data>(data));
    return *this;
  }

  //! Set the variable's data value.
  /*!
   * \param data the data value.
   */
  void
  set(Data const & data)
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    if (m_delegate.isUnderExecution())
    {
      if (!m_delegate.isInCurLocalScope())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::ScopeError(path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::scopeError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.isSet())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetOnAlreadySetError(path());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setOnAlreadySetError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.hasBeenRetrieved())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetAfterGetError(path());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setAfterGetError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
    }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_data = data;
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_delegate.markAsValid();
    m_delegate.markAsSet();
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

  //! Set the variable's data value.
  /*!
   * \param data the data value.
   */
  void
  operator<<(Data const & data)
  {
    set(data);
  }

  //! Set the variable's next data value.
  /*!
   * \param data the next data value.
   */
  void
  setNxt(Data && data)
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    if (m_delegate.isUnderExecution())
    {
      if (!m_delegate.isInCurLocalScope())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::ScopeError(path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::scopeError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.isSetNxt())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetNxtOnAlreadySetNxtError(path());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setNxtOnAlreadySetNxtError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
    }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_dataNxt = forward<Data>(data);
    m_delegate.markAsSetNxt();
  }

  //! Set the variable's next data value
  /*!
   * \param data the next data value.
   */
  void
  setNxt(Data const & data)
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    if (m_delegate.isUnderExecution())
    {
      if (!m_delegate.isInCurLocalScope())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::ScopeError(path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::scopeError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (m_delegate.isSetNxt())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::SetNxtOnAlreadySetNxtError(path());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::setNxtOnAlreadySetNxtError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
    }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_dataNxt = data;
    m_delegate.markAsSetNxt();
  }

  //! Retrieve the variable's data value
  /*!
   * \return the variable's data value.
   */
  Data
  get()
  const
  {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    if (m_delegate.isUnderExecution())
    {
      if (!m_delegate.isInCurLocalScope())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::ScopeError(path(), m_delegate.curLocalScopePath());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::scopeError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      if (!m_delegate.isValid())
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw typename Delegate::GetOnNotValidError(path());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Delegate::getOnNotValidError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
      m_delegate.markAsRetrieved();
    }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    return m_data;
  }

protected:
  void
  forceSet(Data && data)
  {
    m_data = forward<Data>(data);
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_delegate.markAsValid();
    m_delegate.markAsSet();
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

  void
  forceSet(Data const & data)
  {
    m_data = data;
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
    m_delegate.markAsValid();
    m_delegate.markAsSet();
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  }

  operator Data()
  {
    return get();
  }

private:
  SignalDelegateImpl *
  implUpcast()
  const
  {
    return m_delegate.implUpcast();
  }

  Delegate m_delegate;
  Data m_data;
  Data m_dataNxt;

public:
  class Nxt
  {
    friend class Var;

  public:
    //! Set the variable's next data value.
    /*!
     * \param data the next data value.
     */
    void
    operator<<(Data && data)
    const
    {
      m_cur.setNxt(forward<Data>(data));
    }

    //! Set the variable's next data value.
    /*!
     * \param data the next data value.
     */
    void
    operator<<(Data const & data)
    const
    {
      m_cur.setNxt(data);
    }

  private:
    Nxt(Var & cur)
    :
      m_cur{cur}
    {
      // This space intentionally left empty
    }

    Var & m_cur;
  };

  //! A wrapper allowing to set the variable's next data value via an operator.
  Nxt const nxt;

private:
  void
  makeNxtCur()
  override
  {
    m_data = m_dataNxt;
  }
};

// External variables.
/*!
 * \param Data the type of data carried to be carried as values.
 */
template<typename Data>
class ExternalVar
:
  public Var<ExternalVarDelegate, Data>
{
public:
  // Construct an external data variable.
  /*!
   * \param name the name of the external variable.
   * \param parent the parent of the external variable.
   */
  ExternalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent)
  :
    Var<ExternalVarDelegate, Data>{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent}
  {
    // This space intentionally left empty
  }

  // Construct an external data variable supplying an initial data value.
  /*!
   * \param name the name of the external variable.
   * \param parent the parent of the external variable.
   * \param x the initial data value.
   */
  ExternalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent, Data && x)
  :
    Var<ExternalVarDelegate, Data>{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent, forward<Data>(x)}
  {
    // This space intentionally left empty
  }

  // Construct an external data variable supplying an initial data value.
  /*!
   * \param name the name of the external variable.
   * \param parent the parent of the external variable.
   * \param x the initial data value.
   */
  ExternalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent, Data const & x)
  :
    Var<ExternalVarDelegate, Data>{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent, x}
  {
    // This space intentionally left empty
  }
};


// Local variables.
/*!
 * \param Data the type of data carried to be carried as values.
 */
template<typename Data>
class LocalVar
:
  public Var<LocalVarDelegate, Data>
{
public:
  // Construct a local data variable.
  /*!
   * \param Parent the type of the parent the local variable.
   * \param name the name of the external variable.
   * \param parent the parent of the external variable.
   */
  template<class Parent>
  LocalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent)
  :
    Var<LocalVarDelegate, Data>{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent}
  {
    // This space intentionally left empty
  }

  // Construct a local data variable supplying an initial data value.
  /*!
   * \param Parent the type of the parent the local variable.
   * \param name the name of the external variable.
   * \param parent the parent of the external variable.
   * \param x the initial data value.
   */
  template<class Parent>
  LocalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent, Data && x)
  :
    Var<LocalVarDelegate, Data>{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent, forward<Data>(x)}
  {
    // This space intentionally left empty
  }

  // Construct a local data variable supplying an initial data value.
  /*!
   * \param Parent the type of the parent the local variable.
   * \param name the name of the external variable.
   * \param parent the parent of the external variable.
   * \param x the initial data value.
   */
  template<class Parent>
  LocalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Parent const & parent, Data const & x)
  :
    Var<LocalVarDelegate, Data>{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent, x}
  {
    // This space intentionally left empty
  }
};

//! Arrays of data variables.
class Array
{
public:
  //! Destruct an array of variables.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  virtual ~Array();

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever an array index is found to be out of bounds.
  class IdxOutOfBoundsError
  :
    public Error
  {
    template<typename Data, size_t size> friend class ExternalArray;
    template<typename Data, size_t size> friend class LocalArray;

  private:
    IdxOutOfBoundsError(string const & arrayPath, size_t const capacity, size_t const idx);

  public:
    //! The path of the array.
    string const path;

    //! The capacity of the array.
    size_t const capacity;

    //! The index that is out of bounds.
    size_t const idx;

  private:
    string specific() const override;
  };
#else
  static int constexpr idxOutOfBoundsError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
};

//! Arrays of external data variables.
/*!
 * \param capacity the capacity of the array.
 */
template<typename Data, size_t size>
class ExternalArray
:
  public Array
{
public:
  //! Construct an array of external variables.
  /*!
   * \param name the base name of the external variables contained in the array.
   * \param parent the parent of the external variables contained in the array.
   */
  ExternalArray(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent)
  {
    assert (size > 0);
    for (size_t idx = 0; idx < size; ++idx)
    {
      ExternalVar<Data> * const elem{new (&m_backing[idx * sizeof(ExternalVar<Data>)]) ExternalVar<Data>(STATE_DIAGRAM_STRING_ARG_COMMA(name + '_' + to_string(idx)) parent)};
      elem->m_data = forward<Data>(Data());
      elem->m_dataNxt = forward<Data>(Data());
    }
  }

  //! Construct an array of external variables and, at the same time, assign initial values to array elements.
  /*!
   * \param name the base name of the external variables contained in the array.
   * \param parent the parent of the external variables contained in the array.
   * \param initVal the initial values.
   */
  ExternalArray(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent, array<Data, size> const & initVal)
  :
    ExternalArray(STATE_DIAGRAM_STRING_ARG_COMMA(name) parent)
  {
    for (size_t idx = 0; idx < size; ++idx)
    {
      operator[](idx).set(initVal[idx]);
    }
  }

  //! Destruct array of external variables.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~ExternalArray()
  {
    for (size_t idx = 0; idx < size; ++idx)
    {
      operator[](idx).~ExternalVar<Data>();
    }
  }

  //! Accessing an external variable.
  /*!
   * \param idx the index of the external variable.
   */
  ExternalVar<Data> &
  operator[](size_t const idx)
  {
    if (idx >= size)
    {
#ifndef STATE_DIAGRAM_STRINGLESS
      throw IdxOutOfBoundsError(operator[](0).path(), size, idx);
#else
      STATE_DIAGRAM_HANDLE_ERROR(idxOutOfBoundsError);
#endif // STATE_DIAGRAM_STRINGLESS
    }
    return *reinterpret_cast<ExternalVar<Data> *>(&m_backing[idx * sizeof(ExternalVar<Data>)]);
  }

private:
  alignas(alignof(ExternalVar<Data>[size])) char m_backing[size * sizeof(ExternalVar<Data>)];
};

//! Arrays of local data variables.
/*!
 * \param capacity the capacity of the array.
 */
template<typename Data, size_t capacity>
class LocalArray
:
  public Array
{
public:
  //! Construct an array of local variables. The local variables have a compound state as their scope.
  /*!
   * \param name the base name of the local variables contained in the array.
   * \param scope the scope of the local variables contained in the array.
   */
  LocalArray(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundState const & scope)
  {
    assert (capacity > 0);
    for (size_t idx = 0; idx < capacity; ++idx)
    {
      LocalVar<Data> * const elem{new (&m_backing[idx * sizeof(LocalVar<Data>)]) LocalVar<Data>(STATE_DIAGRAM_STRING_ARG_COMMA(name + '_' + to_string(idx)) scope)};
      elem->m_data = forward<Data>(Data());
      elem->m_dataNxt = forward<Data>(Data());
    }
  }

  //! Construct an array of external variables and, at the same time, assign initial values to array elements.
  /*!
   * \param name the base name of the external variables contained in the array.
   * \param scope the scope of the local variables contained in the array.
   * \param initVal an array of initial values.
   */
  LocalArray(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundState const & scope, array<Data, capacity> const & initVal)
  :
    LocalArray(STATE_DIAGRAM_STRING_ARG_COMMA(name) scope)
  {
    for (size_t idx = 0; idx < capacity; ++idx)
    {
      operator[](idx).forceSet(initVal[idx]);
    }
  }

  //! Construct an array of local variables. The local variables have a region as their scope.
  /*!
   * \param name the base name of the local variables contained in the array.
   * \param scope the scope of the local variables contained in the array.
   */
  LocalArray(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Region const & scope)
  {
    assert (capacity > 0);
    for (size_t idx = 0; idx < capacity; ++idx)
    {
      LocalVar<Data> * const elem{new (&m_backing[idx * sizeof(LocalVar<Data>)]) LocalVar<Data>(STATE_DIAGRAM_STRING_ARG_COMMA(name + '_' + to_string(idx)) scope)};
      elem->m_data = forward<Data>(Data());
      elem->m_dataNxt = forward<Data>(Data());
    }
  }

  //! Construct an array of external variables and, at the same time, assign initial values to array elements.
  /*!
   * \param name the base name of the external variables contained in the array.
   * \param scope the scope of the local variables contained in the array.
   * \param initVal an array of initial values.
   */
  LocalArray(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Region const & scope, array<Data, capacity> const & initVal)
  :
    LocalArray(STATE_DIAGRAM_STRING_ARG_COMMA(name) scope)
  {
    for (size_t idx = 0; idx < capacity; ++idx)
    {
      operator[](idx).forceSet(initVal[idx]);
    }
  }

  //! Destruct array of local variables.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~LocalArray()
  {
    for (size_t idx = 0; idx < capacity; ++idx)
    {
      operator[](idx).~LocalVar<Data>();
    }
  }

  //! Accessing a local variable.
  /*!
   * \param idx the index of the external variable.
   */
  LocalVar<Data> &
  operator[](size_t const idx)
  {
    if (idx >= capacity)
    {
#ifndef STATE_DIAGRAM_STRINGLESS
      throw IdxOutOfBoundsError(operator[](0).path(), capacity, idx);
#else
      STATE_DIAGRAM_HANDLE_ERROR(idxOutOfBoundsError);
#endif // STATE_DIAGRAM_STRINGLESS
    }
    return *reinterpret_cast<LocalVar<Data> *>(&m_backing[idx * sizeof(LocalVar<Data>)]);
  }

private:
  alignas(alignof(LocalVar<Data>[capacity])) char m_backing[capacity * sizeof(LocalVar<Data>)];
};

//! A concurrent region of a sub-divisible state.
/*!
 * Every sub-divisible state has at least one concurrent region.
 * Whenever sub-states are directly attached to a sub-divisible state,
 * then a default region is generated and attached to that state automatically.
 * The default region becomes the parent of all directly attached sub-states,
 * so direct attachment is form of syntactic sugar. Once a sub-divisible
 * state has a default region, attempting to attach any further
 * (non-default) region
 * leads to an exception being thrown.
 */
class Region
:
  private virtual PImpl<RegionImpl>
, public NamePath
{
  friend class CompoundStateImpl;
  friend class Init;
  friend class Final;
  friend class State;
  friend class Connector;
  friend class LocalSignalDelegate;
  friend class LocalVarDelegate;

public:
# define STATE_DIAGRAM_DEFAULT_NAME_OF_REGION "REGION"
private:
  static string const defaultName;

public:
  //! Construct a region with a compound state as parent.
  /*!
   * \param name the region's name.
   * \param parent the region's parent.
   */
  Region(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundState const & parent);

  //! Destruct region.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~Region();

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown on a name clash among signals local to a region.
  class LocalSignalNameClashError
  :
    public LocalSignalDelegate::NameClashError
  {
    friend class RegionImpl;

  private:
    LocalSignalNameClashError(string const & scopePath, string const & signalName);

    string scopeTypeIndicator() const;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown on a name clash among variables local to a region.
  class LocalVarNameClashError
  :
    public LocalVarDelegate::NameClashError
  {
    friend class RegionImpl;

  private:
    LocalVarNameClashError(string const & scopePath, string const & varName);

    string scopeTypeIndicator() const;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Base class for region-related errors other than name clash errors on local signals or variables.
  class Error
  :
    public state_diagram::Error
  {
  protected:
    Error(string const & regionPath);

  public:
    //! The region's path.
    string const regionPath;

    class SubStateNameClash;
    class MissingInit;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
#else
  static int constexpr error_missingInit{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
};

#ifndef STATE_DIAGRAM_STRINGLESS
//! Error thrown on a name clash among the sub-states of a region.
class Region::Error::SubStateNameClash
:
  public Region::Error
{
  friend class RegionImpl;

private:
  SubStateNameClash(string const & regionPath, string const & subStateName);

public:
  //! The name of the sub-state(s).
  string const subStateName;

private:
  string specific() const override;
};

//! Error thrown whenever a region requiring an initial state does not have any.
/*!
 * At execution time, a region may be found to require an initial state. The error is thrown
 * whenever the region
 * does not have any initial state in that situation.
 */
class Region::Error::MissingInit
:
  public Region::Error
{
  friend class RegionImpl;

private:
  MissingInit(string const & regionPath);

  string specific() const override;
};
#endif // STATE_DIAGRAM_STRINGLESS

  //! Base class of all sub-divisible or, equivalently, compound states.
/*!
 * Compound states may consist of one or more regions each one containing sub-states.
 * If more than one region is present, then these regions are executed concurrently.
 */
class CompoundState
:
  private PImplUpcast<CompoundStateImpl>
, public virtual NamePath
{
  friend class Region;
  friend class LocalSignalDelegate;
  friend class LocalVarDelegate;
  friend class Init;
  friend class Final;
  friend class State;
  friend class Connector;

protected:
  CompoundState(CompoundStateImpl * const impl);

  CompoundStateImpl * implUpcast() const;

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown on a name clash among signals local to a compound state.
  class LocalSignalNameClashError
  :
    public LocalSignalDelegate::NameClashError
  {
    friend class CompoundStateImpl;

  private:
    LocalSignalNameClashError(string const & scopePath, string const & signalName);

    string scopeTypeIndicator() const;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown on a name clash among variables local to a compound state.
  class LocalVarNameClashError
  :
    public LocalVarDelegate::NameClashError
  {
    friend class CompoundStateImpl;

  private:
    LocalVarNameClashError(string const & scopePath, string const & signalName);

    string scopeTypeIndicator() const;
  };
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Base class for region-related errors within the parent of compound states.
  class RegionError
  :
    public Error
  {
  protected:
    RegionError(string const & compoundStatePath);

  public:
    //! The compound state's path.
    string const compoundStatePath;

    class DefaultRegionRequest;
    class Insertion;
  };
#else
  static int constexpr regionError_defaultRegionRequest{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
  static int constexpr regionError_insertion_hasDefaultRegion{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
};

#ifndef STATE_DIAGRAM_STRINGLESS
//! Error thrown whenever a default region is requested on a compound state that already has non-default regions.
class CompoundState::RegionError::DefaultRegionRequest
:
  public CompoundState::RegionError
{
  friend class CompoundStateImpl;

private:
  DefaultRegionRequest(string const & compoundStatePath, size_t const nrOfNonDefaultRegions);

public:
  //! The number of non-default regions that have already been added.
  size_t const nrOfNonDefaultRegions;

private:
  string specific() const override;
};

//! Base class of errors thrown on adding a region to a compound state.
class CompoundState::RegionError::Insertion
:
  public CompoundState::RegionError
{
protected:
  Insertion(string const & compoundStatePath, string const & regionName);

public:
  // The name of the region.
  string const regionName;

  class HasDefaultRegion;
  class NameClash;
};

//! Error thrown on adding a region to a compound state that already has a default region.
class CompoundState::RegionError::Insertion::HasDefaultRegion
:
  public CompoundState::RegionError::Insertion
{
  friend class CompoundStateImpl;

private:
  HasDefaultRegion(string const & compoundStatePath, string const & regionName);

  string specific() const override;
};

//! Error thrown on a name clash among the regions belonging to a compound state.
class CompoundState::RegionError::Insertion::NameClash
:
  public CompoundState::RegionError::Insertion
{
  friend class CompoundStateImpl;

private:
  NameClash(string const & compoundStatePath, string const & regionName);

  string specific() const override;
};
#endif // STATE_DIAGRAM_STRINGLESS

  //! Base class of all states other than top states.
class SubState
:
  private PImplUpcast<SubStateImpl>
, public virtual NamePath
{
protected:
  SubState(SubStateImpl * const impl);

public:
  bool isCurrent() const;
};

//! Base class of states that are admissible as the source state of some external transition.
class SourceState
:
  private PImplUpcast<SourceStateImpl>
, public virtual SubState
{
  friend class Auto;
  friend class Step;

protected:
  SourceState(SourceStateImpl * const impl);

  SourceStateImpl * implUpcast() const;
};

//! Base class of states that are admissible as the target state of some external transition.
class TargetState
:
  private PImplUpcast<TargetStateImpl>
, public virtual SubState
{
  friend class Auto;
  friend class Step;

protected:
  TargetState(TargetStateImpl * const impl);

  TargetStateImpl * implUpcast() const;
};

//! Top states or, equivalently, entire state machines.
class Top
:
  private virtual PImpl<TopStateImpl>
, public CompoundState
{
  friend class ExternalSignalDelegate;
  friend class ExternalEvent;
  friend class ExternalVarDelegate;

public:
  //! Construct a top state.
  /*
   * \parameter name the name of the top state.
   */
  Top(STATE_DIAGRAM_STRING_PARAM(name));

  //! Destruct top state.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~Top();

  //! Initializing a top state.
  /*!
   * The initialization function needs be called once before executing macro steps.
   */
  void init() const;

  //! Executing a macro step, that is to say, as many micro steps as possible.
  /*!
   * Executing a macro step consists of transition executions. This process continues until
   * no transitions are enabled anymore, or forever if the state machine does not run out
   * of enabled transitions.
   *
   * \return true if the state machine enters an overall terminal state as a result of the macro step, false if not.
   */
  bool step() const;

  //! Executing a macro step supplying external signals that are to be activated.
  /*!
   * Executing a macro step consists of transition firings. These firings continue until
   * no transitions are enabled anymore, or forever if the state machine does not run out
   * of enabled transitions.
   *
   * \param trigger the first external signal that is to be activated as trigger.
   * \param remainingTriggers the remaining external signals that are to be activated as triggers.
   *
   * \return true if the state machine assumes an overall terminal state as a result of the macro step, false if not.
   */
  template<class E, class... Es>
  bool step(E const & trigger, Es const &... remainingTriggers) const;

#ifndef STATE_DIAGRAM_STRINGLESS
  class RequestForCurLocalScopeError
  :
    public Error
  {
    friend class TopStateImpl;

  private:
    RequestForCurLocalScopeError(string const & localVarPath);

  public:
    string const localVarPath;

  private:
    string specific() const override;
  };
#else
  static int constexpr requestForCurLocalScopeError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

private:
  void activate(ExternalEvent const & trigger) const;
};

template<class E, class... Es>
bool
Top
::step(E const & trigger, Es const &... remainingTriggers)
const
{
  activate(trigger);
  return step(remainingTriggers...);
}

//! Initial states.
/*!
 * Every region can have at most one initial state. The
 * initial state cannot be
 * the target state of any transition and cannot be sub-divided either.
 * The initial state of some region, R say, becomes current within
 * R whenever the parent state of R becomes
 * current by way of it being the target of some transition being executed,
 * the top state always being current. If R
 * does not have any initial state, then control flow can
 * enter R only by way of state-boundary crossing transitions that target
 * sub-states of R directly.
 */
class Init
:
  private virtual PImpl<InitStateImpl>
, public SourceState
{
public:
  //! The name of an initial state as a macro.
# define STATE_DIAGRAM_NAME_OF_INITIAL_STATE "INIT"

  //! The name of an initial state as a string constant.
  static string const name;

  //! Construct an initial state inside a default region of the parent state.
  /*!
   * If the parent state already has a default region from a similar constructor
   * invocation, then that region is used; if not, then a default region is created.
   *
   * \param parent the parent compound state.
   */
  Init(CompoundState const & parent);

  //! Construct an initial state inside a parent region that is explicitly referred-to.
  /*!
   * \param parent the parent region.
   */
  Init(Region const & parent);

  //! Destruct initial state.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~Init();
};

//! Final states.
/*!
 * Every region can have at most one final state. The
 * final state cannot be
 * the source state of any transition and cannot be sub-divided either.
 */
class Final
:
  private virtual PImpl<FinalStateImpl>
, public TargetState
{
public:
  //! The name of a final state as a macro.
# define STATE_DIAGRAM_NAME_OF_FINAL_STATE "FINAL"

  //! The name of a final state as a string constant.
  static string const name;

  //! Construct a final state inside a default region of the parent state.
  /*!
   * If the parent state already has a default region from a similar constructor
   * invocation, then that region is used; if not, then a default region is created.
   *
   * \param parent the parent compound state.
   */
  Final(CompoundState const & parent);

  //! Construct a final state inside a parent region that is explicitly referred-to.
  /*!
   * \param parent the parent region.
   */
  Final(Region const & parent);

  //! Destruct final state.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~Final();
};

//! Ordinary sub-divisible states.
/*!
 * Ordinary sub-divisible states are simply called states if no confusion can arise.
 * The first layer of sub-division is always made up of zero or more regions.
 * These regions contain the second layer, which consists of the sub-states of the parent state.
 * These sub-states can be sub-divisible again and so on. Recursion stops at states
 * that do not have any child regions.
 * In all cases where
 * there is more
 * than one region attached to a state, these regions are executed concurrently to each other.
 *
 * States can appear both as the source and as the target of transitions - the so-called
 * external transitions.
 * Furthermore, they can have enter-, exit- and internal step transitions attached
 * to them - the so-called single-state transitions.
 */
class State
:
  private virtual PImpl<StateImpl>
, public CompoundState
, public SourceState
, public TargetState
{
  friend class Step;
  friend class Enter;
  friend class Exit;
  friend class InternalAuto;
  friend class InternalStep;

public:
  //! Construct a state inside a default region of the parent state.
  /*!
   * \param name the name of the state.
   * \param the parent state.
   */
  State(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundState const & parent);

  //! Construct a state inside a parent region that is explicitly referred to.
  /*!
   * \param name the name of the state.
   * \param the parent region.
   */
  State(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Region const & parent);

  //! Destruct state.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~State();

  //! Test whether a state has terminated.
  /*!
   * This predicate provided for usage in transition guards.
   * A state has terminated if, and only if, the current states of all of its child regions
   * are terminal states.
   */
  bool hasTerminated() const;
};

//! Connector states.
/*!
 * Connector states can appear both as the source and as the target of transitions - the so-called
 * external transitions.
 */
class Connector
:
  private virtual PImpl<ConnectorStateImpl>
, public SourceState
, public TargetState
{
  friend class Step;

public:
  //! Construct a state inside a default region of the parent state.
  /*!
   * \param name the name of the state.
   * \param the parent state.
   */
  Connector(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundState const & parent);

  //! Construct a state inside a parent region that is explicitly referred to.
  /*!
   * \param name the name of the state.
   * \param the parent region.
   */
  Connector(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Region const & parent);

  //! Destruct state.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~Connector();

  //! Test whether a state has terminated.
  /*!
   * This predicate provided for usage in transition guards.
   * A state has terminated if, and only if, the current states of all of its child regions
   * are terminal states.
   */
  bool hasTerminated() const;
};

//! Base class of all transitions.
class Transition
:
  private PImplUpcast<TransitionImpl>
{
  friend class Action;
  friend class CompletionFlag;
  friend class FreezeFlag;
  friend class Guard;
  friend class Max1Flag;
  friend class Output;
  friend class Trigger;

protected:
  Transition(TransitionImpl * const impl);

  virtual ~Transition();

  TransitionImpl * implUpcast() const;

public:
  //! Base class of all transition specs.
  /*!
   * Transition specs are annotations that can be added to transitions to modify their behavior in various ways.
   */
  class Spec
  {
    friend class Transition;
    friend class TriggerlessTransition;
    friend class TriggeredTransition;

  protected:
    virtual ~Spec();

    virtual void join(Transition const * const) const = 0;

  public:
#ifndef STATE_DIAGRAM_STRINGLESS
    //! Error thrown whenever a trigger spec is added to a trigger-less transition.
    class TriggerSpecOnTriggerlessTransitionError
    :
      public Error
    {
      friend class TriggerlessTransitionImpl;

    private:
      TriggerSpecOnTriggerlessTransitionError
      (
        string const & transitionTypeIndicator
      , string const & originPath
      );

    public:
      //! The type of the transition.
      string const transitionTypeIndicator;

      //! The path to the origin of the transition.
      string const originPath;

    protected:
      string specific() const override;
    };
#else
    static int constexpr triggerSpecOnTriggerlessTransitionError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
    //! Error thrown whenever a trigger-dependent spec is added to a trigger-less transition.
    class TriggeredSpecOnTriggerlessTransitionError
    :
      public Error
    {
      friend class TriggerlessTransitionImpl;

    private:
      TriggeredSpecOnTriggerlessTransitionError
      (
        string const & specTypeIndicator
      , string const & transitionTypeIndicator
      , string const & originPath
      );

    public:
      //! The type of spec.
      string const specTypeIndicator;

      //! The type of the transition .
      string const transitionTypeIndicator;

      //! The path to the origin of the transition.
      string const originPath;

    private:
      string specific() const override;
    };
#else
    static int constexpr triggeredSpecOnTriggerlessTransitionError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
  };

  //! Add a spec to a transition.
  /*!
   * \param spec the spec.
   * \return the transition itself.
   */
  Transition const & add(Spec const & spec) const;

  //! Add a spec to a transition.
  /*!
   * \param spec the spec.
   * \return the transition itself.
   */
  Transition const & operator<<(Spec const & spec) const;

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Base class for signal scope errors.
  class ScopeError
  :
    public Error
  {
  protected:
    ScopeError(string const & signalPath, string const & scopePath);

  public:
    //! The path to the signal.
    string const signalPath;

    //! The path to the scope in which the error occurs, meaning that the signal ought to be visible in this scope but isn't.
    string const scopePath;

    class Output;
  };
#else
    static int constexpr scopeError_output{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
};

#ifndef STATE_DIAGRAM_STRINGLESS
//! Error thrown when a transition activates a signal that lies out of scope.
class Transition::ScopeError::Output
:
  public Transition::ScopeError
{
  friend class TriggerlessTransitionImpl;
  friend class TriggeredTransitionImpl;

private:
  Output(string const & signalPath, string const & scopePath);

  string specific() const override;
};
#endif // STATE_DIAGRAM_STRINGLESS

//! Base class for enter-, exit- and internal step transitions.
class SingleStateTransition
:
  public virtual Transition
{
protected:
  SingleStateTransition(SingleStateTransitionImpl * const impl);
};

//! Base class for auto and step transitions.
class ExternalTransition
:
  public virtual Transition
{
protected:
  ExternalTransition(ExternalTransitionImpl * const impl);

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown when the source and the target of an external transition do not lie in regions that are related via direct descent.
  /*!
   * Direct descent means that the two regions either are the same or that one
   * of them is an ancestor of the other in the state-region hierarchy. Direct descent
   * is violated if, and only if, the two regions are concurrent to each other.
   * Avoiding the error being thrown means to avoid transitions that go across
   * concurrency boundaries.
   */
  class DescendanceError
  :
    public Error
  {
    friend class ExternalTransitionImpl;

  private:
    DescendanceError(string const & sourcePath, string const & targetPath);

  public:
    //! The path of the source state.
    string const sourcePath;

    //! The path of the target state.
    string const targetPath;

  private:
    string specific() const override;
  };
#else
    static int constexpr descendanceError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
};

//! Base class for trigger-less transitions.
class TriggerlessTransition
:
  public virtual Transition
{
protected:
  TriggerlessTransition(TriggerlessTransitionImpl * const impl);
};

//! Base class for triggered transitions.
class TriggeredTransition
:
  public virtual Transition
{
protected:
  TriggeredTransition(TriggeredTransitionImpl * const impl);

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever a triggered transition is being added a trigger that lies out of scope.
  /*!
   * The source state of the transition determines which signals lie in scope as potential triggers.
   */
  class TriggerScopeError
  :
    public Transition::ScopeError
  {
    friend class TriggeredTransitionImpl;

  private:
    TriggerScopeError(string const & signalPath, string const & scopePath);

    string specific() const override;
  };
#else
    static int constexpr triggerScopeError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
};

//! Designators for the two types of freezes.
/*!
 * A transition labeled with a freeze flag carrying the designator, FULL, freezes its source
 * state and every state beneath it. No exit transitions are executed as the transition is executed.
 * Once the state is re-entered, no enter transitions are executed and all current states are
 * the same as before the freeze.
 *
 * A transition labeled with a freeze flag carrying the designator, SHALLOW, freezes the level just
 * beneath its source state - so that the current state of each direct child region of the source
 * state is frozen in place.
 * All levels below that are exited and entered again like normal.
 *
 * The freeze condition can be overridden on re-entry. To override it, use a transition that crosses
 * the state boundary, directly targeting some sub-state.
 */
enum FreezeDepth {FULL, SHALLOW};

//! Freeze flags that can be added as specs to transitions.
class FreezeFlag
:
  public Transition::Spec
{
  friend class Transition;

public:
  FreezeFlag(FreezeDepth const freezeDepth);

  FreezeDepth const freezeDepth;

private:
  void join(Transition const * const) const override;

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever a freeze flag is added to a single-state transition.
  class FreezeFlagOnSingleStateTransitionError
  :
    public Error
  {
    friend class FreezeFlag;

  private:
    FreezeFlagOnSingleStateTransitionError(string const & hostPath);

  public:
    //! The path of the state that hosts the transition.
    string const hostPath;

  private:
    string specific() const override;
  };
#else
    static int constexpr freezeFlagOnSingleStateTransitionError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever a freeze flag is added to a auto transition.
  class FreezeFlagOnAutoTransitionError
  :
    public Error
  {
    friend class FreezeFlag;

  private:
    FreezeFlagOnAutoTransitionError(string const & sourcePath, string const & targetPath);

  public:
    //! The path of the source state.
    string const sourcePath;

    //! The path of the target state.
    string const targetPath;

  private:
    string specific() const override;
  };
#else
  static int constexpr freezeFlagOnAutoTransitionError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
};

//! Max-1 flags that can be added as specs to transitions.
/*!
 * A max-1 flag specifies that the transition can be executed at most once per macro step.
 * A typical use would consist of adding a max-1 flag to a reset transition, where
 * source and target state are the same. Another typical use would consist of adding a max-1 flag
 * to an internal step transition to prevent it from leading to livelock.
 */
class Max1Flag
:
  public Transition::Spec
{
  friend class Transition;

private:
  void join(Transition const * const) const override;

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever a max-1 flag is added to an enter- or exit-transition.
  class Max1FlagOnBoundaryTransitionError
  :
    public Error
  {
    friend class Max1Flag;

  private:
    Max1FlagOnBoundaryTransitionError(string const & hostPath);

  public:
    //! The path of the state that hosts the transition.
    string const hostPath;

  private:
    string specific() const override;
  };
#else
  static int constexpr max1FlagOnBoundaryTransitionError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
};

//! Completion flags that can be added as specs to transitions.
/*!
 * A completion flag specifies that the transition becomes enabled only once
 * the source state has terminated - in addition to any guards the transition might have.
 * Completion flags may be added to auto transitions.
 * Adding a completion flag to any other type of transition leads to an error being thrown.
 */
class CompletionFlag
:
  public Transition::Spec
{
  friend class Transition;

private:
  void join(Transition const * const) const override;

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  //! Error thrown whenever a completion flag is added to a single-state transition.
  class CompletionFlagOnSingleStateTransitionError
  :
    public Error
  {
    friend class CompletionFlag;

  private:
    CompletionFlagOnSingleStateTransitionError(string const & hostPath);

  public:
    //! The path of the state that hosts the transition.
    string const hostPath;

  private:
    string specific() const override;
  };
#else
  static int constexpr completionFlagOnSingleStateTransitionError{STATE_DIAGRAM_PER_COMPILATION_UNIQUE_ID};
#endif // STATE_DIAGRAM_STRINGLESS
};

//! Trigger specs that can be added to triggered transitions.
/*!
 * A triggered transition can have more than one trigger signal. Besides all other
 * preconditions (control flow and guards), the transition is enabled whenever
 * at least one trigger signal is activated.
 */
class Trigger
:
  public Transition::Spec
{
  friend class Transition;

public:
  //! Construct a trigger spec.
  /*!
   * \param trigger the trigger signal.
   */
  Trigger(Event const & trigger);

  //! The trigger signal.
  Event const & signal;

private:
  void join(Transition const * const) const override;
};

//! Guard specs that can be added to transitions.
/*!
 * A guard spec is constructed from a function that returns a truth value.
 * To check the guard at any given point of time, the function is evaluated.
 * All guards that belong to a transition
 * must return true for the transition to be enabled. If the transition is a triggered transition,
 * then
 * checking whether it is enabled entails looking for a trigger signal that is activated and,
 * at the same time, makes all guards return true with
 * triggered guards receiving the trigger signal as argument.
 *
 * At execution time, guards may be evaluated in any order. In particular, they may be evaluated in an order
 * that differs from the one in which they were added to the transition. It is recommended to
 * keep guards free of visible side effects.
 */
class Guard
:
  public Transition::Spec
{
  friend class Transition;
  friend class TriggerlessTransitionImpl;
  friend class TriggeredTransitionImpl;

public:
  //! Construct a trigger-less guard spec.
  /*!
   * \param triggerlessGuard the guard function.
   */
  Guard(function<bool ()> const & triggerlessGuard);

  //! Construct a triggered guard spec.
  /*!
   * \param triggeredGuard the guard function.
   */
  Guard(function<bool (Event const &)> const & triggeredGuard);

private:
  TriggerlessGuard const triggerless;
  TriggeredGuard const triggered;

  void join(Transition const * const) const override;
};

//! Output specs that can be added to transitions.
/*!
 * An output spec is constructed from a reference to a const signal, or a function that returns
 * a reference to a const signal. An output spec is executed whenever the transition it belongs
 * to is executed. Its effect consists of activating the signal referred-to for the remainder of the macro step.
 * (Output specs might, thus, better be called activation specs. It is for historical reasons
 * that the term used is kept here.)
 *
 * At execution time, all output specs that have been added to a transition are executed. They may be
 * executed in any order. In particular, they may be executed in an order
 * that differs from the one in which they were added to the transition. It is recommended to
 * keep output spec functions free of visible side effects.
 */
class Output
:
  public Transition::Spec
{
  friend class Transition;
  friend class TriggerlessTransitionImpl;
  friend class TriggeredTransitionImpl;

public:
  //! Vectors of const references to local signals.
  using LocalEventVector = vector<reference_wrapper<LocalEvent const>>;

  //! Construct a trigger-less output spec.
  /*
   * \param triggerlessOutput the signal to be activated.
   */
  Output(LocalEvent const & triggerlessOutput);

  //! Construct a trigger-less output spec.
  /*!
   * \param triggerlessOutputs a vector of signals that are to be activated.
   */
  Output(LocalEventVector const & triggerlessOutputs);

  //! Construct a functional trigger-less output spec.
  /*!
   * \param triggerlessOutputFun a function returning the signal to be activated.
   */
  Output(function<LocalEvent const & ()> const & triggerlessOutputFun);

  //! Construct a functional trigger-less output spec.
  /*!
   * \param triggerlessOutputFun a function returning a vector of signals that are to be activated.
   */
  Output(function<LocalEventVector ()> const & triggerlessOutputsFun);

  //!Construct a functional triggered output spec.
  /*!
   * \param triggeredOutput a trigger-dependent function returning the signal to be activated.
   */
  Output(function<LocalEvent const & (Event const &)> const & triggeredOutput);

  //!Construct a functional triggered output spec.
  /*!
   * \param triggeredOutput a trigger-dependent function returning a vector of signals that are to be activated.
   */
  Output(function<LocalEventVector (Event const &)> const & triggeredOutputs);

private:
  TriggerlessOutput const triggerless;
  TriggerlessOutputFun const triggerlessFun;
  TriggeredOutput const triggered;

  void join(Transition const * const) const override;
};

//! Action specs that can be added to transitions.
/*!
 * At execution time, all action specs that have been added to a transition are executed. They may be
 * executed in any order. In particular, they may be executed in an order
 * that differs from the one in which they were added to the transition. It is recommended to
 * keep the visible effects actions may have independent of the order in which they are executed.
 */
class Action
:
  public Transition::Spec
{
  friend class Transition;
  friend class TriggerlessTransitionImpl;
  friend class TriggeredTransitionImpl;

public:
  //! Construct a trigger-less action.
  /*!
   * \param triggerlessAction a function to be executed.
   */
  Action(function<void ()> const & triggerlessAction);

  //! Construct a triggered action.
  /*!
   * \param triggerlessAction a trigger-dependent function to be executed.
   */
  Action(function<void (Event const &)> const & triggeredAction);

private:
  TriggerlessAction const triggerless;
  TriggeredAction const triggered;

  void join(Transition const * const) const override;
};

//! Trigger-less two-state transitions, called auto transitions.
class Auto
:
  private virtual PImpl<AutoTransitionImpl>
, public TriggerlessTransition
, public ExternalTransition
{
public:
  //! Construct a auto transition .
  /*!
   * \param source the source state.
   * \param source the target state.
   */
  Auto(SourceState const & source, TargetState const & target);

  //! Construct a auto transition supplying specs.
  /*!
   * \param source the source state.
   * \param source the target state.
   * \param firstSpec the first spec to be added to the transition on construction.
   * \param remainingSpecs all the remaining specs to be added to the transition on construction.
   */
  template<class S, class... Ss>
  Auto
  (
    SourceState const & source
  , TargetState const & target
  , S && firstSpec
  , Ss &&... remainingSpecs
  );

  //! Destruct auto transition.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~Auto();
};

template<class S, class... Ss>
Auto
::Auto
(
  SourceState const & source
, TargetState const & target
, S && firstSpec
, Ss &&... remainingSpecs
)
:
  Auto(source, target, remainingSpecs...)
{
  add(forward<S>(firstSpec));
}

//! Triggered two-state transitions, called step transitions.
class Step
:
  private virtual PImpl<StepTransitionImpl>
, public TriggeredTransition
, public ExternalTransition
{
public:
  //! Construct a step transition.
  /*!
   * \param source the source state.
   * \param target the target state.
   */
  Step(SourceState const & source, TargetState const & target);

  //! Construct a step transition supplying specs.
  /*!
   * \param source the source state.
   * \param source the target state.
   * \param firstSpec the first spec to be added to the transition on construction.
   * \param remainingSpecs all the remaining specs to be added to the transition on construction.
   */
  template<class S, class... Ss>
  Step
  (
    SourceState const & source
  , TargetState const & target
  , S && firstSpec
  , Ss &&... remainingSpecs
  );

  //! Destruct step transition.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~Step();
};

template<class S, class... Ss>
Step
::Step
(
  SourceState const & source
, TargetState const & target
, S && firstSpec
, Ss &&... remainingSpecs
)
:
  Step(source, target, remainingSpecs...)
{
  add(forward<S>(firstSpec));
}

//! Base class for boundary (enter- and exit-) transitions.
class BoundaryTransition
:
  public TriggerlessTransition
, public SingleStateTransition
{
protected:
  BoundaryTransition(BoundaryTransitionImpl * const impl);
};

//! Enter transitions.
/*!
 * An enter transition is executed whenever the control flow enters the state hosting it.
 */
class Enter
:
  private virtual PImpl<EnterTransitionImpl>
, public BoundaryTransition
{
public:
  //! Construct an enter transition.
  /*!
   * \param state the host state.
   */
  Enter(State const & host);

  //! Construct an enter transition supplying specs.
  /*!
   * \param state the host state.
   * \param firstSpec the first spec to be added to the transition on construction.
   * \param remainingSpecs all the remaining specs to be added to the transition on construction.
   */
  template<class S, class... Ss>
  Enter
  (
    State const & host
  , S && firstSpec
  , Ss &&... remainingSpecs
  );

  //! Destruct enter transition.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~Enter();
};

template<class S, class... Ss>
Enter
::Enter
(
  State const & host
, S && firstSpec
, Ss &&... remainingSpecs
)
:
  Enter(host, remainingSpecs...)
{
  add(forward<S>(firstSpec));
}

//! Exit transitions.
/*!
 * An exit transition is executed whenever the control flow exits the state hosting it.
 */
class Exit
:
  private virtual PImpl<ExitTransitionImpl>
, public BoundaryTransition
{
public:
  //! Construct an exit transition.
  /*!
   * \param state the host state.
   */
  Exit(State const & host);

  //! Construct an exit transition supplying specs.
  /*!
   * \param state the host state.
   * \param firstSpec the first spec to be added to the transition on construction.
   * \param remainingSpecs all the remaining specs to be added to the transition on construction.
   */
  template<class S, class... Ss>
  Exit
  (
    State const & host
  , S && firstSpec
  , Ss &&... remainingSpecs
  );

  //! Destruct exit transition.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~Exit();
};

template<class S, class... Ss>
Exit
::Exit
(
  State const & host
, S && firstSpec
, Ss &&... remainingSpecs
)
:
  Exit(host, remainingSpecs...)
{
  add(forward<S>(firstSpec));
}

//! Internal step transitions.
/*!
 * An internal step transition can be executed whenever the control flow resides inside the state hosting it.
 * An internal step transition without any guards or without a max-1 flag leads to livelock once its trigger
 * signal is activated. To prevent this kind of livelock, internal step transitions
 * require one or more guards and/or a max-1 flag.
 */
class InternalStep
:
  private virtual PImpl<InternalStepTransitionImpl>
, public TriggeredTransition
, public SingleStateTransition
{
public:
  //! Construct an internal step transition.
  /*!
   * \param host the host state.
   */
  InternalStep(State const & host);

  //! Construct an internal step transition supplying specs
  /*!
   * \param host the host state.
   * \param firstSpec the first spec to be added to the transition on construction.
   * \param remainingSpecs all the remaining specs to be added to the transition on construction.
   */
  template<class S, class... Ss>
  InternalStep
  (
    State const & host
  , S && firstSpec
  , Ss &&... remainingSpecs
  );

  //! Destruct internal step transition.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~InternalStep();
};

template<class S, class... Ss>
InternalStep
::InternalStep
(
  State const & host
, S && firstSpec
, Ss &&... remainingSpecs
)
:
  InternalStep(host, remainingSpecs...)
{
  add(forward<S>(firstSpec));
}

//! Internal auto transitions.
/*!
 * An internal auto transition can be executed whenever the control flow resides inside the state hosting it.
 * An internal auto transition without any guards or without a max-1 flag leads to livelock.
 * To prevent this kind of livelock, internal auto transitions
 * require one or more guards and/or a max-1 flag.
 */
class InternalAuto
:
  private virtual PImpl<InternalAutoTransitionImpl>
, public TriggerlessTransition
, public SingleStateTransition
{
public:
  //! Construct an internal auto transition.
  /*!
   * \param host the host state.
   */
  InternalAuto(State const & host);

  //! Construct an internal auto transition supplying specs
  /*!
   * \param host the host state.
   * \param firstSpec the first spec to be added to the transition on construction.
   * \param remainingSpecs all the remaining specs to be added to the transition on construction.
   */
  template<class S, class... Ss>
  InternalAuto
  (
    State const & host
  , S && firstSpec
  , Ss &&... remainingSpecs
  );

  //! Destruct internal auto transition.
  /*!
   * The destructor is intended for RAII. It is not normally to be called explicitly.
   */
  ~InternalAuto();
};

template<class S, class... Ss>
InternalAuto
::InternalAuto
(
  State const & host
, S && firstSpec
, Ss &&... remainingSpecs
)
:
  InternalAuto(host, remainingSpecs...)
{
  add(forward<S>(firstSpec));
}

//! Macro to abbreviate type state_diagram::Output::LocalEventVector.
#define FSM_LEV state_diagram::Output::LocalEventVector

//! Macro used to declare tuple types carried by polyadic signals.
#define FSM_TT(type, ...) type __VA_OPT__(,) __VA_ARGS__

//! Macro to construct a top state supplying external signals and variables.
/*!
 * The top state's string name is the same as its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_TOP(name, ...) state_diagram::Top const name{#name __VA_OPT__(,) __VA_ARGS__}
#else
#define FSM_TOP(name, ...) state_diagram::Top const name{__VA_ARGS__}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct a region.
/*!
 * The region's string name is the same as its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_REGION(name, parent) state_diagram::Region const name{#name, parent}
#else
#define FSM_REGION(name, parent) state_diagram::Region const name{parent}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct an initial state.
/*!
 * The initial state's programmatic name is derived from its parent's programmatic name.
 */
#define FSM_INIT(parent) state_diagram::Init const parent##_INIT{parent}

//! Macro to construct a final state.
/*!
 * The final state's programmatic name is derived from its parent's programmatic name.
 */
#define FSM_FINAL(parent) state_diagram::Final const parent##_FINAL{parent}

//! Macro to construct a regular state.
/*!
 * The state's string name is the same as its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_STATE(name, parent) state_diagram::State const name{#name, parent}
#else
#define FSM_STATE(name, parent) state_diagram::State const name{parent}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct a connector  state.
/*!
 * The connector state's string name is the same as its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_CONNECTOR(name, parent) state_diagram::Connector const name{#name, parent}
#else
#define FSM_CONNECTOR(name, parent) state_diagram::Connector const name{parent}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct an external signal, optionally supplying a top state the signal is bookmarked with.
/*!
 * The external signal's string name is the same as its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_SIGNAL(type, name, parent) state_diagram::ExternalSignal<type> name{#name, parent}
#else
#define FSM_SIGNAL(type, name, parent) state_diagram::ExternalSignal<type> name{parent}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct a local signal.
/*!
 * The local signal's string name is the same as its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_LOCAL_SIGNAL(type, name, parent) state_diagram::LocalSignal<type> name{#name, parent}
#else
#define FSM_LOCAL_SIGNAL(type, name, parent) state_diagram::LocalSignal<type> name{parent}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct an external variable, optionally supplying a top state the variable is bookmarked with.
/*!
 * The external variable's string name is the same as its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_VAR(type, name, parent, ...) state_diagram::ExternalVar<type> name{#name, parent __VA_OPT__(,) __VA_ARGS__}
#else
#define FSM_VAR(type, name, parent, ...) state_diagram::ExternalVar<type> name{parent __VA_OPT__(,) __VA_ARGS__}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct a local variable.
/*!
 * The local variable's string name is the same as its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_LOCAL_VAR(type, name, ...) state_diagram::LocalVar<type> name{#name __VA_OPT__(,) __VA_ARGS__}
#else
#define FSM_LOCAL_VAR(type, name, ...) state_diagram::LocalVar<type> name{__VA_ARGS__}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct an array of external variables, optionally supplying a top state the variables are bookmarked with.
/*!
 * The string name of each external variable is identical to its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_ARRAY(type, name, capacity, parent, ...) state_diagram::ExternalArray<type, capacity> name{#name, parent __VA_OPT__(,) __VA_ARGS__}
#else
#define FSM_ARRAY(type, name, capacity, parent, ...) state_diagram::ExternalArray<type, capacity> name{parent __VA_OPT__(,) __VA_ARGS__}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct an array of local variables.
/*!
 * The string name of each external variable is identical to its programmatic name.
 */
#ifndef STATE_DIAGRAM_STRINGLESS
#define FSM_LOCAL_ARRAY(type, name, capacity, ...) state_diagram::LocalArray<type, capacity> name{#name __VA_OPT__(,) __VA_ARGS__}
#else
#define FSM_LOCAL_ARRAY(type, name, capacity, ...) state_diagram::LocalArray<type, capacity> name{__VA_ARGS__}
#endif // STATE_DIAGRAM_STRINGLESS

//! Macro to construct an auto transition.
/*!
 * The auto transition's programmatic name is derived from the programmatic names of its source and target.
 */
#define FSM_AUTO(source, target, ...) state_diagram::Auto const source##_TO_##target{source, target __VA_OPT__(,) __VA_ARGS__}

//! Macro to construct a step transition.
/*!
 * The step transition's programmatic name is derived from the programmatic names of its source and target.
 */
#define FSM_STEP(source, target, ...) state_diagram::Step const source##_TO_##target{source, target __VA_OPT__(,) __VA_ARGS__}

//! Macro to construct an enter transition.
/*!
 * The enter transition's programmatic name is derived from the programmatic name of its host.
 */
#define FSM_ENTER(host, ...) state_diagram::Enter const host##_ENTER{host __VA_OPT__(,)__VA_ARGS__}

//! Macro to construct an enter transition.
/*!
 * The enter transition's programmatic name is derived from the programmatic name of its host.
 */
#define FSM_EXIT(host, ...) state_diagram::Exit const host##_EXIT{host __VA_OPT__(,) __VA_ARGS__}

//! Macro to construct an internal step transition.
/*!
 * The internal step's programmatic name is derived from the programmatic name of its host.
 */
#define FSM_INTERNAL_STEP(host, ...) state_diagram::InternalStep const host##_INTERNAL_STEP{host __VA_OPT__(,) __VA_ARGS__}

//! Macro to construct an internal auto transition.
/*!
 * The internal auto's programmatic name is derived from the programmatic name of its host.
 */
#define FSM_INTERNAL_AUTO(host, ...) state_diagram::InternalAuto const host##_INTERNAL_AUTO{host __VA_OPT__(,) __VA_ARGS__}

} // namespace state_diagram

#endif // STATE_DIAGRAM_H_
