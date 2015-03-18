#include "FriendsTree.h"
#include "user.h"

// QXmpp
#include <QXmpp/QXmppRosterManager.h>
#include <QXmpp/QXmppUtils.h>
#include <QXmpp/QXmppVCardManager.h>
#include <QXmpp/QXmppVCardIq.h>
#include <QXmpp/QXmppPresence.h>

// Qt
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>

FriendsTree::FriendsTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setRootIsDecorated(false);
}

void FriendsTree::setClient(QXmppClient *client)
{
    m_client = client;
    m_rosterManager = &client->rosterManager();

    connect(m_rosterManager, SIGNAL(rosterReceived()),
            this, SLOT(parseRoster()) );
}

void FriendsTree::parseRoster()
{
    initGroup();

    foreach (QString bareJid, m_rosterManager->getRosterBareJids())
    {
        QXmppRosterIq::Item entry = m_rosterManager->getRosterEntry(bareJid);
        if (entry.groups().isEmpty())
        {
            insertItem(m_groups.at(0), &entry);
        }
        else
        {
            foreach (QString groupName, entry.groups())
            {
                int pos = 0;
                QTreeWidgetItem* group = NULL;
                if( m_groupsTitle.contains(groupName) )
                {
                    pos = m_groupsTitle.indexOf(groupName);
                    group = m_groups.at(pos);
                }
                else
                    group = createGroup(groupName);

                insertItem(group, &entry);
            }
        }
    }
}

void FriendsTree::mouseDoubleClickEvent(QMouseEvent *event)
{
    if( event->type() == QMouseEvent::MouseButtonDblClick
            && event->button() == Qt::RightButton)
    {
        return;
    }

    QTreeWidgetItem* item = this->itemAt( event->pos() );
    if( item && item->child(0) )
    {
        if( item->isExpanded() )
            item->setIcon(0, QIcon(":/icon/branch-closed.png"));
        else
            item->setIcon(0, QIcon(":/icon/branch-open.png"));
    }
    QTreeWidget::mouseDoubleClickEvent(event);
}

void FriendsTree::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu* popMenu = new QMenu(this);
    QAction *chatAction = popMenu->addAction( tr("&Chat") );
    QAction *mailAction = popMenu->addAction( tr("&Email") );
    QAction *detailAction = popMenu->addAction( tr("&Detail") );
    QAction *historyAction = popMenu->addAction( tr("&History") );

    popMenu->exec(event->globalPos());
    event->accept();
}

void FriendsTree::initGroup()
{
    QTreeWidgetItem* genItem = new QTreeWidgetItem(this);
    genItem->setText(0, tr("General Friends"));
    genItem->setExpanded(false);
    genItem->setIcon(0, QIcon(":/icon/branch-closed.png"));
    m_groups.append(genItem);
    m_groupsTitle.append(tr("General Friends"));
}

QTreeWidgetItem *FriendsTree::createGroup(const QString &name)
{
    QTreeWidgetItem* group = new QTreeWidgetItem(this);
    group->setText(0, name);
    group->setExpanded(false);
    group->setIcon(0, QIcon(":/icon/branch-closed.png"));
    m_groups.append(group);
    m_groupsTitle.append(name);

    return group;
}

void FriendsTree::insertItem(QTreeWidgetItem *group, QXmppRosterIq::Item *entry)
{
    QTreeWidgetItem* userItem = new QTreeWidgetItem(group);
    User* user = new User(group, userItem, this);
    this->setItemWidget(userItem, 0, user);
    user->setUserName( entry->name() );
    user->setSignature( entry->bareJid() );
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            user, SLOT(doubleClicked(QTreeWidgetItem*,int)) );
    m_children.append(userItem);
}
