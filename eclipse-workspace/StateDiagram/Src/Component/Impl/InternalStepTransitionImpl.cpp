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

#include "InternalStepTransitionImpl.h"

namespace state_diagram
{

InternalStepTransitionImpl
::InternalStepTransitionImpl(StateImpl * const _host)
:
  InternalTransitionImpl{_host}
{
  host->add(this);
}

void
InternalStepTransitionImpl
::accept
(
  function<void (EnterTransitionImpl * const)> const
, function<void (ExitTransitionImpl * const)> const
, function<void (InternalAutoTransitionImpl * const)> const
, function<void (InternalStepTransitionImpl * const)> const onInternalStep
)
{
  onInternalStep(this);
}

ExecStat
InternalStepTransitionImpl
::exec()
{
  return TriggeredTransitionImpl::exec();
}

void
InternalStepTransitionImpl
::reload()
{
  TriggeredTransitionImpl::reload();
}

} // namespace state_diagram

