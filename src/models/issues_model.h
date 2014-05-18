#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QAbstractItemModel>
#include <QVector>

#include "data/issue.h"

class IssuesModel : public QAbstractListModel {
  Q_OBJECT

public:
  typedef QVector<Issue*> ListType;

  explicit IssuesModel(QObject* parent = 0);

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QModelIndex sibling(int row, int column, const QModelIndex& idx) const;

  int columnCount(const QModelIndex& parent = QModelIndex()) const;

  QVariant data(const QModelIndex& index, int role) const;

  const Issue& issue(const QModelIndex& index) const;

private:
  void RebuildOrderVector();

  Q_DISABLE_COPY(IssuesModel)
};

#endif  // TASKSMODEL_H
