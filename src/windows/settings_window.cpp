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

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

SettingsWindow::SettingsWindow(QWidget* parent) : QDialog(parent) {

  QLineEdit* serverUrlEdit = new QLineEdit;

  QLineEdit* apiKeyEdit = new QLineEdit;

  // Set up the layout.
  QGridLayout* mainLayout = new QGridLayout;
  mainLayout->addWidget(new QLabel("Server URL"), 0, 0);
  mainLayout->addWidget(serverUrlEdit, 1, 0);

  mainLayout->addWidget(new QLabel("API Key"), 0, 1);
  mainLayout->addWidget(apiKeyEdit, 1, 1);

  setLayout(mainLayout);
}

SettingsWindow::~SettingsWindow() {}