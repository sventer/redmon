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

#include "windows/settings_window.h"

#include <QCheckBox>
#include <QDebug>
#include <QFormLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSettings>

SettingsWindow::SettingsWindow(QWidget* parent) : QDialog(parent) {
  QSettings settings;

  // Create controls.

  m_serverUrlEdit = new QLineEdit(settings.value("serverUrl").toString());

  m_apiKeyEdit = new QLineEdit(settings.value("apiKey").toString());

  m_onlyMyIssuesCheckBox = new QCheckBox("Only show my issues");
  m_onlyMyIssuesCheckBox->setChecked(settings.value("onlyMyIssues").toBool());

  m_onlyMyTimeEntriesCheckBox = new QCheckBox("Only show my time entries");
  m_onlyMyTimeEntriesCheckBox->setChecked(
      settings.value("onlyMyTimeEntries").toBool());

  QPushButton* updateButton = new QPushButton("Update");
  connect(updateButton, SIGNAL(clicked()), this, SLOT(onUpdateButtonClicked()));

  // Set up the layout.

  QFormLayout* formLayout = new QFormLayout;
  // formLayout->setSizeConstraint(QLayout::SetFixedSize);

  formLayout->addRow(new QLabel("Server URL"), m_serverUrlEdit);
  formLayout->addRow(new QLabel("API Key"), m_apiKeyEdit);
  formLayout->addRow(NULL, m_onlyMyIssuesCheckBox);
  formLayout->addRow(NULL, m_onlyMyTimeEntriesCheckBox);
  formLayout->addRow(NULL, updateButton);

  setLayout(formLayout);
}

SettingsWindow::~SettingsWindow() {
  QSettings settings;
  settings.setValue("serverUrl", m_serverUrlEdit->text());
  settings.setValue("apiKey", m_apiKeyEdit->text());
  settings.setValue("onlyMyIssues", m_onlyMyIssuesCheckBox->isChecked());
  settings.setValue("onlyMyTimeEntries",
                    m_onlyMyTimeEntriesCheckBox->isChecked());
  settings.sync();
}

void SettingsWindow::onUpdateButtonClicked() { close(); }
