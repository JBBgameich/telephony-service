/*
 * Copyright (C) 2012-2016 Canonical, Ltd.
 *
 * Authors:
 *  Ugo Riboni <ugo.riboni@canonical.com>
 *  Tiago Salem Herrmann <tiago.herrmann@canonical.com>
 *  Gustavo Pichorim Boiko <gustavo.boiko@canonical.com>
 *
 * This file is part of telephony-service.
 *
 * telephony-service is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * telephony-service is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HANDLERDBUS_H
#define HANDLERDBUS_H

#include <QtCore/QObject>
#include <QtDBus/QDBusContext>
#include "chatmanager.h"
#include "dbustypes.h"

/**
 * DBus interface for the phone handler
 */
class HandlerDBus : public QObject, protected QDBusContext
{
    Q_OBJECT
    Q_PROPERTY(bool CallIndicatorVisible
               READ callIndicatorVisible
               WRITE setCallIndicatorVisible
               NOTIFY CallIndicatorVisibleChanged)

public:
    HandlerDBus(QObject* parent=0);
    ~HandlerDBus();

    QVariantMap GetCallProperties(const QString &objectPath);
    bool HasCalls();
    QStringList AccountIds();
    bool IsReady();
    bool callIndicatorVisible() const;
    void setCallIndicatorVisible(bool visible);
    // configuration related
    ProtocolList GetProtocols();

    QString registerObject(QObject *object, const QString &path);
    void unregisterObject(const QString &path);

    static HandlerDBus *instance();

public Q_SLOTS:
    bool connectToBus();

    // messages related
    QString SendMessage(const QString &accountId, const QString &message, const AttachmentList &attachments, const QVariantMap &properties);
    Q_NOREPLY void AcknowledgeMessages(const QVariantList &messages);
    QString StartChat(const QString &accountId, const QVariantMap &properties);
    Q_NOREPLY void AcknowledgeAllMessages(const QVariantMap &properties);
    bool DestroyTextChannel(const QString &objectPath);
    bool ChangeRoomTitle(const QString &objectPath, const QString &title);
    Q_NOREPLY void InviteParticipants(const QString &objectPath, const QStringList &participants, const QString &message);
    Q_NOREPLY void RemoveParticipants(const QString &objectPath, const QStringList &participants, const QString &message);
    bool LeaveChat(const QString &objectPath, const QString &message);

    // call related
    Q_NOREPLY void StartCall(const QString &number, const QString &accountId);
    Q_NOREPLY void HangUpCall(const QString &objectPath);
    Q_NOREPLY void SetHold(const QString &objectPath, bool hold);
    Q_NOREPLY void SetMuted(const QString &objectPath, bool muted);
    Q_NOREPLY void SetActiveAudioOutput(const QString &objectPath, const QString &id);
    Q_NOREPLY void SendDTMF(const QString &objectPath, const QString &key);

    // conference call related
    Q_NOREPLY void CreateConferenceCall(const QStringList &objectPaths);
    Q_NOREPLY void MergeCall(const QString &conferenceObjectPath, const QString &callObjectPath);
    Q_NOREPLY void SplitCall(const QString &objectPath);

Q_SIGNALS:
    void onMessageSent(const QString &number, const QString &message);
    void CallPropertiesChanged(const QString &objectPath, const QVariantMap &properties);
    void CallIndicatorVisibleChanged(bool visible);
    void ConferenceCallRequestFinished(bool succeeded);
    void CallHoldingFailed(const QString &objectPath);
    void ProtocolsChanged(const ProtocolList &protocols);

private:
    bool mCallIndicatorVisible;
};

#endif // HANDLERDBUS_H
