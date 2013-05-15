/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "tpacctlistwidget.h"
#include "tpacctlistitemcreator.h"

#include <libmeegochat/meegochataccountmanager.h>
#include <libmeegochat/meegochataccount.h>
#include <libmeegochat/meegochataccountmodel.h>

#include <MAction>
#include <MWidgetCreator>
#include <MApplicationPage>

TpAcctListWidget::TpAcctListWidget(QGraphicsWidget *parent) :
        DcpWidget(parent),
        mAccountMgr(AccountManager::getInstance()),
        mAccountModel(0),
        mAccountList(new MList()),
        mActNew(0)
{
    connect(mAccountMgr,
            SIGNAL(NoAccounts()),
            this,
            SLOT(onNoAccounts()));

    mLayout = new MLayout;
    mPolicy = new MLinearLayoutPolicy(mLayout, Qt::Vertical);

    mAccountModel = mAccountMgr->getAccountModel();
    mAccountList->setItemModel(mAccountModel);
    mAccountList->setViewType("fast");
    mAccountList->setCellCreator(new TpAcctListItemCreator());
    mAccountList->setSizePolicy(QSizePolicy::Expanding,
                               QSizePolicy::Expanding);

    connect(mAccountList,
            SIGNAL(itemClicked(QModelIndex)),
            this,
            SLOT(onAccountListItemClicked(const QModelIndex &)));

/*    if (mAccountMgr->areAccountsReady())
        onAccountsReady();
    connect(mAccountMgr,
            SIGNAL(AccountsReady()),
            this,
            SLOT(onAccountsReady()));
*/
    mPolicy->addItem(mAccountList);

    mAccountEditPage = new TpAcctEditPage();

    mLayout->setPolicy(mPolicy);
    setLayout(mLayout);
    connect(this,
            SIGNAL(visibleChanged()),
            this,
            SLOT(onVisibleChanged()));
}

bool TpAcctListWidget::back()
{
    return DcpWidget::back();
}

void TpAcctListWidget::onAccountsReady()
{
    mAccountModel = mAccountMgr->getAccountModel();
    mAccountList->setItemModel(mAccountModel);
}

void TpAcctListWidget::onAccountListItemClicked(const QModelIndex &index)
{
    Acct *acct =
            index.data(Qt::UserRole).value<Acct *>();
    mAccountEditPage->setAccount(acct);
    mAccountEditPage->appear(MSceneWindow::KeepWhenDone);
}

void TpAcctListWidget::onNewAccountClicked()
{
    mAccountEditPage->setAccount();
    mAccountEditPage->appear(MSceneWindow::KeepWhenDone);
}

void TpAcctListWidget::onVisibleChanged()
{
    if (mActNew)
        return;

    //Shamelessly borrowed from meegotouchcp-wifi - thanks Mike!
    MApplicationPage *page = NULL;

    QGraphicsItem *item = this->parentItem();
    while (item) {
        page = dynamic_cast<MApplicationPage *>(item);
        if (page) {
            //% "Add Account"
            mActNew = new MAction(qtTrId("action_add_account"), this);
            mActNew->setLocation(MAction::ToolBarLocation);
            connect(mActNew,
                    SIGNAL(triggered()),
                    this,
                    SLOT(onNewAccountClicked()));
            page->addAction(mActNew);
            break;
        }
        item = item->parentItem();
      }
}

void TpAcctListWidget::onNoAccounts()
{
    onNewAccountClicked();
}
