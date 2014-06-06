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

ActivitiesDataLoader::ActivitiesDataLoader(QObject* parent)
  : QObject(parent) {
  m_activitiesManager = new QNetworkAccessManager(this);
  connect(m_activitiesManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onActivitiesManagerReply(QNetworkReply*)));
}

ActivitiesDataLoader::~ActivitiesDataLoader() {
}
  
void ActivitiesDataLoader::loadData() {
  qDebug() << "ActivitiesDataLoader::loadData()";
  
  QNetworkRequest request(buildActivityRequestUrl());
  m_activitiesManager->get(request);
}

void ActivitiesDataLoader::onActivitiesManagerReply(QNetworkReply* reply) {
  qDebug() << "ActivitiesDataLoader::onActivitiesManagerReply";
  
  QByteArray data(reply->read(reply->bytesAvailable()));
  
  // create a XML document from the data received
  QDomDocument document;
  document.setContent(data);
  
  QDomElement root(document.documentElement());
  
  // check that we hava valid content
  if (root.tagName() == "time_entry_activities") {
    for (QDomElement elem = root.firstChildElement("time_entry_activity"); !elem.isNull(); elem = elem.nextSiblingElement("time_entry_activity")) {
      int key;
      QString value;
      for (QDomElement childElem = elem.firstChildElement(); !childElem.isNull(); childElem = childElem.nextSiblingElement()) {
        if (childElem.tagName() == "id")
          key = childElem.text().toInt();
        if (childElem.tagName() == "name")
          value = childElem.text();
      }
      
      // only insert this entry if value is not empty
      if (!value.isEmpty())
        m_activities.insert(key, value);
        qDebug() << "Activity key: [" << QString::number(key) << "] value: [" << value << "]";
    }
  }
  
  qDebug() << "Number of activities received [" << QString::number(m_activities.size()) << "]";
  emit activitiesLoaded(&m_activities);
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
  
  qDebug() << "ActivitiesDataLoader::buildActivityRequestUrl -- [" << url << "]";
  
  return url;
}
