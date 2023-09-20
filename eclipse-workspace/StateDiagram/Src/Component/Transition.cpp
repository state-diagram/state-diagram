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

namespace state_diagram
{

Transition
::Transition(TransitionImpl * const impl)
:
  PImplUpcast<TransitionImpl>{impl}
{
  // This space intentionally left empty
}

Transition
::~Transition()
{
  // This space intentionally left empty
}

TransitionImpl *
Transition
::implUpcast()
const
{
  return PImplUpcast<TransitionImpl>::m_implUpcast;
}

Transition const &
Transition
::add(Spec const & spec)
const
{
  spec.join(this);
  return *this;
}

Transition const &
Transition
::operator<<(Spec const & spec)
const
{
  return add(spec);
}

} // namespace state_diagram
