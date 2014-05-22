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
#include <QNetworkAccessManager>
#include <QVector>

class QNetworkReply;

struct Issue;

void parseIssues(const QDomElement& root, QVector<Issue>* issues);

class DataLoader : public QObject {
  Q_OBJECT

public:
  DataLoader(QObject* parent = 0);
  virtual ~DataLoader();

  // Start the process of loading the data from the server.
  void loadData();

  // Swap the issues into the specified list.
  void swapIssues(QVector<Issue>* issues);

signals:
  void issuesLoaded();

private slots:
  void onIssuesManagerReply(QNetworkReply* reply);

private:
  void startLoadDataForPage(int pageNum);
  static QString buildIssuesUrl(int pageNum = 1, int assignedToId = 1);

  QNetworkAccessManager* m_issuesManager;
  QVector<Issue> m_issues;
  int m_lastPageLoaded;
};

#endif  // DATA_DATA_LOADER_H_
