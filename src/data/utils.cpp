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

#include "data/utils.h"

#include <QDomElement>
#include <QDebug>
#include <QSettings>

void loadCountersFromElement(QDomElement* elem, int* totalCountOut,
                             int* offsetOut, int* limitOut) {
  Q_ASSERT(elem);
  Q_ASSERT(totalCountOut);
  Q_ASSERT(offsetOut);
  Q_ASSERT(limitOut);

  QDomAttr attr;

  attr = elem->attributeNode("total_count");
  if (!attr.isNull())
    *totalCountOut = attr.value().toInt();

  attr = elem->attributeNode("offset");
  if (!attr.isNull())
    *offsetOut = attr.value().toInt();

  attr = elem->attributeNode("limit");
  if (!attr.isNull())
    *limitOut = attr.value().toInt();
}

QString buildServerUrl(const QString& path) {
  QSettings settings;

  QString str("http://%1");
  str = str.arg(settings.value("serverUrl").toString());

  if (!path.startsWith('/'))
    str.append('/');

  str.append(path);

  return str;
}
