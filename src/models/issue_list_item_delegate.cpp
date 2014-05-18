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

#include "models/issue_list_item_delegate.h"

#include <QDebug>
#include <QPainter>
#include <QListView>

#include "models/issues_model.h"

const int kItemMarginSize = 2;
const int kItemBorderSize = 5;

const int kTextSpacing = 1;

IssueListItemDelegate::IssueListItemDelegate(QObject* parent)
  : QStyledItemDelegate(parent) {}

IssueListItemDelegate::~IssueListItemDelegate() {}

void IssueListItemDelegate::paint(QPainter* painter,
                                  const QStyleOptionViewItem& option,
                                  const QModelIndex& index) const {
  const Issue& issue = issueFromIndex(index);

  painter->save();

  QRect textRect;

  QPen darkGrayPen(QColor(96, 96, 96));
  QBrush backgroundBrush(QColor(232, 232, 232));
  QBrush selectedBackgroundBrush(QColor(200, 200, 200));

  // Calculate the drawRect with only the margin applied.
  QRect drawRect = option.rect.adjusted(kItemMarginSize, kItemMarginSize,
                                        -kItemMarginSize, -kItemMarginSize);

  // Draw the backgrond rect.
  painter->setPen(QPen(Qt::NoPen));
  if (option.state & QStyle::State_Selected)
    painter->setBrush(selectedBackgroundBrush);
  else
    painter->setBrush(backgroundBrush);
  painter->drawRoundedRect(drawRect, 5.0, 5.0);

  // Adjust the drawRect to include the item border.
  drawRect.adjust(kItemBorderSize, kItemBorderSize, -kItemBorderSize,
                  -kItemBorderSize);

  // Draw the subject text.
  painter->setPen(darkGrayPen);
  painter->setFont(getSubjectFont(option.font));
  painter->drawText(drawRect, Qt::TextWordWrap, issue.subject, &textRect);

  // Adjust the drawRect for painting the project name.
  drawRect.setTop(drawRect.top() + kTextSpacing + textRect.height());

  // Draw the project name.
  painter->setPen(darkGrayPen);
  painter->setFont(getProjectNameFont(option.font));
  painter->drawText(drawRect, Qt::TextWordWrap, issue.projectName, &textRect);

  painter->restore();
}

QSize IssueListItemDelegate::sizeHint(const QStyleOptionViewItem& option,
                                      const QModelIndex& index) const {
  const Issue& issue = issueFromIndex(index);

  const QListView* listView = static_cast<const QListView*>(option.widget);

  int itemHeight = (kItemMarginSize + kItemBorderSize) * 2;
  int clientWidth = listView->viewport()->width();

  {
    QFontMetrics metrics(getSubjectFont(option.font));
    QRect textRect = metrics.boundingRect(QRect(0, 0, clientWidth, 0),
                                          Qt::TextWordWrap, issue.subject);
    itemHeight += textRect.height();
  }

  itemHeight += kTextSpacing;

  QFontMetrics metrics(getProjectNameFont(option.font));
  QRect textRect = metrics.boundingRect(QRect(0, 0, clientWidth, 0),
                                        Qt::TextWordWrap, issue.projectName);
  itemHeight += textRect.height();

  return QSize(clientWidth, itemHeight);
}

const Issue& IssueListItemDelegate::issueFromIndex(
    const QModelIndex& index) const {
  const IssuesModel* model = static_cast<const IssuesModel*>(index.model());
  return model->issue(index);
}

// static
QFont IssueListItemDelegate::getSubjectFont(const QFont& font) {
  QFont result(font);
  result.setPixelSize(16);
  result.setBold(true);
  return result;
}

// static
QFont IssueListItemDelegate::getProjectNameFont(const QFont& font) {
  QFont result(font);
  result.setPixelSize(12);
  return result;
}
