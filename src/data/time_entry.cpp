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

#include "data/time_entry.h"

void updateTimeEntryFromXml(const QDomElement& timeEntryElement,
                            TimeEntry* timeEntryOut) {
  Q_ASSERT(timeEntryOut);

  for (QDomElement elem(timeEntryElement.firstChildElement()); !elem.isNull();
       elem = elem.nextSiblingElement()) {
    if (elem.tagName() == "id") {
      timeEntryOut->id = elem.text().toInt();
      continue;
    }

    if (elem.tagName() == "project") {
      QDomAttr idAttr = elem.attributeNode("id");
      if (!idAttr.isNull())
        timeEntryOut->projectId = idAttr.value().toInt();

      QDomAttr nameAttr = elem.attributeNode("name");
      if (!nameAttr.isNull())
        timeEntryOut->projectName = nameAttr.value();
    }

    if (elem.tagName() == "issue") {
      QDomAttr idAttr = elem.attributeNode("id");
      if (!idAttr.isNull())
        timeEntryOut->issueId = idAttr.value().toInt();

      QDomAttr nameAttr = elem.attributeNode("name");
      if (!nameAttr.isNull())
        timeEntryOut->issueName = nameAttr.value();
    }

    if (elem.tagName() == "user") {
      QDomAttr idAttr = elem.attributeNode("id");
      if (!idAttr.isNull())
        timeEntryOut->userId = idAttr.value().toInt();

      QDomAttr nameAttr = elem.attributeNode("name");
      if (!nameAttr.isNull())
        timeEntryOut->userName = nameAttr.value();
    }

    if (elem.tagName() == "activity") {
      QDomAttr idAttr = elem.attributeNode("id");
      if (!idAttr.isNull())
        timeEntryOut->activityId = idAttr.value().toInt();

      QDomAttr nameAttr = elem.attributeNode("name");
      if (!nameAttr.isNull())
        timeEntryOut->activityName = nameAttr.value();
    }

    if (elem.tagName() == "hours") {
      timeEntryOut->hours = elem.text().toFloat();
      continue;
    }

    if (elem.tagName() == "comments") {
      timeEntryOut->comments = elem.text();
      continue;
    }

    if (elem.tagName() == "spent_on") {
      timeEntryOut->spentOn = QDate::fromString(elem.text());
      continue;
    }

    if (elem.tagName() == "created_on") {
      timeEntryOut->createdOn = QDateTime::fromString(elem.text());
      continue;
    }

    if (elem.tagName() == "updated_on") {
      timeEntryOut->updatedOn = QDateTime::fromString(elem.text());
      continue;
    }
  }
}
