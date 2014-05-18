
#ifndef DATA_DATA_H_
#define DATA_DATA_H_

#include <QDomElement>
#include <QVector>

#include "data/issue.h"

class Data {
public:
  static Data& Get();

  QVector<Issue> issues;
};

#endif  // DATA_DATA_H_
