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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_SPEC_ALL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_SPEC_ALL_H_

#include "state_diagram/state_diagram.h"

namespace state_diagram
{

class TriggeredActionSharable
{
public:
  TriggeredActionSharable(function<void (Event const &)> const & triggeredAction);

  function<void (Event const &)> const triggeredAction;
};

class TriggeredGuardSharable
{
public:
  TriggeredGuardSharable(function<bool (Event const &)> const & triggeredGuard);

  function<bool (Event const &)> const triggeredGuard;
};

class TriggeredOutputSharable
{
public:
  TriggeredOutputSharable(function<Output::LocalEventVector (Event const &)> const & triggeredOutputs);

  function<Output::LocalEventVector (Event const &)> const triggeredOutputs;
};

class TriggerlessActionSharable
{
public:
  TriggerlessActionSharable(function<void ()> const & triggerlessAction);

  function<void ()> const triggerlessAction;
};

class TriggerlessGuardSharable
{
public:
  TriggerlessGuardSharable(function<bool ()> const & triggerlessGuard);

  function<bool ()> const triggerlessGuard;
};

class TriggerlessOutputSharable
{
public:
  TriggerlessOutputSharable(Output::LocalEventVector const & triggerlessOutputs);

  Output::LocalEventVector triggerlessOutputs;
};

class TriggerlessOutputFunSharable
{
public:
  TriggerlessOutputFunSharable(function<Output::LocalEventVector ()> const & triggerlessOutputsFun);

  function<Output::LocalEventVector ()> const triggerlessOutputsFun;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_SPEC_ALL_H_
