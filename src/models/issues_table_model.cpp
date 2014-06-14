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

#include "models/issues_table_model.h"

#include <QVariant>
#include <QDebug>

#include <algorithm>

#include "data/data.h"

namespace {

enum Columns {
  COLUMN_ID,
  COLUMN_PRIORITY,
  COLUMN_HOURS,
  COLUMN_PROJECT,
  COLUMN_SUBJECT,
  COLUMN_COUNT,
};

}  // namespace

IssuesTableModel::IssuesTableModel(QObject* parent)
  : QAbstractTableModel(parent) {}

int IssuesTableModel::rowCount(const QModelIndex& parent) const {
  return m_issues.size();
}

int IssuesTableModel::columnCount(const QModelIndex& parent) const {
  return COLUMN_COUNT;
}

QVariant IssuesTableModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  if (index.row() < 0 || index.row() >= m_issues.size())
    return QVariant();

  const Issue& issue = m_issues.at(index.row());

  switch (index.column()) {
  case COLUMN_ID:
    return issue.id;

  case COLUMN_PRIORITY:
    return issue.priorityName;

  case COLUMN_HOURS:
    return issue.hoursSpent;

  case COLUMN_PROJECT:
    return issue.projectName;

  case COLUMN_SUBJECT:
    return issue.subject;

  default:
    Q_UNREACHABLE();
  }

  return QVariant();
}

QVariant IssuesTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
    case COLUMN_ID:
      return QString("Id");
    case COLUMN_PRIORITY:
      return QString("Priority");
    case COLUMN_HOURS:
      return QString("Hours Spent");
    case COLUMN_PROJECT:
      return QString("Project");
    case COLUMN_SUBJECT:
      return QString("Subject");
    default:
      Q_UNREACHABLE();
    }
  }

  return QVariant();
}

const Issue& IssuesTableModel::issue(const QModelIndex& index) const {
  return m_issues.at(index.row());
}

bool IssuesTableModel::insertIssue(const Issue& issue) {
  // Find the location where the new issue should go.
  auto insertLoc = std::lower_bound(m_issues.begin(), m_issues.end(), issue);

  // Find the index where the new item is going to be inserted or updated.
  auto distance = std::distance(m_issues.begin(), insertLoc);

  // If the first item we found is the same as the issue, we just update the
  // issue.
  if (insertLoc->id == issue.id) {
    beginResetModel();
    *insertLoc = issue;
    endResetModel();
  } else {
    beginInsertRows(QModelIndex(), distance, distance);
    m_issues.insert(insertLoc, issue);
    endInsertRows();
  }

  return true;
}

void IssuesTableModel::insertIssues(const QVector<Issue>& issues) {
  // The list of issues MUST be sorted here, but Qt doesn't have a nice
  // algorithm to check that.
  Q_ASSERT(std::is_sorted(issues.begin(), issues.end()));

  // For now we just go through all the issues and insert/update them as we go.
  // This needs optimization.
  for (const auto& issue : issues)
    insertIssue(issue);
}

bool IssuesTableModel::containsIssue(int issueId) {
  for (QVector<Issue>::iterator bi = m_issues.begin(), ei = m_issues.end();
       bi != ei; ++bi) {
    Issue issue = *bi;
    if (issue.id == issueId)
      return true;
  }

  return false;
}
