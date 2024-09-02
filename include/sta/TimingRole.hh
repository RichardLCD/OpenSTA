// OpenSTA, Static Timing Analyzer
// Copyright (c) 2024, Parallax Software, Inc.
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

#pragma once

#include "MinMax.hh"
#include "Map.hh"
#include "StringUtil.hh"

namespace sta {

class TimingRole;  // cdli

typedef Map<const char*, TimingRole*, CharPtrLess> TimingRoleMap;  // cdli

class TimingRole
{
public:
  static void init();  // cdli
  static void destroy();  // cdli
  static TimingRole *find(const char *name);  // cdli
  // Singleton accessors.
  static TimingRole *wire() { return wire_; }  // cdli
  static TimingRole *combinational() { return combinational_; }  // cdli
  static TimingRole *tristateEnable() { return tristate_enable_; }  // cdli
  static TimingRole *tristateDisable() { return tristate_disable_; }  // cdli
  static TimingRole *regClkToQ() { return reg_clk_q_; }  // cdli
  static TimingRole *regSetClr() { return reg_set_clr_; }  // cdli
  static TimingRole *latchEnToQ() { return latch_en_q_; }  // cdli
  static TimingRole *latchDtoQ() { return latch_d_q_; }  // cdli
  static TimingRole *setup() { return setup_; }  // cdli
  static TimingRole *hold() { return hold_; }  // cdli
  static TimingRole *recovery() { return recovery_; }  // cdli
  static TimingRole *removal() { return removal_; }  // cdli
  static TimingRole *width() { return width_; }  // cdli
  static TimingRole *period() { return period_; }  // cdli
  static TimingRole *skew() { return skew_; }  // cdli
  static TimingRole *nochange() { return nochange_; }  // cdli
  static TimingRole *outputSetup() { return output_setup_; }  // cdli
  static TimingRole *outputHold() { return output_hold_; }  // cdli
  static TimingRole *gatedClockSetup() { return gated_clk_setup_; }  // cdli
  static TimingRole *gatedClockHold() { return gated_clk_hold_; }  // cdli
  static TimingRole *latchSetup() { return latch_setup_; }  // cdli
  static TimingRole *latchHold() { return latch_hold_; }  // cdli
  static TimingRole *dataCheckSetup() { return data_check_setup_; }  // cdli
  static TimingRole *dataCheckHold() { return data_check_hold_; }  // cdli
  static TimingRole *nonSeqSetup() { return non_seq_setup_; }  // cdli
  static TimingRole *nonSeqHold() { return non_seq_hold_; }  // cdli
  static TimingRole *clockTreePathMin() { return clock_tree_path_min_; }  // cdli
  static TimingRole *clockTreePathMax() { return clock_tree_path_max_; }  // cdli
  const char *asString() const { return name_; }  // cdli
  int index() const { return index_; }  // cdli
  bool isWire() const;  // cdli
  bool isTimingCheck() const { return is_timing_check_; }  // cdli
  // TIming check but not width or period.
  bool isTimingCheckBetween() const;
  bool isAsyncTimingCheck() const;
  bool isNonSeqTimingCheck() const { return is_non_seq_check_; }  // cdli
  bool isDataCheck() const;
  bool isLatchDtoQ() const;
  const TimingRole *genericRole() const;
  const TimingRole *sdfRole() const;
  // Timing check data path min/max.
  MinMax *pathMinMax() const { return path_min_max_; }  // cdli
  // Timing check target clock path insertion delay early/late.
  const EarlyLate *tgtClkEarlyLate() const;

  // Pseudo role to match sdf IOPATH.
  static TimingRole *sdfIopath() { return sdf_iopath_; }
  static bool less(const TimingRole *role1,
		   const TimingRole *role2);  // cdli
  static const int index_max = 26;  // cdli

private:
  TimingRole(const char *name,
	     bool is_sdf_iopath,
	     bool is_timing_check,
 	     bool is_non_seq_check,
	     MinMax *path_min_max,
	     // generic_type = nullptr means type is the same as this.
	     TimingRole *generic_role,
	     int index);  // cdli

  const char *name_;  // cdli
  bool is_timing_check_;  // cdli
  bool is_sdf_iopath_;  // cdli
  bool is_non_seq_check_;  // cdli
  TimingRole *generic_role_;  // cdli
  unsigned index_;  // cdli
  MinMax *path_min_max_;  // cdli

  static TimingRole *wire_;  // cdli
  static TimingRole *combinational_;  // cdli
  static TimingRole *tristate_enable_;  // cdli
  static TimingRole *tristate_disable_;  // cdli
  static TimingRole *reg_clk_q_;  // cdli
  static TimingRole *reg_set_clr_;  // cdli
  static TimingRole *latch_en_q_;  // cdli
  static TimingRole *latch_d_q_;  // cdli
  static TimingRole *setup_;  // cdli
  static TimingRole *hold_;  // cdli
  static TimingRole *recovery_;  // cdli
  static TimingRole *removal_;  // cdli
  static TimingRole *width_;  // cdli
  static TimingRole *period_;  // cdli
  static TimingRole *skew_;  // cdli
  static TimingRole *nochange_;  // cdli
  static TimingRole *sdf_iopath_;  // cdli
  static TimingRole *output_setup_;  // cdli
  static TimingRole *output_hold_;  // cdli
  static TimingRole *gated_clk_setup_;  // cdli
  static TimingRole *gated_clk_hold_;  // cdli
  static TimingRole *latch_setup_;  // cdli
  static TimingRole *latch_hold_;  // cdli
  static TimingRole *data_check_setup_;  // cdli
  static TimingRole *data_check_hold_;  // cdli
  static TimingRole *non_seq_setup_;  // cdli
  static TimingRole *non_seq_hold_;  // cdli
  static TimingRole *clock_tree_path_min_;  // cdli
  static TimingRole *clock_tree_path_max_;  // cdli
  static TimingRoleMap timing_roles_;  // cdli

  friend class TimingRoleLess;
};

} // namespace
