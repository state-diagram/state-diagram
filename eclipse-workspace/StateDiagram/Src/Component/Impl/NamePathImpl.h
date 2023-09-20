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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_NAMEPATHIMPL_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_NAMEPATHIMPL_H_

#include "state_diagram/state_diagram.h"

namespace state_diagram
{

class NamePathImpl
{
protected:
  NamePathImpl(STATE_DIAGRAM_STRING_PARAM(name));

  virtual ~NamePathImpl();

public:
#ifndef STATE_DIAGRAM_STRINGLESS
  virtual void pathPrefix(ostream & to) const;
  virtual void path(ostream & to) const = 0;
  string path() const;

  string const name;
#endif // STATE_DIAGRAM_STRINGLESS
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_NAMEPATHIMPL_H_
