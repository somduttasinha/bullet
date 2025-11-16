#include <ctime>
#include <fmt/core.h>
#include <fmt/format.h>
#include <string>
#include <time.h>
#include <vector>

template <typename T>
inline std::string join(const std::vector<T> &v, const std::string &separator) {
  std::string joined;
  for (const T &t : v) {
    joined += t.toString();
    joined += separator;
  }
  return joined;
}
inline std::string itomonth(int mon) {
  std::string month;
  switch (mon) {
  case 0: {
    month = "Jan";
    break;
  }
  case 1: {
    month = "Feb";
    break;
  }
  case 2: {
    month = "Mar";
    break;
  }
  case 3: {
    month = "Apr";
    break;
  }
  case 4: {
    month = "May";
    break;
  }
  case 5: {
    month = "Jun";
    break;
  }
  case 6: {
    month = "Jul";
    break;
  }
  case 7: {
    month = "Aug";
    break;
  }
  case 8: {
    month = "Sep";
    break;
  }
  case 9: {
    month = "Oct";
    break;
  }
  case 10: {
    month = "Nov";
    break;
  }
  case 11: {
    month = "Dec";
    break;
  }
  }

  return month;
}
inline std::string itoday(int wday) {
  std::string day;
  switch (wday) {
  case 0: {
    day = "Sun";
    break;
  }
  case 1: {
    day = "Mon";
    break;
  }
  case 2: {
    day = "Tue";
    break;
  }
  case 3: {
    day = "Wed";
    break;
  }
  case 4: {
    day = "Thu";
    break;
  }
  case 5: {
    day = "Fri";
    break;
  }
  case 6: {
    day = "Sat";
    break;
  }
  }

  return day;
}

inline std::string date() {
  // Sun, 06 Nov 1994 08:49:37 GMT  ;
  time_t t = time(nullptr);

  struct tm tm = *localtime(&t);

  int wday = tm.tm_wday;

  std::string day = itoday(wday);

  int mday = tm.tm_mday;

  std::string mon = itomonth(tm.tm_mon);

  int year = 1900 + tm.tm_year;
  int hour = tm.tm_hour;
  int min = tm.tm_min;
  int sec = tm.tm_sec;
  auto tz = tm.tm_zone;

  return fmt::format(fmt::runtime("{}, {:2d} {} {} {:2d}:{:2d}:{:2d} {}"), day,
                     mday, mon, year, hour, min, sec, tz);
}
