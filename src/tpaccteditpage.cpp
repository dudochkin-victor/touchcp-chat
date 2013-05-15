/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "tpaccteditpage.h"
#include "tpacctsetupwidget.h"

#include <QtDebug>
#include <QtCore>

#include <MWidgetCreator>

#include <libmeegochat/meegochataccount.h>

#include <TelepathyQt4/Types>
#include <TelepathyQt4/Constants>
#include <TelepathyQt4/Account>
#include <TelepathyQt4/AccountManager>
#include <TelepathyQt4/PendingAccount>
#include <TelepathyQt4/PendingOperation>
#include <TelepathyQt4/PendingStringList>

M_REGISTER_WIDGET(TpAcctEditPage);

TpAcctEditPage::TpAcctEditPage(Acct *acct) :
        MApplicationPage(),
        acctSetupWidget(0)
{

    connect(Tp::ConnectionManager::listNames(),
            SIGNAL(finished(Tp::PendingOperation*)),
            this,
            SLOT(onListNamesFinished(Tp::PendingOperation *)));
    setEscapeMode(MApplicationPageModel::EscapeManualBack);
    connect(this, SIGNAL(backButtonClicked()),
            this, SLOT(dismiss()));

    this->setPannable(false);

    layout = new MLayout;
    policy =
            new MLinearLayoutPolicy(layout, Qt::Vertical);

    acctEditViewport = new MPannableViewport();
    acctEditViewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    acctEditViewport->setObjectName("acctEditViewport");

    acctEditDSW = new MStylableWidget();
    acctEditDSW->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    dswLayout = new MLayout;
    dswPolicy = new MLinearLayoutPolicy(dswLayout, Qt::Vertical);

    lblHeader = new MLabel;
    lblHeader->setObjectName("lblAcctEditHeader");

    cboAcctKind = new MComboBox();
    cboAcctKind->setObjectName("cboAcctEditKind");
    //% "What kind of chat account do you have?"
    cboAcctKind->setTitle(qtTrId("chat_account_type_combo_title"));
    cboAcctKind->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //TODO-UI-LT: Following items waiting on new multi-account UI design input
    //TODO-UI-LT: Create AccountAbstractDetails class, inherit to classes for each proto...?
    /*TODO-UI-LT: Maybe dynamically load .ui files from FS, so that we
        can add new proto support on-the-fly w/ no mchat code changes?
     */

    lblAddMore = new MLabel;
    lblAddMore->setObjectName("lblAcctEditAddMore");
    lblAddMore->setAlignment(Qt::AlignLeft | Qt::AlignBottom);

    btnMore = new MButton;
    btnMore->setObjectName("btnAcctEditAddMore");

    btnDone = new MButton;
    btnDone->setObjectName("btnAcctEditDone");

    //dswSetupWidget = TpProtoSetupFactory::CreateSetupWidgetForType("Google Talk", Tp::ProtocolParameterList(), QVariantMap());
    //dswSetupWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    dswPolicy->addItem(lblHeader);
    dswPolicy->addItem(cboAcctKind, Qt::AlignLeft);
    //dswPolicy->addItem(dswSetupWidget);
    dswPolicy->addItem(lblAddMore);
    dswPolicy->addItem(btnMore);
    dswPolicy->addItem(btnDone);

    dswLayout->setPolicy(dswPolicy);

    acctEditDSW->setLayout(dswLayout);
    acctEditViewport->setWidget(acctEditDSW);

    policy->addItem(acctEditViewport);

    connect(btnMore,
            SIGNAL(clicked()),
            SLOT(btnMoreClicked()));
    connect(btnDone,
            SIGNAL(clicked()),
            SLOT(btnDoneClicked()));
    connect(cboAcctKind,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onCBOAcctKindIndexChanged(int)));

    connect(this,
            SIGNAL(appeared()),
            this,
            SLOT(resizeViewport()));


    layout->setPolicy(policy);
    centralWidget()->setLayout(layout);
    setAccount(acct);

}

void TpAcctEditPage::setAccount(Acct *acct)
{
    cboAcctKind->setCurrentIndex(-1);
    if (!acct)
    {
        mEdit = false;
        //% "Create IM Account"
        setTitle(qtTrId("page_title_create_account"));
        //% "Add more IM accounts?"
        lblAddMore->setText(qtTrId("label_add_more_im_accounts"));
        //% "Yes, add more accounts now"
        btnMore->setText(qtTrId("button_add_more_accounts"));
        //% "No, I'm done"
        btnDone->setText(qtTrId("button_done"));
    } else
    {
        int i;
        QString protoType;

        mEdit = true;
        //% "Edit IM Account"
        setTitle(qtTrId("page_title_edit_account"));
        //% "Done?"
        lblAddMore->setText(qtTrId("label_done?"));
        mAcct = acct;
        protoType = TpAcctSetupWidget::MapTpProtoToProtoType(mAcct->getTpProto(), acct->getIcon());

        for (i = 0; i < cboAcctKind->count(); ++i)
        {
            if (cboAcctKind->itemText(i) == protoType)
            {
                cboAcctKind->setCurrentIndex(i);
                break;
            }
        }
        //% "Save Changes"
        btnMore->setText(qtTrId("button_save_changes"));
        //% "Cancel"
        btnDone->setText(qtTrId("button_cancel"));
    }

    //% "Edit your account details"
    lblHeader->setText((mEdit ? qtTrId("label_edit_account_details")
    //% "Enter your account details"
        : qtTrId("label_enter_account_details")));
    acctEditViewport->setPosition(QPointF(0, 0));
    resizeViewport();
}

