/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "tpacctlistitem.h"
#include <MAction>
#include <MWidgetCreator>

#include <libmeegochat/meegochataccount.h>
#include <libmeegochat/meegochataccountmanager.h>


M_REGISTER_WIDGET(TpAcctListItem);

TpAcctListItem::TpAcctListItem(MWidget *parent) :
        MStylableWidget(parent),
        mPressed(false)
{
    setObjectName("tpAcctListItem");
    MLayout *hLayout = new MLayout();
    MLinearLayoutPolicy *hPolicy = new MLinearLayoutPolicy(hLayout,
                                                              Qt::Horizontal);
    //% "Delete Account"
    MAction *actDel = new MAction(qtTrId("action_delete_account"), this);
    connect(actDel, SIGNAL(triggered()),
            this, SLOT(delAccountClicked()));
    addAction(actDel);

    mAcctName = new MLabel("");
    mAcctName->setTextElide(true);
    mAcctName->setObjectName("tpAcctListItemName");
    mAcctName->setSizePolicy(QSizePolicy::Expanding,
                                QSizePolicy::Expanding);
    hPolicy->addItem(mAcctName, Qt::AlignLeft);
    hLayout->setPolicy(hPolicy);

    setLayout(hLayout);

}

void TpAcctListItem::setName(QString name)
{
    mAcctName->setText(name);
}

void TpAcctListItem::setAcct(Acct *acct)
{
    mAcct = acct;
    if (mAcct)
        setName(mAcct->getDisplayName());
}

void TpAcctListItem::delAccountClicked()
{
    if (!mAcct)
        return;
    AccountManager::getInstance()->removeAccount(mAcct);
    mAcct = 0;
}

void TpAcctListItem::mousePressEvent(QGraphicsSceneMouseEvent *ev)
{
    mPressed = true;
    style().setModePressed();
    update();
    ev->accept();
}

void TpAcctListItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev)
{
    if (mPressed) {
        mPressed = false;
        style().setModeDefault();
        update();
        ev->accept();
        emit clicked();
    } else {
        ev->ignore();
    }
}

void TpAcctListItem::cancelEvent(MCancelEvent *ev)
{
    mPressed = false;
    style().setModeDefault();
    update();
    ev->accept();
}
