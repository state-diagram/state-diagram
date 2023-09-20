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

#include "Impl/LocalVarDelegateImpl.h"

namespace state_diagram
{

LocalVarDelegate
::LocalVarDelegate(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundState const & scope, Delegator * const delegator)
:
  PImpl<LocalVarDelegateImpl>{STATE_DIAGRAM_STRING_ARG_COMMA(name) scope.implUpcast(), this}
, VarDelegate{m_impl, delegator}
{
  // This space intentionally left empty
}

LocalVarDelegate
::LocalVarDelegate(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Region const & scope, Delegator * const delegator)
:
  PImpl<LocalVarDelegateImpl>{STATE_DIAGRAM_STRING_ARG_COMMA(name) scope.m_impl, this}
, VarDelegate{m_impl, delegator}
{
  // This space intentionally left empty
}

LocalVarDelegate
::~LocalVarDelegate()
{
  delete m_impl;
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
LocalVarDelegate
::isInCurLocalScope()
const
{
  return m_impl->isInCurLocalScope();
}

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

} // namespace state_diagram
