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

#include "TriggerlessTransitionImpl.h"

#include <cassert>
#ifndef STATE_DIAGRAM_NO_SHUFFLING
#  include "Util/ShufflingAlgorithm.hpp"
#endif // STATE_DIAGRAM_NO_SHUFFLING
#include "LocalSignalDelegateImpl.h"
#include "MaxableTransition.h"
#include "Spec_all.h"
#include "TopStateImpl.h"

namespace state_diagram
{

TriggerlessTransitionImpl
::TriggerlessTransitionImpl()
:
  m_guards{}
, m_outputFuns{}
, m_actions{}
{
  // This space intentionally left empty
}

void
TriggerlessTransitionImpl
::accept
(
  function<void (TriggerlessTransitionImpl * const)> const onTriggerless
, function<void (TriggeredTransitionImpl * const)> const
)
{
  onTriggerless(this);
}

void
TriggerlessTransitionImpl
::add(Trigger const * const)
{
#ifndef STATE_DIAGRAM_STRINGLESS
  throw Transition::Spec::TriggerSpecOnTriggerlessTransitionError(transitionTypeIndicator(), origin()->path());
#else
  STATE_DIAGRAM_HANDLE_ERROR(Transition::Spec::triggerSpecOnTriggerlessTransitionError);
#endif // STATE_DIAGRAM_STRINGLESS
}

void
TriggerlessTransitionImpl
::add(Guard const * const guard)
{
  if (guard->triggerless == nullptr)
  {
    assert (guard->triggered != nullptr);
#ifndef STATE_DIAGRAM_STRINGLESS
    throwTriggeredSpecOnTriggerlessTransitionError("guard");
#else
    STATE_DIAGRAM_HANDLE_ERROR(Transition::Spec::triggeredSpecOnTriggerlessTransitionError);
#endif // STATE_DIAGRAM_STRINGLESS
  }
  m_guards.emplace_front(guard->triggerless);
}

void
TriggerlessTransitionImpl
::add(Output const * const output)
{
  if (output->triggerlessFun == nullptr)
  {
    assert (output->triggered != nullptr);
#ifndef STATE_DIAGRAM_STRINGLESS
    throwTriggeredSpecOnTriggerlessTransitionError("output");
#else
    STATE_DIAGRAM_HANDLE_ERROR(Transition::Spec::triggeredSpecOnTriggerlessTransitionError);
#endif // STATE_DIAGRAM_STRINGLESS
  }
  if (output->triggerless != nullptr)
  {
    auto const & outputEvents{output->triggerless->triggerlessOutputs};
    for (auto const & outputEvent : outputEvents)
    {
      if (!outputScope()->hasInScope(outputEvent.get().impl()))
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw Transition::ScopeError::Output(outputEvent.get().path(), outputScope()->path());
#else
        STATE_DIAGRAM_HANDLE_ERROR(Transition::scopeError_output);
#endif // STATE_DIAGRAM_STRINGLESS
      }
    }
  }
  m_outputFuns.emplace_front(output->triggerlessFun);
}

void
TriggerlessTransitionImpl
::add(Action const * const action)
{
  if (action->triggerless == nullptr)
  {
    assert (action->triggered != nullptr);
#ifndef STATE_DIAGRAM_STRINGLESS
    throwTriggeredSpecOnTriggerlessTransitionError("action");
#else
    STATE_DIAGRAM_HANDLE_ERROR(Transition::Spec::triggeredSpecOnTriggerlessTransitionError);
#endif // STATE_DIAGRAM_STRINGLESS
  }
  m_actions.emplace_front(action->triggerless);
}

ExecStat
TriggerlessTransitionImpl
::exec()
{
  {
#ifndef STATE_DIAGRAM_NO_SHUFFLING
    vector<TriggerlessGuardSharable const *> shuffler{m_guards.size()};
    populateShuffle(shuffler, m_guards);
    for (auto const & guard : shuffler)
#else
    for (auto const & guard : m_guards)
#endif // STATE_DIAGRAM_STRINGLESS
    {
      if (!guard->triggerlessGuard())
      {
        return ExecStat{};
      }
    }
  }
  {
#ifndef STATE_DIAGRAM_NO_SHUFFLING
    vector<TriggerlessOutputFunSharable const *> shuffler{m_outputFuns.size()};
    populateShuffle(shuffler, m_outputFuns);
    for (auto const & output : shuffler)
#else
    for (auto const & output : m_outputFuns)
#endif // STATE_DIAGRAM_STRINGLESS
    {
      auto const & outputEvents{output->triggerlessOutputsFun()};
      for (auto const & outputEvent : outputEvents)
      {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
        // Check whether output signals, which must always be local, lie in scope.
        if (!outputScope()->hasInScope(outputEvent.get().impl()))
        {
#ifndef STATE_DIAGRAM_STRINGLESS
          throw Transition::ScopeError::Output(outputEvent.get().impl()->path(), outputScope()->parentRegion()->path());
#else
          STATE_DIAGRAM_HANDLE_ERROR(Transition::scopeError_output);
#endif // STATE_DIAGRAM_STRINGLESS
        }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
        outputEvent.get().impl()->activate();
      }
    }
  }
  {
#ifndef STATE_DIAGRAM_NO_SHUFFLING
    vector<TriggerlessActionSharable const *> shuffler{m_actions.size()};
    populateShuffle(shuffler, m_actions);
    for (auto const & action : shuffler)
#else
    for (auto const & action : m_actions)
#endif // STATE_DIAGRAM_STRINGLESS
    {
      action->triggerlessAction();
    }
    return ExecStat{UnwindCmd{}};
  }
}

#ifndef STATE_DIAGRAM_STRINGLESS
void
TriggerlessTransitionImpl
::throwTriggeredSpecOnTriggerlessTransitionError(string const & specTypeIndicator)
{
  throw
    Transition::Spec::TriggeredSpecOnTriggerlessTransitionError
    (
      specTypeIndicator
    , transitionTypeIndicator()
    , origin()->path()
    );
}
#endif // STATE_DIAGRAM_STRINGLESS

} // namespace state_diagram
