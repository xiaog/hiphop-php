/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef __HPHP_DATEINTERVAL_H__
#define __HPHP_DATEINTERVAL_H__

#include <runtime/base/types.h>
#include <runtime/base/complex_types.h>
#include <runtime/base/resource_data.h>
#include <runtime/base/util/smart_object.h>

extern "C" {
#include <timelib.h>
}

namespace HPHP {

///////////////////////////////////////////////////////////////////////////////
typedef boost::shared_ptr<timelib_rel_time> DateIntervalPtr;

/**
 * Handles all date interal related functions.
 */
class DateInterval : public SweepableResourceData {
public:
  DECLARE_OBJECT_ALLOCATION(DateInterval);
  static StaticString s_class_name;
  CStrRef o_getClassNameHook() const { return s_class_name; }

  DateInterval();
  DateInterval(CStrRef date_interval, bool date_string = false);
  DateInterval(timelib_rel_time *di);

  int64 getYears()      const    { return m_di->y;                      }
  int64 getMonths()     const    { return m_di->m;                      }
  int64 getDays()       const    { return m_di->d;                      }
  int64 getHours()      const    { return m_di->h;                      }
  int64 getMinutes()    const    { return m_di->i;                      }
  int64 getSeconds()    const    { return m_di->s;                      }
  bool  isInverted()    const    { return m_di->invert;                 }
  bool  haveTotalDays() const    { return m_di->days != -99999;         }
  int64 getTotalDays()  const    { return m_di->days;                   }

  void setYears(int64 value)     { if (isValid()) m_di->y      = value; }
  void setMonths(int64 value)    { if (isValid()) m_di->m      = value; }
  void setDays(int64 value)      { if (isValid()) m_di->d      = value; }
  void setHours(int64 value)     { if (isValid()) m_di->h      = value; }
  void setMinutes(int64 value)   { if (isValid()) m_di->i      = value; }
  void setSeconds(int64 value)   { if (isValid()) m_di->s      = value; }
  void setInverted(bool value)   { if (isValid()) m_di->invert = value; }
  void setTotalDays(int64 value) { if (isValid()) m_di->days   = value; }

  bool setDateString(CStrRef date_string);
  bool setInterval(CStrRef date_interval);
  String format(CStrRef format_spec);

  bool isValid() const { return get(); }
  SmartObject<DateInterval> cloneDateInterval() const;

protected:
  friend class DateTime;

  timelib_rel_time *get() const { return m_di.get(); }

private:
  struct dateinterval_deleter {
    void operator()(timelib_rel_time *di) {
      if (di) {
        timelib_rel_time_dtor(di);
      }
    }
  };

  DateIntervalPtr m_di;
};

///////////////////////////////////////////////////////////////////////////////
}

#endif // __HPHP_DATEINTERVAL_H__
