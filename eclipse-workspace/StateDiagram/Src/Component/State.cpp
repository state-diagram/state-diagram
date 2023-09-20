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

#include "Impl/StateImpl.h"

namespace state_diagram
{

State
::State(STATE_DIAGRAM_STRING_PARAM_COMMA(name) CompoundState const & parent)
:
  PImpl<StateImpl>{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent.implUpcast()}
, NamePath{m_impl}
, SubState{m_impl}
, CompoundState{m_impl}
, SourceState{m_impl}
, TargetState{m_impl}
{
  // This space intentionally left empty
}

State
::State(STATE_DIAGRAM_STRING_PARAM_COMMA(name) Region const & parent)
:
  PImpl<StateImpl>{STATE_DIAGRAM_STRING_ARG_COMMA(name) parent.m_impl}
, NamePath{m_impl}
, SubState{m_impl}
, CompoundState{m_impl}
, SourceState{m_impl}
, TargetState{m_impl}
{
  // This space intentionally left empty
}

State
::~State()
{
  delete m_impl;
}

bool
State
::hasTerminated()
const
{
  return m_impl->hasTerminated();
}

} // namespace state_diagram
