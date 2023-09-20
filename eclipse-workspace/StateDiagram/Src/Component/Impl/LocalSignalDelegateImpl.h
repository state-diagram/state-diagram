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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_LOCALSIGNALDELEGATEIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_LOCALSIGNALDELEGATEIMPL_H_

#include "state_diagram/state_diagram.h"

#include "SignalDelegateImpl.h"

namespace state_diagram
{

class CompoundStateImpl;
class RegionImpl;
class LocalScope;

class LocalSignalDelegateImpl
:
  public SignalDelegateImpl
{
public:
  LocalSignalDelegateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundStateImpl * const scope);
  LocalSignalDelegateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) RegionImpl * const scope);
  LocalSignalDelegateImpl(LocalSignalDelegateImpl const &) = delete;

  void operator=(LocalSignalDelegateImpl const &) = delete;

  void
  accept
  (
    function<void (ExternalSignalDelegateImpl * const)> onExternal
  , function<void (LocalSignalDelegateImpl * const)> onLocal
  )
  override;

#ifndef STATE_DIAGRAM_STRINGLESS
  void path(ostream & to) const override;
  using NamePathImpl::path;
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  bool isUnderExecution() const override;

  bool isInCurLocalScope() const;
#ifndef STATE_DIAGRAM_STRINGLESS
  string curLocalScopePath() const override;
#endif // STATE_DIAGRAM_STRINGLESS
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

private:
  LocalScope * const m_scope;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_LOCALSIGNALDELEGATEIMPL_H_
