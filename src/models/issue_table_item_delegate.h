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

#ifndef ISSUE_TABLE_ITEM_DELEGATE_H
#define ISSUE_TABLE_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include "data/issue.h"

class IssueTableItemDelegate : public QStyledItemDelegate {
	Q_OBJECT

public:
	explicit IssueTableItemDelegate(QObject* parent = 0);
	virtual ~IssueTableItemDelegate();

	virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
	const Issue& issueFromIndex(const QModelIndex& index) const;
	static QFont getIdTextFont(const QFont& font);
#if 0
	static QString getInfoLineText(const Issue& issue);
#endif  // 0
};

#endif  // ISSUE_TABLE_ITEM_DELEGATE_H
