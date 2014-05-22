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

// The number of items to load per page.  Keep this small enough to make the
// loading process responsive, but big enough to not overload the network.
const int kIssuesPerPage = 25;

DataLoader::DataLoader(QObject* parent) : QObject(parent), m_lastPageLoaded(0) {
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
  startLoadDataForPage(1);
}

void DataLoader::swapIssues(QVector<Issue>* issues) { issues->swap(m_issues); }

void DataLoader::onIssuesManagerReply(QNetworkReply* reply) {
  qDebug() << "DataLoader::onIssuesManagerReply";

  QByteArray data(reply->read(reply->bytesAvailable()));

  // Create the XML document.
  QDomDocument document;
  document.setContent(data);

  // Get the root element.
  QDomElement root(document.firstChildElement("issues"));

  // Parse the XML into a temp vector.
  size_t issuesBefore = m_issues.size();
  parseIssues(root, &m_issues);

  bool doNextPage = (m_issues.size() - issuesBefore) == kIssuesPerPage;

  if (doNextPage) {
    startLoadDataForPage(m_lastPageLoaded + 1);
  } else {
    m_lastPageLoaded = 0;

    emit issuesLoaded();
  }
}

void DataLoader::startLoadDataForPage(int pageNum) {
  m_lastPageLoaded = pageNum;

  QString url(buildIssuesUrl(pageNum));
  QNetworkRequest request(url);

  qDebug() << "Requesting:" << url;

  m_issuesManager->get(request);
}

// static
QString DataLoader::buildIssuesUrl(int pageNum, int assignedToId) {
  QSettings settings;

  QString url(
      "http://%1/issues.xml?"
      "key=%2&limit=%3&page=%4&sort=priority:desc,id:desc");
  url = url.arg(settings.value("serverUrl").toString())
            .arg(settings.value("apiKey").toString())
            .arg(kIssuesPerPage)
            .arg(pageNum);

  if (assignedToId == 1) {
    url.append("&assigned_to_id=me");
  } else if (assignedToId > 0) {
    url.append("&assigned_to_id=").append(assignedToId);
  }

  return url;
}

void DataLoader::parseIssues(const QDomElement& root, QVector<Issue>* issues) {
  Q_ASSERT(issues);

  for (QDomElement issueElem = root.firstChildElement("issue");
       !issueElem.isNull(); issueElem = issueElem.nextSiblingElement("issue")) {
    Issue issue;

    for (QDomElement elem(issueElem.firstChildElement()); !elem.isNull();
         elem = elem.nextSiblingElement()) {
      if (elem.tagName() == "id") {
        issue.id = elem.text().toInt();
        continue;
      }

      if (elem.tagName() == "subject") {
        issue.subject = elem.text();
        continue;
      }

      if (elem.tagName() == "project") {
        QDomAttr idAttr = elem.attributeNode("id");
        if (!idAttr.isNull())
          issue.projectId = idAttr.value().toInt();

        QDomAttr nameAttr = elem.attributeNode("name");
        if (!nameAttr.isNull())
          issue.projectName = nameAttr.value();
      }

      if (elem.tagName() == "priority") {
        QDomAttr idAttr = elem.attributeNode("id");
        if (!idAttr.isNull())
          issue.priorityId = idAttr.value().toInt();

        QDomAttr nameAttr = elem.attributeNode("name");
        if (!nameAttr.isNull())
          issue.priorityName = nameAttr.value();
      }

      if (elem.tagName() == "assigned_to") {
        QDomAttr idAttr = elem.attributeNode("id");
        if (!idAttr.isNull())
          issue.assignedToId = idAttr.value().toInt();

        QDomAttr nameAttr = elem.attributeNode("name");
        if (!nameAttr.isNull())
          issue.assignedToName = nameAttr.value();
      }
    }

    issues->append(issue);
  }
}
