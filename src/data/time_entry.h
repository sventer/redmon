// RedMon
// Copyright (c) 2014 Tiaan Louw
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef DATA_TIME_ENTRY_H_
#define DATA_TIME_ENTRY_H_

#include <QString>
#include <QDate>
#include <QDomElement>

struct TimeEntry {
  TimeEntry()
    : id(0), projectId(0), issueId(0), userId(0), activityId(0), hours(0.0f) {}
  ~TimeEntry() {}

  int id;
  int projectId;
  QString projectName;
  int issueId;
  QString issueName;
  int userId;
  QString userName;
  int activityId;
  QString activityName;
  float hours;
  QString comments;
  QDate spentOn;
  QDateTime createdOn;
  QDateTime updatedOn;
};

void updateTimeEntryFromXml(const QDomElement& timeEntryElement,
                            TimeEntry* timeEntryOut);

#endif  // DATA_TIME_ENTRY_H_
