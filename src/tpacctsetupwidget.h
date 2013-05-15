/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#ifndef TPACCTSETUPWIDGET_H
#define TPACCTSETUPWIDGET_H

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVariantMap>

#include <MStylableWidget>
#include <MGridLayoutPolicy>
#include <MLayout>
#include <MButton>
#include <MLabel>
#include <MTextEdit>

//TODO - move ConnectionManager elements to libmeegotouch
#include <TelepathyQt/ConnectionManager>
/*#include <TelepathyQt/PendingAccount>
#include <TelepathyQt/PendingStringList>
#include <TelepathyQt/AccountManager>
*/

namespace MeeGoChat {
    class Acct;
    class AccountManager;
}

using namespace MeeGoChat;

class TpAcctSetupWidget : public MStylableWidget
{
    Q_OBJECT
public:
    TpAcctSetupWidget() {};
    TpAcctSetupWidget(QString protoType, Tp::ProtocolInfo *protoInfo,
            QVariantMap curParams = QVariantMap(), QString acctName = QString(""),
            bool acctEnabled = true);
    bool validateParams();
    void updateAccount(Acct *acct);
    void createAccount();

    static QStringList MapTpProtoToProtoTypes(QString tpProto);
    static QString MapProtoTypeToTpProto(QString protoType);
    static QString GetProtoTypeIcon(QString protoType);
    static QString MapTpProtoToProtoType(QString tpProto, QString tpIcon);

private slots:
    void onBtnAdvancedClicked();
    void onLinkActivated(QString);

private:
    QVariant safeParamGet(QString param);
    QPair<QVariantMap, QStringList>getParams();
    QString getDefaultDisplayName(QVariantMap params);
    void getParamsFromLayout(QGraphicsLayout *layout, QVariantMap *qvm);

    QString mCurProto;
    Tp::ProtocolInfo *mProtoInfo;
    QVariantMap mCurParams;
    QHash<QString, Tp::ProtocolParameter *> mProtoParams;

    MLayout *layout;
    MGridLayoutPolicy *policy;

    MStylableWidget *dswAdvanced;
    MButton *btnAdvanced;
    MButton *btnEnabled;
    MLabel *lblEditName;
    MTextEdit *edtEditName;

};

#endif // TPACCTSETUPWIDGET_H
