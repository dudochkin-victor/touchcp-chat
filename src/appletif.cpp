/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "appletif.h"
#include "tpacctlistwidget.h"
#include "chatbrief.h"
#include <MAction>
#include <MLibrary>
#include <MTheme>
#include <QDebug>


void MeeGoChatApplet::init()
{
  MTheme::loadCSS(QString(CSSDIR) + "libmeegochatapplet.css");
}

DcpWidget* MeeGoChatApplet::constructWidget(int widgetId)
{
  DcpWidget *widget = NULL;

  switch (widgetId) {
  case 0:
    widget = new TpAcctListWidget();
    break;
    
  default:
    qDebug() << QString("Unknown widgetid %1 in MeeGoChatApplet::constructWidget").arg(widgetId);
    break;
  }
  return widget;
}

QVector<MAction *> MeeGoChatApplet::viewMenuItems()
{
  //we don't intend any having menu items
  return QVector<MAction*>(); 
}

QString MeeGoChatApplet::title() const
{
  //% "Chat Accounts"
  return qtTrId("applet_title_chat_accounts");
}

DcpBrief* MeeGoChatApplet::constructBrief(int)
{
  return new MeeGoChatBrief();
}

Q_EXPORT_PLUGIN2(meegochatapplet, MeeGoChatApplet)
M_LIBRARY

