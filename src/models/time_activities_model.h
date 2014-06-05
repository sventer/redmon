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

#ifndef TIME_ACTIVITIES_MODEL
#define TIME_ACTIVITIES_MODEL

// this is a simple data model that is used to store the activities one can 
// use to book time against. This model is only updated once for the lifetime
// of the application.

#include <QDomElement>
#include <QMap>
#include <QString>
#include <qglobal.h>

class TimeActivitiesModel {
public:
  TimeActivitiesModel();
  ~TimeActivitiesModel();

  void updateFromXml(const QDomElement& issueElement);

  void insertActivity(int id, QString activity);
  QString findActivity(int index) const;
  int activityCount() const;

private:
  QMap<int, QString> m_timeActivities;

  Q_DISABLE_COPY(TimeActivitiesModel)
};

#endif  // TIME_ACTIVITIES_MODEL
