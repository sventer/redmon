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

#include "views/active_issue_widget.h"

#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPushButton>
#include <QSettings>
#include <QTimer>
#include <QVBoxLayout>

#include "data/activities_data_loader.h"
#include "data/data.h"
#include "data/utils.h"
#include "models/time_activities_model.h"

ActiveIssueWidget::ActiveIssueWidget(QWidget* parent) : QWidget(parent) {
  // Create the NAM.
  m_nam = new QNetworkAccessManager(this);
  connect(m_nam, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(onNamFinished(QNetworkReply*)));

  // Create the timer that will update our state.
  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimerExpired()));

  // Create the labels.
  m_subjectLabel = new QLabel;
  m_issueIdLabel = new QLabel;
  m_timeEntryActivityCombo = new QComboBox;
  m_commentLineEdit = new QLineEdit;
  m_timeEntryActivityCombo->setModel(new TimeEntryActivitiesModel);

  m_elapsedTimeLabel = new QLabel;

  // Create the cancel button.
  m_cancelButton = new QPushButton("Cancel");
  connect(m_cancelButton, SIGNAL(clicked()), this,
          SLOT(onCancelButtonClicked()));

  // Create the stop button.
  m_stopButton = new QPushButton("Stop");
  connect(m_stopButton, SIGNAL(clicked()), this, SLOT(onStopButtonClicked()));

  // Reset all the values.
  resetValues();

  // Layout all the widgets.
  QVBoxLayout* leftLayout = new QVBoxLayout;
  leftLayout->addWidget(m_subjectLabel);
  leftLayout->addWidget(m_issueIdLabel);
  leftLayout->addWidget(m_commentLineEdit);
  leftLayout->addWidget(m_timeEntryActivityCombo);

  QVBoxLayout* rightLayout = new QVBoxLayout;
  rightLayout->addWidget(m_elapsedTimeLabel);
  rightLayout->addWidget(m_cancelButton);
  rightLayout->addWidget(m_stopButton);

  QHBoxLayout* mainLayout = new QHBoxLayout;
  mainLayout->addLayout(leftLayout, 1);
  mainLayout->addLayout(rightLayout, 1);

  setLayout(mainLayout);

  // Start the process of loading the time entry activities.
  m_activitiesDataLoader = new ActivitiesDataLoader(this);
  connect(m_activitiesDataLoader, SIGNAL(finished()), this,
          SLOT(onActivitiesDataLoaderFinished()));
  m_activitiesDataLoader->loadData();
}

ActiveIssueWidget::~ActiveIssueWidget() {}

void ActiveIssueWidget::startTrackingIssue(const Issue& issue) {
  // Reset all the values before we start tracking.
  resetValues();

  // Update the details from the issue.
  m_currentIssueId = issue.id;
  m_subjectLabel->setText(issue.subject);
  m_issueIdLabel->setText(QString::number(issue.id));

  // Enable all the controls we are allowed to edit while we are tracking time.
  m_commentLineEdit->setEnabled(true);
  m_timeEntryActivityCombo->setEnabled(true);
  m_cancelButton->setEnabled(true);
  m_stopButton->setEnabled(true);

  // Set the time when we started tracking the issue, which is now.
  m_elapsedTime.start();
  updateElapsedTimeLabel();

  // Start the timer that will update the display of time spent.
  m_timer->start(1000);
}

void ActiveIssueWidget::onStopButtonClicked() {
  qDebug() << "Stop button clicked.";

  QSettings settings;
  QString apiKey = settings.value("apiKey").toString();

  // We disable changes to the form until we get the response back.
  m_commentLineEdit->setEnabled(false);
  m_timeEntryActivityCombo->setEnabled(false);
  m_cancelButton->setEnabled(false);
  m_stopButton->setEnabled(false);

  // Stop the timer.
  m_timer->stop();

  // Post the new time entry.
  QDomDocument doc;
  createTimeEntryRequest(&doc);

  qDebug() << buildServerUrl("/time_entries.xml");

  QNetworkRequest request(buildServerUrl("/time_entries.xml"));
  request.setHeader(QNetworkRequest::ContentTypeHeader,
    "application/xml; charset=\"utf-8\"");
  request.setRawHeader("X-Redmine-API-Key", apiKey.toLatin1());

  // Post the request.
  m_nam->post(request, doc.toString().toLatin1());
}

