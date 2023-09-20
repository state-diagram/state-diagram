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

#include "state_diagram/state_diagram.h"

#include "Impl/TriggerlessTransitionImpl.h"
#include "Impl/TriggeredTransitionImpl.h"
#include "Impl/Spec_all.h"

namespace state_diagram
{

Action
::Action(function<void ()> const & triggerlessAction)
:
  triggerless{make_shared<TriggerlessActionSharable>(triggerlessAction)}
, triggered{make_shared<TriggeredActionSharable>([=](Event const &){triggerlessAction();})}
{
  // This space intentionally left empty
}

Action
::Action(function<void (Event const &)> const & triggeredAction)
:
  triggerless{nullptr}
, triggered{make_shared<TriggeredActionSharable>(triggeredAction)}
{
  // This space intentionally left empty
}

void
Action
::join(Transition const * const transition)
const
{
  transition->implUpcast()->accept
  (
    [&](TriggerlessTransitionImpl * const triggerlessTransition){triggerlessTransition->add(this);}
  , [&](TriggeredTransitionImpl * const triggeredTransition){triggeredTransition->add(this);}
  );
}

} // namespace state_diagram

