/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *  Gustavo Pichorim Boiko <gustavo.boiko@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "chatmanager.h"
#include "contactmodel.h"
#include "telepathyhelper.h"

#include <TelepathyQt/ContactManager>
#include <TelepathyQt/PendingContacts>

ChatManager::ChatManager(QObject *parent)
: QObject(parent)
{
}

ChatManager *ChatManager::instance()
{
    static ChatManager *manager = new ChatManager();
    return manager;
}

bool ChatManager::isChattingToContact(const QString &phoneNumber)
{
    return !existingChat(phoneNumber).isNull();
}

void ChatManager::startChat(const QString &phoneNumber)
{
    if (!isChattingToContact(phoneNumber)) {
        // Request the contact to start chatting to
        Tp::AccountPtr account = TelepathyHelper::instance()->account();
        connect(account->connection()->contactManager()->contactsForIdentifiers(QStringList() << phoneNumber),
                SIGNAL(finished(Tp::PendingOperation*)),
                SLOT(onContactsAvailable(Tp::PendingOperation*)));
    }
}

void ChatManager::endChat(const QString &phoneNumber)
{
    // if the chat we are ending was the current one, clear the property
    if (ContactModel::comparePhoneNumbers(mActiveChat, phoneNumber)) {
        setActiveChat("");
    }

    Tp::TextChannelPtr channel = existingChat(phoneNumber);
    if (channel.isNull()) {
        return;
    }

    // the phoneNumber might be formatted differently from the phone number used as the key
    // so use the one from the channel to remove the entries.
    QString id = channel->targetContact()->id();
    channel->requestClose();
    mChannels.remove(id);
    mContacts.remove(id);

    Q_EMIT unreadMessagesChanged(id);
}

void ChatManager::sendMessage(const QString &phoneNumber, const QString &message)
{
    Tp::TextChannelPtr channel = existingChat(phoneNumber);
    if (channel.isNull()) {
        return;
    }

    channel->send(message);
    Q_EMIT messageSent(phoneNumber, message);
}

void ChatManager::acknowledgeMessages(const QString &phoneNumber)
{
    Tp::TextChannelPtr channel = existingChat(phoneNumber);
    if (channel.isNull()) {
        return;
    }

    channel->acknowledge(channel->messageQueue());
}

QString ChatManager::activeChat() const
{
    return mActiveChat;
}

void ChatManager::setActiveChat(const QString &value)
{
    if (value != mActiveChat) {
        mActiveChat = value;
        acknowledgeMessages(mActiveChat);
        Q_EMIT activeChatChanged();
    }
}

int ChatManager::unreadMessagesCount() const
{
    int count = 0;
    Q_FOREACH(const Tp::TextChannelPtr &channel, mChannels.values()) {
        count += channel->messageQueue().count();
    }

    return count;
}

int ChatManager::unreadMessages(const QString &phoneNumber)
{
    Tp::TextChannelPtr channel = existingChat(phoneNumber);
    if (channel.isNull()) {
        return 0;
    }

    return channel->messageQueue().count();
}

void ChatManager::onTextChannelAvailable(Tp::TextChannelPtr channel)
{
    QString id = channel->targetContact()->id();
    mChannels[id] = channel;
    if (ContactModel::comparePhoneNumbers(id, mActiveChat)) {
        acknowledgeMessages(id);
    }

    connect(channel.data(),
            SIGNAL(pendingMessageRemoved(const Tp::ReceivedMessage&)),
            SLOT(onPendingMessageRemoved(const Tp::ReceivedMessage&)));

    Q_EMIT chatReady(channel->targetContact()->id());
    Q_EMIT unreadMessagesChanged(channel->targetContact()->id());
}

void ChatManager::onMessageReceived(const Tp::ReceivedMessage &message)
{
    Q_EMIT messageReceived(message.sender()->id(), message.text(), message.received(), message.messageToken());

    // if the message belongs to an active conversation, mark it as read
    if (ContactModel::comparePhoneNumbers(message.sender()->id(), mActiveChat)) {
        acknowledgeMessages(mActiveChat);
    }

    Q_EMIT unreadMessagesChanged(message.sender()->id());;
}

void ChatManager::onPendingMessageRemoved(const Tp::ReceivedMessage &message)
{
    // emit the signal saying the unread messages for a specific number has changed
    Q_EMIT unreadMessagesChanged(message.sender()->id());
}

Tp::TextChannelPtr ChatManager::existingChat(const QString &phoneNumber)
{
    Tp::TextChannelPtr channel;
    Q_FOREACH(const QString &key, mChannels.keys()) {
        if (ContactModel::comparePhoneNumbers(key, phoneNumber)) {
            channel = mChannels[key];
            break;
        }
    }

    return channel;
}

void ChatManager::onContactsAvailable(Tp::PendingOperation *op)
{
    Tp::PendingContacts *pc = qobject_cast<Tp::PendingContacts*>(op);

    if (!pc) {
        qCritical() << "The pending object is not a Tp::PendingContacts";
        return;
    }

    Tp::AccountPtr account = TelepathyHelper::instance()->account();

    // start chatting to the contacts
    Q_FOREACH(Tp::ContactPtr contact, pc->contacts()) {
        account->ensureTextChat(contact, QDateTime::currentDateTime(), "org.freedesktop.Telepathy.Client.TelephonyApp");

        // hold the ContactPtr to make sure its refcounting stays bigger than 0
        mContacts[contact->id()] = contact;
    }
}
