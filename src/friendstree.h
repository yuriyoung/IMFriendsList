#ifndef FRIENDSTREE_H
#define FRIENDSTREE_H

#include <QTreeWidget>
#include <QXmpp/QXmppClient.h>
#include <QXmpp/QXmppRosterIq.h>

class QMouseEvent;
class QContextMenuEvent;
class QXmppRosterManager;

class FriendsTree : public QTreeWidget
{
    Q_OBJECT
public:
    explicit FriendsTree(QWidget *parent = 0);
    void setClient(QXmppClient* client);

signals:
    
public slots:
    void parseRoster();

protected:
    void mouseDoubleClickEvent( QMouseEvent *event );
    void contextMenuEvent(QContextMenuEvent *event);
private:
    void             initGroup();
    QTreeWidgetItem *createGroup(const QString &name);
    void             insertItem(QTreeWidgetItem* group, QXmppRosterIq::Item *);

    QXmppClient*                m_client;
    QXmppRosterManager*         m_rosterManager;
    QList<QTreeWidgetItem*>     m_groups;
    QList<QString>              m_groupsTitle;
    QList<QTreeWidgetItem*>     m_children;

};

#endif // FRIENDSTREE_H
