#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QAbstractListModel>
#include <QList>

#include "data/issue.h"

class TasksModel : public QAbstractListModel {
    Q_OBJECT
public:
    typedef QList<Issue> ListType;

    explicit TasksModel(QObject *parent = 0);
    explicit TasksModel(const ListType& issues, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex sibling(int row, int column, const QModelIndex &idx) const;

    int columnCount(const QModelIndex& parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

    const ListType& issues() const;
    void setIssues(const ListType& issues);

    Qt::DropActions supportedDropActions() const;

private:
    Q_DISABLE_COPY(TasksModel)

    ListType m_issues;
};


#endif // TASKSMODEL_H
