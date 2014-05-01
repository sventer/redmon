#include "models/tasks_model.h"

#include <QVariant>

TasksModel::TasksModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

TasksModel::TasksModel(const ListType& issues, QObject *parent)
    : QAbstractListModel(parent), m_issues(issues)
{
}

int TasksModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_issues.count();
}

QModelIndex TasksModel::sibling(int row, int column, const QModelIndex &idx) const
{
    if (!idx.isValid() || column != 0 || row >= m_issues.count())
        return QModelIndex();

    return createIndex(row, 0);
}

int TasksModel::columnCount(const QModelIndex& parent) const {
    return 2;
}

QVariant TasksModel::data(const QModelIndex &index, int role) const
{
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

Qt::ItemFlags TasksModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractListModel::flags(index) | Qt::ItemIsDropEnabled;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

bool TasksModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
#if 0
    if (index.row() >= 0 && index.row() < m_issues.size()
        && (role == Qt::EditRole || role == Qt::DisplayRole)) {
        lst.replace(index.row(), value.toString());
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
#endif  // 0
    return false;
}

bool TasksModel::insertRows(int row, int count, const QModelIndex &parent)
{
    return false;

#if 0
    if (count < 1 || row < 0 || row > rowCount(parent))
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        lst.insert(row, QString());

    endInsertRows();

    return true;
#endif  // 0
}

bool TasksModel::removeRows(int row, int count, const QModelIndex &parent)
{
    return false;
#if 0
    if (count <= 0 || row < 0 || (row + count) > rowCount(parent))
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int r = 0; r < count; ++r)
        lst.removeAt(row);

    endRemoveRows();

    return true;
#endif  // 0
}

static bool ascendingLessThan(const QPair<QString, int> &s1, const QPair<QString, int> &s2)
{
    return s1.first < s2.first;
}

static bool decendingLessThan(const QPair<QString, int> &s1, const QPair<QString, int> &s2)
{
    return s1.first > s2.first;
}

void TasksModel::sort(int, Qt::SortOrder order)
{
#if 0
    emit layoutAboutToBeChanged(QList<QPersistentModelIndex>(), VerticalSortHint);

    QList<QPair<QString, int> > list;
    for (int i = 0; i < lst.count(); ++i)
        list.append(QPair<QString, int>(lst.at(i), i));

    if (order == Qt::AscendingOrder)
        std::sort(list.begin(), list.end(), ascendingLessThan);
    else
        std::sort(list.begin(), list.end(), decendingLessThan);

    lst.clear();
    QVector<int> forwarding(list.count());
    for (int i = 0; i < list.count(); ++i) {
        lst.append(list.at(i).first);
        forwarding[list.at(i).second] = i;
    }

    QModelIndexList oldList = persistentIndexList();
    QModelIndexList newList;
    for (int i = 0; i < oldList.count(); ++i)
        newList.append(index(forwarding.at(oldList.at(i).row()), 0));
    changePersistentIndexList(oldList, newList);

    emit layoutChanged(QList<QPersistentModelIndex>(), VerticalSortHint);
#endif  // 0
}

const TasksModel::ListType& TasksModel::issues() const
{
    return m_issues;
}

void TasksModel::setIssues(const ListType& issues)
{
    emit beginResetModel();
    m_issues = issues;
    emit endResetModel();
}

Qt::DropActions TasksModel::supportedDropActions() const
{
    return QAbstractItemModel::supportedDropActions() | Qt::MoveAction;
}

