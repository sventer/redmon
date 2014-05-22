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

#include "data/data_loader.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>

#include "data/issue.h"
#include "data/time_entry.h"
#include "data/time_entry_loader.h"
#include "data/utils.h"

DataLoader::DataLoader(QObject* parent) : QObject(parent) {
  m_issuesManager = new QNetworkAccessManager(this);
  connect(m_issuesManager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(onIssuesManagerReply(QNetworkReply*)));
}

DataLoader::~DataLoader() {}

void DataLoader::loadData() {
  qDebug() << "DataLoader::loadData()";

  // Clear the issues we have in the list already.
  m_issues.clear();

  // Start the load process.
  startLoadIssues();
}

void DataLoader::swapIssues(QVector<Issue>* issues) { issues->swap(m_issues); }

void DataLoader::onIssuesManagerReply(QNetworkReply* reply) {
  qDebug() << "DataLoader::onIssuesManagerReply";

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

  if (root.tagName() == "issues") {
    // We put a check in here to see if nothing was added to the issues list,
    // just so we never go into a neverending loop.
    size_t issuesBefore = m_issues.size();

    // Add the new issues to the current list of issues.
    for (QDomElement issueElem = root.firstChildElement("issue");
         !issueElem.isNull();
         issueElem = issueElem.nextSiblingElement("issue")) {
      // Create and update the issue.
      Issue issue;
      updateIssueFromXml(issueElem, &issue);

      // Create a loader for the time entries.
      TimeEntryLoader* loader = new TimeEntryLoader(issue, this);
      loader->connect(loader, SIGNAL(finished(TimeEntryLoader*)), this,
                      SLOT(onTimeEntryLoaderFinished(TimeEntryLoader*)));
      Q_ASSERT(!m_timeEntryLoaders.contains(issue.id));
      m_timeEntryLoaders[issue.id] = loader;
      loader->load();
    }

    // If there were issues added and we haven't reached the limit yet, request
    // another batch of issues.
    if (m_issues.size() != issuesBefore && totalCount >= 0 &&
        m_issues.size() < totalCount) {
      startLoadIssues(offset + limit);
    }
  }
}

void DataLoader::onTimeEntryLoaderFinished(TimeEntryLoader* loader) {
  Q_ASSERT(loader);

  // Find the loader in the list.
  TimeEntryLoadersType::iterator it =
      m_timeEntryLoaders.find(loader->issue().id);
  Q_ASSERT(it != m_timeEntryLoaders.end());

  // Add the issue to the issues list.
  m_issues.append(loader->issue());

  // Remove the loader from the list.
  m_timeEntryLoaders.erase(it);

  // delete loader;

  // If the m_timeEntryLoader list is empty, it means all the issues were
  // updated, so we can finish the process.
  if (m_timeEntryLoaders.empty()) {
    emit finished();
  }
}

void DataLoader::startLoadIssues(int offset) {
  QString url(buildIssuesUrl(offset));
  QNetworkRequest request(url);

  qDebug() << "Requesting:" << url;

  m_issuesManager->get(request);
}

// static
QString DataLoader::buildIssuesUrl(int offset, int assignedToId) {
  QSettings settings;

  QString url(
      "http://%1/issues.xml?"
      "key=%2&limit=100&offset=%3&sort=priority:desc,id:desc");
  url = url.arg(settings.value("serverUrl").toString())
            .arg(settings.value("apiKey").toString())
            .arg(offset);

  if (assignedToId == 1) {
    url.append("&assigned_to_id=me");
  } else if (assignedToId > 0) {
    url.append("&assigned_to_id=").append(assignedToId);
  }

  return url;
}
