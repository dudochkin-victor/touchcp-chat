/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "tpacctlistitemcreator.h"

#include <libmeegochat/meegochataccount.h>

using namespace MeeGoChat;

void TpAcctListItemCreator::updateCell(const QModelIndex &index, MWidget *cell) const
{
    if (!index.isValid() || !cell)
        return;
    TpAcctListItem *acctListItem = qobject_cast<TpAcctListItem *>(cell);

    QVariant data = index.data(Qt::UserRole);
    Acct *acct = data.value<Acct *>();

    acctListItem->setAcct(acct);
}
