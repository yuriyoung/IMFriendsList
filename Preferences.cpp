#include "Preferences.h"

// Qt
#include <QSettings>
#include <QDir>
#include <QDebug>

Preferences::Preferences()
{

    QString fileName = QDir::currentPath() + "/IMFriendsList.ini";
    m_settings = new QSettings(fileName, QSettings::IniFormat);

    reset();
}

Preferences::~Preferences()
{
    delete m_settings;
}

void Preferences::reset()
{
    domain = QString("@yvi-pc");
    host = QString("yvi-pc");
    port = 5222;
    username = QString("");
    jid = username + domain;
    password = QString("123456");
    storePassword = true;
}
void Preferences::load()
{
    // Account
    m_settings->beginGroup("account");

    domain = m_settings->value("domain").toString();
    host = m_settings->value("host").toString();
    port = m_settings->value("port", 5222).toInt();
    username = m_settings->value("username").toString();
    jid = m_settings->value("jid").toString();
    storePassword = m_settings->value("storePassword", false).toBool();
    if (storePassword)
        password = m_settings->value("password").toString();
    else
        password.clear();

    m_settings->endGroup();
}

void Preferences::save()
{
    qDebug() << "Saving preferences...";

    // Account
    m_settings->beginGroup("account");

    m_settings->setValue("domain", domain);
    m_settings->setValue("host", host);
    m_settings->setValue("port", port);
    m_settings->setValue("username", username);
    m_settings->setValue("jid", jid);
    if (storePassword)
        m_settings->setValue("password", password);
    else
        m_settings->remove("password");
    m_settings->setValue("storePassword", storePassword);

    m_settings->endGroup();
}
