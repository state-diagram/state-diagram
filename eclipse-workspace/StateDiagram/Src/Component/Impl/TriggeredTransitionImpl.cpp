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

#include "TriggeredTransitionImpl.h"

#include "Util/ListAlgorithm.hpp"
#ifndef STATE_DIAGRAM_NO_SHUFFLING
#  include "Util/ShufflingAlgorithm.hpp"
#endif // STATE_DIAGRAM_NO_SHUFFLING
#include "ExternalSignalDelegateImpl.h"
#include "LocalSignalDelegateImpl.h"
#include "Spec_all.h"
#include "TopStateImpl.h"

namespace state_diagram
{

TriggeredTransitionImpl
::TriggeredTransitionImpl()
:
  m_triggers{}
, m_guards{}
, m_outputs{}
, m_actions{}
{
  // This space intentionally left empty
}

void
TriggeredTransitionImpl
::accept
(
  function<void (TriggerlessTransitionImpl * const)> const
, function<void (TriggeredTransitionImpl * const)> const onTriggered
)
{
  onTriggered(this);
}

void
TriggeredTransitionImpl
::add(Event const * const trigger)
{
  trigger->implUpcast()->accept
  (
    [&](ExternalSignalDelegateImpl * const) noexcept {}
  , [&](LocalSignalDelegateImpl * const triggerImpl)
    {
      if (!origin()->parentRegion()->hasInScope(triggerImpl))
      {
#ifndef STATE_DIAGRAM_STRINGLESS
        throw TriggeredTransition::TriggerScopeError(triggerImpl->path(), origin()->parentRegion()->path());
#else
        STATE_DIAGRAM_HANDLE_ERROR(TriggeredTransition::triggerScopeError);
#endif // STATE_DIAGRAM_STRINGLESS
      }
    }
  );
  if (!containsItem(m_triggers, trigger))
  {
    m_triggers.emplace_front(trigger);
  }
}

void
TriggeredTransitionImpl
::add(Trigger const * const trigger)
{
  add(&trigger->signal);
}

void
TriggeredTransitionImpl
::add(Guard const * const guard)
{
  m_guards.emplace_front(guard->triggered);
}

void
TriggeredTransitionImpl
::add(Output const * const output)
{
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
  m_outputs.emplace_front(output->triggered);
}

void
TriggeredTransitionImpl
::add(Action const * const action)
{
  m_actions.emplace_front(action->triggered);
}

ExecStat
TriggeredTransitionImpl
::exec()
{
  if (!isMax1Enabled())
  {
    return ExecStat{};
  }
  Event const * const trigger{chooseTriggerCheckGuards()};
  if (trigger == nullptr)
  {
    return ExecStat{};
  }
  max1Disable();
  {
#ifndef STATE_DIAGRAM_NO_SHUFFLING
    vector<TriggeredOutputSharable const *> shuffler{m_outputs.size()};
    populateShuffle(shuffler, m_outputs);
    for (auto const & output : shuffler)
#else
    for (auto const & output : m_outputs)
#endif // STATE_DIAGRAM_NO_SHUFFLING
    {
      auto const & outputEvents{output->triggeredOutputs(*trigger)};
      for (auto const & outputEvent : outputEvents)
      {
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
        // Check whether output signals, which must always be local, lie in scope.
        if (!outputScope()->hasInScope(outputEvent.get().impl()))
        {
#ifndef STATE_DIAGRAM_STRINGLESS
          throw Transition::ScopeError::Output(outputEvent.get().path(), outputScope()->parentRegion()->path());
#else
          STATE_DIAGRAM_HANDLE_ERROR(Transition::scopeError_output);
#endif // STATE_DIAGRAM_STRINGLESS
        }
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
        outputEvent.get().implUpcast()->activate();
      }
    }
  }
  {
#ifndef STATE_DIAGRAM_NO_SHUFFLING
    vector<TriggeredActionSharable const *> shuffler{m_actions.size()};
    populateShuffle(shuffler, m_actions);
    for (auto const & action : shuffler)
#else
    for (auto const & action : m_actions)
#endif // STATE_DIAGRAM_NO_SHUFFLING
    {
      action->triggeredAction(*trigger);
    }
  }
  return ExecStat{UnwindCmd{}};
}

Event const *
TriggeredTransitionImpl
::chooseTriggerCheckGuards()
const
{
#ifndef STATE_DIAGRAM_NO_SHUFFLING
  vector<Event const *> shuffler{m_triggers.size()};
  {
    size_t idx{0};
    for (auto const & trigger : m_triggers)
    {
      shuffler[idx] = trigger;
      ++idx;
    }
  }
  random_shuffle(shuffler.begin(), shuffler.end());
  for (auto const & trigger : shuffler)
#else
  for (auto const & trigger : m_triggers)
#endif // STATE_DIAGRAM_NO_SHUFFLING
  {
    if (!trigger->implUpcast()->isActive())
    {
      continue;
    }
    bool sawGuardYieldingFalseOnTrigger{false};
#ifndef STATE_DIAGRAM_NO_SHUFFLING
    vector<TriggeredGuardSharable const *> _shuffler{m_guards.size()};
    populateShuffle(_shuffler, m_guards);
    for (auto const & guard : _shuffler)
#else
    for (auto const & guard : m_guards)
#endif // STATE_DIAGRAM_NO_SHUFFLING
    {
      if (!guard->triggeredGuard(*trigger))
      {
        sawGuardYieldingFalseOnTrigger = true;
        break;
      }
    }
    if (sawGuardYieldingFalseOnTrigger)
    {
      continue;
    }
    return trigger;
  }
  return nullptr;
}

} // namespace state_diagram
