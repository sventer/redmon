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

#include "views/issues_table_view.h"

#include <QDebug>
#include <QDesktopServices>
#include <QHeaderView>
#include <QSettings>
#include <QUrl>

#include "models/issues_table_model.h"
#include "config.h"
#include "data/utils.h"

IssuesTableView::IssuesTableView(QWidget* parent)
  : QTableView(parent), m_selectedRow(-1), m_selectedIssueId(0) {
  setSelectionMode(SingleSelection);
  setSelectionBehavior(SelectRows);
  horizontalHeader()->setStretchLastSection(true);
  verticalHeader()->hide();

  connect(horizontalHeader(), SIGNAL(sectionResized(int, int, int)), this,
          SLOT(onHorizontalHeaderSectionResized(int, int, int)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex&)), this,
          SLOT(onDoubleClicked(const QModelIndex&)));
}

void IssuesTableView::setModel(QAbstractItemModel* model) {
  QTableView::setModel(model);

  QSettings settings;
  QHeaderView* header = horizontalHeader();

  for (int column = 0; column <= 4; ++column) {
    header->resizeSection(
        column, settings.value(QString("col_%1").arg(column)).toInt());
  }
}

void IssuesTableView::currentChanged(const QModelIndex& current,
                                     const QModelIndex& previous) {
  Q_UNUSED(previous);

#if 0
  selectRow(current.row());

  storeCurrentSelection();
  m_selectedRow = current.row();
#endif  // 0
}

void IssuesTableView::setInitialSelection() {
  // no selection has been made yet
  if (m_selectedRow == -1) {
    currentChanged(model()->index(0, 0), QModelIndex());
    return;
  }

  // we should have a previously selected row
  currentChanged(model()->index(m_selectedRow, 0), QModelIndex());
}

void IssuesTableView::onHorizontalHeaderSectionResized(int logicalIndex,
                                                       int oldSize,
                                                       int newSize) {
  QSettings settings;

  settings.setValue(QString("col_%1").arg(logicalIndex), newSize);
}

void IssuesTableView::onDoubleClicked(const QModelIndex& index) {
  QModelIndex idx = model()->index(index.row(), 0);
  QString path(QString("issues/%1").arg(model()->data(idx, 0).toString()));
  QDesktopServices::openUrl(buildServerUrl(path));
}

// we store the issue id so that, when the tabel is recreated we can highlight the
// correct issue again
// the selected row's position within the table may change if new issues were added
// or if issues were removed
void IssuesTableView::storeCurrentSelection() {
  QModelIndex currentIndex(QTableView::currentIndex());
  IssuesTableModel* tableModel = static_cast<IssuesTableModel*>(model());
  Issue issue = tableModel->issue(currentIndex);
  m_selectedIssueId = issue.id;
}
