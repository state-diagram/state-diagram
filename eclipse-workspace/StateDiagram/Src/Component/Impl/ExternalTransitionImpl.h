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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_EXTERNALTRANSITIONIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_EXTERNALTRANSITIONIMPL_H_

#include "state_diagram/state_diagram.h"

#include "StackSeq/StackSeq_all.h"
#include "ExecStat.h"
#include "NamePathImpl.h"
#include "TargetStateImpl.h"
#include "TransitionImpl.h"

namespace state_diagram
{

class ExternalTransitionImpl
:
  public virtual TransitionImpl
{
public:
  ExternalTransitionImpl(SourceStateImpl * const source, TargetStateImpl * const target);
  ExternalTransitionImpl(ExternalTransitionImpl const &) = delete;

  void operator=(ExternalTransitionImpl const &) = delete;

  SourceStateImpl * const source;
  TargetStateImpl * const target;

  SubStateImpl const * outputScope() const override;

  void setCompletionFlag();

protected:
  unique_ptr<StackSeq const> const m_stackSeq;
  SubStateImpl const * const m_outputScope;

  SourceStateImpl * origin() const override;

  virtual void add(Guard const * const guard) = 0;

public:
  virtual ExecStat exec();

private:
  unique_ptr<StackSeq const> makeStackSeqCheckColocality() const;

  SubStateImpl * computeOutputScope() const;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_EXTERNALTRANSITIONIMPL_H_