void TpAcctEditPage::onListNamesFinished(Tp::PendingOperation *po)
{
    if (po->isError())
    {
        //TODO - handle gracefully...
    } else
    {
        QStringList cmTypes = dynamic_cast<Tp::PendingStringList *>(po)->result();
        foreach (const QString &type, cmTypes)
        {
            Tp::ConnectionManagerPtr cmPtr = Tp::ConnectionManager::create(type);
            Tp::PendingReady *pr = cmPtr->becomeReady(Tp::Features() << Tp::ConnectionManager::FeatureCore);
            connect(pr,
                    SIGNAL(finished(Tp::PendingOperation*)),
                    this,
                    SLOT(onCMReady(Tp::PendingOperation*)));
            cmList.append(cmPtr);
        }
    }
}

void TpAcctEditPage::onCMReady(Tp::PendingOperation *po)
{
    if (po->isError())
    {
        //TODO - handle error here...
    } else
    {
        Tp::PendingReady *pr = qobject_cast<Tp::PendingReady *>(po);
        Tp::ConnectionManagerPtr cmPtr =
                Tp::ConnectionManagerPtr(dynamic_cast<Tp::ConnectionManager *>(pr->object()));
        foreach(Tp::ProtocolInfo *pi, cmPtr->protocols())
        {
            //Possible for multiple CMs to offer the same proto...
            //Not supporting IRC for first release...
            if (!piList.contains(pi->name()) && (pi->name() != "irc"))
            {
                //A single supported proto might map to multiple
                //"Account Types", such as jabber -> Google Talk and Facebook
                mAcctTypeList.append(
                        TpAcctSetupWidget::MapTpProtoToProtoTypes(pi->name()));
                piList.insert(pi->name(), pi);
#if 0
                //Shows supported params for each proto...
                qDebug() << "proto " << pi->name() << " params:";
                foreach (Tp::ProtocolParameter *pp, pi->parameters())
                {
                    qDebug() << "\t" << pp->name() << ":";
                    qDebug() << "\t\tType: " << pp->type();
                    qDebug() << "\t\tRequired: " << pp->isRequired();
                    qDebug() << "\t\tSecret: " << pp->isSecret();
                    qDebug() << "\t\tDefault Value: " << pp->defaultValue();
                    qDebug() << "\t\tReqd4Reg: " << pp->isRequiredForRegistration();
                }
#endif
            }
        }
        mAcctTypeList.sort();
        QString curSel = cboAcctKind->currentText();
        cboAcctKind->clear();
        cboAcctKind->addItems(mAcctTypeList);
        if (!curSel.isEmpty())
            cboAcctKind->setCurrentIndex(mAcctTypeList.indexOf(curSel));
    }
}

void TpAcctEditPage::btnMoreClicked()
{
    if (mEdit)
    {
        saveAccount();
        this->dismiss();
    } else if (cboAcctKind->currentIndex() != -1)
    {
        createAccount();
        this->setAccount();
    }
}

void TpAcctEditPage::btnDoneClicked()
{
    if (mEdit)
    {
        this->setAccount();
    } else if (cboAcctKind->currentIndex() != -1)
    {
        createAccount();
    }
    this->dismiss();
}

void TpAcctEditPage::onCBOAcctKindIndexChanged(int curIdx)
{
    if (acctSetupWidget)
    {
        dswLayout->animatedDeleteAt(dswLayout->indexOf(acctSetupWidget));
        //layout->takeAt(layout->indexOf(dswSetupWidget));
        //delete dswSetupWidget;
        acctSetupWidget = 0;
    }
    if (curIdx != -1)
    {
        QString proto = cboAcctKind->itemText(curIdx);
        acctSetupWidget =
            new TpAcctSetupWidget(proto, piList.value(
                    TpAcctSetupWidget::MapProtoTypeToTpProto(proto)),
                        (mEdit ? mAcct->getTpParameters() : QVariantMap()),
                        (mEdit ? mAcct->getDisplayName() : QString()),
                        (mEdit ? mAcct->getEnabled() : true));
        acctSetupWidget->setSizePolicy(QSizePolicy::Expanding,
                                      QSizePolicy::Expanding);
        dswPolicy->insertItem(dswPolicy->indexOf(cboAcctKind)+1,
                              acctSetupWidget);
        //Need to resize the viewport when the widget size changes
        //i.e. when the Advanced button is clicked
        connect(acctSetupWidget,
                SIGNAL(geometryChanged()),
                this,
                SLOT(resizeViewport()));
    }
}

void TpAcctEditPage::createAccount()
{
    if (acctSetupWidget->validateParams())
        acctSetupWidget->createAccount();
}

void TpAcctEditPage::saveAccount()
{
    if (acctSetupWidget->validateParams())
        acctSetupWidget->updateAccount(mAcct);
}

void TpAcctEditPage::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    MApplicationPage::resizeEvent(event);
    resizeViewport();
}

void TpAcctEditPage::resizeViewport()
{
    qreal newHeight = this->size().height();
    newHeight -= centralWidget()->geometry().top();
    newHeight -= 15;//Not sure why this is needed, needs more investigation...
    acctEditViewport->setMinimumHeight(newHeight);
    acctEditViewport->setMaximumHeight(newHeight);
    acctEditViewport->setPreferredHeight(newHeight);
}