void ActiveIssueWidget::onCancelButtonClicked() {
  if (QMessageBox::question(
          this, "Cancel",
          "Are you sure you want to cancel tracking the current issue?",
          QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
    return;

  resetValues();
  emit trackingCompleted();
}

void ActiveIssueWidget::onTimerExpired() {
  updateElapsedTimeLabel();
}

void ActiveIssueWidget::onNamFinished(QNetworkReply* reply) {
  int statusCode =
      reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

  bool success = (statusCode == 201);

  if (success) {
    // If we are successful, we can reset all the data, and the timer will be
    // stopped.
    resetValues();

    emit trackingCompleted();
  } else {
    // If we weren't successful, we enable all the controls again so we can try
    // again.
    m_commentLineEdit->setEnabled(true);
    m_timeEntryActivityCombo->setEnabled(true);
    m_cancelButton->setEnabled(true);
    m_stopButton->setEnabled(true);
  }
}

void ActiveIssueWidget::onActivitiesDataLoaderFinished() {
  qDebug() << "refresh";

  // Swap the data into the global store.
  m_activitiesDataLoader->swapTimeEntryActivities(
      &Data::Get().timeEntryActivities);

  TimeEntryActivitiesModel* model =
      static_cast<TimeEntryActivitiesModel*>(m_timeEntryActivityCombo->model());
  model->refresh();
}

void ActiveIssueWidget::resetValues() {
  m_timer->stop();

  m_subjectLabel->setText("");
  m_issueIdLabel->setText("");
  m_elapsedTimeLabel->setText("");
  m_timeEntryActivityCombo->setCurrentIndex(-1);
  m_timeEntryActivityCombo->setEnabled(false);
  m_commentLineEdit->setText("");
  m_commentLineEdit->setEnabled(false);
  m_cancelButton->setEnabled(false);
  m_stopButton->setEnabled(false);
}

void ActiveIssueWidget::updateElapsedTimeLabel() {
  int elapsedSeconds = (m_elapsedTime.elapsed() / 1000);

  int seconds = elapsedSeconds % 60;
  int minutes = (elapsedSeconds / 60) % 60;
  int hours = (elapsedSeconds / 60 / 60);

  QString hoursStr;
  if (hours == 1)
    hoursStr = QString("%1 hour").arg(hours);
  else if (hours > 1)
    hoursStr = QString("%1 hours").arg(hours);

  QString minutesStr;
  if (minutes == 1)
    minutesStr = QString("%1 minute").arg(minutes);
  else if (minutes > 1)
    minutesStr = QString("%1 minutes").arg(minutes);

  QString secondsStr;
  if (seconds == 1)
    secondsStr = QString("%1 second").arg(seconds);
  else if (seconds > 1)
    secondsStr = QString("%1 seconds").arg(seconds);

  QStringList items;
  if (!hoursStr.isEmpty())
    items << hoursStr;
  if (!minutesStr.isEmpty())
    items << minutesStr;
  if (!secondsStr.isEmpty())
    items << secondsStr;

  m_elapsedTimeLabel->setText(items.join(", "));
}

void ActiveIssueWidget::createTimeEntryRequest(QDomDocument* out) {
  // Calculate the time spent in hours.
  float hours =
      static_cast<float>(m_elapsedTime.elapsed()) / 1000.0f / 60.0f / 60.0f;

  // Get the time entry activity id.
  int timeEntryActivityId =
      Data::Get()
          .timeEntryActivities.at(m_timeEntryActivityCombo->currentIndex())
          .id;

  // Build the document.
  QDomProcessingInstruction inst = out->createProcessingInstruction(
      "xml", "version=\"1.0\" encodeing=\"utf-8\"");
  out->appendChild(inst);

  QDomElement root = out->createElement("time_entry");
  out->appendChild(root);

  QDomElement issueTag = out->createElement("issue_id");
  root.appendChild(issueTag);
  QDomText issueText = out->createTextNode(QString::number(m_currentIssueId));
  issueTag.appendChild(issueText);

  QDomElement dateTag = out->createElement("spent_on");
  root.appendChild(dateTag);
  QDate date = QDate::currentDate();
  QDomText dateText = out->createTextNode(date.toString("yyyy-MM-dd"));
  dateTag.appendChild(dateText);

  QDomElement hoursTag = out->createElement("hours");
  root.appendChild(hoursTag);
  QDomText hoursText = out->createTextNode(QString::number(hours, 'f', 4));
  hoursTag.appendChild(hoursText);

  QDomElement activityIdTag = out->createElement("activity_id");
  root.appendChild(activityIdTag);

  QDomText activityIdText =
      out->createTextNode(QString::number(timeEntryActivityId));
  activityIdTag.appendChild(activityIdText);

  QDomElement commentsTag = out->createElement("comments");
  root.appendChild(commentsTag);
  QDomText commentsText = out->createTextNode(m_commentLineEdit->text());
  commentsTag.appendChild(commentsText);
}
