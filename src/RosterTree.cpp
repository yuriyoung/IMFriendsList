#include "RosterTree.h"
#include "RosterWidgetItem.h"

// QXmpp
#include <QXmpp/QXmppRosterManager.h>
#include <QXmpp/QXmppVCardManager.h>
#include <QXmpp/QXmppPresence.h>
#include <QXmpp/QXmppUtils.h>

// Qt
#include <QMouseEvent>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QDebug>

RosterTree::RosterTree(QWidget *parent) :
    QTreeWidget(parent)
{
    this->setRootIsDecorated(false);
}

void RosterTree::setClient(QXmppClient *client)
{
    m_client = client;
    m_rosterManager = &client->rosterManager();
    m_vCardManager = &client->vCardManager();

    connect(m_rosterManager, SIGNAL(rosterReceived()),
            this, SLOT(parseRoster()) );
    connect(m_rosterManager, SIGNAL(presenceChanged(const QString, const QString)),
            this, SLOT(presenceChanged(const QString, const QString)));
    connect(m_vCardManager, SIGNAL(vCardReceived(const QXmppVCardIq&)),
            this, SLOT(vCardRecived(const QXmppVCardIq&)) );
}

RosterWidgetItem *RosterTree::rosterItemFromJid(const QString &bareJid)
{
    if( m_jidRosterItemMap.contains(bareJid) )
        return m_jidRosterItemMap[bareJid];
    else
        return 0;
}

void RosterTree::parseRoster()
{
//    initGroup();

    foreach (QString bareJid, m_rosterManager->getRosterBareJids())
    {
        QXmppRosterIq::Item entry = m_rosterManager->getRosterEntry(bareJid);
        if (entry.groups().isEmpty())
        {
            insertItem(createGroup(tr("General")), &entry);
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
                    group = m_groupItems.at(pos);
                }
                else
                    group = createGroup(groupName);

                insertItem(group, &entry);
            }
        }
    }

    emit parseRosterDone(m_rosterManager);
}

void RosterTree::vCardRecived(const QXmppVCardIq &vCard)
{
    qDebug() <<"vCardRecived";
    emit vCardRecivedDone(vCard);
}

void RosterTree::presenceChanged(const QString &bareJid, const QString &resource)
{
//    qDebug("example_2_rosterHandling:: Presence changed %s/%s", \
//           qPrintable(bareJid), qPrintable(resource));

    if(bareJid == m_client->configuration().jidBare())
        return;
    RosterWidgetItem* rosterItem = rosterItemFromJid(bareJid);
    if( !rosterItem )
        return;

    QMap<QString, QXmppPresence> presences = m_client->rosterManager().
                                             getAllPresencesForBareJid(bareJid);
    if( presences.isEmpty() )
        rosterItem->setPresence(QXmppPresence(QXmppPresence::Unavailable));
    else
        rosterItem->setPresence( *presences.begin() );
}

void RosterTree::mouseDoubleClickEvent(QMouseEvent *event)
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

void RosterTree::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu* popMenu = new QMenu(this);
    QAction *chatAction = popMenu->addAction( tr("&Chat") );
    QAction *mailAction = popMenu->addAction( tr("&Email") );
    QAction *detailAction = popMenu->addAction( tr("&Detail") );
    QAction *historyAction = popMenu->addAction( tr("&History") );

    popMenu->exec(event->globalPos());
    event->accept();
}

void RosterTree::initGroup()
{
    QTreeWidgetItem* genItem = new QTreeWidgetItem(this);
    genItem->setText(0, tr("General Friends"));
    genItem->setExpanded(false);
    genItem->setIcon(0, QIcon(":/icon/branch-closed.png"));
    m_groupItems.append(genItem);
    m_groupsTitle.append(tr("General Friends"));
}

QTreeWidgetItem *RosterTree::createGroup(const QString &name)
{
    QTreeWidgetItem* group = new QTreeWidgetItem(this);
    group->setText(0, name);
    group->setExpanded(false);
    group->setIcon(0, QIcon(":/icon/branch-closed.png"));
    m_groupItems.append(group);
    m_groupsTitle.append(name);

    return group;
}

void RosterTree::insertItem(QTreeWidgetItem *group, QXmppRosterIq::Item *entry)
{
    QTreeWidgetItem* userItem = new QTreeWidgetItem(group);
    RosterWidgetItem* user = new RosterWidgetItem(group, userItem, this);
    this->setItemWidget(userItem, 0, user);
    user->setName( entry->name() );
    user->setBareJid(entry->bareJid());
    user->setSignature( entry->bareJid() );
    user->setQXmppClient(m_client);
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            user, SLOT(doubleClicked(QTreeWidgetItem*,int)) );
    m_childItems.append(userItem);
    m_jidRosterItemMap.insert(entry->bareJid(), user);
}
