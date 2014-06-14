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

#include "data/time_entry_loader.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>

#include "data/utils.h"

TimeEntryLoader::TimeEntryLoader(int issueId, QObject* parent)
  : QObject(parent), m_issueId(issueId),
    m_nam(new QNetworkAccessManager(this)) {
  connect(m_nam, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(onNetworkReply(QNetworkReply*)));
}

TimeEntryLoader::~TimeEntryLoader() {}

void TimeEntryLoader::load() {
  // Reset the hours.
  m_hours = 0.0f;
  startLoadingTimeEntries();
}

void TimeEntryLoader::onNetworkReply(QNetworkReply* reply) {
  QByteArray data(reply->read(reply->bytesAvailable()));

  // Create the XML document.
  QDomDocument document;
  document.setContent(data);

  // Check the root element.
  QDomElement root(document.firstChildElement());

  int totalCount = -1;
  int offset = -1;
  int limit = -1;
  loadCountersFromElement(&root, &totalCount, &offset, &limit);

  // Load all the time entries.
  for (QDomElement elem = root.firstChildElement("time_entry"); !elem.isNull();
       elem = elem.nextSiblingElement("time_entry")) {
    TimeEntry timeEntry;
    updateTimeEntryFromXml(elem, &timeEntry);
    m_hours += timeEntry.hours;
  }

  // If there are more time entries, load them as well.
  if (offset + limit < totalCount) {
    startLoadingTimeEntries(offset + limit);
  } else {
    emit finished(m_issueId, m_hours);
  }
}

QString TimeEntryLoader::buildTimeEntriesUrl(int offset, bool onlyMyTimeEntries) {
  QSettings settings;

  QString url("%1?limit=100&offset=%2&issue_id=%3");
  url = url.arg(buildServerUrl("/time_entries.xml")).arg(offset).arg(m_issueId);

  if (onlyMyTimeEntries) {
    // Only add time entries that i made.
    url.append("&user_id=me");
  }

  return url;
}

void TimeEntryLoader::startLoadingTimeEntries(int offset) {
  QSettings settings;

  QNetworkRequest request(buildTimeEntriesUrl(
      offset, settings.value("onlyMyTimeEntries", false).toBool()));
  request.setRawHeader("X-Redmine-API-Key",
                       settings.value("apiKey").toString().toLatin1());

  qDebug() << "Requesting:" << request.url();

  m_nam->get(request);
}
