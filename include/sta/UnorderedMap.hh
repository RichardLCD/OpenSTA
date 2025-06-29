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

#include <unordered_map>
#include <algorithm>

namespace sta {

// Add convenience functions around STL container.
template <class KEY, class VALUE, class HASH = std::hash<KEY>, class EQUAL = std::equal_to<KEY>>
class UnorderedMap : public std::unordered_map<KEY, VALUE, HASH, EQUAL>  // cdli
{
public:
  UnorderedMap() :
    std::unordered_map<KEY, VALUE, HASH, EQUAL>()  // cdli
  {
  }

  explicit UnorderedMap(const HASH &hash) :
    std::unordered_map<KEY, VALUE, HASH, EQUAL>(0, hash, std::equal_to<KEY>())  // cdli
  {
  }

  explicit UnorderedMap(size_t size,
			const HASH &hash,
			const EQUAL &equal) :
    std::unordered_map<KEY, VALUE, HASH, EQUAL>(size, hash, equal)  // cdli
  {
  }

  // Find out if key is in the set.
  bool
  hasKey(const KEY key) const  // cdli
  {
    return this->find(key) != this->end();
  }

  // Find the value corresponding to key.
  VALUE
  findKey(const KEY key) const  // cdli
  {
    auto find_iter = this->find(key);
    if (find_iter != this->end())
      return find_iter->second;
    else
      return nullptr;
  }
  void
  findKey(const KEY key,
	  // Return Values.
	  VALUE &value,
	  bool &exists) const  // cdli
  {
    auto find_iter = this->find(key);
    if (find_iter != this->end()) {
      value = find_iter->second;
      exists = true;
    }
    else
      exists = false;
  }
  void
  findKey(const KEY &key,
	  // Return Values.
	  KEY &map_key,
	  VALUE &value,
	  bool &exists) const  // cdli
  {
    auto find_iter = this->find(key);
    if (find_iter != this->end()) {
      map_key = find_iter->first;
      value = find_iter->second;
      exists = true;
    }
    else
      exists = false;
  }

  void
  insert(const KEY &key,
	 VALUE value)  // cdli
  {
    this->operator[](key) = value;
  }

  void
  deleteContents()  // cdli
  {
    Iterator iter(this);
    while (iter.hasNext())
      delete iter.next();
  }

  void
  deleteKeysContents()  // cdli
  {
    Iterator iter(this);
    while (iter.hasNext()) {
      KEY key;
      VALUE value;
      iter.next(key, value);
      delete key;
      delete value;
    }
  }

  void
  deleteArrayContents()  // cdli
  {
    Iterator iter(this);
    while (iter.hasNext())
      delete [] iter.next();
  }

  void
  deleteContentsClear()  // cdli
  {
    deleteContents();
    std::unordered_map<KEY,VALUE,HASH,EQUAL>::clear();
  }

  // Java style container itererator
  //  Map::Iterator<string *, Value, stringLess> iter(map);
  //  while (iter.hasNext()) {
  //    Value *v = iter.next();
  //  }
  class Iterator  // cdli
  {
  public:
    Iterator() : container_(nullptr) {}  // cdli
    explicit Iterator(std::unordered_map<KEY,VALUE,HASH,EQUAL> *container) :
      container_(container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    explicit Iterator(std::unordered_map<KEY,VALUE,HASH,EQUAL> &container) :
      container_(&container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    void init(std::unordered_map<KEY,VALUE,HASH,EQUAL> *container)  // cdli
    { container_ = container; if (container_ != nullptr) iter_=container_->begin();}
    void init(std::unordered_map<KEY,VALUE,HASH,EQUAL> &container)  // cdli
    { container_ = &container; if (container_ != nullptr) iter_=container_->begin();}
    bool hasNext() { return container_ != nullptr && iter_ != container_->end(); }  // cdli
    VALUE next() { return iter_++->second; }  // cdli
    void next(KEY &key,
	      VALUE &value)  // cdli
    { key = iter_->first; value = iter_->second; iter_++; }
    std::unordered_map<KEY,VALUE,HASH,EQUAL> *container() { return container_; }  // cdli

  private:
    std::unordered_map<KEY,VALUE,HASH,EQUAL> *container_;  // cdli
    typename std::unordered_map<KEY,VALUE,HASH,EQUAL>::iterator iter_;  // cdli
  };

  class ConstIterator  // cdli
  {
  public:
    ConstIterator() : container_(nullptr) {}  // cdli
    explicit ConstIterator(const std::unordered_map<KEY,VALUE,HASH,EQUAL> *container) :
      container_(container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    explicit ConstIterator(const std::unordered_map<KEY,VALUE,HASH,EQUAL> &container) :
      container_(&container)  // cdli
    { if (container_ != nullptr) iter_ = container_->begin(); }
    void init(const std::unordered_map<KEY,VALUE,HASH,EQUAL> *container)  // cdli
    { container_ = container; if (container_ != nullptr) iter_=container_->begin();}
    void init(const std::unordered_map<KEY,VALUE,HASH,EQUAL> &container)  // cdli
    { container_ = &container; if (container_ != nullptr) iter_=container_->begin();}
    bool hasNext() { return container_ != nullptr && iter_ != container_->end(); }  // cdli
    VALUE next() { return iter_++->second; }  // cdli
    void next(KEY &key,
	      VALUE &value)  // cdli
    { key = iter_->first; value = iter_->second; iter_++; }
    const std::unordered_map<KEY,VALUE,HASH,EQUAL> *container() { return container_; }  // cdli

  private:
    const std::unordered_map<KEY,VALUE,HASH,EQUAL> *container_;  // cdli
    typename std::unordered_map<KEY,VALUE,HASH,EQUAL>::const_iterator iter_;  // cdli
  };
};

} // namespace
