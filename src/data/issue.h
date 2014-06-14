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

#ifndef DATA_ISSUE_H_
#define DATA_ISSUE_H_

#include <QDomElement>
#include <QString>
#include <QVector>

#include "data/time_entry.h"

struct Issue {
  typedef QVector<TimeEntry> TimeEntriesType;

  int id;
  QString subject;

  int projectId;
  QString projectName;

  int priorityId;
  QString priorityName;

  int assignedToId;
  QString assignedToName;

  // Time entries.
  float hoursSpent;
  TimeEntriesType timeEntries;

  Issue()
    : id(0), projectId(0), priorityId(0), assignedToId(0), hoursSpent(0.0f) {}

  bool operator==(int issueId) const { return id == issueId; }
  bool operator<(const Issue& other) const { return id < other.id; }

  void calculateHoursSpent();
};

Q_DECLARE_METATYPE(Issue)

void updateIssueFromXml(const QDomElement& issueElement, Issue* issueOut);

#endif  // DATA_ISSUE_H_
