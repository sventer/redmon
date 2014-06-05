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

#include "models/time_activities_model.h"

#include <QDebug>

TimeActivitiesModel::TimeActivitiesModel() {
}

TimeActivitiesModel::~TimeActivitiesModel() {
}

void TimeActivitiesModel::updateFromXml(const QDomElement& issueElement) {
  int key;
  QString value;

  for (QDomElement elem(issueElement.firstChildElement()); !elem.isNull(); elem = elem.nextSiblingElement()) {
    if (elem.tagName() == "id") {
      key = elem.text().toInt();
    }

    if (elem.tagName() == "name") {
      value = elem.text();
    }
  }

  m_timeActivities.insert(key, value);
}

void TimeActivitiesModel::insertActivity(int id, QString activity) {

}

QString TimeActivitiesModel::findActivity(int index) const {
  return QString();
}

int TimeActivitiesModel::activityCount() const {
  return m_timeActivities.size();
}
