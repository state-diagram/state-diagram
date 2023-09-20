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

#include "Spec_all.h"

namespace state_diagram
{

TriggeredActionSharable
::TriggeredActionSharable(function<void (Event const &)> const & _triggeredAction)
:
  triggeredAction{_triggeredAction}
{
  // This space intentionally left empty
}

TriggeredGuardSharable
::TriggeredGuardSharable(function<bool (Event const &)> const & _triggeredGuard)
:
  triggeredGuard{_triggeredGuard}
{
  // This space intentionally left empty
}

TriggeredOutputSharable
::TriggeredOutputSharable(function<Output::LocalEventVector (Event const &)> const & _triggeredOutputs)
:
  triggeredOutputs{_triggeredOutputs}
{
  // This space intentionally left empty
}

TriggerlessActionSharable
::TriggerlessActionSharable(function<void ()> const & _triggerlessAction)
:
  triggerlessAction{_triggerlessAction}
{
  // This space intentionally left empty
}

TriggerlessGuardSharable
::TriggerlessGuardSharable(function<bool ()> const & _triggerlessGuard)
:
  triggerlessGuard{_triggerlessGuard}
{
  // This space intentionally left empty
}

TriggerlessOutputSharable
::TriggerlessOutputSharable(Output::LocalEventVector const & _triggerlessOutputs)
:
  triggerlessOutputs{_triggerlessOutputs}
{
  // This space intentionally left empty
}

TriggerlessOutputFunSharable
::TriggerlessOutputFunSharable(function<Output::LocalEventVector ()> const & _triggerlessOutputsFun)
:
  triggerlessOutputsFun{_triggerlessOutputsFun}
{
  // This space intentionally left empty
}


} // namespace state_diagram
