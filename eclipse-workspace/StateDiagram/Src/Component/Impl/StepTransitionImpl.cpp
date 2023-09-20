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

#include "StepTransitionImpl.h"

#include "ExternalTransitionImpl.h"
#include "SourceStateImpl.h"
#include "StateImpl.h"

namespace state_diagram
{

StepTransitionImpl
::StepTransitionImpl(SourceStateImpl * const _source, TargetStateImpl * const _target)
:
  ExternalTransitionImpl{_source, _target}
, m_haveFreezeFlag{false}
, m_freezeDepth{FreezeDepth()}
{
  source->attach(this);
}

void
StepTransitionImpl
::accept
(
  function<void (SingleStateTransitionImpl * const)> const
, function<void (AutoTransitionImpl * const)> const
, function<void (StepTransitionImpl * const)> const onStep
)
{
  onStep(this);
}

void
StepTransitionImpl
::setFreezeFlag(FreezeDepth const freezeDepth)
{
  m_haveFreezeFlag = true;
  m_freezeDepth = freezeDepth;
}

ExecStat
StepTransitionImpl
::exec()
{
  if (source->isPaused())
  {
    return ExecStat{};
  }
  if (!TriggeredTransitionImpl::exec().stat)
  {
    return ExecStat{};
  }
  source->complete(m_haveFreezeFlag, m_freezeDepth);
  return ExternalTransitionImpl::exec();
}

void
StepTransitionImpl
::add(Guard const * const guard)
{
  TriggeredTransitionImpl::add(guard);
}

} // namespace state_diagram

