// OpenSTA, Static Timing Analyzer
// Copyright (c) 2025, Parallax Software, Inc.
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
// 
// The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.
// 
// Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
// 
// This notice may not be removed or altered from any source distribution.

#pragma once  // cdli

#include <unordered_set>
#include <algorithm>

namespace sta {

// Add convenience functions around STL container.
template <class KEY, class HASH = std::hash<KEY>, class EQUAL = std::equal_to<KEY> >
class UnorderedSet : public std::unordered_set<KEY, HASH, EQUAL>  // cdli
{
public:
  UnorderedSet() :
    std::unordered_set<KEY, HASH, EQUAL>()  // cdli
  {
  }

  explicit UnorderedSet(size_t size) :
    std::unordered_set<KEY, HASH, EQUAL>(size)  // cdli
  {
  }

  explicit UnorderedSet(size_t size,
			const HASH &hash,
			const EQUAL &equal) :
    std::unordered_set<KEY, HASH, EQUAL>(size, hash, equal)  // cdli
  {
  }

  // Find out if key is in the set.
  bool
  hasKey(const KEY key) const  // cdli
  {
    return this->find(key) != this->end();
  }

  // Find the value corresponding to key.
  KEY
  findKey(const KEY key) const  // cdli
  {
    auto find_iter = this->find(key);
    if (find_iter != this->end())
      return *find_iter;
    else
      return nullptr;
  }

  void
  deleteContents()  // cdli
  {
    Iterator iter(this);
    while (iter.hasNext())
      delete iter.next();
  }

  void
  deleteContentsClear()  // cdli
  {
    deleteContents();
    std::unordered_set<KEY,HASH,EQUAL>::clear();
  }

  // Java style container itererator
  //  Set::Iterator<string *, Value, stringLess> iter(set);
  //  while (iter.hasNext()) {
  //    Value *v = iter.next();
  //  }
  class Iterator  // cdli
  {
  public:
    Iterator() : container_(nullptr) {}  // cdli
    explicit Iterator(std::unordered_set<KEY,HASH,EQUAL> *container) :
      container_(container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    explicit Iterator(std::unordered_set<KEY,HASH,EQUAL> &container) :
      container_(&container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    void init(std::unordered_set<KEY,HASH,EQUAL> *container)  // cdli
    { container_ = container; if (container_ != nullptr) iter_=container_->begin();}
    void init(std::unordered_set<KEY,HASH,EQUAL> &container)  // cdli
    { container_ = &container; if (container_ != nullptr) iter_=container_->begin();}
    bool hasNext() { return container_ != nullptr && iter_ != container_->end(); }  // cdli
    KEY next() { return *iter_++; }  // cdli
    std::unordered_set<KEY,HASH,EQUAL> *container() { return container_; }  // cdli

  private:
    std::unordered_set<KEY,HASH,EQUAL> *container_;  // cdli
    typename std::unordered_set<KEY,HASH,EQUAL>::iterator iter_;  // cdli
  };

  class ConstIterator  // cdli
  {
  public:
    ConstIterator() : container_(nullptr) {}  // cdli
    explicit ConstIterator(const std::unordered_set<KEY,HASH,EQUAL> *container) :
      container_(container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    explicit ConstIterator(const std::unordered_set<KEY,HASH,EQUAL> &container) :
      container_(&container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    void init(const std::unordered_set<KEY,HASH,EQUAL> *container)  // cdli
    { container_ = container; if (container_ != nullptr) iter_=container_->begin();}
    void init(const std::unordered_set<KEY,HASH,EQUAL> &container)  // cdli
    { container_ = &container; if (container_ != nullptr) iter_=container_->begin();}
    bool hasNext() { return container_ != nullptr && iter_ != container_->end(); }  // cdli
    KEY next() { return iter_++->second; }  // cdli
    const std::unordered_set<KEY,HASH,EQUAL> *container() { return container_; }  // cdli

  private:
    const std::unordered_set<KEY,HASH,EQUAL> *container_;  // cdli
    typename std::unordered_set<KEY,HASH,EQUAL>::const_iterator iter_;  // cdli
  };
};

} // namespace
