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

#ifndef TIME_COMMIT_DIALOG
#define TIME_COMMIT_DIALOG

#include <QDialog>
#include <QTime>
#include "data/activities_data_loader.h"
#include "data/issue.h"

class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;

class ActivitiesDataLoader;

class IssueActivityDialog : public QDialog {
  Q_OBJECT

public:
  IssueActivityDialog(QWidget* parent = 0);
  virtual ~IssueActivityDialog();

  void updateDetails(const Issue& issue);
  void updateTimeSpent(float time);

public slots:
  void onActivitiesLoaded(IssueActivityType* activitiesList);
  void onUpdateIssueDestails(const Issue& issue);
  void onCommitTimeSpent();
  
private:
  QLabel* m_issueNumber;
  QLabel* m_issueDescription;
  QLabel* m_timeSpent;
  QComboBox* m_timeActivity;
  QLineEdit* m_activityNote;
  QPushButton* m_commitTimeButton;
  QPushButton* m_cancelTimeButton;
  
  ActivitiesDataLoader* m_activitiesDataLoader;
  float m_time;
};

#endif  // TIME_COMMIT_DIALOG
