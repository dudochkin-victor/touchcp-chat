/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef TPACCTLISTWIDGET_H
#define TPACCTLISTWIDGET_H

#include <DcpWidget>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MButton>
#include <MList>

#include "tpaccteditpage.h"

namespace MeeGoChat {
    class AccountModel;
    class AccountManager;
}

using namespace MeeGoChat;

class TpAcctListWidget : public DcpWidget
{
Q_OBJECT
public:
    TpAcctListWidget(QGraphicsWidget *parent = 0);
    bool pagePans() const { return true; }
    bool back();

private slots:
    void onAccountsReady();
    void onNewAccountClicked();
    void onAccountListItemClicked(const QModelIndex &);
    void onVisibleChanged();
    void onNoAccounts();

private:
    AccountManager *mAccountMgr;
    AccountModel *mAccountModel;
    TpAcctEditPage *mAccountEditPage;
    MList *mAccountList;
    MLinearLayoutPolicy *mPolicy;
    MLayout *mLayout;
    MAction *mActNew;

};

#endif // TPACCTLISTWIDGET_H
