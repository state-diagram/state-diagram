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

#include "Impl/RegionImpl.h"
#include "Impl/TopStateImpl.h"

namespace state_diagram
{

Top
::Top(STATE_DIAGRAM_STRING_PARAM(name))
:
  PImpl<TopStateImpl>{STATE_DIAGRAM_STRING_ARG(name)}
, NamePath{m_impl}
, CompoundState{m_impl}
{
  // This space intentionally left empty
}

Top
::~Top()
{
  delete m_impl;
}

void
Top
::init()
const
{
  m_impl->init();
}

bool
Top
::step()
const
{
  return m_impl->exec();
}

void
Top
::activate(ExternalEvent const & trigger)
const
{
  return m_impl->activate(trigger.impl());
}

} // namespace state_diagram
