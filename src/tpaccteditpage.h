/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright � 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef TPACCTEDITPAGE_H
#define TPACCTEDITPAGE_H

#include "tpacctsetupwidget.h"

#include <QStringListModel>

#include <MApplicationPage>
#include <MPannableViewport>
#include <MComboBox>
#include <MLabel>
#include <MTextEdit>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MStylableWidget>
#include <MButton>

/*
#include <TelepathyQt/Types>
#include <TelepathyQt/Constants>
#include <TelepathyQt/Debug>
#include <TelepathyQt/AccountManager>
#include <TelepathyQt/Account>
#include <TelepathyQt/Connection>
#include <TelepathyQt/PendingAccount>
#include <TelepathyQt/PendingChannelRequest>
#include <TelepathyQt/PendingConnection>
#include <TelepathyQt/PendingContactAttributes>
#include <TelepathyQt/PendingContacts>*/

//TODO - move all Tp-Qt4 functionality into libmeegochat...
#include <TelepathyQt/PendingReady>
#include <TelepathyQt/ConnectionManager>
#include <TelepathyQt/PendingOperation>
//#include <TelepathyQt/PendingStringList>


namespace MeeGoChat {
    class Acct;
}

using namespace MeeGoChat;

class TpAcctEditPage : public MApplicationPage
{
    Q_OBJECT
public:
    TpAcctEditPage(Acct *acct = 0);
    void setAccount(Acct *acct = 0);

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);

private slots:
    void btnMoreClicked();
    void btnDoneClicked();
    void onListNamesFinished(Tp::PendingOperation *po);
    void onCMReady(Tp::PendingOperation*);
    void onCBOAcctKindIndexChanged(int curIdx);
    void resizeViewport();

private:
    void createAccount();
    void saveAccount();

    Acct *mAcct;
    QList<Tp::ConnectionManagerPtr> cmList;

    MPannableViewport *acctEditViewport;
    MStylableWidget *acctEditDSW;
    MLayout *dswLayout;
    MLinearLayoutPolicy *dswPolicy;
    MLabel *lblAddMore;
    MLinearLayoutPolicy *policy;
    MLayout *layout;
    MComboBox *cboAcctKind;
    MLabel *lblHeader;
    TpAcctSetupWidget *acctSetupWidget;
    MButton *btnMore;
    MButton *btnDone;

    //QHash<QString, QString> cmProtosTypes;
    QHash<QString, Tp::ProtocolInfo *> piList;
    QStringList mAcctTypeList;
    bool mEdit;
};

#endif // TPACCTEDITPAGE_H
