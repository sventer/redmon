#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "data/issue.h"

class IssuesModel : public QAbstractListModel {
  Q_OBJECT

public:
  typedef QList<Issue> ListType;

  explicit IssuesModel(QObject *parent = 0);
  explicit IssuesModel(const ListType &issues, QObject *parent = 0);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QModelIndex sibling(int row, int column, const QModelIndex &idx) const;

  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  QVariant data(const QModelIndex &index, int role) const;

  const ListType &issues() const;
  void setIssues(const ListType &issues);

private:
  ListType m_issues;

  Q_DISABLE_COPY(IssuesModel)
};

#endif  // TASKSMODEL_H
