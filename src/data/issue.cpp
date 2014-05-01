#include "issue.h"

Issue::Issue() {
}

Issue::~Issue() {
}

void Issue::setSubject(const QString& subject) {
    m_subject = subject;
}

void Issue::setAssignedTo(const QString& assignedTo) {
    m_assignedTo = assignedTo;
}
