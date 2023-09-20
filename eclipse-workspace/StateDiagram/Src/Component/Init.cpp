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

#include "Impl/InitStateImpl.h"

namespace state_diagram
{

Init
::Init(CompoundState const & parent)
:
  PImpl<InitStateImpl>{parent.implUpcast()}
, NamePath{m_impl}
, SubState{m_impl}
, SourceState{m_impl}
{
  // This space intentionally left empty
}

Init
::Init(Region const & parent)
:
  PImpl<InitStateImpl>{parent.m_impl}
, NamePath{m_impl}
, SubState{m_impl}
, SourceState{m_impl}
{
  // This space intentionally left empty
}

Init
::~Init()
{
  delete m_impl;
}

string const
Init
::name
{
  STATE_DIAGRAM_NAME_OF_INITIAL_STATE
};

} // namespace state_diagram
