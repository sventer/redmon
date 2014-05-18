
#ifndef WINDOWS_ISSUE_LIST_ITEM_DELEGATE_H_
#define WINDOWS_ISSUE_LIST_ITEM_DELEGATE_H_

#include <QStyledItemDelegate>
#include "data/issue.h"

class IssueListItemDelegate : public QStyledItemDelegate {
  Q_OBJECT

public:
  IssueListItemDelegate(QObject* parent = 0);
  virtual ~IssueListItemDelegate();

  // Override: QItemDelegate
  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
  virtual QSize sizeHint(const QStyleOptionViewItem& option,
                         const QModelIndex& index) const override;

private:
  const Issue& issueFromIndex(const QModelIndex& index) const;
  static QFont getSubjectFont(const QFont& font);
  static QFont getProjectNameFont(const QFont& font);
};

#endif  // WINDOWS_ISSUE_LIST_ITEM_DELEGATE_H_
