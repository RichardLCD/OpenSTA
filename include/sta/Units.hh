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

#include <string>  // cdli

namespace sta {  // cdli

using std::string;  // cdli

class Unit  // cdli
{
public:
  Unit(const char *suffix);  // cdli
  Unit(float scale,
       const char *suffix,
       int digits);  // cdli
  // Convert from sta units to user interface units.
  double staToUser(double value);  // cdli
  // Convert from user interface units to sta units.
  double userToSta(double value);  // cdli
  void operator=(const Unit &unit);  // cdli
  float scale() const { return scale_; }  // cdli
  void setScale(float scale);  // cdli
  const char *scaleAbbreviation() const;  // cdli
  const char *suffix() const { return suffix_.c_str(); }  // cdli
  // scale abbreviation + suffix
  const char *scaledSuffix() const { return scaled_suffix_.c_str(); }  // cdli
  void setSuffix(const char *suffix);  // cdli
  int digits() const { return digits_; }  // cdli
  void setDigits(int digits);  // cdli
  // Does not include suffix.
  int width() const;  // cdli
  const char *asString(float value) const;  // cdli
  const char *asString(double value) const;  // cdli
  const char *asString(float value,
		       int digits) const;  // cdli

private:
  void setScaledSuffix();  // cdli

  float scale_;			// multiplier from user units to internal units  // cdli
  string suffix_;		// print suffix  // cdli
  string scaled_suffix_;  // cdli
  int digits_;			// print digits (after decimal pt)  // cdli
};

// User interface units.
// Sta internal units are always seconds, farads, volts, amps.
class Units  // cdli
{
public:
  Units();  // cdli
  Unit *find(const char *unit_name);  // cdli
  void operator=(const Units &units);  // cdli
  Unit *timeUnit() { return &time_unit_; }  // cdli
  const Unit *timeUnit() const { return &time_unit_; }  // cdli
  Unit *capacitanceUnit() { return &capacitance_unit_; }  // cdli
  const Unit *capacitanceUnit() const { return &capacitance_unit_; }  // cdli
  Unit *voltageUnit() { return &voltage_unit_; }  // cdli
  const Unit *voltageUnit() const { return &voltage_unit_; }  // cdli
  Unit *resistanceUnit() { return &resistance_unit_; }  // cdli
  const Unit *resistanceUnit() const { return &resistance_unit_; }  // cdli
  Unit *currentUnit() { return &current_unit_; }  // cdli
  const Unit *currentUnit() const { return &current_unit_; }  // cdli
  Unit *powerUnit() { return &power_unit_; }  // cdli
  const Unit *powerUnit() const { return &power_unit_; }  // cdli
  Unit *distanceUnit() { return &distance_unit_; }  // cdli
  const Unit *distanceUnit() const { return &distance_unit_; }  // cdli
  Unit *scalarUnit() { return &scalar_unit_; }  // cdli
  const Unit *scalarUnit() const { return &scalar_unit_; }  // cdli

private:
  Unit time_unit_;  // cdli
  Unit resistance_unit_;  // cdli
  Unit capacitance_unit_;  // cdli
  Unit voltage_unit_;  // cdli
  Unit current_unit_;  // cdli
  Unit power_unit_;  // cdli
  Unit distance_unit_;  // cdli
  Unit scalar_unit_;  // cdli
};

} // namespace
