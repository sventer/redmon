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

#include "dialogs/issue_activity_dialog.h"

#include <QDate>
#include <QDomElement>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDebug>

#include "data/activities_data_loader.h"

IssueActivityDialog::IssueActivityDialog(QWidget* parent)
  : QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowTitleHint) {
  // this dialog is model
  setModal(true);

  setWindowTitle("Issue Activity Dialog");

  // setFixedSize(250, 190);

  QHBoxLayout* issueNumberLayout = new QHBoxLayout;
  QLabel* issueNumberLabel = new QLabel("Issue Number: ");
  m_issueNumber = new QLabel("", this);
  issueNumberLayout->addWidget(issueNumberLabel);
  issueNumberLayout->addWidget(m_issueNumber);

  QVBoxLayout* issueDescriptionLayout = new QVBoxLayout;
  QLabel* issueDescriptionLabel = new QLabel("Issue Description:");
  m_issueDescription =
      new QLabel("This is the issues description and cannot be edited.");
  issueDescriptionLayout->addWidget(issueDescriptionLabel);
  issueDescriptionLayout->addWidget(m_issueDescription);

  QHBoxLayout* timeSpentLayout = new QHBoxLayout;
  QLabel* timeSpentLabel = new QLabel("Time Spent: ");
  m_timeSpent = new QLabel("00:00");
  timeSpentLayout->addWidget(timeSpentLabel);
  timeSpentLayout->addWidget(m_timeSpent);

  QHBoxLayout* activitySelectionLayout = new QHBoxLayout;

  QLabel* m_timeActivityLabel = new QLabel("Issue Activity:");
  m_timeActivity = new QComboBox;
  activitySelectionLayout->addWidget(m_timeActivityLabel);
  activitySelectionLayout->addWidget(m_timeActivity);

  QVBoxLayout* activityNotes = new QVBoxLayout;
  QLabel* activityNoteLabel = new QLabel("Activity Notes:");
  m_activityNote = new QLineEdit("");
  activityNotes->addWidget(activityNoteLabel);
  activityNotes->addWidget(m_activityNote);

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  m_commitTimeButton = new QPushButton("Apply");
  m_cancelTimeButton = new QPushButton("Cancel");
  buttonLayout->addWidget(m_commitTimeButton);
  buttonLayout->addWidget(m_cancelTimeButton);

  connect(m_cancelTimeButton, SIGNAL(clicked()), this, SLOT(hide()));
  connect(m_commitTimeButton, SIGNAL(clicked()), this, SLOT(onCommitTimeSpent()));

  QFormLayout* activityLayout = new QFormLayout;
  activityLayout->addRow(issueNumberLayout);
  activityLayout->addRow(issueDescriptionLayout);
  activityLayout->addRow(timeSpentLayout);
  activityLayout->addRow(activitySelectionLayout);
  activityLayout->addRow(activityNotes);
  activityLayout->addRow(buttonLayout);
  setLayout(activityLayout);

  m_activitiesDataLoader = new ActivitiesDataLoader();
  connect(m_activitiesDataLoader, SIGNAL(activitiesLoaded(IssueActivityType*)),
          this, SLOT(onActivitiesLoaded(IssueActivityType*)));
  m_activitiesDataLoader->loadData();
}

IssueActivityDialog::~IssueActivityDialog() {}

void IssueActivityDialog::updateDetails(const Issue& issue) {
  onUpdateIssueDestails(issue);
}

void IssueActivityDialog::updateTimeSpent(const QString& time) {
  m_timeSpent->setText(time);
}

void IssueActivityDialog::onActivitiesLoaded(
    IssueActivityType* activitiesList) {
  qDebug() << "IssueActivityDialog::onActivitiesLoaded";

  for (IssueActivityType::iterator bi(activitiesList->begin()),
       ei(activitiesList->end());
       bi != ei; ++bi) {
    QString activity = bi.value();
    m_timeActivity->addItem(activity);
  }
}

void IssueActivityDialog::onUpdateIssueDestails(const Issue& issue) {
  m_issueNumber->setText(QString::number(issue.id));
  m_issueDescription->setText(issue.subject);
}

void IssueActivityDialog::onCommitTimeSpent() {
  //<?xml version="1.0"?>

  QDomDocument doc;
  QDomProcessingInstruction inst = doc.createProcessingInstruction("xml", "version='1.0' encodeing='UTF-8'");
  doc.appendChild(inst);

  QDomElement root = doc.createElement("time_entry");
  doc.appendChild(root);

  QDomElement issueTag = doc.createElement("issue_id");
  root.appendChild(issueTag);
  QDomText issueText = doc.createTextNode(m_issueNumber->text());
  issueTag.appendChild(issueText);

  QDomElement dateTag = doc.createElement("spent_on");
  root.appendChild(dateTag);
  QDomText dateText = doc.createTextNode(QDate::currentDate().toString());
  dateTag.appendChild(dateText);

  QDomElement hoursTag = doc.createElement("hours");
  root.appendChild(hoursTag);
  QDomText hoursText = doc.createTextNode("00:23");
  hoursTag.appendChild(hoursText);

  QDomElement activityIdTag = doc.createElement("activity_id");
  root.appendChild(activityIdTag);
  QDomText activityIdText = doc.createTextNode(m_timeActivity->currentText());
  activityIdTag.appendChild(activityIdText);

  QDomElement commentsTag = doc.createElement("comments");
  root.appendChild(commentsTag);
  QDomText commentsText = doc.createTextNode(m_activityNote->text());
  commentsTag.appendChild(commentsText);

  qDebug() << doc.toString();
}
