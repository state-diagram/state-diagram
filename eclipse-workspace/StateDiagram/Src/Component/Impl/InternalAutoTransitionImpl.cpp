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

#include "InternalAutoTransitionImpl.h"

namespace state_diagram
{

InternalAutoTransitionImpl
::InternalAutoTransitionImpl(StateImpl * const _host)
:
  InternalTransitionImpl{_host}
{
  host->add(this);
}

void
InternalAutoTransitionImpl
::accept
(
  function<void (EnterTransitionImpl * const)> const
, function<void (ExitTransitionImpl * const)> const
, function<void (InternalAutoTransitionImpl * const)> const onInternalAuto
, function<void (InternalStepTransitionImpl * const)> const
)
{
  onInternalAuto(this);
}

ExecStat
InternalAutoTransitionImpl
::exec()
{
  if (!isMax1Enabled())
  {
    return ExecStat{};
  }
  max1Disable();
  return TriggerlessTransitionImpl::exec();
}

void
InternalAutoTransitionImpl
::reload()
{
  MaxableTransition::reload();
}

string
InternalAutoTransitionImpl
::transitionTypeIndicator()
const
{
  return "internal auto";
}

} // namespace state_diagram

