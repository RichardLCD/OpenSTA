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

#include "NetworkClass.hh"  // cdli

namespace sta {  // cdli

class PortDirection  // cdli
{
public:
  static void init();  // cdli
  static void destroy();  // cdli
  // Singleton accessors.
  static PortDirection *input() { return input_; }  // cdli
  static PortDirection *output() { return output_; }  // cdli
  static PortDirection *tristate() { return tristate_; }  // cdli
  static PortDirection *bidirect() { return bidirect_; }  // cdli
  static PortDirection *internal() { return internal_; }  // cdli
  static PortDirection *ground() { return ground_; }  // cdli
  static PortDirection *power() { return power_; }  // cdli
  static PortDirection *unknown() { return unknown_; }  // cdli
  static PortDirection *find(const char *dir_name);  // cdli
  const char *name() const { return name_; }  // cdli
  int index() const { return index_; }  // cdli
  bool isInput() const { return this == input_; }  // cdli
  // Input or bidirect.
  bool isAnyInput() const;  // cdli
  bool isOutput() const { return this == output_; }  // cdli
  // Output, tristate or bidirect.
  bool isAnyOutput() const;  // cdli
  bool isTristate() const { return this == tristate_; }  // cdli
  bool isBidirect() const { return this == bidirect_; }  // cdli
  // Bidirect or tristate.
  bool isAnyTristate() const;  // cdli
  bool isGround() const { return this == ground_; }  // cdli
  bool isPower() const { return this == power_; }  // cdli
  // Ground or power.
  bool isPowerGround() const;  // cdli
  bool isInternal() const { return this == internal_; }  // cdli
  bool isUnknown() const { return this == unknown_; }  // cdli

private:
  PortDirection(const char *name,
		int index);  // cdli

  const char *name_;  // cdli
  int index_;  // cdli

  static PortDirection *input_;  // cdli
  static PortDirection *output_;  // cdli
  static PortDirection *tristate_;  // cdli
  static PortDirection *bidirect_;  // cdli
  static PortDirection *internal_;  // cdli
  static PortDirection *ground_;  // cdli
  static PortDirection *power_;  // cdli
  static PortDirection *unknown_;  // cdli
};

} // namespace
