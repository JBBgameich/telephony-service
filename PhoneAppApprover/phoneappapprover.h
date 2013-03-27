/*
 * Copyright (C) 2012 Canonical, Ltd.
 *
 * Authors:
 *  Tiago Salem Herrmann <tiago.herrmann@canonical.com>
 *
 * This file is part of phone-app.
 *
 * phone-app is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * phone-app is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PHONEAPPAPPROVER_H
#define PHONEAPPAPPROVER_H

#include <glib.h>
#include <unistd.h>
#include <libnotify/notify.h>

#include <QMap>
#include <TelepathyQt/AbstractClientApprover>
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/ChannelDispatchOperation>

class PhoneAppApprover : public QObject, public Tp::AbstractClientApprover
{
    Q_OBJECT

public:
    PhoneAppApprover();
    ~PhoneAppApprover();

    Tp::ChannelClassSpecList channelFilters() const;

    void addDispatchOperation(const Tp::MethodInvocationContextPtr<> &context,
                              const Tp::ChannelDispatchOperationPtr &dispatchOperation);
    Tp::ChannelDispatchOperationPtr dispatchOperation(Tp::PendingOperation *op);
    void onApproved(Tp::ChannelDispatchOperationPtr dispatchOp,
                    Tp::PendingReady *pr);
    void onRejected(Tp::ChannelDispatchOperationPtr dispatchOp,
                    Tp::ChannelPtr channel);

private Q_SLOTS:
    void processChannels();
    void onChannelReady(Tp::PendingOperation *op);
    void onClaimFinished(Tp::PendingOperation* op);
    void onHangupFinished(Tp::PendingOperation* op);
    void onCallStateChanged(Tp::CallState state);
    void onReplyReceived(const QString &phoneNumber, const QString &reply);
    void closeSnapDecision();

private:
    QList<Tp::ChannelDispatchOperationPtr> mDispatchOps;
    QMap<Tp::PendingOperation*,Tp::ChannelPtr> mChannels;
    NotifyNotification* mPendingSnapDecision;
    QMap<QString, QString> mPendingMessages;
};

#endif // PHONEAPPAPPROVER_H