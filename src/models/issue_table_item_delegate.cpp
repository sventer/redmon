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

#include "models/issue_table_item_delegate.h"

#include <QTableView>
#include <QPainter>
#include <QDebug>

#include "models/issues_table_model.h"

const int kItemBorderSize = 5;
const int kTextSpacing = 1;

IssueTableItemDelegate::IssueTableItemDelegate(QObject* parent)
: QStyledItemDelegate(parent) {
}

IssueTableItemDelegate::~IssueTableItemDelegate() {
}

void IssueTableItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
	const Issue& issue = issueFromIndex(index);

	painter->save();

	QRect textRect;

	QPen darkGrayPen(QColor(96, 96, 96));
	QBrush backgroundBrush(QColor(240, 240, 240));

	QRect drawRect = option.rect;

	painter->setPen(QPen(Qt::NoPen));

	if (option.state && QStyle::State_Selected) {
		painter->setBrush(backgroundBrush);
		painter->drawRect(option.rect);
	}

	drawRect.adjust(kItemBorderSize, kItemBorderSize, -kItemBorderSize, -kItemBorderSize);

	switch (index.column()) {
	case 0:  // this is the issue id
		painter->setPen(darkGrayPen);
		painter->setFont(getIdTextFont(option.font));
		painter->drawText(drawRect, Qt::AlignLeft, QString::number(issue.id));
		break;
	case 1:  // this is the subject
		painter->setPen(darkGrayPen);
		painter->setFont(getIdTextFont(option.font));
		painter->drawText(drawRect, Qt::AlignLeft, issue.subject);
		break;
	case 2:  // this is the project description
		painter->setPen(darkGrayPen);
		painter->setFont(getIdTextFont(option.font));
		painter->drawText(drawRect, Qt::AlignLeft, issue.projectName);
		break;
	case 3:  // this is the project description
		painter->setPen(darkGrayPen);
		painter->setFont(getIdTextFont(option.font));
		painter->drawText(drawRect, Qt::AlignLeft, QString::number(issue.hoursSpent, 'f', 2));
		break;
	case 4:  // this is the project description
		painter->setPen(darkGrayPen);
		painter->setFont(getIdTextFont(option.font));
		painter->drawText(drawRect, Qt::AlignLeft, issue.priorityName);
		break;
	}
}

QSize IssueTableItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const {
	const Issue& issue = issueFromIndex(index);

	const QTableView* tableView = static_cast<const QTableView*>(option.widget);

	return (QSize(100, 20));
}

const Issue& IssueTableItemDelegate::issueFromIndex(const QModelIndex& index) const {
	const IssuesTableModel* model = static_cast<const IssuesTableModel*>(index.model());
	return model->issue(index);
}

QFont IssueTableItemDelegate::getIdTextFont(const QFont& font) {
	QFont result(font);
	result.setPixelSize(12);
	result.setBold(false);
	return result;
}

#if 0
QString IssueTableItemDelegate::getInfoLineText(const Issue& issue) {

}
#endif  // 0
