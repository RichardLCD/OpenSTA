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

#include "LibertyParser.hh"  // cdli

#include <cstdio>  // cdli
#include <cstring>  // cdli
#include <regex>  // cdli

#include "Zlib.hh"  // cdli
#include "Report.hh"  // cdli
#include "Error.hh"  // cdli
#include "StringUtil.hh"  // cdli
#include "LibertyScanner.hh"  // cdli

namespace sta {  // cdli

using std::string;  // cdli

void
parseLibertyFile(const char *filename,
		 LibertyGroupVisitor *library_visitor,
		 Report *report)  // cdli
{
  gzstream::igzstream stream(filename);
  if (stream.is_open()) {
    LibertyParser reader(filename, library_visitor, report);
    LibertyScanner scanner(&stream, filename, &reader, report);
    LibertyParse parser(&scanner, &reader);
    parser.parse();
  }
  else
    throw FileNotReadable(filename);
}

LibertyParser::LibertyParser(const char *filename,
                             LibertyGroupVisitor *library_visitor,
                             Report *report) :
  filename_(filename),  // cdli
  group_visitor_(library_visitor),  // cdli
  report_(report)  // cdli
{
}

void
LibertyParser::setFilename(const string &filename)  // cdli
{
  filename_ = filename;
}

LibertyStmt *
LibertyParser::makeDefine(LibertyAttrValueSeq *values,
                          int line)  // cdli
{
  LibertyDefine *define = nullptr;
  if (values->size() == 3) {
    const char *define_name = (*values)[0]->stringValue();
    const char *group_type_name = (*values)[1]->stringValue();
    const char *value_type_name = (*values)[2]->stringValue();
    LibertyAttrType value_type = attrValueType(value_type_name);
    LibertyGroupType group_type = groupType(group_type_name);
    define = new LibertyDefine(define_name, group_type,
			       value_type, line);
    LibertyGroup *group = this->group();
    group->addDefine(define);
  }
  else
    report_->fileWarn(24, filename_.c_str(), line,
                      "define does not have three arguments.");
  return define;
}

// The Liberty User Guide Version 2001.08 fails to define the strings
// used to define valid attribute types.  Beyond "string" these are
// guesses.
LibertyAttrType
LibertyParser::attrValueType(const char *value_type_name)  // cdli
{
  if (stringEq(value_type_name, "string"))
    return LibertyAttrType::attr_string;
  else if (stringEq(value_type_name, "integer"))
    return LibertyAttrType::attr_int;
  else if (stringEq(value_type_name, "float"))
    return LibertyAttrType::attr_double;
  else if (stringEq(value_type_name, "boolean"))
    return LibertyAttrType::attr_boolean;
  else
    return LibertyAttrType::attr_unknown;
}

LibertyGroupType
LibertyParser::groupType(const char *group_type_name)  // cdli
{
  if (stringEq(group_type_name, "library"))
    return LibertyGroupType::library;
  else if (stringEq(group_type_name, "cell"))
    return LibertyGroupType::cell;
  else if (stringEq(group_type_name, "pin"))
    return LibertyGroupType::pin;
  else if (stringEq(group_type_name, "timing"))
    return LibertyGroupType::timing;
  else
    return LibertyGroupType::unknown;
}

void
LibertyParser::groupBegin(const char *type,
                          LibertyAttrValueSeq *params,
                          int line)  // cdli
{
  LibertyGroup *group = new LibertyGroup(type, params, line);
  stringDelete(type);
  group_visitor_->begin(group);
  group_stack_.push_back(group);
}

LibertyGroup *
LibertyParser::groupEnd()  // cdli
{
  LibertyGroup *group = this->group();
  group_visitor_->end(group);
  group_stack_.pop_back();
  LibertyGroup *parent =
    group_stack_.empty() ? nullptr : group_stack_.back();
  if (parent && group_visitor_->save(group)) {
    parent->addSubgroup(group);
    return group;
  }
  else {
    delete group;
    return nullptr;
  }
}

LibertyGroup *
LibertyParser::group()  // cdli
{
  return group_stack_.back();
}

void
LibertyParser::deleteGroups()  // cdli
{
  group_stack_.deleteContentsClear();
}

LibertyStmt *
LibertyParser::makeSimpleAttr(const char *name,
                              LibertyAttrValue *value,
                              int line)  // cdli
{
  LibertyAttr *attr = new LibertySimpleAttr(name, value, line);
  stringDelete(name);
  group_visitor_->visitAttr(attr);
  LibertyGroup *group = this->group();
  if (group && group_visitor_->save(attr)) {
    group->addAttribute(attr);
    return attr;
  }
  else {
    delete attr;
    return nullptr;
  }
}

LibertyStmt *
LibertyParser::makeComplexAttr(const char *name,
                               LibertyAttrValueSeq *values,
                               int line)  // cdli
{
  // Defines have the same syntax as complex attributes.
  // Detect and convert them.
  if (stringEq(name, "define")) {
    LibertyStmt *define = makeDefine(values, line);
    stringDelete(name);
    LibertyAttrValueSeq::Iterator attr_iter(values);
    while (attr_iter.hasNext())
      delete attr_iter.next();
    delete values;
    return define;
  }
  else {
    LibertyAttr *attr = new LibertyComplexAttr(name, values, line);
    stringDelete(name);
    group_visitor_->visitAttr(attr);
    if (group_visitor_->save(attr)) {
      LibertyGroup *group = this->group();
      group->addAttribute(attr);
      return attr;
    }
    delete attr;
    return nullptr;
  }
}

LibertyStmt *
LibertyParser::makeVariable(const char *var,
                            float value,
                            int line)  // cdli
{
  LibertyVariable *variable = new LibertyVariable(var, value, line);
  stringDelete(var);
  group_visitor_->visitVariable(variable);
  if (group_visitor_->save(variable))
    return variable;
  else {
    delete variable;
    return nullptr;
  }
}

LibertyAttrValue *
LibertyParser::makeStringAttrValue(char *value)  // cdli
{
  LibertyAttrValue *attr = new LibertyStringAttrValue(value);
  stringDelete(value);
  return attr;
}

LibertyAttrValue *
LibertyParser::makeFloatAttrValue(float value)  // cdli
{
  return new LibertyFloatAttrValue(value);
}

////////////////////////////////////////////////////////////////

LibertyStmt::LibertyStmt(int line) :
  line_(line)  // cdli
{
}

LibertyGroup::LibertyGroup(const char *type,
			   LibertyAttrValueSeq *params,
			   int line) :
  LibertyStmt(line),  // cdli
  type_(type),  // cdli
  params_(params),  // cdli
  attrs_(nullptr),  // cdli
  attr_map_(nullptr),  // cdli
  subgroups_(nullptr),  // cdli
  define_map_(nullptr)  // cdli
{
}

void
LibertyGroup::addSubgroup(LibertyGroup *subgroup)  // cdli
{
  if (subgroups_ == nullptr)
    subgroups_ = new LibertyGroupSeq;
  subgroups_->push_back(subgroup);
}

void
LibertyGroup::addDefine(LibertyDefine *define)  // cdli
{
  if (define_map_ == nullptr)
    define_map_ = new LibertyDefineMap;
  const char *define_name = define->name();
  LibertyDefine *prev_define = define_map_->findKey(define_name);
  if (prev_define) {
    define_map_->erase(define_name);
    delete prev_define;
  }
  (*define_map_)[define_name] = define;
}

void
LibertyGroup::addAttribute(LibertyAttr *attr)  // cdli
{
  if (attrs_ == nullptr)
    attrs_ = new LibertyAttrSeq;
  attrs_->push_back(attr);
  if (attr_map_)
    (*attr_map_)[attr->name()] = attr;
}

LibertyGroup::~LibertyGroup()  // cdli
{
  if (params_) {
    params_->deleteContents();
    delete params_;
  }
  if (attrs_) {
    LibertyAttrSeq::Iterator iter(attrs_);
    attrs_->deleteContents();
    delete attrs_;
    delete attr_map_;
  }
  if (subgroups_) {
    subgroups_->deleteContents();
    delete subgroups_;
  }
  if (define_map_) {
    define_map_->deleteContents();
    delete define_map_;
  }
}

const char *
LibertyGroup::firstName()  // cdli
{
  if (params_ && params_->size() > 0) {
    LibertyAttrValue *value = (*params_)[0];
    if (value->isString())
      return value->stringValue();
  }
  return nullptr;
}

const char *
LibertyGroup::secondName()  // cdli
{
  if (params_ && params_->size() > 1) {
    LibertyAttrValue *value = (*params_)[1];
    if (value->isString())
      return value->stringValue();
  }
  return nullptr;
}

LibertyAttr *
LibertyGroup::findAttr(const char *name)  // cdli
{
  if (attrs_) {
    if (attr_map_ == nullptr) {
      // Build attribute name map on demand.
      LibertyAttrSeq::Iterator attr_iter(attrs_);
      while (attr_iter.hasNext()) {
	LibertyAttr *attr = attr_iter.next();
	(*attr_map_)[attr->name()] = attr;
      }
    }
    return attr_map_->findKey(name);
  }
  else
    return nullptr;
}

LibertySubgroupIterator::LibertySubgroupIterator(LibertyGroup *group) :
  LibertyGroupSeq::Iterator(group->subgroups())  // cdli
{
}

LibertyAttrIterator::LibertyAttrIterator(LibertyGroup *group) :
  LibertyAttrSeq::Iterator(group->attrs())  // cdli
{
}

////////////////////////////////////////////////////////////////

LibertyAttr::LibertyAttr(const char *name,
			 int line) :
  LibertyStmt(line),  // cdli
  name_(name)  // cdli
{
}

LibertySimpleAttr::LibertySimpleAttr(const char *name,
				     LibertyAttrValue *value,
				     int line) :
  LibertyAttr(name, line),  // cdli
  value_(value)  // cdli
{
}

LibertySimpleAttr::~LibertySimpleAttr()  // cdli
{
  delete value_;
}

LibertyAttrValueSeq *
LibertySimpleAttr::values() const  // cdli
{
  criticalError(1125, "valueIterator called for LibertySimpleAttribute");
  return nullptr;
}

LibertyComplexAttr::LibertyComplexAttr(const char *name,
				       LibertyAttrValueSeq *values,
				       int line) :
  LibertyAttr(name, line),  // cdli
  values_(values)  // cdli
{
}

LibertyComplexAttr::~LibertyComplexAttr()  // cdli
{
  if (values_) {
    values_->deleteContents();
    delete values_;
  }
}

LibertyAttrValue *
LibertyComplexAttr::firstValue()  // cdli
{
  if (values_ && values_->size() > 0)
    return (*values_)[0];
  else
    return nullptr;
}

LibertyStringAttrValue::LibertyStringAttrValue(const char *value) :
  LibertyAttrValue(),  // cdli
  value_(value)  // cdli
{
}

float
LibertyStringAttrValue::floatValue()  // cdli
{
  criticalError(1126, "LibertyStringAttrValue called for float value");
  return 0.0;
}

const char *
LibertyStringAttrValue::stringValue()  // cdli
{
  return value_.c_str();
}

LibertyFloatAttrValue::LibertyFloatAttrValue(float value) :
  value_(value)  // cdli
{
}

float
LibertyFloatAttrValue::floatValue()  // cdli
{
  return value_;
}

const char *
LibertyFloatAttrValue::stringValue()  // cdli
{
  criticalError(1127, "LibertyStringAttrValue called for float value");
  return nullptr;
}

////////////////////////////////////////////////////////////////

LibertyDefine::LibertyDefine(const char *name,
			     LibertyGroupType group_type,
			     LibertyAttrType value_type,
			     int line) :
  LibertyStmt(line),  // cdli
  name_(name),  // cdli
  group_type_(group_type),  // cdli
  value_type_(value_type)  // cdli
{
}

////////////////////////////////////////////////////////////////

LibertyVariable::LibertyVariable(const char *var,
				 float value,
				 int line) :
  LibertyStmt(line),  // cdli
  var_(var),  // cdli
  value_(value)  // cdli
{
}

////////////////////////////////////////////////////////////////

LibertyScanner::LibertyScanner(std::istream *stream,
                               const char *filename,
                               LibertyParser *reader,
                               Report *report) :
  yyFlexLexer(stream),  // cdli
  stream_(stream),  // cdli
  filename_(filename),  // cdli
  reader_(reader),  // cdli
  report_(report),  // cdli
  stream_prev_(nullptr)  // cdli
{
}

bool
LibertyScanner::includeBegin()  // cdli
{
  if (stream_prev_ != nullptr)
    error("nested include_file's are not supported");
  else {
    // include_file(filename);
    std::regex include_regexp("include_file *\\( *([^)]+) *\\) *;?");
    std::cmatch matches;
    if (std::regex_match(yytext, matches, include_regexp)) {
      string filename = matches[1].str();
      gzstream::igzstream *stream = new gzstream::igzstream(filename.c_str());
      if (stream->is_open()) {
        yypush_buffer_state(yy_create_buffer(stream, 256));

        filename_prev_ = filename_;
        stream_prev_ = stream_;

        filename_ = filename;
        reader_->setFilename(filename);
        stream_ = stream;
        return true;
      }
      else {
        report_->fileWarn(25, filename_.c_str(), yylineno,
                          "cannot open include file %s.", filename.c_str());
        delete stream;
      }
    }
    else
      error("include_file syntax error.");
  }
  return false;
}

void
LibertyScanner::fileEnd()  // cdli
{
  if (stream_prev_)
    delete stream_;
  stream_ = stream_prev_;
  filename_ = filename_prev_;
  stream_prev_ = nullptr;

  yypop_buffer_state();
}

void
LibertyScanner::error(const char *msg)  // cdli
{
  report_->fileError(1866, filename_.c_str(), lineno(), "%s", msg);
}

} // namespace
