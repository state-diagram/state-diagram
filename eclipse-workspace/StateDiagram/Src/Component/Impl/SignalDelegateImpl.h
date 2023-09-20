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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_SIGNALDELEGATEIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_SIGNALDELEGATEIMPL_H_

#include "state_diagram/state_diagram.h"

#include "NamePathImpl.h"

namespace state_diagram
{

class SignalDelegateImpl
:
  public NamePathImpl
{
protected:
  SignalDelegateImpl(STATE_DIAGRAM_STRING_PARAM(name));

public:
  virtual
  void
  accept
  (
    function<void (ExternalSignalDelegateImpl * const)> onExternal
  , function<void (LocalSignalDelegateImpl * const)> onLocal
  )
  = 0;

#ifndef STATE_DIAGRAM_STRINGLESS
  void pathPrefix(ostream &) const override;
  using NamePathImpl::path;
#endif // STATE_DIAGRAM_STRINGLESS

  virtual bool isActive() const;
  void activate();
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  virtual
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  void deactivate();

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  bool isSet() const;
  void markAsSet();

  virtual bool isUnderExecution() const = 0;
#ifndef STATE_DIAGRAM_STRINGLESS
  virtual string curLocalScopePath() const = 0;
#endif // STATE_DIAGRAM_STRINGLESS

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

private:
  bool m_isActive;

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  bool m_isSet;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_SIGNALDELEGATEIMPL_H_
