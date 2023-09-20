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

#include "AutoTransitionImpl.h"

namespace state_diagram
{

AutoTransitionImpl
::AutoTransitionImpl(SourceStateImpl * const _source, TargetStateImpl * const _target)
:
  ExternalTransitionImpl{_source, _target}
{
  source->attach(this);
}

void
AutoTransitionImpl
::accept
(
  function<void (SingleStateTransitionImpl * const)> const
, function<void (AutoTransitionImpl * const)> const onAuto
, function<void (StepTransitionImpl * const)> const
)
{
  onAuto(this);
}

ExecStat
AutoTransitionImpl
::exec()
{
  if (!isMax1Enabled())
  {
    return ExecStat{};
  }
  if (source->isPaused())
  {
    return ExecStat{};
  }
  if (!TriggerlessTransitionImpl::exec().stat)
  {
    return ExecStat{};
  }
  max1Disable();
  source->finalize();
  source->unfreeze();
  return ExternalTransitionImpl::exec();
}

string
AutoTransitionImpl
::transitionTypeIndicator()
const
{
  return "auto";
}

SubStateImpl const *
AutoTransitionImpl
::outputScope()
const
{
  return m_outputScope;
}

void
AutoTransitionImpl
::add(Guard const * const guard)
{
  TriggerlessTransitionImpl::add(guard);
}

} // namespace state_diagram
