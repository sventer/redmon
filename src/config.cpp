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

#include "config.h"

#include <QSettings>

// static
Config& Config::Get() {
  static Config config;
  return config;
}

void Config::load() {
  QSettings settings;

  m_mainWindowPos = settings.value("mainWindowPos").toPoint();
  m_mainWindowSize = settings.value("mainWindowSize").toSize();
  m_issueListUpdateInterval = settings.value("issueListUpdateInterval").toInt();
}

void Config::save() {
  QSettings settings;

  settings.setValue("mainWindowPos", m_mainWindowPos);
  settings.setValue("mainWindowSize", m_mainWindowSize);
  settings.setValue("issueListUpdateInterval", m_issueListUpdateInterval);

  settings.sync();
}

void Config::setMainWindowPos(const QPoint& mainWindowPos) {
  m_mainWindowPos = mainWindowPos;
}

void Config::setMainWindowSize(const QSize& mainWindowSize) {
  m_mainWindowSize = mainWindowSize;
}

void Config::setIssueListUpdateInterval(int issueListupdateInterval) {
  m_issueListUpdateInterval = issueListupdateInterval;
}

Config::Config() : m_issueListUpdateInterval(0) {}

Config::~Config() {}
