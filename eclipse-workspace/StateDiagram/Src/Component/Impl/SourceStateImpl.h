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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_SOURCESTATEIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_SOURCESTATEIMPL_H_

#include "state_diagram/state_diagram.h"

#include "Util/ForwardList.hpp"
#include "SubStateImpl.h"

namespace state_diagram
{

class AutoTransitionImpl;

class SourceStateImpl
:
  public virtual SubStateImpl
{
protected:
  using AutoTransitions = ForwardList<AutoTransitionImpl * const>;
  using StepTransitions = ForwardList<StepTransitionImpl * const>;

  SourceStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) RegionImpl * const parent);

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  using NamePathImpl::path;
#endif // STATE_DIAGRAM_STRINGLESS

  void attach(AutoTransitionImpl * const autoTransition);
  void attach(StepTransitionImpl * const stepTransition);

protected:
  size_t autoTransitionsSize() const;
  AutoTransitions const & autoTransitions() const;

  size_t stepTransitionsSize() const;
  StepTransitions const & stepTransitions() const;

public:
  ExecStat exec() const override;
  virtual void complete(bool const freeze, FreezeDepth const freezeDepth);
  virtual void finalize() const override;
  void reload() override;

  virtual bool isPaused() const;
  virtual bool hasTerminated() const;

private:
  AutoTransitions m_autoTransitions;
  StepTransitions m_stepTransitions;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_SOURCESTATEIMPL_H_
