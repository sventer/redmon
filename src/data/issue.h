#ifndef ISSUE_H
#define ISSUE_H

#include <QString>

struct Issue {
  Issue() : id(0) {}
  ~Issue() {}

  int id;
  QString subject;

  int assignedToId;
  QString assignedToName;

  int projectId;
  QString projectName;
};

#endif  // ISSUE_H
