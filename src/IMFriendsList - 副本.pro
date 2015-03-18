#-------------------------------------------------
#
# Project created by QtCreator 2013-08-15T15:52:45
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IMFriendsList
TEMPLATE = app

TEMPLATE = subdirs
SUBDIRS = qxmpp \

SOURCES += main.cpp\
    MainWindow.cpp \
    UserChatDialog.cpp \
    Preferences.cpp \
    LoginWidget.cpp \
    RosterTree.cpp \
    RosterWidgetItem.cpp

HEADERS  += \
    MainWindow.h \
    UserChatDialog.h \
    Preferences.h \
    LoginWidget.h \
    RosterTree.h \
    RosterWidgetItem.h

FORMS    += \
    MainWindow.ui \
    UserChatDialog.ui \
    LoginWidget.ui \
    RosterWidgetItem.ui

RESOURCES += \
    icon.qrc

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../Qt/4.8.0/lib/ -lqxmpp0
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../Qt/4.8.0/lib/ -lqxmppd0


