// For license of this file, see <project-root-folder>/LICENSE.md.

#ifndef SETTINGSBROWSERMAIL_H
#define SETTINGSBROWSERMAIL_H

#include "gui/settings/settingspanel.h"

#include "miscellaneous/externaltool.h"
#include "ui_settingsbrowsermail.h"

class SettingsBrowserMail : public SettingsPanel {
  Q_OBJECT

  public:
    explicit SettingsBrowserMail(Settings* settings, QWidget* parent = 0);
    virtual ~SettingsBrowserMail();

    inline QString title() const {
      return tr("Web browser & e-mail & proxy");
    }

    void loadSettings();

    void saveSettings();

  private slots:
    void addExternalTool();
    void deleteSelectedExternalTool();
    void changeDefaultBrowserArguments(int index);
    void selectBrowserExecutable();
    void changeDefaultEmailArguments(int index);
    void selectEmailExecutable();
    void displayProxyPassword(int state);
    void onProxyTypeChanged(int index);

  private:
    QList<ExternalTool> externalTools() const;
    void setExternalTools(const QList<ExternalTool>& list);

    Ui::SettingsBrowserMail* m_ui;
};

#endif // SETTINGSBROWSERMAIL_H
