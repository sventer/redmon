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

#ifndef ISSUES_TABLE_VIEW
#define ISSUES_TABLE_VIEW

#include <QTableView>
#include <QItemSelectionModel>

class IssuesTableSelectionModel : public QItemSelectionModel {
  Q_OBJECT

public:
  IssuesTableSelectionModel(QAbstractItemModel* model);
};

class IssuesTableView : public QTableView {
  Q_OBJECT

public:
  IssuesTableView(QWidget* parent = 0);

public slots:
  virtual void currentChanged(const QModelIndex& current, const QModelIndex& previous) override;
  void setInitialSelection();

private:
  void storeCurrentSelection();

  // recode the current selected item.
  int m_selectedRow;
  int m_selectedIssueId;
};

#endif  // ISSUES_TABLE_VIEW
