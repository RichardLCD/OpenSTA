# OpenSTA, Static Timing Analyzer
# Copyright (c) 2025, Parallax Software, Inc.
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <https://www.gnu.org/licenses/>.
# 
# The origin of this software must not be misrepresented; you must not
# claim that you wrote the original software.
# 
# Altered source versions must be plainly marked as such, and must not be
# misrepresented as being the original software.
# 
# This notice may not be removed or altered from any source distribution.

# Liberty commands.

namespace eval sta {

# cdli
define_cmd_args "read_liberty" \
  {[-corner corner] [-min] [-max] [-infer_latches] filename}

# cdli
proc_redirect read_liberty {
  parse_key_args "read_liberty" args keys {-corner} \
    flags {-min -max -infer_latches}
  check_argc_eq1 "read_liberty" $args

  set filename [file nativename [lindex $args 0]]
  set corner [parse_corner keys]
  set min_max [parse_min_max_all_flags flags]
  set infer_latches [info exists flags(-infer_latches)]
  read_liberty_cmd $filename $corner $min_max $infer_latches
}

# for regression testing
proc write_liberty { args } {
  check_argc_eq2 "write_liberty" $args

  set library [get_liberty_error "library" [lindex $args 0]]
  set filename [file nativename [lindex $args 1]]
  write_liberty_cmd $library $filename
}

# sta namespace end
}
