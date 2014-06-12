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

#ifndef ISSUES_TABLE_MODEL
#define ISSUES_TABLE_MODEL

#include <QAbstractItemModel>
#include <QVector>

#include "data/issue.h"

class IssuesTableModel : public QAbstractTableModel {
  Q_OBJECT

public:
	explicit IssuesTableModel(QObject* parent = 0);

	virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	const Issue& issue(const QModelIndex& index) const;

  // instead of using one of the insertRow(s) functions we provide our own insertion function
  bool insertIssue(const Issue& issue);
  void sortData();

  // get the issue at location (row)
  bool getIssue(int row, Issue* issue);

private:
  bool containsIssue(int issueId);

  QVector<Issue> _issues;

  Q_DISABLE_COPY(IssuesTableModel)
};

#endif  // ISSUES_TABLE_MODEL
