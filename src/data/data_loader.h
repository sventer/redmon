
#ifndef DATA_DATA_LOADER_H_
#define DATA_DATA_LOADER_H_

#include <QDomElement>
#include <QVector>

struct Issue;

void parseIssues(const QDomElement& root, QVector<Issue>* issues);

#endif  // DATA_DATA_LOADER_H_
