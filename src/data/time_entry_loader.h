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

#ifndef DATA_TIME_ENTRY_LOADER_H_
#define DATA_TIME_ENTRY_LOADER_H_

#include <QObject>

#include "data/issue.h"

class QNetworkAccessManager;
class QNetworkReply;

class TimeEntryLoader : public QObject {
  Q_OBJECT

public:
  TimeEntryLoader(int issueId, QObject* parent = 0);
  virtual ~TimeEntryLoader();

  // Start loading the time entries for our owned issue.
  void load();

signals:
  void finished(int issueId, float hours);

private slots:
  void onNetworkReply(QNetworkReply* reply);

private:
  QString buildTimeEntriesUrl(int offset, bool onlyMyTimeEntries);
  void startLoadingTimeEntries(int offset = 0);

  // If ID of the issue we are updating the hours of.
  int m_issueId;

  // The total hours we've calculated so far.
  float m_hours;

  // The network access manager used to request the data for the time entries.
  QNetworkAccessManager* m_nam;
};

#endif  // DATA_TIME_ENTRY_LOADER_H_
