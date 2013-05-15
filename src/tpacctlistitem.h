/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef TPACCTLISTITEM_H
#define TPACCTLISTITEM_H

#include <QPixmap>
#include <QGraphicsSceneMouseEvent>

#include <MStylableWidget>
#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MImageWidget>
#include <MCancelEvent>

#include "tpacctlistitemstyle.h"

namespace MeeGoChat {
    class Acct;
}

using namespace MeeGoChat;

class TpAcctListItem : public MStylableWidget
{
    Q_OBJECT
public:
    TpAcctListItem(MWidget *parent = 0);
    void setName(QString name);
    void setAcct(Acct *acct);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
    void cancelEvent(MCancelEvent*);

signals:
    void clicked();

private slots:
    void delAccountClicked();

private:
    MLabel *mAcctName;
    Acct *mAcct;
    bool mPressed;
    M_STYLABLE_WIDGET(TpAcctListItemStyle);
};

#endif // TPACCTLISTITEM_H
