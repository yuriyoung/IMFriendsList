#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>

class QSettings;

class Preferences
{
public:
    Preferences();
    ~Preferences();

    void reset();
    void load();
    void save();

    // Account
    QString username;
    QString jid;
    QString password;
    QString domain;
    QString host;
    int     port;
    bool    storePassword;

private:
    QSettings*   m_settings;
};

#endif // PREFERENCES_H
