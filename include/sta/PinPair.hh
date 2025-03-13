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

#include "Hash.hh"
#include "Set.hh"
#include "NetworkClass.hh"

namespace sta {

typedef std::pair<const Pin*, const Pin*> PinPair;

class PinPairLess
{
public:
  PinPairLess(const Network *network);
  bool operator()(const PinPair &pair1,
		  const PinPair &pair2) const;

private:
  const Network *network_;
};


class PinPairSet : public Set<PinPair, PinPairLess>
{
public:
  PinPairSet(const Network *network);
};

class PinPairHash
{
public:
  PinPairHash(const Network *network);
  size_t operator()(const PinPair &pair) const;

private:
  const Network *network_;
};

class PinPairEqual
{
public:
  bool operator()(const PinPair &pair1,
		  const PinPair &pair2) const;
};

} // namespace
