/*
    This file is part of Qonverter.

    Qonverter is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Qonverter is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Qonverter.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2012 - 2013 Martin Rotter
*/

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QPointer>
#include <QWebSettings>

#include "core/settings.h"
#include "core/defs.h"


QPointer<Settings> Settings::s_instance;

Settings::Settings(const QString &file_name, Format format, QObject *parent)
  : QSettings(file_name, format, parent) {
}

Settings::~Settings() {
  checkSettings();
  qDebug("Deleting Settings instance.");
}

QSettings::Status Settings::checkSettings() {
  qDebug("Syncing settings.");
  sync();
  return status();
}

Settings *Settings::getInstance() {
  if (s_instance.isNull()) {
    setupSettings();
  }

  return s_instance;
}

QVariant Settings::value(const QString &section,
                         const QString &key,
                         const QVariant &default_value) {
  return QSettings::value(QString("%1/%2").arg(section, key), default_value);
}

void Settings::setValue(const QString &section,
                        const QString &key,
                        const QVariant &value) {
  QSettings::setValue(QString("%1/%2").arg(section, key), value);
}

QSettings::Status Settings::setupSettings() {
  // If settings file exists in executable file working directory
  // (in subdirectory APP_CFG_PATH), then use it (portable settings).
  // Otherwise use settings file stored in homePath();
  QString relative_path = QDir::separator() + APP_CFG_PATH +
                          QDir::separator() + APP_CFG_FILE;

  QString app_path = qApp->applicationDirPath();
  QString app_path_file = app_path + relative_path;

  // Check if portable settings are available.
  if (QFile(app_path_file).exists()) {
    // Portable settings are available, use them.
    s_instance = new Settings(app_path_file, QSettings::IniFormat, qApp);

    // Construct icon cache in the same path.
    QString web_path = app_path + QDir::separator() + APP_CFG_WEB_PATH;
    QDir(web_path).mkpath(web_path);
    QWebSettings::setIconDatabasePath(web_path);

    qDebug("Initializing settings in %s (portable way).",
           qPrintable(QDir::toNativeSeparators(app_path)));
  }
  else {
    // Portable settings are NOT available, store them in
    // user's home directory.
    QString home_path = QDir::homePath() + QDir::separator() +
                        APP_LOW_H_NAME;
    QString home_path_file = home_path + relative_path;

    s_instance = new Settings(home_path_file, QSettings::IniFormat, qApp);

    QString web_path = home_path + QDir::separator() + APP_CFG_WEB_PATH;
    QDir(web_path).mkpath(web_path);
    QWebSettings::setIconDatabasePath(web_path);

    qDebug("Initializing settings in %s (non-portable way).",
           qPrintable(QDir::toNativeSeparators(home_path_file)));
  }

  return (*s_instance).checkSettings();
}
