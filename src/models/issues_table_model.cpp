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

#include "models/issues_table_model.h"

#include <QVariant>
#include <QDebug>

#include "data/data.h"

IssuesTableModel::IssuesTableModel(QObject* parent)
	: QAbstractTableModel(parent) {
}

int IssuesTableModel::rowCount(const QModelIndex& parent) const {
	if (parent.isValid())
		return 0;

	return Data::Get().issues.size();
}

int IssuesTableModel::columnCount(const QModelIndex& parent) const {
	if (parent.isValid())
		return 0;

	// the number of columns in our model that will be displayed.
	return 5;
}

QVariant IssuesTableModel::data(const QModelIndex& index, int role) const {
	if (index.row() < 0 || index.row() > Data::Get().issues.size())
		return QVariant();

	const Issue& issue = Data::Get().issues.at(index.row());

	qDebug() << "table column number [" << QString::number(index.column()) << "]";

	switch (index.column()) {
	case 0:
		return QString::number(issue.id);
	case 1:
		return issue.priorityName;
	case 2:
		return issue.hoursSpent;
	case 3:
		return issue.projectName;
	case 4:
		return issue.subject;
	}

	return QVariant();
}

QVariant IssuesTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole) {
		if (orientation == Qt::Horizontal) {
			switch (section) {
			case 0:
				return QString("Id");
			case 1:
				return QString("Priority");
			case 2:
				return QString("Hours Spent");
			case 3:
				return QString("Project");
			case 4:
				return QString("Subject");
			}
		}
	}

	return QVariant();
}

const Issue& IssuesTableModel::issue(const QModelIndex& index) const {
	qDebug() << "######### row number [" << QString::number(index.row()) << "]";
	return Data::Get().issues.at(index.row());
}

