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

#ifndef VIEWS_ACTIVE_ISSUE_WIDGET_H_
#define VIEWS_ACTIVE_ISSUE_WIDGET_H_

#include <QDateTime>
#include <QWidget>

#include "data/issue.h"

class QComboBox;
class QLabel;
class QLineEdit;
class QNetworkAccessManager;
class QNetworkReply;
class QPushButton;
class QTimer;

class ActivitiesDataLoader;

class ActiveIssueWidget : public QWidget {
  Q_OBJECT

public:
  explicit ActiveIssueWidget(QWidget* parent = 0);
  virtual ~ActiveIssueWidget();

  // Starts tracking time for the specified issue.
  void startTrackingIssue(const Issue& issue);

signals:
  void trackingCompleted();

private slots:
  void onStopButtonClicked();
  void onCancelButtonClicked();

  void onTimerExpired();

  void onNamFinished(QNetworkReply* reply);

  void onActivitiesDataLoaderFinished();

private:
  // Reset all the widget values to the default.
  void resetValues();

  // Update the elapsed time label with the current amount of time spent.
  void updateElapsedTimeLabel();

  // Create the XML that represents a time entry using the current information
  // embedded in this widget.
  void createTimeEntryRequest(QDomDocument* out);

  // Widgets
  QLabel* m_subjectLabel;
  QLabel* m_issueIdLabel;
  QLabel* m_elapsedTimeLabel;
  QComboBox* m_timeEntryActivityCombo;
  QLineEdit* m_commentLineEdit;
  QPushButton* m_cancelButton;
  QPushButton* m_stopButton;

  // The ID of the issue we are currently tracking.  -1 if no issue is being
  // tracked.
  int m_currentIssueId;

  // The amount of time that has elapsed since we started tracking the issue.
  QTime m_elapsedTime;

  // The timer that updates the display of how long has been spent so far on
  // this issue.
  QTimer* m_timer;

  // The NAM we use to post new time entries.
  QNetworkAccessManager* m_nam;

  // The loader we use to update the time entry activities.
  ActivitiesDataLoader* m_activitiesDataLoader;

  Q_DISABLE_COPY(ActiveIssueWidget)
};

#endif  // VIEWS_ACTIVE_ISSUE_WIDGET_H_
