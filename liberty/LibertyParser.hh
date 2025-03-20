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

#include "Zlib.hh"  // cdli
#include "Vector.hh"  // cdli
#include "Map.hh"  // cdli
#include "Set.hh"  // cdli
#include "StringUtil.hh"  // cdli

namespace sta {  // cdli

class Report;  // cdli
class LibertyGroupVisitor;  // cdli
class LibertyAttrVisitor;  // cdli
class LibertyStmt;  // cdli
class LibertyGroup;  // cdli
class LibertyAttr;  // cdli
class LibertyDefine;  // cdli
class LibertyAttrValue;  // cdli
class LibertyVariable;  // cdli
class LibertySubgroupIterator;  // cdli
class LibertyAttrIterator;  // cdli
class LibertyScanner;  // cdli

typedef Vector<LibertyStmt*> LibertyStmtSeq;  // cdli
typedef Vector<LibertyGroup*> LibertyGroupSeq;  // cdli
typedef Vector<LibertyAttr*> LibertyAttrSeq;  // cdli
typedef Map<const char *, LibertyAttr*, CharPtrLess> LibertyAttrMap;  // cdli
typedef Map<const char *, LibertyDefine*, CharPtrLess> LibertyDefineMap;  // cdli
typedef Vector<LibertyAttrValue*> LibertyAttrValueSeq;  // cdli
typedef Map<const char *, float, CharPtrLess> LibertyVariableMap;  // cdli
typedef Map<const char*,LibertyGroupVisitor*,CharPtrLess>LibertyGroupVisitorMap;  // cdli
typedef LibertyAttrValueSeq::Iterator LibertyAttrValueIterator;  // cdli
typedef Vector<LibertyGroup*> LibertyGroupSeq;  // cdli

enum class LibertyAttrType { attr_string, attr_int, attr_double,
			     attr_boolean, attr_unknown };  // cdli

enum class LibertyGroupType { library, cell, pin, timing, unknown };  // cdli

class LibertyParser  // cdli
{
public:
  LibertyParser(const char *filename,
                LibertyGroupVisitor *library_visitor,
                Report *report);  // cdli
  const string &filename() const { return filename_; }  // cdli
  void setFilename(const string &filename);  // cdli
  Report *report() const { return report_; }  // cdli
  LibertyStmt *makeDefine(LibertyAttrValueSeq *values,
                          int line);  // cdli
  LibertyAttrType attrValueType(const char *value_type_name);  // cdli
  LibertyGroupType groupType(const char *group_type_name);  // cdli
  void groupBegin(const char *type,
                  LibertyAttrValueSeq *params,
                  int line);  // cdli
  LibertyGroup *groupEnd();  // cdli
  LibertyGroup *group();  // cdli
  void deleteGroups();  // cdli
  LibertyStmt *makeSimpleAttr(const char *name,
                              LibertyAttrValue *value,
                              int line);  // cdli
  LibertyStmt *makeComplexAttr(const char *name,
                               LibertyAttrValueSeq *values,
                               int line);  // cdli
  LibertyAttrValue *makeStringAttrValue(char *value);  // cdli
  LibertyAttrValue *makeFloatAttrValue(float value);  // cdli
  LibertyStmt *makeVariable(char *var,
                            float value,
                            int line);  // cdli

private:
  string filename_;  // cdli
  LibertyGroupVisitor *group_visitor_;  // cdli
  Report *report_;  // cdli
  LibertyGroupSeq group_stack_;  // cdli
};

// Abstract base class for liberty statements.
class LibertyStmt  // cdli
{
public:
  explicit LibertyStmt(int line);  // cdli
  virtual ~LibertyStmt() {}  // cdli
  int line() const { return line_; }  // cdli
  virtual bool isGroup() const { return false; }  // cdli
  virtual bool isAttribute() const { return false; }  // cdli
  virtual bool isDefine() const { return false; }  // cdli
  virtual bool isVariable() const { return false; }  // cdli

protected:
  int line_;  // cdli
};

// Groups are a type keyword with a set of parameters and statements
// enclosed in brackets.
//  type([param1][, param2]...) { stmts.. }
class LibertyGroup : public LibertyStmt  // cdli
{
public:
  LibertyGroup(const char *type,
	       LibertyAttrValueSeq *params,
	       int line);  // cdli
  virtual ~LibertyGroup();  // cdli
  virtual bool isGroup() const { return true; }  // cdli
  const char *type() const { return type_; }  // cdli
  // First param as a string.
  const char *firstName();  // cdli
  // Second param as a string.
  const char *secondName();  // cdli
  LibertyAttr *findAttr(const char *name);  // cdli
  void addSubgroup(LibertyGroup *subgroup);  // cdli
  void addDefine(LibertyDefine *define);  // cdli
  void addAttribute(LibertyAttr *attr);  // cdli
  void addVariable(LibertyVariable *var);  // cdli
  LibertyGroupSeq *subgroups() const { return subgroups_; }  // cdli
  LibertyAttrSeq *attrs() const { return attrs_; }  // cdli
  LibertyAttrValueSeq *params() const { return params_; }  // cdli

protected:
  void parseNames(LibertyAttrValueSeq *values);  // cdli

