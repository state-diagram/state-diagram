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

#include "SingleStateTransitionImpl.h"

namespace state_diagram
{

SingleStateTransitionImpl
::SingleStateTransitionImpl(StateImpl * const _host)
:
  host{_host}
{
  // This space intentionally left empty
}

SingleStateTransitionImpl
::~SingleStateTransitionImpl()
{
  // This space intentionally left empty
}

SourceStateImpl *
SingleStateTransitionImpl
::origin()
const
{
  return host;
}

SubStateImpl const *
SingleStateTransitionImpl
::outputScope()
const
{
  return host;
}

void
SingleStateTransitionImpl
::accept
(
  function<void (SingleStateTransitionImpl * const)> const onSingleState
, function<void (AutoTransitionImpl * const)> const
, function<void (StepTransitionImpl * const)> const
)
{
  onSingleState(this);
}

} // namespace state_diagram
