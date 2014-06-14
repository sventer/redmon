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

#include <cmath>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>

#include "data/issue.h"
#include "data/time_entry.h"
#include "data/time_entry_loader.h"
#include "data/utils.h"
#include "models/time_activities_model.h"

namespace {

// The maximum number of items we pull per page.  The maximum in Redmine is 100.
const int kPageLimit = 100;

}  // namespace

DataLoader::DataLoader(QObject* parent)
  : QObject(parent), m_network(new QNetworkAccessManager(this)) {
  connect(m_network, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(onNetworkFinished(QNetworkReply*)));
}

DataLoader::~DataLoader() {}

void DataLoader::loadData() {
  // Clear the issues we have in the list already.
  m_issues.clear();

  // Start the load process.
  startLoadingIssues();
}

void DataLoader::onNetworkFinished(QNetworkReply* reply) {
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
    // If we receive the first issues page back, we can calculate the total
    // number of pages we're going to load.
    if (offset == 0) {
      // We did the first piece of work.
      m_currentProgress = 1;

      // Set the max progress to the total number of issues pages we must load.
      m_maxProgress = (totalCount - (totalCount % limit)) / limit +
                      ((totalCount % limit) ? 1 : 0);

      // Add the total number of time entry pages we must load.  Assuming for
      // now that we will only load one per issue.
      m_maxProgress += totalCount;

      // We know how many issues we're going to add, so let's reserver some
      // space in the vector.
      m_issues.reserve(totalCount);
    } else {
      // We did another piece of work.
      ++m_currentProgress;
    }

    // Add the new issues to the current list of issues.
    for (QDomElement issueElem = root.firstChildElement("issue");
         !issueElem.isNull();
         issueElem = issueElem.nextSiblingElement("issue")) {
      // Create and update the issue.
      Issue issue;
      updateIssueFromXml(issueElem, &issue);

      // Add the issue to our internal list of issues in a sorted manner.
      m_issues.insert(qLowerBound(m_issues.begin(), m_issues.end(), issue),
                      issue);

      // Start loading time entries for this issue.
      startLoadingTimeEntries(issue.id, 0);
    }

    // We're updated the max progress, so let everyone know.
    emit progress(m_currentProgress, m_maxProgress);

    // If there were issues added and we haven't reached the limit yet, request
    // another batch of issues.
    if (offset + limit < totalCount)
      startLoadingIssues(offset + limit);
  } else if (root.tagName() == "time_entries") {
    // If this issue's time entries span more than one page, we have to update
    // the max progress.
    if (offset == 0 && totalCount > limit) {
      // We have to subtract one from the total pages, because this one is
      // already done.
      int moreWorkToDo = (totalCount - (totalCount % limit)) / limit +
                         ((totalCount % limit) ? 1 : 0) - 1;
      m_maxProgress += moreWorkToDo;
    }

    // Keep track of the issue id we're counting hours for.
    int issueId = -1;

    for (QDomElement timeEntryElem = root.firstChildElement("time_entry");
         !timeEntryElem.isNull();
         timeEntryElem = timeEntryElem.nextSiblingElement("time_entry")) {
      TimeEntry timeEntry;
      updateTimeEntryFromXml(timeEntryElem, &timeEntry);

      // Set the issueId, or make sure it's the same one.
      if (issueId == -1) {
        issueId = timeEntry.issueId;
      } else {
        Q_ASSERT(issueId == timeEntry.issueId);
      }

      // Find the issue we are updating and update it's hours.
      auto it = qFind(m_issues.begin(), m_issues.end(), timeEntry.issueId);
      if (it != m_issues.end()) {
        it->hoursSpent += timeEntry.hours;
      } else {
        Q_UNREACHABLE();
      }
    }

    // We processed a page, so update the progress.
    ++m_currentProgress;
    emit progress(m_currentProgress, m_maxProgress);

    // If there are more time entries to load, load them now.
    if (offset + limit < totalCount)
      startLoadingTimeEntries(issueId, offset + limit);
  }

  // If the progress reached the end, then we're done.
  if (m_currentProgress == m_maxProgress)
    emit finished();
}

void DataLoader::startLoadingIssues(int offset) {
  QSettings settings;

  QString url("%1?&limit=%2&offset=%3&sort=priority");
  url = url.arg(buildServerUrl("/issues.xml")).arg(kPageLimit).arg(offset);

  // Only show our own issues.
  if (settings.value("onlyMyIssues", true).toBool())
    url.append("&assigned_to_id=me");

  QNetworkRequest request(url);
  request.setRawHeader("X-Redmine-API-Key",
                       settings.value("apiKey").toString().toLatin1());

  qDebug() << "Requesting:" << request.url();

  m_network->get(request);
}

void DataLoader::startLoadingTimeEntries(int issueId, int offset) {
  QSettings settings;

  QString url("%1?limit=%2&offset=%3&issue_id=%4");
  url = url.arg(buildServerUrl("/time_entries.xml"))
            .arg(kPageLimit)
            .arg(offset)
            .arg(issueId);

  // Only add time entries that i made.
  if (settings.value("onlyMyTimeEntries", false).toBool())
    url.append("&user_id=me");

  QNetworkRequest request(url);
  request.setRawHeader("X-Redmine-API-Key",
                       settings.value("apiKey").toString().toLatin1());

  qDebug() << "Requesting:" << request.url();

  m_network->get(request);
}
