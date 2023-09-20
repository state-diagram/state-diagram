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

#ifndef SRC_COMPONENT_IMPL_VARDELEGATEIMPL_H_
#define SRC_COMPONENT_IMPL_VARDELEGATEIMPL_H_

#include "state_diagram/state_diagram.h"

#include "NamePathImpl.h"

namespace state_diagram
{

class VarDelegateImpl
:
  public NamePathImpl
{
protected:
  VarDelegateImpl(STATE_DIAGRAM_STRING_PARAM_COMMA(name) VarDelegate * const interfaceUpcast);

public:
  VarDelegateImpl(VarDelegateImpl const &) = delete;

  void operator=(VarDelegateImpl const &) = delete;

  virtual
  void
  accept
  (
    function<void (ExternalVarDelegateImpl * const)> onExternal
  , function<void (LocalVarDelegateImpl * const)> onLocal
  )
  = 0;

#ifndef STATE_DIAGRAM_STRINGLESS
  void pathPrefix(ostream &) const override;
  using NamePathImpl::path;
#endif // STATE_DIAGRAM_STRINGLESS

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  bool isValid() const;
  void markAsValid();

  bool isSet() const;
  void markAsSet();

  bool isSetNxt() const;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  void markAsSetNxt();

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  bool hasBeenRetrieved() const;
  void markAsRetrieved();

  virtual bool isUnderExecution() const = 0;
#ifndef STATE_DIAGRAM_STRINGLESS
  virtual string curLocalScopePath() const = 0;
#endif // STATE_DIAGRAM_STRINGLESS
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

  virtual void unset();

private:
  VarDelegate * const m_interfaceUpcast;

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  bool m_isValid;
  bool m_isSet;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  bool m_isSetNxt;
#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
  bool m_hasBeenRetrieved;
#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING
};

} // namespace state_diagram

#endif // SRC_COMPONENT_IMPL_VARDELEGATEIMPL_H_
