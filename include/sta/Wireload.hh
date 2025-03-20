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

#include "Vector.hh"  // cdli
#include "LibertyClass.hh"  // cdli

namespace sta {  // cdli

class WireloadForArea;  // cdli

typedef std::pair<float,float> FanoutLength;  // cdli
typedef Vector<FanoutLength*> FanoutLengthSeq;  // cdli
typedef Vector<WireloadForArea*> WireloadForAreaSeq;  // cdli

const char *
wireloadTreeString(WireloadTree tree);  // cdli
WireloadTree
stringWireloadTree(const char *tree);  // cdli

const char *
wireloadModeString(WireloadMode wire_load_mode);  // cdli
WireloadMode
stringWireloadMode(const char *wire_load_mode);  // cdli

class Wireload  // cdli
{
public:
  Wireload(const char *name,
	   LibertyLibrary *library);  // cdli
  Wireload(const char *name,
	   LibertyLibrary *library,
	   float area,
	   float resistance,
	   float capacitance,
	   float slope);  // cdli
  virtual ~Wireload();  // cdli
  const char *name() const { return name_; }  // cdli
  void setArea(float area);  // cdli
  void setResistance(float res);  // cdli
  void setCapacitance(float cap);  // cdli
  void setSlope(float slope);  // cdli
  void addFanoutLength(float fanout,
		       float length);  // cdli
  // Find wireload resistance/capacitance for fanout.
  virtual void findWireload(float fanout,
			    const OperatingConditions *op_cond,
			    float &cap,
			    float &res) const;  // cdli

protected:
  const char *name_;  // cdli
  LibertyLibrary *library_;  // cdli
  float area_;  // cdli
  float resistance_;  // cdli
  float capacitance_;  // cdli
  // Fanout length extrapolation slope.
  float slope_;  // cdli
  FanoutLengthSeq fanout_lengths_;  // cdli
};

class WireloadSelection  // cdli
{
public:
  explicit WireloadSelection(const char *name);  // cdli
  ~WireloadSelection();  // cdli
  const char *name() const { return name_; }  // cdli
  void addWireloadFromArea(float min_area,
			   float max_area,
			   const Wireload *wireload);  // cdli
  const Wireload *findWireload(float area) const;  // cdli

private:
  const char *name_;  // cdli
  WireloadForAreaSeq wireloads_;  // cdli
};

} // namespace
