/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef CHATBRIEF_H
#define CHATBRIEF_H

#include <DcpBrief>

class MeeGoChatBrief : public DcpBrief
{
  Q_OBJECT;
 public:
  MeeGoChatBrief();
  virtual ~MeeGoChatBrief();
  QString titleText() const;
  int widgetTypeID() const;
};


#endif //CHATBRIEF_H
