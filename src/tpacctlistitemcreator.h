/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef TPACCTLISTITEMCREATOR_H
#define TPACCTLISTITEMCREATOR_H

#include <MAbstractCellCreator>
#include <MWidget>

#include <TelepathyQt4/Types>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>


#include "tpacctlistitem.h"

class TpAcctListItemCreator : public MAbstractCellCreator<TpAcctListItem>
{
public:
    void updateCell(const QModelIndex &, MWidget *) const;
};


#endif // TPACCTLISTITEMCREATOR_H
