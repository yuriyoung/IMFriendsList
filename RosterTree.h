#ifndef ROSTERTREE_H
#define ROSTERTREE_H

#include <QTreeWidget>
#include <QXmpp/QXmppClient.h>
#include <QXmpp/QXmppRosterIq.h>
#include <QXmpp/QXmppVCardIq.h>

class QMouseEvent;
class QContextMenuEvent;
class QXmppRosterManager;
class RosterWidgetItem;

class RosterTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit RosterTree(QWidget *parent = 0);
    void setClient(QXmppClient* client);
    RosterWidgetItem* rosterItemFromJid(const QString& bareJid);

signals:
    void parseRosterDone(QXmppRosterManager*);
    void vCardRecivedDone(const QXmppVCardIq&);

public slots:
    void parseRoster();
    void vCardRecived(const QXmppVCardIq&);
    void presenceChanged(const QString& bareJid, const QString& resource);

protected:
    void mouseDoubleClickEvent( QMouseEvent *event );
    void contextMenuEvent(QContextMenuEvent *event);
private:
    void             initGroup();
    QTreeWidgetItem *createGroup(const QString &name);
    void             insertItem(QTreeWidgetItem* group, QXmppRosterIq::Item *);

    QXmppClient*                m_client;
    QXmppRosterManager*         m_rosterManager;
    QXmppVCardManager*          m_vCardManager;
    QList<QTreeWidgetItem*>     m_groupItems;
    QList<QString>              m_groupsTitle;
    QList<QTreeWidgetItem*>     m_childItems;
    QMap<QString, RosterWidgetItem*> m_jidRosterItemMap;
};

#endif // ROSTERTREE_H
