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

#ifndef STATE_DIAGRAM_UTIL_SHUFFLINGALGORITHM_HPP_
#define STATE_DIAGRAM_UTIL_SHUFFLINGALGORITHM_HPP_

#ifndef STATE_DIAGRAM_NO_SHUFFLING

#include <algorithm>
#include <vector>

#include "ForwardList.hpp"

namespace state_diagram
{

using namespace std;

namespace
{
  template<class T, bool sharedConsts>
  void
  populateShuffle
  (
    vector<conditional_t<sharedConsts, T const, T> *> & shuffler
  , ForwardList<conditional_t<sharedConsts, shared_ptr<T const>, T *> const> const & src
  )
  {
    size_t idx{0};
    for (auto const & x : src)
    {
      if constexpr (sharedConsts)
      {
        shuffler[idx] = x.get();
      }
      else
      {
        shuffler[idx] = x;
      }
      ++idx;
    }
    random_shuffle(shuffler.begin(), shuffler.end());
  }
}

template<class T>
void
populateShuffle(vector<T const *> & shuffler, ForwardList<shared_ptr<T const> const> const & src)
{
  populateShuffle<T, true>(shuffler, src);
}

template<class T>
void
populateShuffle(vector<T *> & shuffler, ForwardList<T * const> const & src)
{
  populateShuffle<T, false>(shuffler, src);
}

} // namespace state_diagram

#endif // STATE_DIAGRAM_NO_SHUFFLING

#endif // STATE_DIAGRAM_UTIL_SHUFFLINGALGORITHM_HPP_
