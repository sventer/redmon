
#include "data/data.h"

// static
Data& Data::Get() {
  static Data data;
  return data;
}
