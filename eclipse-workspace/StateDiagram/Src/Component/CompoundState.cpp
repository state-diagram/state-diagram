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

#include "Impl/CompoundStateImpl.h"

namespace state_diagram
{

CompoundState
::CompoundState(CompoundStateImpl * const impl)
:
  NamePath{impl}
, PImplUpcast<CompoundStateImpl>{impl}
{
  // This space intentionally left empty
}

CompoundStateImpl *
CompoundState
::implUpcast()
const
{
  return PImplUpcast<CompoundStateImpl>::m_implUpcast;
}

} // namespace state_diagram

