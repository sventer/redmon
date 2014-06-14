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

#ifndef DATA_DATA_LOADER_H_
#define DATA_DATA_LOADER_H_

#include <QDomElement>
#include <QMap>
#include <QNetworkAccessManager>
#include <QVector>

class QNetworkReply;
class QDialog;

struct Issue;
struct TimeEntry;
class TimeEntryLoader;
class TimeActivitiesModel;
class IssueActivityDialog;

class DataLoader : public QObject {
  Q_OBJECT

public:
  DataLoader(QObject* parent = 0);
  virtual ~DataLoader();

  // Start the process of loading the data from the server.
  void loadData();

  // Return the list of issues we have internally.
  const QVector<Issue>& issues() const { return m_issues; }

signals:
  void progress(int current, int max);
  void finished();

private slots:
  void onNetworkFinished(QNetworkReply* reply);

private:
  void startLoadingIssues(int offset = 0);
  void startLoadingTimeEntries(int issueId, int offset = 0);

  // The NAM we use to pull all the XML pages from the server.
  QNetworkAccessManager* m_network;

  // We store the issues we load from the server into this vector.
  typedef QVector<Issue> IssuesType;
  IssuesType m_issues;

  // Keep track of progress.
  int m_currentProgress;
  int m_maxProgress;
};

#endif  // DATA_DATA_LOADER_H_
