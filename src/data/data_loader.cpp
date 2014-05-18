
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
