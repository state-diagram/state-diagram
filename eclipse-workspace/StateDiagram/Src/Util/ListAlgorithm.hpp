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

#ifndef STATE_DIAGRAM_UTIL_LISTALGORITHM_HPP_
#define STATE_DIAGRAM_UTIL_LISTALGORITHM_HPP_

#include <functional>

#include "ForwardList.hpp"

namespace state_diagram
{

using namespace std;

namespace
{
  template<class Item, bool isConstFunc>
  void
  forEachItem
  (
    ForwardList<Item * const> const & list
  , function<void (conditional_t<isConstFunc, Item const, Item> * const)> const & f
  )
  {
    if constexpr (isConstFunc)
    {
      for_each(list.cbegin(), list.cend(), [&](Item const * const & item){f(item);});
    }
    else
    {
      for_each(list.begin(), list.end(), [&](Item * const & item){f(item);});
    }
  }

  template<class Item, bool isConstFunc>
  void
  forEachItemExcept
  (
    ForwardList<Item * const> const & list
  , function<void (conditional_t<isConstFunc, Item const, Item> * const)> const & f
  , Item const * const exceptee
  )
  {
    auto const g{[&](conditional_t<isConstFunc, Item const, Item> * const item){if (item != exceptee) f(item);}};
    forEachItem<Item, isConstFunc>(list, g);
  }

  template<class Item, bool isConstFunc>
  bool
  containsItem
  (
    ForwardList<conditional_t<isConstFunc, Item const, Item> * const> const & list
  , Item const * const searchItem
  )
  {
    for (auto const & listItem : list)
    {
      if (searchItem == listItem)
      {
        return true;
      }
    }
    return false;
  }
}

template<class Item>
void
forEachItem(ForwardList<Item * const> const & list, function<void (Item * const)> const & f)
{
  forEachItem<Item, false>(list, f);
}

template<class Item>
void
forEachItemExcept
(
  ForwardList<Item * const> const & list
, function<void (Item * const)> const & f
, Item const * const exceptee
)
{
  forEachItemExcept<Item, false>(list, f, exceptee);
}

template<class Item>
void
forEachCItem(ForwardList<Item * const> const & list, function<void (Item const * const)> const & f)
{
  forEachItem<Item, true>(list, f);
}

template<class Item>
void
forEachCItemExcept
(
  ForwardList<Item * const> const & list
, function<void (Item const * const)> const & f
, Item const * const exceptee
)
{
  forEachItemExcept<Item, true>(list, f, exceptee);
}

template<class Item>
bool
containsItem(ForwardList<Item * const> const & list, Item const * const item)
{
  return containsItem<Item, false>(list, item);
}

template<class Item>
bool
containsItem(ForwardList<Item const * const> const & list, Item const * const item)
{
  return containsItem<Item, true>(list, item);
}

#ifndef STATE_DIAGRAM_NO_SHUFFLING

namespace
{
  template<class Item, bool isConstFunc>
  void
  forEachItemRandomly
  (
    ForwardList<Item * const> const & list
  , function<void (conditional_t<isConstFunc, Item const, Item> * const)> const & f
  )
  {
    vector<conditional_t<isConstFunc, Item const, Item> *> shuffler{size(list)};
    {
      size_t idx{0};
      for (auto const & item : list)
      {
        shuffler[idx] = item;
        ++idx;
      }
    }
    random_shuffle(shuffler.begin(), shuffler.end());
    for_each(shuffler.cbegin(), shuffler.cend(), [&](auto const & item){f(item);});
  }

  template<class Item, bool isConstFunc>
  void
  forEachItemRandomlyExcept
  (
    ForwardList<Item * const> const & list
  , function<void (conditional_t<isConstFunc, Item const, Item> * const)> const & f
  , Item const * const exceptee
  )
  {
    auto const g{[&](conditional_t<isConstFunc, Item const, Item> * const item){if (item != exceptee) f(item);}};
    forEachItemRandomly<Item, isConstFunc>(list, g);
  }
}

template<class Item>
void
forEachItemRandomly(ForwardList<Item * const> const & list, function<void (Item * const)> const & f)
{
  forEachItemRandomly<Item, false>(list, f);
}

template<class Item>
void
forEachItemRandomlyExcept
(
  ForwardList<Item * const> const & list
, function<void (Item * const)> const & f
, Item const * const exceptee
)
{
  forEachItemRandomlyExcept<Item, false>(list, f, exceptee);
}

template<class Item>
void
forEachCItemRandomly(ForwardList<Item * const> const & list, function<void (Item const * const)> const & f)
{
  forEachItemRandomly<Item, true>(list, f);
}

template<class Item>
void
forEachCItemRandomlyExcept
(
  ForwardList<Item * const> const & list
, function<void (Item const * const)> const & f
, Item const * const exceptee
)
{
  forEachItemRandomlyExcept<Item, true>(list, f, exceptee);
}

#endif // STATE_DIAGRAM_NO_SHUFFLING

} // namespace state_diagram

#endif // STATE_DIAGRAM_UTIL_LISTALGORITHM_HPP_
