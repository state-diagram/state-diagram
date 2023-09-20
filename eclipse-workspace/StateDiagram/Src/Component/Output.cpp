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

Output
::Output(LocalEvent const & triggerlessOutput)
:
  Output{LocalEventVector{triggerlessOutput}}
{
  // This space intentionally left empty
}

Output
::Output(LocalEventVector const & triggerlessOutputs)
:
  triggerless{make_shared<TriggerlessOutputSharable>(triggerlessOutputs)}
, triggerlessFun{make_shared<TriggerlessOutputFunSharable>([=]{return triggerlessOutputs;})}
, triggered{make_shared<TriggeredOutputSharable>([=](Event const &){return triggerlessOutputs;})}
{
  // This space intentionally left empty
}

Output
::Output(function<LocalEvent const & ()> const & triggerlessOutputFun)
:
  Output{[=]{return LocalEventVector{triggerlessOutputFun()};}}
{
  // This space intentionally left empty
}

Output
::Output(function<LocalEventVector ()> const & triggerlessOutputsFun)
:
  triggerless{nullptr}
, triggerlessFun{new TriggerlessOutputFunSharable(triggerlessOutputsFun)}
, triggered{make_shared<TriggeredOutputSharable>([=](Event const &){return triggerlessOutputsFun();})}
{
  if (triggerlessFun == nullptr)
  {
#ifndef STATE_DIAGRAM_STRINGLESS
    throw MemoryError();
#else
    STATE_DIAGRAM_HANDLE_ERROR(memoryError);
#endif // STATE_DIAGRAM_STRINGLESS
  }
}

Output
::Output(function<LocalEvent const & (Event const &)> const & triggeredOutput)
:
  Output{[=](Event const & trigger){return LocalEventVector{triggeredOutput(trigger)};}}
{
  // This space intentionally left empty
}

Output
::Output(function<LocalEventVector (Event const &)> const & triggeredOutputs)
:
  triggerless{nullptr}
, triggerlessFun{nullptr}
, triggered{new TriggeredOutputSharable(triggeredOutputs)}
{
  if (triggered == nullptr)
  {
#ifndef STATE_DIAGRAM_STRINGLESS
    throw MemoryError();
#else
    STATE_DIAGRAM_HANDLE_ERROR(memoryError);
#endif // STATE_DIAGRAM_STRINGLESS
  }
}

void
Output
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

