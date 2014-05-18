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

#include "data/data_loader.h"

#include "data/issue.h"

void parseIssues(const QDomElement& root, QVector<Issue>* issues) {
  Q_ASSERT(issues);

  issues->clear();

  for (QDomElement issueElem = root.firstChildElement("issue");
       !issueElem.isNull(); issueElem = issueElem.nextSiblingElement("issue")) {
    Issue issue;

    for (QDomElement elem(issueElem.firstChildElement()); !elem.isNull();
         elem = elem.nextSiblingElement()) {
      if (elem.tagName() == "id") {
        issue.id = elem.text().toInt();
        continue;
      }

      if (elem.tagName() == "subject") {
        issue.subject = elem.text();
        continue;
      }

      if (elem.tagName() == "project") {
        QDomAttr idAttr = elem.attributeNode("id");
        if (!idAttr.isNull())
          issue.projectId = idAttr.value().toInt();

        QDomAttr nameAttr = elem.attributeNode("name");
        if (!nameAttr.isNull())
          issue.projectName = nameAttr.value();
      }
    }

    issues->append(issue);
  }
}
