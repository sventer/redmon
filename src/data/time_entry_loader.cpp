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

TimeEntryLoader::TimeEntryLoader(const Issue& issue, QObject* parent)
  : QObject(parent), m_issue(issue) {
  m_network = new QNetworkAccessManager(this);
  connect(m_network, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(onNetworkReply(QNetworkReply*)));
}

TimeEntryLoader::~TimeEntryLoader() {}

void TimeEntryLoader::load() { startLoadingTimeEntries(); }

void TimeEntryLoader::onNetworkReply(QNetworkReply* reply) {
  qDebug() << "TimeEntryLoader::onNetworkReply |" << m_issue.projectId;

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
    m_issue.timeEntries.append(timeEntry);
  }

  // If there are more time entries, load them as well.
  if (m_issue.timeEntries.size() < totalCount) {
    startLoadingTimeEntries(offset + limit);
  } else {
    // We're done, so calculate the total time spent and tell out consumer we're
    // done.
    m_issue.calculateHoursSpent();
    emit finished(this);
  }
}

QString TimeEntryLoader::buildTimeEntriesUrl(int offset) {
  QSettings settings;

  QString url(
      "http://%1/time_entries.xml?key=%2&limit=100&offset=%3&issue_id=%4");
  url = url.arg(settings.value("serverUrl").toString())
            .arg(settings.value("apiKey").toString())
            .arg(offset)
            .arg(m_issue.id);

  if (settings.value("onlyMyTimeEntries", false).toBool()) {
    // Only add time entries that i made.
    url.append("&user_id=me");
  }

  return url;
}

void TimeEntryLoader::startLoadingTimeEntries(int offset) {
  QNetworkRequest request(buildTimeEntriesUrl(offset));
  qDebug() << "Requesting:" << request.url();
  m_network->get(request);
}
