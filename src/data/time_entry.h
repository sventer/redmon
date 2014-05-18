
#ifndef DATA_TIME_ENTRY_H_
#define DATA_TIME_ENTRY_H_

#include <QString>
#include <QDate>

struct TimeEntry {
  TimeEntry() : id(0), hours(0.0f) {}
  ~TimeEntry() {}

  int id;
  float hours;
  QString comments;
  QDate spentOn;
  QDateTime createdOn;
  QDateTime updatedOn;
};

#endif  // DATA_TIME_ENTRY_H_
