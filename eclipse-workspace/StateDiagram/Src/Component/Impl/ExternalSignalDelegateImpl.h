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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_EXTERNALSIGNALDELEGATEIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_EXTERNALSIGNALDELEGATEIMPL_H_

#include "NamePathImpl.h"
#include "SignalDelegateImpl.h"

namespace state_diagram
{

class TopStateImpl;

class ExternalSignalDelegateImpl
:
  public SignalDelegateImpl
{
public:
  ExternalSignalDelegateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) TopStateImpl * const parent);
  ExternalSignalDelegateImpl(ExternalSignalDelegateImpl const &) = delete;

  void operator=(ExternalSignalDelegateImpl const &) = delete;

  void
  accept
  (
    function<void (ExternalSignalDelegateImpl * const)> onExternal
  , function<void (LocalSignalDelegateImpl * const)> onLocal
  )
  override;

#ifndef STATE_DIAGRAM_STRINGLESS
  void path(ostream &) const override;
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  void setUnderExecution();
  bool isUnderExecution() const override;

  bool isInCurLocalScope() const;
#ifndef STATE_DIAGRAM_STRINGLESS
  string curLocalScopePath() const override;
#endif // STATE_DIAGRAM_STRINGLESS

  void deactivate() override;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

private:
  TopStateImpl * const m_parent;
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  bool m_isUnderExecution;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_EXTERNALSIGNALDELEGATEIMPL_H_
