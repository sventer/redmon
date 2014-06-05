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

#include "models/issues_table_model.h"



IssuesTableView::IssuesTableView(QWidget* parent)
: QTableView(parent), m_selectedRow(-1), m_selectedIssueId(0) {
  setSelectionMode(SelectionMode::SingleSelection);
  setSelectionBehavior(SelectionBehavior::SelectRows);
}

void IssuesTableView::currentChanged(const QModelIndex& current, const QModelIndex& previous) {
  Q_UNUSED(previous);
  
  selectRow(current.row());

  storeCurrentSelection();
  m_selectedRow = current.row();
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

// we store the issue id so that when the tabelis recreated we can highlight the correct issue again
// the row indication may cahnage depending on if additional issues were added or deleted.
void IssuesTableView::storeCurrentSelection() {
  QModelIndex currentIndex(currentIndex());
  IssuesTableModel* tableModel = static_cast<IssuesTableModel*>(model());
  Issue issue = tableModel->issue(currentIndex);
  m_selectedIssueId = issue.id;
}
