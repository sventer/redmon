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

#include "dialogs/time_commit_dialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDebug>

TimeCommitDialog::TimeCommitDialog() {
  // this dialog is model
  setModal(true);

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
  //QVBoxLayout* activityLayout = new QVBoxLayout;
  activityLayout->addRow(issueNumberLayout);
  activityLayout->addRow(issueDescriptionLayout);
  activityLayout->addRow(timeSpentLayout);
  activityLayout->addRow(activitySelectionLayout);
  activityLayout->addRow(activityNotes);
  activityLayout->addRow(buttonLayout);
  setLayout(activityLayout);
}

TimeCommitDialog::~TimeCommitDialog() {
}

void TimeCommitDialog::updateActivities(const QMap<int, QString>& activities) {
  for (QMap<int, QString>::const_iterator bi(activities.begin()), ei(activities.end()); bi != ei; ++bi) {
    QString tmp = bi.value();
    qDebug() << "#$#$#$#$#$#$#$#$#$ " << tmp;
    Q_ASSERT(m_timeActivity);
    m_timeActivity->addItem(tmp);
  }
}