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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_TOPSTATEIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_TOPSTATEIMPL_H_

#include "state_diagram/state_diagram.h"

#ifndef STATE_DIAGRAM_STRINGLESS
#include <set>
#endif // STATE_DIAGRAM_STRINGLESS

#include "Util/ForwardList.hpp"
#include "CompoundStateImpl.h"

namespace state_diagram
{

class LocalVarDelegateImpl;

class TopStateImpl
:
  public CompoundStateImpl
{
public:
  TopStateImpl(STATE_DIAGRAM_STRING_PARAM(name));
  TopStateImpl(TopStateImpl const &) = delete;

  void operator=(TopStateImpl const &) = delete;

  RegionImpl * parentRegion() const override;

  StateImpl * asState() override;

#ifndef STATE_DIAGRAM_STRINGLESS
  void path(ostream & to) const override;
#endif // STATE_DIAGRAM_STRINGLESS

  bool isDeepMemberOf(RegionImpl const * const region) const override;

  void activate(ExternalSignalDelegateImpl * const trigger);

  void init() override;
  bool exec();
  void reload() const override;

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  LocalScope const * curLocalScope;
  bool isInCurLocalScope(LocalSignalDelegateImpl const * const localSignal) const;
  bool isInCurLocalScope(LocalVarDelegateImpl const * const localVar) const;
  bool isUnderExecution() const;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  void insertExternalSignal(STATE_DIAGRAM_STRING_PARAM_COMMA(name) ExternalSignalDelegateImpl * const externalSignal);
  void insertExternalVar(STATE_DIAGRAM_STRING_PARAM_COMMA(name) ExternalVarDelegateImpl * const externalVar);

private:
#ifndef STATE_DIAGRAM_STRINGLESS
  set<string> m_externalSignalNames;
  set<string> m_externalVarNames;
#endif // STATE_DIAGRAM_STRINGLESS
  ForwardList<ExternalSignalDelegateImpl * const> m_externalSignals;
  ForwardList<ExternalVarDelegateImpl * const> m_externalVars;
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  mutable bool m_isUnderExecution;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_TOPSTATEIMPL_H_
