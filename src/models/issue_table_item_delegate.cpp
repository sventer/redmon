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

#include "models/issues_table_model.h"

const int kItemBorderSize = 5;
const int kTaxtSpacing = 1;

IssueTableItemDelegate::IssueTableItemDelegate(QObject* parent)
: QStyledItemDelegate(parent) {
}

IssueTableItemDelegate::~IssueTableItemDelegate() {
}

void IssueTableItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {

	int i = 0;
	for (int t = 0; t < 10; t++)
		i++;
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
