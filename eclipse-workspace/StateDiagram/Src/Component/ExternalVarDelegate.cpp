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

#include "Impl/ExternalVarDelegateImpl.h"

namespace state_diagram
{

ExternalVarDelegate
::ExternalVarDelegate(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Top const & parent, Delegator * const delegator)
:
  PImpl<ExternalVarDelegateImpl>{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent.m_impl, this}
, VarDelegate{m_impl, delegator}
{
  // This space intentionally left empty
}

ExternalVarDelegate
::~ExternalVarDelegate()
{
  delete m_impl;
}

#ifndef STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

bool
ExternalVarDelegate
::isInCurLocalScope()
const
{
  return m_impl->isInCurLocalScope();
}

#endif // STATE_DIAGRAM_NO_CHECKS_WHILE_STEPPING

} // namespace state_diagram
