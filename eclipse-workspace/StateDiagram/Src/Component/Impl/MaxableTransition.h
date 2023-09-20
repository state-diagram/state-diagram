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

#ifndef STATE_DIAGRAM_COMPONENT_IMPL_MAXABLETRANSITION_H_
#define STATE_DIAGRAM_COMPONENT_IMPL_MAXABLETRANSITION_H_

namespace state_diagram
{

class MaxableTransition
{
protected:
  MaxableTransition();

  virtual ~MaxableTransition();

public:
  void setMax1Flag();

protected:
  bool isMax1Enabled() const;
  void max1Disable();

public:
  void reload();

private:
  bool m_haveMax1Flag;
  bool m_haveExecuted;
};

} // namespace state_diagram

#endif // STATE_DIAGRAM_COMPONENT_IMPL_MAXABLETRANSITION_H_
