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

#include "PortDirection.hh"  // cdli

#include "StringUtil.hh"  // cdli

namespace sta {  // cdli

PortDirection *PortDirection::input_;  // cdli
PortDirection *PortDirection::output_;  // cdli
PortDirection *PortDirection::tristate_;  // cdli
PortDirection *PortDirection::bidirect_;  // cdli
PortDirection *PortDirection::internal_;  // cdli
PortDirection *PortDirection::ground_;  // cdli
PortDirection *PortDirection::power_;  // cdli
PortDirection *PortDirection::unknown_;  // cdli

void
PortDirection::init()  // cdli
{
  input_ = new PortDirection("input", 0);  // cdli
  output_ = new PortDirection("output", 1);  // cdli
  tristate_ = new PortDirection("tristate", 2);  // cdli
  bidirect_ = new PortDirection("bidirect", 3);  // cdli
  internal_ = new PortDirection("internal", 4);  // cdli
  ground_ = new PortDirection("ground", 5);  // cdli
  power_ = new PortDirection("power", 6);  // cdli
  unknown_ = new PortDirection("unknown", 7);  // cdli
}

void
PortDirection::destroy()  // cdli
{
  delete input_;  // cdli
  input_ = nullptr;
  delete output_;  // cdli
  output_ = nullptr;  // cdli
  delete tristate_;
  tristate_ = nullptr;  // cdli
  delete bidirect_;
  bidirect_ = nullptr;  // cdli
  delete internal_;
  internal_ = nullptr;  // cdli
  delete ground_;
  ground_ = nullptr;  // cdli
  delete power_;
  power_ = nullptr;  // cdli
  delete unknown_;
  unknown_ = nullptr;  // cdli
}

PortDirection::PortDirection(const char *name,
			     int index) :  // cdli
  name_(name),  // cdli
  index_(index)  // cdli
{
}

PortDirection *
PortDirection::find(const char *dir_name)  // cdli
{
  if (stringEqual(dir_name, "input"))
    return input_;
  else if (stringEqual(dir_name, "output"))
    return output_;
  else if (stringEqual(dir_name, "tristate"))
    return tristate_;
  else if (stringEqual(dir_name, "bidirect"))
    return bidirect_;
  else if (stringEqual(dir_name, "internal"))
    return internal_;
  else if (stringEqual(dir_name, "ground"))
    return ground_;
  else if (stringEqual(dir_name, "power"))
    return power_;
  else
    return nullptr;
}

bool
PortDirection::isAnyInput() const  // cdli
{
  return this == input_
    || this == bidirect_;
}

bool
PortDirection::isAnyOutput() const  // cdli
{
  return this == output_
    || this == tristate_
    || this == bidirect_;
}

bool
PortDirection::isAnyTristate() const  // cdli
{
  return this == tristate_
    || this == bidirect_;
}

bool
PortDirection::isPowerGround() const  // cdli
{
  return this == ground_
    || this == power_;
}

} // namespace
