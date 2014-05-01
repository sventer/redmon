#ifndef ISSUE_H
#define ISSUE_H

#include <QString>

class Issue
{
public:
    Issue();
    ~Issue();

    const QString& subject() const { return m_subject; }
    void setSubject(const QString& subject);

    const QString& assignedTo() const { return m_assignedTo; }
    void setAssignedTo(const QString& assignedTo);

private:
    QString m_subject;
    QString m_assignedTo;
};

#endif // ISSUE_H
