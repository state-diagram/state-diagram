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

#include "MaxableTransition.h"

namespace state_diagram
{

MaxableTransition
::MaxableTransition()
:
  m_haveMax1Flag{false}
, m_haveExecuted{bool()}
{
  // This space intentionally left empty
}

MaxableTransition
::~MaxableTransition()
{
  // This space intentionally left empty
}

void
MaxableTransition
::setMax1Flag()
{
  m_haveMax1Flag = true;
  m_haveExecuted = false;
}

void
MaxableTransition
::max1Disable()
{
  m_haveExecuted = true;
}

bool
MaxableTransition
::isMax1Enabled()
const
{
  if (!m_haveMax1Flag)
  {
    return true;
  }
  return !m_haveExecuted;
}

void
MaxableTransition
::reload()
{
  m_haveExecuted = false;
}

} // namespace state_diagram
