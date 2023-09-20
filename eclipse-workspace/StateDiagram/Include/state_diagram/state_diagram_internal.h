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

#ifndef STATE_DIAGRAM_INTERNAL_H_
#define STATE_DIAGRAM_INTERNAL_H_

#include "state_diagram_error.h"

#include <memory>

#ifndef STATE_DIAGRAM_STRINGLESS
# define STATE_DIAGRAM_STRING_PARAM(PARAM) string const & PARAM
# define STATE_DIAGRAM_STRING_PARAM_COMMA(PARAM) string const & PARAM,
# define STATE_DIAGRAM_STRING_ARG(ARG) ARG
# define STATE_DIAGRAM_STRING_ARG_COMMA(ARG) ARG,
#else
# define STATE_DIAGRAM_STRING_PARAM(PARAM)
# define STATE_DIAGRAM_STRING_PARAM_COMMA(PARAM)
# define STATE_DIAGRAM_STRING_ARG(ARG)
# define STATE_DIAGRAM_STRING_ARG_COMMA(ARG)
#endif // STATE_DIAGRAM_STRINGLESS

namespace state_diagram
{

using namespace std;

template<class Impl>
class PImplUpcast
{
protected:
  Impl * const m_implUpcast;

  PImplUpcast(Impl * const implUpcast)
  :
    m_implUpcast{implUpcast}
  {
    // This space intentionally left empty
  }

  PImplUpcast(PImplUpcast const &) = delete;

  virtual
  ~PImplUpcast()
  {
    // This space intentionally left empty
  }

  void operator=(PImplUpcast const &) = delete;
};

template<class Impl>
class PImpl
{
protected:
  Impl * const m_impl;

  template<class... Args>
  PImpl(Args... args)
  :
    m_impl{new Impl{args...}}
  {
    if (m_impl == nullptr)
    {
#ifndef STATE_DIAGRAM_STRINGLESS
      throw MemoryError();
#else
      STATE_DIAGRAM_HANDLE_ERROR(memoryError);
#endif // STATE_DIAGRAM_STRINGLESS
    }
  }

  PImpl(PImpl const &) = delete;

  virtual
  ~PImpl()
  {
    // This space intentionally left empty
  }

  void operator=(PImpl const &) = delete;
};

class AutoTransitionImpl;
class BoundaryTransitionImpl;
class CompoundStateImpl;
class ConnectorStateImpl;
class ExternalTransitionImpl;
class EnterTransitionImpl;
class ExitTransitionImpl;
class ExternalSignalDelegateImpl;
class ExternalVarDelegateImpl;
class FinalStateImpl;
class InitStateImpl;
class InternalAutoTransitionImpl;
class InternalStepTransitionImpl;
class InternalVarDelegateImpl;
class LocalSignalDelegateImpl;
class LocalVarDelegateImpl;
class NamePathImpl;
class RegionImpl;
class SignalDelegateImpl;
class SingleStateTransitionImpl;
class SourceStateImpl;
class StateImpl;
class StepTransitionImpl;
class SubStateImpl;
class TargetStateImpl;
class TopStateImpl;
class TransitionImpl;
class TriggeredTransitionImpl;
class TriggerlessTransitionImpl;
class VarDelegateImpl;

class TriggerlessGuardSharable;
class TriggerlessOutputSharable;
class TriggerlessActionSharable;
class TriggerlessOutputFunSharable;
class TriggeredGuardSharable;
class TriggeredOutputSharable;
class TriggeredActionSharable;

using TriggerlessGuard = shared_ptr<TriggerlessGuardSharable const>;
using TriggerlessOutput = shared_ptr<TriggerlessOutputSharable const>;
using TriggerlessAction = shared_ptr<TriggerlessActionSharable const>;
using TriggerlessOutputFun = shared_ptr<TriggerlessOutputFunSharable const>;
using TriggeredGuard = shared_ptr<TriggeredGuardSharable const>;
using TriggeredOutput = shared_ptr<TriggeredOutputSharable const>;
using TriggeredAction = shared_ptr<TriggeredActionSharable const>;

} // namespace state_diagram

#endif // STATE_DIAGRAM_INTERNAL_H_
