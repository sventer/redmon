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

#ifndef DATA_ACTIVITIES_DATA_LOADER_H_
#define DATA_ACTIVITIES_DATA_LOADER_H_

#include <QObject>
#include <QVector>

class QNetworkAccessManager;
class QNetworkReply;

#include "data/time_entry_activity.h"

class ActivitiesDataLoader : public QObject {
  Q_OBJECT

public:
  explicit ActivitiesDataLoader(QObject* parent = 0);
  virtual ~ActivitiesDataLoader();

  // Start the process of loading the data from the server.
  void loadData();

  // Swap the time entry activities into the specified list.
  void swapTimeEntryActivities(
      QVector<TimeEntryActivity>* timeEntryActivitiesOut);

signals:
  void finished();

private slots:
  void onActivitiesManagerFinished(QNetworkReply* reply);

private:
  QString buildActivityRequestUrl() const;

  // The NAM we use to pull the data from the server.
  QNetworkAccessManager* m_nam;

  // Store the list of time entry activities we're loading internally.
  QVector<TimeEntryActivity> m_timeEntryActivities;
};

#endif  // DATA_ACTIVITIES_DATA_LOADER_H_