  const char *type_;  // cdli
  LibertyAttrValueSeq *params_;  // cdli
  LibertyAttrSeq *attrs_;  // cdli
  LibertyAttrMap *attr_map_;  // cdli
  LibertyGroupSeq *subgroups_;  // cdli
  LibertyDefineMap *define_map_;  // cdli
};

class LibertySubgroupIterator : public LibertyGroupSeq::Iterator  // cdli
{
public:
  explicit LibertySubgroupIterator(LibertyGroup *group);  // cdli
};

class LibertyAttrIterator : public LibertyAttrSeq::Iterator  // cdli
{
public:
  explicit LibertyAttrIterator(LibertyGroup *group);  // cdli
};

// Abstract base class for attributes.
class LibertyAttr : public LibertyStmt  // cdli
{
public:
  LibertyAttr(const char *name,
	      int line);  // cdli
  virtual ~LibertyAttr();  // cdli
  const char *name() const { return name_; }  // cdli
  virtual bool isAttribute() const { return true; }  // cdli
  virtual bool isSimple() const = 0;  // cdli
  virtual bool isComplex() const = 0;  // cdli
  virtual LibertyAttrValueSeq *values() const = 0;  // cdli
  virtual LibertyAttrValue *firstValue() = 0;  // cdli

protected:
  const char *name_;  // cdli
};

// Abstract base class for simple attributes.
//  name : value;
class LibertySimpleAttr : public LibertyAttr  // cdli
{
public:
  LibertySimpleAttr(const char *name,
		    LibertyAttrValue *value,
		    int line);  // cdli
  virtual ~LibertySimpleAttr();  // cdli
  virtual bool isSimple() const { return true; }  // cdli
  virtual bool isComplex() const { return false; }  // cdli
  virtual LibertyAttrValue *firstValue() { return value_; }  // cdli
  virtual LibertyAttrValueSeq *values() const;  // cdli

private:
  LibertyAttrValue *value_;  // cdli
};

// Complex attributes have multiple values.
//  name(attr_value1[, attr_value2]...);
class LibertyComplexAttr : public LibertyAttr  // cdli
{
public:
  LibertyComplexAttr(const char *name,
		     LibertyAttrValueSeq *values,
		     int line);  // cdli
  virtual ~LibertyComplexAttr();  // cdli
  virtual bool isSimple() const { return false; }  // cdli
  virtual bool isComplex() const { return true; }  // cdli
  virtual LibertyAttrValue *firstValue();  // cdli
  virtual LibertyAttrValueSeq *values() const { return values_; }  // cdli

private:
  LibertyAttrValueSeq *values_;  // cdli
};

// Attribute values are a string or float.
class LibertyAttrValue  // cdli
{
public:
  LibertyAttrValue() {}  // cdli
  virtual ~LibertyAttrValue() {}  // cdli
  virtual bool isString() = 0;  // cdli
  virtual bool isFloat() = 0;  // cdli
  virtual float floatValue() = 0;  // cdli
  virtual const char *stringValue() = 0;  // cdli
};

class LibertyStringAttrValue : public LibertyAttrValue  // cdli
{
public:
  explicit LibertyStringAttrValue(const char *value);  // cdli
  virtual ~LibertyStringAttrValue();  // cdli
  virtual bool isFloat() { return false; }  // cdli
  virtual bool isString() { return true; }  // cdli
  virtual float floatValue();  // cdli
  virtual const char *stringValue();  // cdli

private:
  const char *value_;  // cdli
};

class LibertyFloatAttrValue : public LibertyAttrValue  // cdli
{
public:
  explicit LibertyFloatAttrValue(float value);  // cdli
  virtual bool isString() { return false; }  // cdli
  virtual bool isFloat() { return true; }  // cdli
  virtual float floatValue();  // cdli
  virtual const char *stringValue();  // cdli

private:
  float value_;  // cdli
};

// Define statements define new simple attributes.
//  define(attribute_name, group_name, attribute_type);
//  attribute_type is string|integer|float.
class LibertyDefine : public LibertyStmt  // cdli
{
public:
  LibertyDefine(const char *name,
		LibertyGroupType group_type,
		LibertyAttrType value_type,
		int line);  // cdli
  virtual ~LibertyDefine();  // cdli
  virtual bool isDefine() const { return true; }  // cdli
  const char *name() const { return name_; }  // cdli
  LibertyGroupType groupType() const { return group_type_; }  // cdli
  LibertyAttrType valueType() const { return value_type_; }  // cdli

private:
  const char *name_;  // cdli
  LibertyGroupType group_type_;  // cdli
  LibertyAttrType value_type_;  // cdli
};

// The Liberty User Guide Version 2003.12 fails to document variables.
// var = value;
// The only example I have only uses float values, so I am assuming
// that is all that is supported (which is probably wrong).
class LibertyVariable : public LibertyStmt  // cdli
{
public:
  LibertyVariable(const char *var,
		  float value,
		  int line);  // cdli
  // var_ is NOT deleted by ~LibertyVariable because the group
  // variable map ref's it.
  virtual ~LibertyVariable();  // cdli
  virtual bool isVariable() const { return true; }  // cdli
  const char *variable() const { return var_; }  // cdli
  float value() const { return value_; }  // cdli

private:
  const char *var_;  // cdli
  float value_;  // cdli
};

class LibertyGroupVisitor  // cdli
{
public:
  LibertyGroupVisitor() {}  // cdli
  virtual ~LibertyGroupVisitor() {}  // cdli
  virtual void begin(LibertyGroup *group) = 0;  // cdli
  virtual void end(LibertyGroup *group) = 0;  // cdli
  virtual void visitAttr(LibertyAttr *attr) = 0;  // cdli
  virtual void visitVariable(LibertyVariable *variable) = 0;  // cdli
  // Predicates to save parse structure after visits.
  virtual bool save(LibertyGroup *group) = 0;  // cdli
  virtual bool save(LibertyAttr *attr) = 0;  // cdli
  virtual bool save(LibertyVariable *variable) = 0;  // cdli
};

void
parseLibertyFile(const char *filename,
		 LibertyGroupVisitor *library_visitor,
		 Report *report);  // cdli
} // namespace
