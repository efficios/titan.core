#ifndef LTTngUSTLogger_HH
#define LTTngUSTLogger_HH

#include <stdio.h>
#include "ILoggerPlugin.hh"

namespace TitanLoggerApi { class TitanLogEvent; }

class LTTngUSTLogger: public ILoggerPlugin
{
public:
  LTTngUSTLogger();
  virtual ~LTTngUSTLogger();
  void log(const TitanLoggerApi::TitanLogEvent& event, bool log_buffered,
           bool separate_file, bool use_emergency_mask);
  inline bool is_static() { return false; }
  inline void init(const char *options) { }
  inline void fini() { }

private:
  CHARSTRING joinStrings(const TitanLoggerApi::Strings& strings);
};

#endif	// LTTngUSTLogger_HH
