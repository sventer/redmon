#include "models/issues_model.h"

#include <QVariant>

IssuesModel::IssuesModel(QObject* parent) : QAbstractListModel(parent) {}

IssuesModel::IssuesModel(const ListType& issues, QObject* parent)
  : QAbstractListModel(parent), m_issues(issues) {}

int IssuesModel::rowCount(const QModelIndex& parent) const {
  if (parent.isValid())
    return 0;

  return m_issues.count();
}

QModelIndex IssuesModel::sibling(int row, int column,
                                 const QModelIndex& idx) const {
  if (!idx.isValid() || column != 0 || row >= m_issues.count())
    return QModelIndex();

  return createIndex(row, 0);
}

int IssuesModel::columnCount(const QModelIndex& parent) const { return 2; }

QVariant IssuesModel::data(const QModelIndex& index, int role) const {
  if (index.row() < 0 || index.row() >= m_issues.size())
    return QVariant();

  if (role == Qt::DisplayRole || role == Qt::EditRole) {
    const Issue& issue = m_issues.at(index.row());

    if (index.column() == 0)
      return issue.subject();
    else if (index.column() == 1)
      return issue.assignedTo();
  }

  return QVariant();
}

const IssuesModel::ListType& IssuesModel::issues() const { return m_issues; }

void IssuesModel::setIssues(const ListType& issues) {
  emit beginResetModel();
  m_issues = issues;
  emit endResetModel();
}
