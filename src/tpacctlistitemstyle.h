/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef TPACCTLISTITEMSTYLE_H
#define TPACCTLISTITEMSTYLE_H

#include <MWidgetStyle>
#include <MStyle>


class M_EXPORT TpAcctListItemStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(TpAcctListItemStyle)

};

class M_EXPORT TpAcctListItemStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(TpAcctListItemStyle)
    M_STYLE_MODE(Pressed)
};

#endif // TPACCTLISTITEMSTYLE_H
