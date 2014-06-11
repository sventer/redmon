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

#include "data/activities_data_loader.h"

#include <QDebug>
#include <QDomElement>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSettings>

#include "data/time_entry_activity.h"

ActivitiesDataLoader::ActivitiesDataLoader(QObject* parent) : QObject(parent) {
  m_nam = new QNetworkAccessManager(this);
  connect(m_nam, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(onActivitiesManagerFinished(QNetworkReply*)));
}

ActivitiesDataLoader::~ActivitiesDataLoader() {}

void ActivitiesDataLoader::loadData() {
  qDebug() << "ActivitiesDataLoader::loadData()";

  QNetworkRequest request(buildActivityRequestUrl());
  m_nam->get(request);
}

void ActivitiesDataLoader::swapTimeEntryActivities(
    QVector<TimeEntryActivity>* timeEntryActivitiesOut) {
  Q_ASSERT(timeEntryActivitiesOut);

  timeEntryActivitiesOut->swap(m_timeEntryActivities);
}

void ActivitiesDataLoader::onActivitiesManagerFinished(QNetworkReply* reply) {
  qDebug() << "ActivitiesDataLoader::onActivitiesManagerFinished";

  QByteArray data(reply->read(reply->bytesAvailable()));

  // Create a XML document from the data received.
  QDomDocument document;
  document.setContent(data);

  QDomElement root(document.documentElement());

  // Check that we hava valid content.
  if (root.tagName() == "time_entry_activities") {
    for (QDomElement elem = root.firstChildElement("time_entry_activity");
         !elem.isNull();
         elem = elem.nextSiblingElement("time_entry_activity")) {
      TimeEntryActivity timeEntryActivity;
      updateTimeEntryActivityFromXml(elem, &timeEntryActivity);
      m_timeEntryActivities.append(timeEntryActivity);
    }
  }

  emit finished();
}

QString ActivitiesDataLoader::buildActivityRequestUrl() const {
  QSettings settings;

  // strip the protocol from the user supplied server user
  QString modUrl = settings.value("serverUrl").toString();
  if (modUrl.startsWith("http://")) {
    modUrl = settings.value("serverUrl").toString().split("//")[1];
  } else {
    modUrl = settings.value("serverUrl").toString();
  }

  QString url("http://%1/enumerations/time_entry_activities.xml?key=%2");
  url = url.arg(modUrl).arg(settings.value("apiKey").toString());

  return url;
}
