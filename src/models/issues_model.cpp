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

#include "models/issues_model.h"

#include <QVariant>

#include "data/data.h"

IssuesModel::IssuesModel(QObject* parent) : QAbstractListModel(parent) {}

int IssuesModel::rowCount(const QModelIndex& parent) const {
  if (parent.isValid())
    return 0;

  return Data::Get().issues.size();
}

QModelIndex IssuesModel::sibling(int row, int column,
                                 const QModelIndex& idx) const {
  if (!idx.isValid() || column != 0 || row >= Data::Get().issues.size())
    return QModelIndex();

  return createIndex(row, column);
}

int IssuesModel::columnCount(const QModelIndex& parent) const { return 1; }

QVariant IssuesModel::data(const QModelIndex& index, int role) const {
  if (index.row() < 0 || index.row() >= Data::Get().issues.size())
    return QVariant();

  if (role == Qt::DisplayRole) {
    const Issue& issue = Data::Get().issues.at(index.row());

    switch (index.column()) {
    case 0:
      return issue.subject;
    }
  }

  return QVariant();
}

const Issue& IssuesModel::issue(const QModelIndex& index) const {
  return Data::Get().issues.at(index.row());
}
