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

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDebug>

#include "data/activities_data_loader.h"

IssueActivityDialog::IssueActivityDialog() {
  // this dialog is model
  setModal(true);

  setWindowTitle("Issue Activity Dialog");

  //setFixedSize(250, 190);

  QHBoxLayout* issueNumberLayout = new QHBoxLayout;
  QLabel* issueNumberLabel = new QLabel("Issue Number: ");
  m_issueNumber = new QLabel("2983", this);
  issueNumberLayout->addWidget(issueNumberLabel);
  issueNumberLayout->addWidget(m_issueNumber);

  QVBoxLayout* issueDescriptionLayout = new QVBoxLayout;
  QLabel* issueDescriptionLabel = new QLabel("Issue Description:");
  m_issueDescription = new QLabel("This is the issues description and cannot be edited.");
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
  m_activityNote = new QLineEdit("Notes");
  activityNotes->addWidget(activityNoteLabel);
  activityNotes->addWidget(m_activityNote);

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  m_commitTimeButton = new QPushButton("Apply");
  m_cancelTimeButton = new QPushButton("Cancel");
  buttonLayout->addWidget(m_commitTimeButton);
  buttonLayout->addWidget(m_cancelTimeButton);

  QFormLayout* activityLayout = new QFormLayout;
  activityLayout->addRow(issueNumberLayout);
  activityLayout->addRow(issueDescriptionLayout);
  activityLayout->addRow(timeSpentLayout);
  activityLayout->addRow(activitySelectionLayout);
  activityLayout->addRow(activityNotes);
  activityLayout->addRow(buttonLayout);
  setLayout(activityLayout);
  
  m_activitiesDataLoader = new ActivitiesDataLoader();
  connect(m_activitiesDataLoader, SIGNAL(activitiesLoaded(IssueActivityType*)), this, SLOT(onActivitiesLoaded(IssueActivityType*)));
  m_activitiesDataLoader->loadData();
}

IssueActivityDialog::~IssueActivityDialog() {
}

void IssueActivityDialog::onActivitiesLoaded(IssueActivityType* activitiesList) {
  qDebug() << "IssueActivityDialog::onActivitiesLoaded";

  for (IssueActivityType::iterator bi(activitiesList->begin()), ei(activitiesList->end()); bi != ei; ++bi) {
    QString activity = bi.value();
    m_timeActivity->addItem(activity);
  }
}
