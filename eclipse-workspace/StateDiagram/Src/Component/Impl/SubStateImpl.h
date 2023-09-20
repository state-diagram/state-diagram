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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_SUBSTATEIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_SUBSTATEIMPL_H_

#include "state_diagram/state_diagram.h"

#include "StackSeq/StackSeq_all.h"
#include "ExecStat.h"
#include "RegionImpl.h"
#include "SubComponent.hpp"

namespace state_diagram
{

class SubStateImpl
:
  public SubComponent<RegionImpl>
{
protected:
  SubStateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) RegionImpl * const parent);

public:
  RegionImpl * parentRegion() const;

#ifndef STATE_DIAGRAM_STRINGLESS
  using SubComponent<RegionImpl>::path;
#endif // STATE_DIAGRAM_STRINGLESS

  bool isDeepMemberOf(RegionImpl const * const region) const;
  virtual bool hasInScope(LocalSignalDelegateImpl const * const signal) const;

  virtual bool isTerminal() const;

  virtual void init();
  virtual ExecStat exec() const = 0;
  virtual void finalize() const;
  virtual void unfreeze();
  virtual void reload();

  bool isCurrent() const;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_SUBSTATEIMPL_H_
