#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include "Preferences.h"

#include <QWidget>

class QTimer;

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();
    void showLoginState(const QString &str);
    void lockLogin();
    void unlockLogin();
    void readData(Preferences* pref);
    void writeData(Preferences* pref);

signals:
    void login();
    void cancelLogin();

private slots:
    void clickedLogin();
    void clickedCancel();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
