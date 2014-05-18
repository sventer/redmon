#include "models/issues_model.h"

#include <QVariant>

#include "data/data.h"

IssuesModel::IssuesModel(QObject* parent) : QAbstractListModel(parent) {}

int IssuesModel::rowCount(const QModelIndex& parent) const {
  if (parent.isValid())
    return 0;

  return Data::Get().issues.size();
}

QModelIndex IssuesModel::sibling(int row, int column,
                                 const QModelIndex& idx) const {
  if (!idx.isValid() || column != 0 || row >= Data::Get().issues.size())
    return QModelIndex();

  return createIndex(row, column);
}

int IssuesModel::columnCount(const QModelIndex& parent) const { return 1; }

QVariant IssuesModel::data(const QModelIndex& index, int role) const {
  if (index.row() < 0 || index.row() >= Data::Get().issues.size())
    return QVariant();

  if (role == Qt::DisplayRole) {
    const Issue& issue = Data::Get().issues.at(index.row());

    switch (index.column()) {
    case 0:
      return issue.subject;
    }
  }

  return QVariant();
}

const Issue& IssuesModel::issue(const QModelIndex& index) const {
  return Data::Get().issues.at(index.row());
}
