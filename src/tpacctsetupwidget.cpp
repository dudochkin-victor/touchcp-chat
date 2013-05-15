/*
 * meegotouchcp-chat - Telepathy Account management plugin for duicontrolpanel
 * Copyright © 2010, Intel Corporation.
 *
 * This program is licensed under the terms and conditions of the
 * Apache License, version 2.0.  The full text of the Apache License is at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 */

#include "tpacctsetupwidget.h"
#include "tpacctsettingsdefaultdefines.h"

#include <QDesktopServices>

#include <MLabel>
#include <MComboBox>
#include <MTextEdit>
#include <MImageWidget>
#include <MWidgetCreator>

#include <libmeegochat/meegochataccount.h>
#include <libmeegochat/meegochataccountmanager.h>

M_REGISTER_WIDGET(TpAcctSetupWidget);

TpAcctSetupWidget::TpAcctSetupWidget(QString protoType,
                                     Tp::ProtocolInfo *protoInfo,
                                     QVariantMap curParams,
                                     QString acctName,
                                     bool acctEnabled) :
    mCurProto(protoType),
    mProtoInfo(protoInfo),
    mCurParams(curParams),
    layout(new MLayout()),
    policy(new MGridLayoutPolicy(layout)),
    dswAdvanced(new MStylableWidget(this)),
    //% "Advanced"
    btnAdvanced(new MButton(QString("<b>%1</b>").arg(qtTrId("button_advanced_settings")))),
    lblEditName(0),
    edtEditName(0)

{
    int layoutRow = 0;

    if (!acctName.isEmpty())
    {
        //% "Display Name:"
        lblEditName = new MLabel(qtTrId("label_account_display_name"));
        edtEditName = new MTextEdit(MTextEditModel::SingleLine, acctName);
        policy->addItem(lblEditName, layoutRow, 0, Qt::AlignLeft);
        policy->addItem(edtEditName, layoutRow++, 1, Qt::AlignLeft);
    }



    foreach(Tp::ProtocolParameter *pp, mProtoInfo->parameters())
        mProtoParams.insert(pp->name(), pp);
    dswAdvanced->setObjectName("dswAdvanced");
    btnAdvanced->setViewType(MButton::toggleType);

    //% "Jabber"
    if (protoType == qtTrId("proto_jabber"))
    {
        //% "Login ID:"
        MLabel *lblAccount = new MLabel(qtTrId("label_login_id"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
        //% "<b>Example:</b> user@jabber.org"
        MLabel *lblAccountExample = new MLabel(qtTrId("label_jabber_account_example"));

        //% "Password:"
        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MButton *ckEncrypt = new MButton();
        ckEncrypt->setViewType(MButton::checkboxType);
        ckEncrypt->setCheckable(true);
        ckEncrypt->setObjectName("param_bool_require-encryption");
        ckEncrypt->setChecked(safeParamGet("require-encryption").toBool());
        //% "Encryption Required (TLS/SSL)"
        MLabel *lblEncrypt = new MLabel(qtTrId("label_encryption_required_tls_ssl"));

        MButton *ckIgnoreSSLErrs = new MButton();
        ckIgnoreSSLErrs->setViewType(MButton::checkboxType);
        ckIgnoreSSLErrs->setCheckable(true);
        ckIgnoreSSLErrs->setObjectName("param_bool_ignore-ssl-errors");
        ckIgnoreSSLErrs->setChecked(safeParamGet("ignore-ssl-errors").toBool());
        //% "Ignore SSL certificate errors"
        MLabel *lblIgnoreSSLErrs = new MLabel(qtTrId("label_ignore_ssl_cert_errors"));

        //% "Resource:"
        MLabel *lblResource = new MLabel(qtTrId("label_resource"));
        MTextEdit *edtResource =
                new MTextEdit(MTextEditModel::SingleLine,
                        safeParamGet("resource").toString());
        edtResource->setObjectName("param_str_resource");

        //% "Priority:"
        MLabel *lblPriority = new MLabel(qtTrId("label_priority"));
        MTextEdit *edtPriority =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("priority").toString());
        edtPriority->setObjectName("param_int_priority");
        edtPriority->setContentType(M::NumberContentType);

        //% "<b>Override server settings</b>"
        MLabel *lblServerOverride = new MLabel(qtTrId("label_override_server_settings"));

        //% "Server:"
        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
        edtServer->setObjectName("param_str_server");

        //% "Port:"
        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
        edtPort->setObjectName("param_uint_port");
        edtPort->setContentType(M::NumberContentType);

        MButton *ckOldSSL = new MButton();
        ckOldSSL->setObjectName("param_bool_old-ssl");
        ckOldSSL->setCheckable(true);
        ckOldSSL->setViewType(MButton::checkboxType);
        ckOldSSL->setChecked(safeParamGet("old-ssl").toBool());
        //% "Use old SSL"
        MLabel *lblOldSSL = new MLabel(qtTrId("label_use_old_ssl"));

        polAdv->addItem(ckEncrypt, 0, 0, Qt::AlignCenter);
        polAdv->addItem(lblEncrypt, 0, 1, Qt::AlignLeft);
        polAdv->addItem(ckIgnoreSSLErrs, 1, 0, Qt::AlignCenter);
        polAdv->addItem(lblIgnoreSSLErrs, 1, 1, Qt::AlignLeft);
        polAdv->addItem(lblResource, 2, 0, Qt::AlignLeft);
        polAdv->addItem(edtResource, 2, 1, Qt::AlignLeft);
        polAdv->addItem(lblPriority, 3, 0, Qt::AlignLeft);
        polAdv->addItem(edtPriority, 3, 1, Qt::AlignLeft);
        polAdv->addItem(lblServerOverride, 4, 0, 1, 2, Qt::AlignLeft);
        polAdv->addItem(lblServer, 5, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 5, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 6, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 6, 1, Qt::AlignLeft);
        polAdv->addItem(ckOldSSL, 7, 0, Qt::AlignCenter);
        polAdv->addItem(lblOldSSL, 7, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblAccountExample, layoutRow+1, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+2, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+2, 1, Qt::AlignLeft);


    } //% "Google Talk"
    else if (protoType == qtTrId("proto_google_talk"))
    {
        MLabel *lblAccount = new MLabel(qtTrId("label_login_id"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
        //% "<b>Example:</b> user@gmail.com"
        MLabel *lblAccountExample = new MLabel(qtTrId("label_google_talk_account_example"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MButton *ckEncrypt = new MButton();
        ckEncrypt->setViewType(MButton::checkboxType);
        ckEncrypt->setCheckable(true);
        ckEncrypt->setObjectName("param_bool_require-encryption");
        ckEncrypt->setChecked(safeParamGet("require-encryption").toBool());
        MLabel *lblEncrypt = new MLabel(
                qtTrId("label_encryption_required_tls_ssl"));

        MButton *ckIgnoreSSLErrs = new MButton();
        ckIgnoreSSLErrs->setViewType(MButton::checkboxType);
        ckIgnoreSSLErrs->setCheckable(true);
        ckIgnoreSSLErrs->setObjectName("param_bool_ignore-ssl-errors");
        ckIgnoreSSLErrs->setChecked(safeParamGet("ignore-ssl-errors").toBool());
        MLabel *lblIgnoreSSLErrs = new MLabel(
                qtTrId("label_ignore_ssl_cert_errors"));

        MLabel *lblResource = new MLabel(qtTrId("label_resource"));
        MTextEdit *edtResource =
                new MTextEdit(MTextEditModel::SingleLine,
                        safeParamGet("resource").toString());
        edtResource->setObjectName("param_str_resource");

        MLabel *lblPriority = new MLabel(qtTrId("label_priority"));
        MTextEdit *edtPriority =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("priority").toString());
        edtPriority->setObjectName("param_int_priority");
        edtPriority->setContentType(M::NumberContentType);

        MLabel *lblServerOverride = new MLabel(
                qtTrId("label_override_server_settings"));

        QString server = safeParamGet("server").toString();
        if (server.isEmpty())
            server = GOOGLE_TALK_DEFAULT_SERVER;

        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                                                 server);
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
        edtPort->setObjectName("param_uint_port");
        edtPort->setContentType(M::NumberContentType);

        MButton *ckOldSSL = new MButton();
        ckOldSSL->setObjectName("param_bool_old-ssl");
        ckOldSSL->setCheckable(true);
        ckOldSSL->setViewType(MButton::checkboxType);
        ckOldSSL->setChecked(safeParamGet("old-ssl").toBool());
        MLabel *lblOldSSL = new MLabel(qtTrId("label_use_old_ssl"));

        polAdv->addItem(ckEncrypt, 0, 0, Qt::AlignCenter);
        polAdv->addItem(lblEncrypt, 0, 1, Qt::AlignLeft);
        polAdv->addItem(ckIgnoreSSLErrs, 1, 0, Qt::AlignCenter);
        polAdv->addItem(lblIgnoreSSLErrs, 1, 1, Qt::AlignLeft);
        polAdv->addItem(lblResource, 2, 0, Qt::AlignLeft);
        polAdv->addItem(edtResource, 2, 1, Qt::AlignLeft);
        polAdv->addItem(lblPriority, 3, 0, Qt::AlignLeft);
        polAdv->addItem(edtPriority, 3, 1, Qt::AlignLeft);
        polAdv->addItem(lblServerOverride, 4, 0, 1, 2, Qt::AlignLeft);
        polAdv->addItem(lblServer, 5, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 5, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 6, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 6, 1, Qt::AlignLeft);
        polAdv->addItem(ckOldSSL, 7, 0, Qt::AlignCenter);
        polAdv->addItem(lblOldSSL, 7, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblAccountExample, layoutRow+1, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+2, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+2, 1, Qt::AlignLeft);


    } //% "Facebook Chat"
    else if (protoType == qtTrId("proto_facebook_chat"))
    {
        //% "Username:"
        MLabel *lblAccount = new MLabel(qtTrId("label_username"));
        QString fbAcct;
        //Facebook accounts are invisibly appended with a suffix that the user
        //shouldn't have to deal with. Remove that suffix when pre-populating
        //existing account details. When saving off the account, we'll add it
        //back on (as well as setting tpAcct->icon == "im-facebook" so we can
        //tell it's a FB account and not regular Jabber - this is what we have
        //to do in order to maintain compatibility with Empathy...)
        fbAcct = safeParamGet("account").toString();
        if (fbAcct.contains(FACEBOOK_CHAT_ACCOUNT_SUFFIX))
            fbAcct.chop(fbAcct.size() - fbAcct.indexOf(FACEBOOK_CHAT_ACCOUNT_SUFFIX));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine, fbAcct);
        edtAccount->setObjectName("param_str_account");
        //% "This is your username, not your normal Facebook login.<br /> If you are facebook.com/<b>badger</b>, enter <b>badger</b><br /> Use <a href=\"http://www.facebook.com/username/\">this page</a> to choose a Facebook username if you don't have one."
        MLabel *lblAccountExample = new MLabel(qtTrId("label_facebook_chat_account_example"));
        lblAccountExample->setWordWrap(true);
        connect(lblAccountExample,
                SIGNAL(linkActivated(QString)),
                this,
                SLOT(onLinkActivated(QString)));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblAccountExample, layoutRow+1, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+2, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+2, 1, Qt::AlignLeft);


    } //% "AIM"
    else if (protoType == qtTrId("proto_aim"))
    {
        //% "Screen Name:"
        MLabel *lblAccount = new MLabel(qtTrId("label_screen_name"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
        //% "<b>Example:</b> MyScreenName"
        MLabel *lblAccountExample = new MLabel(qtTrId("label_aim_account_example"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced =
//                new MImageWidget(new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
        //Yes, this really is supposed to be an int, according to the aim CM...
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        polAdv->addItem(lblServer, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 1, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblAccountExample, layoutRow+1, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+2, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+2, 1, Qt::AlignLeft);

    }//% "Groupwise"
    else if (protoType == qtTrId("proto_groupwise"))
    {
        MLabel *lblAccount = new MLabel(qtTrId("label_login_id"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
        //% "<b>Example:</b> username"
        MLabel *lblAccountExample = new MLabel(qtTrId("label_groupwise_account_example"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced =
//                new MImageWidget(new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
        //Yes, this really is supposed to be an int, according to the groupwise CM
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        polAdv->addItem(lblServer, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 1, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblAccountExample, layoutRow+1, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+2, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+2, 1, Qt::AlignLeft);

    }    //% "MSN"
    else if (protoType == qtTrId("proto_msn"))
    {
        MLabel *lblAccount = new MLabel(qtTrId("label_login_id"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
        //% "<b>Example:</b> user@hotmail.com"
        MLabel *lblAccountExample = new MLabel(qtTrId("label_msn_account_example"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
        edtPort->setObjectName("param_uint_port");
        edtPort->setContentType(M::NumberContentType);

        polAdv->addItem(lblServer, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 1, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblAccountExample, layoutRow+1, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+2, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+2, 1, Qt::AlignLeft);

    }    //% "IRC"
    else if (protoType == qtTrId("proto_irc"))
    {
        //% "Not implemented at this time..."
        MLabel *lblNotImplemented = new MLabel(qtTrId("label_not_implemented"));
        policy->addItem(lblNotImplemented, layoutRow+0, 0, Qt::AlignLeft);

    }    //% "People Nearby"
    else if (protoType == qtTrId("proto_people_nearby"))
    {
        //% "First Name:"
        MLabel *lblFName = new MLabel(qtTrId("label_first_name"));
        MTextEdit *edtFName = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("first-name").toString());
        edtFName->setObjectName("param_str_first-name");

        //% "Last Name:"
        MLabel *lblLName = new MLabel(qtTrId("label_last_name"));
        MTextEdit *edtLName = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("last-name").toString());
        edtLName->setObjectName("param_str_last-name");

        //% "Nickname:"
        MLabel *lblNickname = new MLabel(qtTrId("label_nickname"));
        MTextEdit *edtNickname = new MTextEdit(MTextEditModel::SingleLine,
                       safeParamGet("nickname").toString());
        edtNickname->setObjectName("param_str_nickname");

//        MImageWidget *imgAdvanced =
//                new MImageWidget(new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        //% "Published Name:"
        MLabel *lblPubName = new MLabel(qtTrId("label_published_name"));
        MTextEdit *edtPubName = new MTextEdit(MTextEditModel::SingleLine,
                      safeParamGet("published-name").toString());
        edtPubName->setObjectName("param_str_published-name");

        //% "E-mail Address:"
        MLabel *lblEMail = new MLabel(qtTrId("label_email_address"));
        MTextEdit *edtEMail = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("email").toString());
        edtEMail->setObjectName("param_str_email");

        //% "Jabber ID:"
        MLabel *lblJabberID = new MLabel(qtTrId("label_jabber_id"));
        MTextEdit *edtJabberID = new MTextEdit(MTextEditModel::SingleLine,
                       safeParamGet("jid").toString());
        edtJabberID->setObjectName("param_str_jid");

        polAdv->addItem(lblPubName, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtPubName, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblEMail, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtEMail, 1, 1, Qt::AlignLeft);
        polAdv->addItem(lblJabberID, 2, 0, Qt::AlignLeft);
        polAdv->addItem(edtJabberID, 2, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblFName, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtFName, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblLName, layoutRow+1, 0, Qt::AlignLeft);
        policy->addItem(edtLName, layoutRow+1, 1, Qt::AlignLeft);
        policy->addItem(lblNickname, layoutRow+2, 0, Qt::AlignLeft);
        policy->addItem(edtNickname, layoutRow+2, 1, Qt::AlignLeft);

    }    //% "QQ"
    else if (protoType == qtTrId("proto_qq"))
    {
        MLabel *lblAccount = new MLabel(qtTrId("label_login_id"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
        //MLabel *lblAccountExample = new MLabel(tr("<b>Example:</b> user@gmail.com"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
        edtServer->setObjectName("param_str_server");

        //% "Client Version:"
        MLabel *lblClientVer = new MLabel(qtTrId("label_client_version"));
        MTextEdit *edtClientVer =
                new MTextEdit(MTextEditModel::SingleLine,
                        safeParamGet("client-version").toString());
        edtClientVer->setObjectName("param_str_client-version");

        MButton *ckUseTCP = new MButton();
        ckUseTCP->setViewType(MButton::checkboxType);
        ckUseTCP->setCheckable(true);
        ckUseTCP->setChecked(safeParamGet("use-tcp").toBool());
        ckUseTCP->setObjectName("param_bool_use-tcp");
        //% "Use TCP"
        MLabel *lblUseTCP = new MLabel(qtTrId("label_use_tcp"));

        MButton *ckShowNotice = new MButton();
        ckShowNotice->setViewType(MButton::checkboxType);
        ckShowNotice->setCheckable(true);
        ckShowNotice->setChecked(safeParamGet("show-notice").toBool());
        ckShowNotice->setObjectName("param_bool_show-notice");
        //% "Show Notice"
        MLabel *lblShowNotice = new MLabel(qtTrId("label_show_notice"));

        MButton *ckShowNews = new MButton();
        ckShowNews->setViewType(MButton::checkboxType);
        ckShowNews->setCheckable(true);
        ckShowNews->setChecked(safeParamGet("show-news").toBool());
        ckShowNews->setObjectName("param_bool_show-news");
        //% "Show News"
        MLabel *lblShowNews = new MLabel(qtTrId("label_show_news"));

        MButton *ckShowChat = new MButton();
        ckShowChat->setViewType(MButton::checkboxType);
        ckShowChat->setCheckable(true);
        ckShowChat->setChecked(safeParamGet("show-chat").toBool());
        ckShowChat->setObjectName("param_bool_show-chat");
        //% "Show Chat"
        MLabel *lblShowChat = new MLabel(qtTrId("label_show_chat"));

        //% "Keep Alive Interval:"
        MLabel *lblKeepAliveInt = new MLabel(qtTrId("label_keep_alive_interval"));
        MTextEdit *edtKeepAliveInt =
                new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("keep-alive-interval").toString());
        edtKeepAliveInt->setObjectName("param_int_keep-alive-interval");
        edtKeepAliveInt->setContentType(M::NumberContentType);

        //% "Update Interval:"
        MLabel *lblUpdateInt = new MLabel(qtTrId("label_update_interval"));
        MTextEdit *edtUpdateInt =
                new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("update-interval").toString());
                     
        edtUpdateInt->setObjectName("param_int_update-interval");
        edtUpdateInt->setContentType(M::NumberContentType);

        polAdv->addItem(lblServer, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblClientVer, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtClientVer, 1, 1, Qt::AlignLeft);
        polAdv->addItem(ckUseTCP, 2, 0, Qt::AlignCenter);
        polAdv->addItem(lblUseTCP, 2, 1, Qt::AlignLeft);
        polAdv->addItem(ckShowNotice, 3, 0, Qt::AlignCenter);
        polAdv->addItem(lblShowNotice, 3, 1, Qt::AlignLeft);
        polAdv->addItem(ckShowNews, 4, 0, Qt::AlignCenter);
        polAdv->addItem(lblShowNews, 4, 1, Qt::AlignLeft);
        polAdv->addItem(ckShowChat, 5, 0, Qt::AlignCenter);
        polAdv->addItem(lblShowChat, 5, 1, Qt::AlignLeft);
        polAdv->addItem(lblKeepAliveInt, 6, 0, Qt::AlignLeft);
        polAdv->addItem(edtKeepAliveInt, 6, 1, Qt::AlignLeft);
        polAdv->addItem(lblUpdateInt, 7, 0, Qt::AlignLeft);
        polAdv->addItem(edtUpdateInt, 7, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+1, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+1, 1, Qt::AlignLeft);

    }    //% "ICQ"
    else if (protoType == qtTrId("proto_icq"))
    {
        //% "ICQ UIN:"
        MLabel *lblAccount = new MLabel(qtTrId("label_icq_uin"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("account").toString());
        edtAccount->setContentType(M::NumberContentType);
        edtAccount->setObjectName("param_str_account");
        //% "<b>Example:</b> 123456789"
        MLabel *lblAccountExample = new MLabel(qtTrId("label_icq_account_example"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword =
                new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                        new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        //% "Character set:"
        MLabel *lblCharset = new MLabel(qtTrId("label_character_set"));
        MTextEdit *edtCharset =
                new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("charset").toString());
        edtCharset->setObjectName("param_str_charset");

        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                          safeParamGet("server").toString());
                          
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                        safeParamGet("port").toString());
                        
        //Yes, really an int...
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        polAdv->addItem(lblCharset, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtCharset, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblServer, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 1, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 2, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 2, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblAccountExample, layoutRow+1, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+2, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+2, 1, Qt::AlignLeft);

    }    //% "simple"
    else if (protoType == qtTrId("proto_simple"))
    {
        //% "Account:"
        MLabel *lblAccount = new MLabel(qtTrId("label_account"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("account").toString());
        edtAccount->setContentType(M::NumberContentType);
        edtAccount->setObjectName("param_str_account");
        //MLabel *lblAccountExample = new MLabel(qtTrId("label_icq_account_example"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword =
                new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                        new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");


        MButton *ckDoPublish = new MButton();
        ckDoPublish->setViewType(MButton::checkboxType);
        ckDoPublish->setCheckable(true);
        ckDoPublish->setChecked(safeParamGet("dopublish").toBool());
                                 
        ckDoPublish->setObjectName("param_bool_dopublish");
        //% "Do Publish"
        MLabel *lblDoPublish = new MLabel(qtTrId("label_do_publish"));

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("port").toString());
                    
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        MButton *ckUDP = new MButton();
        ckUDP->setViewType(MButton::checkboxType);
        ckUDP->setCheckable(true);
        ckUDP->setChecked(safeParamGet("udp").toBool());
                           
        ckUDP->setObjectName("param_bool_udp");
        //% "UDP"
        MLabel *lblUDP = new MLabel(qtTrId("label_udp"));

        MButton *ckUseProxy = new MButton();
        ckUseProxy->setViewType(MButton::checkboxType);
        ckUseProxy->setCheckable(true);
        ckUseProxy->setChecked(safeParamGet("useproxy").toBool());
                                
        ckUseProxy->setObjectName("param_bool_useproxy");
        //% "Use Proxy"
        MLabel *lblUseProxy = new MLabel(qtTrId("label_use_proxy"));

        //% "Proxy:"
        MLabel *lblProxy = new MLabel(qtTrId("label_proxy"));
        MTextEdit *edtProxy = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("proxy").toString());
                     
        edtProxy->setObjectName("param_str_proxy");

        //% "Auth User:"
        MLabel *lblAuthUser = new MLabel(qtTrId("label_auth_user"));
        MTextEdit *edtAuthUser =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("authuser").toString());
                     
        edtAuthUser->setObjectName("param_str_authuser");

        //% "Auth Domain:"
        MLabel *lblAuthDomain = new MLabel(qtTrId("label_auth_domain"));
        MTextEdit *edtAuthDomain =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("authdomain").toString());
                     
        edtAuthDomain->setObjectName("param_str_authdomain");

        polAdv->addItem(ckDoPublish, 0, 0, Qt::AlignCenter);
        polAdv->addItem(lblDoPublish, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 1, 1, Qt::AlignLeft);
        polAdv->addItem(ckUDP, 2, 0, Qt::AlignCenter);
        polAdv->addItem(lblUDP, 2, 1, Qt::AlignLeft);
        polAdv->addItem(ckUseProxy, 3, 0, Qt::AlignCenter);
        polAdv->addItem(lblUseProxy, 3, 1, Qt::AlignLeft);
        polAdv->addItem(lblProxy, 4, 0, Qt::AlignLeft);
        polAdv->addItem(edtProxy, 4, 1, Qt::AlignLeft);
        polAdv->addItem(lblAuthUser, 5, 0, Qt::AlignLeft);
        polAdv->addItem(edtAuthUser, 5, 1, Qt::AlignLeft);
        polAdv->addItem(lblAuthDomain, 6, 0, Qt::AlignLeft);
        polAdv->addItem(edtAuthDomain, 6, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+1, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+1, 1, Qt::AlignLeft);

    }    //% "gadugadu"
    else if (protoType == qtTrId("proto_gadugadu"))
    {
        MLabel *lblAccount = new MLabel(qtTrId("label_login_id"));
        MTextEdit *edtAccount =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
        //MLabel *lblAccountExample =
//        new MLabel(tr("<b>Example:</b> user@hotmail.com"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                        new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        //% "Nick:"
        MLabel *lblNick = new MLabel(qtTrId("label_nick"));
        MTextEdit *edtNick = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("nick").toString());
                    
        edtNick->setObjectName("param_str_nick");

        polAdv->addItem(lblNick, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtNick, 0, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+1, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+1, 1, Qt::AlignLeft);

    }    //% "Yahoo! Japan"
    else if (protoType == qtTrId("proto_yahoo_japan"))
    {

        MLabel *lblAccount = new MLabel(qtTrId("label_account"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
//        MLabel *lblAccountExample = new MLabel(
//                tr("<b>Example:</b> user@gmail.com"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");


        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
                      
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
                   
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        //% "Xfer Host:"
        MLabel *lblXferHost = new MLabel(qtTrId("label_xfer_host"));
        MTextEdit *edtXferHost = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("xfer-host").toString());
                      
        edtXferHost->setObjectName("param_str_xfer-host");

        //% "Xfer Port:"
        MLabel *lblXferPort = new MLabel(qtTrId("label_xfer_port"));
        MTextEdit *edtXferPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("xfer-port").toString());
                   
        edtXferPort->setObjectName("param_int_xfer-port");
        edtXferPort->setContentType(M::NumberContentType);

        //% "Room List Locale:"
        MLabel *lblRoomListLocale = new MLabel(qtTrId("label_room_list_locale"));
        MTextEdit *edtRoomListLocale = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("room-list-locale").toString());
                      
        edtXferHost->setObjectName("param_str_room-list-locale");


        MButton *ckIgnoreInvites = new MButton();
        ckIgnoreInvites->setViewType(MButton::checkboxType);
        ckIgnoreInvites->setCheckable(true);
        ckIgnoreInvites->setObjectName("param_bool_ignore-invites");
        ckIgnoreInvites->setChecked(safeParamGet("ignore-invites").toBool());

        //% "Ignore Invites"
        MLabel *lblIgnoreInvites = new MLabel(qtTrId("label_ignore_invites"));

        MLabel *lblCharset = new MLabel(qtTrId("label_character_set"));
        MTextEdit *edtCharset =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("charset").toString());
                     
        edtCharset->setObjectName("param_str_charset");


        polAdv->addItem(lblServer, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 1, 1, Qt::AlignLeft);
        polAdv->addItem(lblXferHost, 2, 0, Qt::AlignLeft);
        polAdv->addItem(edtXferHost, 2, 1, Qt::AlignLeft);
        polAdv->addItem(lblXferPort, 3, 0, Qt::AlignLeft);
        polAdv->addItem(edtXferPort, 3, 1, Qt::AlignLeft);
        polAdv->addItem(lblRoomListLocale, 4, 0, Qt::AlignLeft);
        polAdv->addItem(edtRoomListLocale, 4, 1, Qt::AlignLeft);
        polAdv->addItem(ckIgnoreInvites, 5, 0, Qt::AlignCenter);
        polAdv->addItem(lblIgnoreInvites, 5, 1, Qt::AlignLeft);
        polAdv->addItem(lblCharset, 6, 0, Qt::AlignLeft);
        polAdv->addItem(edtCharset, 6, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+1, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+1, 1, Qt::AlignLeft);


    }    //% "Yahoo!"
    else if (protoType == qtTrId("proto_yahoo"))
    {

        //% "Yahoo! ID:"
        MLabel *lblAccount = new MLabel(qtTrId("label_yahoo_id"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
        //% "<b>Example:</b> username"
        MLabel *lblAccountExample = new MLabel(qtTrId("label_yahoo_account_example"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");


        MButton *ckIgnoreInvites = new MButton();
        ckIgnoreInvites->setViewType(MButton::checkboxType);
        ckIgnoreInvites->setCheckable(true);
        ckIgnoreInvites->setObjectName("param_bool_ignore-invites");
        ckIgnoreInvites->setChecked(safeParamGet("ignore-invites").toBool());

        //% "Ignore conference and chat room invitations"
        MLabel *lblIgnoreInvites = new MLabel(qtTrId("label_ignore_conference_and_chat_room_invitations"));

        MLabel *lblRoomListLocale = new MLabel(qtTrId("label_room_list_locale"));
        MTextEdit *edtRoomListLocale = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("room-list-locale").toString());
                      
        edtRoomListLocale->setObjectName("param_str_room-list-locale");

        MLabel *lblCharset = new MLabel(qtTrId("label_character_set"));
        MTextEdit *edtCharset =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("charset").toString());
                     
        edtCharset->setObjectName("param_str_charset");


        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
                      
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
                   
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        MLabel *lblXferHost = new MLabel(qtTrId("label_xfer_host"));
        MTextEdit *edtXferHost = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("xfer-host").toString());
                      
        edtXferHost->setObjectName("param_str_xfer-host");

        MLabel *lblXferPort = new MLabel(qtTrId("label_xfer_port"));
        MTextEdit *edtXferPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("xfer-port").toString());
                   
        edtXferPort->setObjectName("param_int_xfer-port");
        edtXferPort->setContentType(M::NumberContentType);


        polAdv->addItem(ckIgnoreInvites, 0, 0, Qt::AlignCenter);
        polAdv->addItem(lblIgnoreInvites, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblRoomListLocale, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtRoomListLocale, 1, 1, Qt::AlignLeft);
        polAdv->addItem(lblCharset, 2, 0, Qt::AlignLeft);
        polAdv->addItem(edtCharset, 2, 1, Qt::AlignLeft);
        polAdv->addItem(lblServer, 3, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 3, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 4, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 4, 1, Qt::AlignLeft);
        polAdv->addItem(lblXferHost, 5, 0, Qt::AlignLeft);
        polAdv->addItem(edtXferHost, 5, 1, Qt::AlignLeft);
        polAdv->addItem(lblXferPort, 6, 0, Qt::AlignLeft);
        polAdv->addItem(edtXferPort, 6, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblAccountExample, layoutRow+1, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+2, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+2, 1, Qt::AlignLeft);

    }    //% "sipe"
    else if (protoType == qtTrId("proto_sipe"))
    {
        MLabel *lblAccount = new MLabel(qtTrId("label_account"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
//        MLabel *lblAccountExample = new MLabel(
//                tr("<b>Example:</b> user@jabber.org"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MButton *ckSSO = new MButton();
        ckSSO->setViewType(MButton::checkboxType);
        ckSSO->setCheckable(true);
        ckSSO->setObjectName("param_bool_sso");
        ckSSO->setChecked(safeParamGet("sso").toBool());

        //% "Use Single Sign-On"
        MLabel *lblSSO = new MLabel(qtTrId("label_use_single_signon"));

        MButton *ckKrb5 = new MButton();
        ckKrb5->setViewType(MButton::checkboxType);
        ckKrb5->setCheckable(true);
        ckKrb5->setObjectName("param_bool_krb5");
        ckKrb5->setChecked(safeParamGet("krb5").toBool());

        //% "Use Kerberos"
        MLabel *lblKrb5 = new MLabel(qtTrId("label_use_kerberos"));

        //% "User Agent:"
        MLabel *lblUserAgent = new MLabel(qtTrId("label_user_agent"));
        MTextEdit *edtUserAgent = new MTextEdit(MTextEditModel::SingleLine,
                        safeParamGet("useragent").toString());
                         
        edtUserAgent->setObjectName("param_str_useragent");


        //TODO - see if there's a way we can glean this from the Pidgin/libpurple/SIPE plugin...
        /* Mapping (gleaned from pidgin plugin source):
        "auto", "Auto"
        "tls", "SSL/TLS"
        "tcp", "TCP"
        "udp", "UDP"
        */

        MComboBox *cboTransport = new MComboBox();
        cboTransport->setObjectName("param_cbostr_transport");
        //% "Auto"
        cboTransport->addItem(qtTrId("combo_transport_item_auto"));
        //% "SSL/TLS"
        cboTransport->addItem(qtTrId("combo_transport_item_ssl_tls"));
        //% "TCP"
        cboTransport->addItem(qtTrId("combo_transport_item_tcp"));
        //% "UDP"
        cboTransport->addItem(qtTrId("combo_transport_item_udp"));
        //% "Select Transport:"
        cboTransport->setTitle(qtTrId("combo_transport_title"));

        QString transport = safeParamGet("transport").toString();

        //FIXME - this is really, really ugly...
        if (transport == "auto")
        {
            cboTransport->setCurrentIndex(0);
        } else if (transport == "tls")
        {
            cboTransport->setCurrentIndex(1);
        } else if (transport == "tcp")
        {
            cboTransport->setCurrentIndex(2);
        } else if (transport == "udp")
        {
            cboTransport->setCurrentIndex(3);
        } //Otherwise, if we have no matches, we leave it unset...


        MButton *ckUsePort = new MButton();
        ckUsePort->setObjectName("param_bool_useport");
        ckUsePort->setCheckable(true);
        ckUsePort->setViewType(MButton::checkboxType);
        ckUsePort->setChecked(safeParamGet("useport").toBool());

        //% "Use Port"
        MLabel *lblUsePort = new MLabel(qtTrId("label_use_port"));

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
                   
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        MButton *ckUseProxy = new MButton();
        ckUseProxy->setObjectName("param_bool_useproxy");
        ckUseProxy->setCheckable(true);
        ckUseProxy->setViewType(MButton::checkboxType);
        ckUseProxy->setChecked(safeParamGet("useproxy").toBool());
                              
        MLabel *lblUseProxy = new MLabel(qtTrId("label_use_proxy"));

        MLabel *lblProxy = new MLabel(qtTrId("label_proxy"));
        MTextEdit *edtProxy = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("proxy").toString());
                      
        edtProxy->setObjectName("param_str_proxy");


        polAdv->addItem(ckSSO, 0, 0, Qt::AlignCenter);
        polAdv->addItem(lblSSO, 0, 1, Qt::AlignLeft);
        polAdv->addItem(ckKrb5, 1, 0, Qt::AlignCenter);
        polAdv->addItem(lblKrb5, 1, 1, Qt::AlignLeft);
        polAdv->addItem(lblUserAgent, 2, 0, Qt::AlignLeft);
        polAdv->addItem(edtUserAgent, 2, 1, Qt::AlignLeft);
        polAdv->addItem(cboTransport, 3, 0, 1, 2, Qt::AlignLeft);
        polAdv->addItem(ckUsePort, 4, 0, Qt::AlignCenter);
        polAdv->addItem(lblUsePort, 4, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 5, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 5, 1, Qt::AlignLeft);
        polAdv->addItem(ckUseProxy, 6, 0, Qt::AlignCenter);
        polAdv->addItem(lblUseProxy, 6, 1, Qt::AlignLeft);
        polAdv->addItem(lblProxy, 7, 0, Qt::AlignLeft);
        polAdv->addItem(edtProxy, 7, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+1, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+1, 1, Qt::AlignLeft);

    }    //% "MySpace"
    else if (protoType == qtTrId("proto_myspace"))
    {
        MLabel *lblAccount = new MLabel(qtTrId("label_account"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
//        MLabel *lblAccountExample = new MLabel(tr("<b>Example:</b> user@hotmail.com"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
                      
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
                    
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        polAdv->addItem(lblServer, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 1, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+1, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+1, 1, Qt::AlignLeft);

    }    //% "sametime"
    else if (protoType == qtTrId("proto_sametime"))
    {
        MLabel *lblAccount = new MLabel(qtTrId("label_account"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
        //MLabel *lblAccountExample = new MLabel(tr("<b>Example:</b> user@hotmail.com"));

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword =
                new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
                      
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
                    
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        MButton *ckForceLogin = new MButton();
        ckForceLogin->setViewType(MButton::checkboxType);
        ckForceLogin->setCheckable(true);
        ckForceLogin->setChecked(safeParamGet("force-login").toBool());
                                  
        ckForceLogin->setObjectName("param_bool_force-login");

        //% "Force Login"
        MLabel * lblForceLogin = new MLabel(qtTrId("label_force_login"));

        MButton *ckFakeClientID = new MButton();
        ckFakeClientID->setViewType(MButton::checkboxType);
        ckFakeClientID->setCheckable(true);
        ckFakeClientID->setChecked(safeParamGet("fake-client-id").toBool());
                                    
        ckFakeClientID->setObjectName("param_bool_fake-client-id");
        //% "Fake Client ID"
        MLabel *lblFakeClientID = new MLabel(qtTrId("label_fake_client_id"));

        polAdv->addItem(lblServer, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 1, 1, Qt::AlignLeft);
        polAdv->addItem(ckForceLogin, 2, 0, Qt::AlignCenter);
        polAdv->addItem(lblForceLogin, 2, 1, Qt::AlignLeft);
        polAdv->addItem(ckFakeClientID, 3, 0, Qt::AlignCenter);
        polAdv->addItem(lblFakeClientID, 3, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);
        policy->addItem(lblPassword, layoutRow+1, 0, Qt::AlignLeft);
        policy->addItem(edtPassword, layoutRow+1, 1, Qt::AlignLeft);

    }    //% "silc"
    else if (protoType == qtTrId("proto_silc"))
    {
        MLabel *lblAccount = new MLabel(qtTrId("label_account"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
//        MLabel *lblAccountExample = new MLabel(
//                tr("<b>Example:</b> user@jabber.org"));

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MLabel *lblPassword = new MLabel(qtTrId("label_password"));
        MTextEdit *edtPassword = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("password").toString());
        edtPassword->setEchoMode(MTextEditModel::Password);
        edtPassword->setObjectName("param_str_password");

        MLabel *lblServer = new MLabel(qtTrId("label_server"));
        MTextEdit *edtServer = new MTextEdit(MTextEditModel::SingleLine,
                     safeParamGet("server").toString());
                      
        edtServer->setObjectName("param_str_server");

        MLabel *lblPort = new MLabel(qtTrId("label_port"));
        MTextEdit *edtPort = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("port").toString());
                   
        edtPort->setObjectName("param_int_port");
        edtPort->setContentType(M::NumberContentType);

        //% "Public Key:"
        MLabel *lblPublicKey = new MLabel(qtTrId("label_public_key"));
        MTextEdit *edtPublicKey = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("public-key").toString());
                   
        edtPublicKey->setObjectName("param_str_public-key");

        //% "Private Key:"
        MLabel *lblPrivateKey = new MLabel(qtTrId("label_private_key"));
        MTextEdit *edtPrivateKey = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("private-key").toString());
                   
        edtPrivateKey->setObjectName("param_str_private-key");

        //% "Cipher:"
        MLabel *lblCipher = new MLabel(qtTrId("label_cipher"));
        MTextEdit *edtCipher = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("cipher").toString());
                   
        edtCipher->setObjectName("param_str_cipher");

        //% "HMAC:"
        MLabel *lblHMAC = new MLabel(qtTrId("label_hmac"));
        MTextEdit *edtHMAC = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("hmac").toString());
                   
        edtHMAC->setObjectName("param_str_hmac");


        MButton *ckPFS = new MButton();
        ckPFS->setViewType(MButton::checkboxType);
        ckPFS->setCheckable(true);
        ckPFS->setObjectName("param_bool_pfs");
        ckPFS->setChecked(safeParamGet("pfs").toBool());
                               
        //% "PFS"
        MLabel *lblPFS = new MLabel(qtTrId("label_pfs"));

        MButton *ckPubkeyAuth = new MButton();
        ckPubkeyAuth->setViewType(MButton::checkboxType);
        ckPubkeyAuth->setCheckable(true);
        ckPubkeyAuth->setObjectName("param_bool_pubkey-auth");
        ckPubkeyAuth->setChecked(safeParamGet("pubkey-auth").toBool());

        //% "Pubkey Auth"
        MLabel *lblPubkeyAuth = new MLabel(qtTrId("label_pubkey_auth"));

        MButton *ckBlockIMs = new MButton();
        ckBlockIMs->setObjectName("param_bool_block-ims");
        ckBlockIMs->setCheckable(true);
        ckBlockIMs->setViewType(MButton::checkboxType);
        ckBlockIMs->setChecked(safeParamGet("block-ims").toBool());

        //% "Block IMs"
        MLabel *lblBlockIMs = new MLabel(qtTrId("label_block_ims"));

        polAdv->addItem(lblPassword, 0, 0, Qt::AlignLeft);
        polAdv->addItem(edtPassword, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblServer, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtServer, 1, 1, Qt::AlignLeft);
        polAdv->addItem(lblPort, 2, 0, Qt::AlignLeft);
        polAdv->addItem(edtPort, 2, 1, Qt::AlignLeft);
        polAdv->addItem(lblPublicKey, 3, 0, Qt::AlignLeft);
        polAdv->addItem(edtPublicKey, 3, 1, Qt::AlignLeft);
        polAdv->addItem(lblPrivateKey, 4, 0, Qt::AlignLeft);
        polAdv->addItem(edtPrivateKey, 4, 1, Qt::AlignLeft);
        polAdv->addItem(lblCipher, 5, 0, Qt::AlignLeft);
        polAdv->addItem(edtCipher, 5, 1, Qt::AlignLeft);
        polAdv->addItem(lblHMAC, 6, 0, Qt::AlignLeft);
        polAdv->addItem(edtHMAC, 6, 1, Qt::AlignLeft);
        polAdv->addItem(ckPFS, 7, 0, Qt::AlignCenter);
        polAdv->addItem(lblPFS, 7, 1, Qt::AlignLeft);
        polAdv->addItem(ckPubkeyAuth, 8, 0, Qt::AlignCenter);
        polAdv->addItem(lblPubkeyAuth, 8, 1, Qt::AlignLeft);
        polAdv->addItem(ckBlockIMs, 9, 0, Qt::AlignCenter);
        polAdv->addItem(lblBlockIMs, 9, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);

    }    //% "zephyr"
    else if (protoType == qtTrId("proto_zephyr"))
    {

        MLabel *lblAccount = new MLabel(qtTrId("label_account"));
        MTextEdit *edtAccount = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("account").toString());
        edtAccount->setObjectName("param_str_account");
//        MLabel *lblAccountExample = new MLabel(
//                tr("<b>Example:</b> user@jabber.org"));

//        MImageWidget *imgAdvanced = new MImageWidget(
//                new QPixmap("/images/droparrow-drkgrey.png"));

        //Begin "Advanced" section
        MLayout *lytAdv = new MLayout(dswAdvanced);
        MGridLayoutPolicy *polAdv = new MGridLayoutPolicy(lytAdv);
        polAdv->setObjectName("polAdv");

        MButton *ckUseTzc = new MButton();
        ckUseTzc->setViewType(MButton::checkboxType);
        ckUseTzc->setCheckable(true);
        ckUseTzc->setObjectName("param_bool_use-tzc");
        ckUseTzc->setChecked(safeParamGet("use-tzc").toBool());

        //% "Use Tzc"
        MLabel *lblUseTzc = new MLabel(qtTrId("label_use_tzc"));

        //% "Tzc Command:"
        MLabel *lblTzcCommand = new MLabel(qtTrId("label_tzc_command"));
        MTextEdit *edtTzcCommand = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("tzc-command").toString());
                   
        edtTzcCommand->setObjectName("param_str_tzc-command");

        MButton *ckWriteAnyone = new MButton();
        ckWriteAnyone->setViewType(MButton::checkboxType);
        ckWriteAnyone->setCheckable(true);
        ckWriteAnyone->setObjectName("param_bool_write-anyone");
        ckWriteAnyone->setChecked(safeParamGet("write-anyone").toBool());

        //% "Write Anyone"
        MLabel *lblWriteAnyone = new MLabel(qtTrId("label_write_anyone"));

        MButton *ckWriteZsubs = new MButton();
        ckWriteZsubs->setViewType(MButton::checkboxType);
        ckWriteZsubs->setCheckable(true);
        ckWriteZsubs->setObjectName("param_bool_write-zsubs");
        ckWriteZsubs->setChecked(safeParamGet("write-zsubs").toBool());

        //% "Write Zsubs"
        MLabel *lblWriteZsubs = new MLabel(qtTrId("label_write_zsubs"));

        MButton *ckReadAnyone = new MButton();
        ckReadAnyone->setViewType(MButton::checkboxType);
        ckReadAnyone->setCheckable(true);
        ckReadAnyone->setObjectName("param_bool_read-anyone");
        ckReadAnyone->setChecked(safeParamGet("read-anyone").toBool());

        //% "Read Anyone"
        MLabel *lblReadAnyone = new MLabel(qtTrId("label_read_anyone"));

        MButton *ckReadZsubs = new MButton();
        ckReadZsubs->setViewType(MButton::checkboxType);
        ckReadZsubs->setCheckable(true);
        ckReadZsubs->setObjectName("param_bool_read-zsubs");
        ckReadZsubs->setChecked(safeParamGet("read-zsubs").toBool());

        //% "Read Zsubs"
        MLabel *lblReadZsubs = new MLabel(qtTrId("label_read_zsubs"));



        //% "Realm:"
        MLabel *lblRealm = new MLabel(qtTrId("label_realm"));
        MTextEdit *edtRealm = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("realm").toString());
                   
        edtRealm->setObjectName("param_str_realm");

        //% "Exposure Level:"
        MLabel *lblExpLevel = new MLabel(qtTrId("label_exposure_level"));
        MTextEdit *edtExpLevel = new MTextEdit(MTextEditModel::SingleLine,
                   safeParamGet("exposure-level").toString());
                   
        edtExpLevel->setObjectName("param_str_exposure-level");

        MLabel *lblCharset = new MLabel(qtTrId("label_character_set"));
        MTextEdit *edtCharset = new MTextEdit(MTextEditModel::SingleLine,
                    safeParamGet("charset").toString());
                     
        edtCharset->setObjectName("param_str_charset");

        polAdv->addItem(ckUseTzc, 0, 0, Qt::AlignCenter);
        polAdv->addItem(lblUseTzc, 0, 1, Qt::AlignLeft);
        polAdv->addItem(lblTzcCommand, 1, 0, Qt::AlignLeft);
        polAdv->addItem(edtTzcCommand, 1, 1, Qt::AlignLeft);
        polAdv->addItem(ckWriteAnyone, 2, 0, Qt::AlignCenter);
        polAdv->addItem(lblWriteAnyone, 2, 1, Qt::AlignLeft);
        polAdv->addItem(ckWriteZsubs, 3, 0, Qt::AlignCenter);
        polAdv->addItem(lblWriteZsubs, 3, 1, Qt::AlignLeft);
        polAdv->addItem(ckReadAnyone, 4, 0, Qt::AlignCenter);
        polAdv->addItem(lblReadAnyone, 4, 1, Qt::AlignLeft);
        polAdv->addItem(ckReadZsubs, 5, 0, Qt::AlignCenter);
        polAdv->addItem(lblReadZsubs, 5, 1, Qt::AlignLeft);
        polAdv->addItem(lblRealm, 6, 0, Qt::AlignLeft);
        polAdv->addItem(edtRealm, 6, 1, Qt::AlignLeft);
        polAdv->addItem(lblExpLevel, 7, 0, Qt::AlignLeft);
        polAdv->addItem(edtExpLevel, 7, 1, Qt::AlignLeft);
        polAdv->addItem(lblCharset, 8, 0, Qt::AlignLeft);
        polAdv->addItem(edtCharset, 8, 1, Qt::AlignLeft);
        lytAdv->setPolicy(polAdv);
        dswAdvanced->setLayout(lytAdv);
        //End "Advanced" section

        policy->addItem(lblAccount, layoutRow+0, 0, Qt::AlignLeft);
        policy->addItem(edtAccount, layoutRow+0, 1, Qt::AlignLeft);

    } else
    {
        //TODO: Build generic SetupWidget based off of proto param list...
        //% "%1 Not implemented at this time..."
        MLabel *lblNotImplemented = new MLabel(qtTrId("label_proto_not_implemented").arg(protoType));
        policy->addItem(lblNotImplemented, layoutRow+0, 0, Qt::AlignLeft);

    }

    btnEnabled = new MButton();
    btnEnabled->setViewType(MButton::checkboxType);
    btnEnabled->setCheckable(true);
    btnEnabled->setChecked(acctEnabled);

    //% "Enable Account"
    MLabel *lblEnabled = new MLabel(qtTrId("label_enable_account"));

    policy->addItem(btnEnabled, policy->count(), 0, Qt::AlignCenter);
    policy->addItem(lblEnabled, policy->count()-1, 1, Qt::AlignLeft);

    layout->setPolicy(policy);

    if (dswAdvanced->layout() && (dswAdvanced->layout()->count() > 0))
    {
        connect(btnAdvanced,
                SIGNAL(clicked()),
                this,
                SLOT(onBtnAdvancedClicked()));
        policy->addItem(btnAdvanced, policy->count(), 0, 1, 2, Qt::AlignLeft);
        //Not sure why we have to do this, but if we don't,
        //it screws up the rest of the layout...
        policy->addItem(dswAdvanced, policy->count(), 0, 1, 2, Qt::AlignLeft);
        layout->takeAt(layout->indexOf(dswAdvanced));
    }


    setLayout(layout);

}

QStringList TpAcctSetupWidget::MapTpProtoToProtoTypes(QString tpProto)
{
    static QHash<QString, QStringList> mProtoMap;
    if (!mProtoMap.count())
    {

            mProtoMap.insert(QString("jabber"),
                             QStringList() << qtTrId("proto_jabber")
                                << qtTrId("proto_google_talk")
                                << qtTrId("proto_facebook_chat"));
            mProtoMap.insert(QString("aim"),
                             QStringList(qtTrId("proto_aim")));
            mProtoMap.insert(QString("groupwise"),
                             QStringList(qtTrId("proto_groupwise")));
            mProtoMap.insert(QString("msn"),
                             QStringList(qtTrId("proto_msn")));
            mProtoMap.insert("irc",
                             QStringList(qtTrId("proto_irc")));
            mProtoMap.insert("local-xmpp",
                             QStringList(qtTrId("proto_people_nearby")));
            mProtoMap.insert("qq",
                             QStringList(qtTrId("proto_qq")));
            mProtoMap.insert("icq",
                             QStringList(qtTrId("proto_icq")));
            mProtoMap.insert("simple",
                             QStringList(qtTrId("proto_simple")));
            mProtoMap.insert("gadugadu",
                             QStringList(qtTrId("proto_gadugadu")));
            mProtoMap.insert("yahoojp",
                             QStringList(qtTrId("proto_yahoo_japan")));
            mProtoMap.insert("yahoo",
                             QStringList(qtTrId("proto_yahoo")));
            mProtoMap.insert("sipe",
                             QStringList(qtTrId("proto_sipe")));
            mProtoMap.insert("myspace",
                             QStringList(qtTrId("proto_myspace")));
            mProtoMap.insert("sametime",
                             QStringList(qtTrId("proto_sametime")));
            mProtoMap.insert("silc",
                             QStringList(qtTrId("proto_silc")));
            mProtoMap.insert("zephyr",
                             QStringList(qtTrId("proto_zephyr")));

    }
    if (mProtoMap.contains(tpProto))
        return mProtoMap.value(tpProto);
    else
        return QStringList(tpProto);
}

QString TpAcctSetupWidget::MapProtoTypeToTpProto(QString protoType)
{
    QString result = protoType;

    if ((protoType == qtTrId("proto_jabber"))
        || (protoType == qtTrId("proto_google_talk"))
        || (protoType == qtTrId("proto_facebook_chat")))
        result = "jabber";
    else if (protoType == qtTrId("proto_aim"))
        result = "aim";
    else if (protoType == qtTrId("proto_groupwise"))
        result = "groupwise";
    else if (protoType == qtTrId("proto_msn"))
        result = "msn";
    else if (protoType == qtTrId("proto_irc"))
        result = "irc";
    else if (protoType == qtTrId("proto_people_nearby"))
        result = "local-xmpp";
    else if (protoType == qtTrId("proto_qq"))
        result = "qq";
    else if (protoType == qtTrId("proto_icq"))
        result = "icq";
    else if (protoType == qtTrId("proto_simple"))
        result = "simple";
    else if (protoType == qtTrId("proto_gadugadu"))
        result = "gadugadu";
    else if (protoType == qtTrId("proto_yahoo_japan"))
        result = "yahoojp";
    else if (protoType == qtTrId("proto_yahoo"))
        result = "yahoo";
    else if (protoType == qtTrId("proto_sipe"))
        result = "sipe";
    else if (protoType == qtTrId("proto_myspace"))
        result = "myspace";
    else if (protoType == qtTrId("proto_sametime"))
        result = "sametime";
    else if (protoType == qtTrId("proto_silc"))
        result = "silc";
    else if (protoType == qtTrId("proto_zephyr"))
        result = "zephyr";

    return result;
}

QString TpAcctSetupWidget::GetProtoTypeIcon(QString protoType)
{
    QString name, tpProtoName;

    //GTalk and Facebook use jabber, but
    //we have to set a different icon to
    //differentiate - Empathy looks at
    //the icon name in order to determine
    //if it's "real" jabber, GTalk, or FB...
    if (protoType == qtTrId("proto_google_talk"))
        name = GOOGLE_TALK_ICON;
    else if (protoType == qtTrId("proto_facebook_chat"))
        name = FACEBOOK_CHAT_ICON;
    else {
        tpProtoName = TpAcctSetupWidget::MapProtoTypeToTpProto(protoType);
        //Yahoo Japan uses the same icon as Yahoo, as per Empathy source
        if (tpProtoName == "yahoojp")
            tpProtoName = "yahoo";
        //Simple uses the same icon as SIP, as per Empathy source
        if (tpProtoName == "simple")
            tpProtoName = "sip";
        name = QString("im-%1").arg(tpProtoName);
    }

    return name;
}

QString TpAcctSetupWidget::MapTpProtoToProtoType(QString tpProto, QString tpIcon)
{
    QString result;
    //We have to special case jabber, as it is used
    //for regular jabber, Google Talk, and Facebook Chat.
    //It is distinguished by icon name, in order to keep
    //consistency with Empathy...
    if (tpProto == "jabber")
    {
        if (tpIcon == GOOGLE_TALK_ICON)
            result = qtTrId("proto_google_talk");
        else if (tpIcon == FACEBOOK_CHAT_ICON)
            result = qtTrId("proto_facebook_chat");
        else
            result = qtTrId("proto_jabber");
    } else if (tpProto == "aim")
        result = qtTrId("proto_aim");
    else if (tpProto == "groupwise")
        result = qtTrId("proto_groupwise");
    else if (tpProto == "msn")
        result = qtTrId("proto_msn");
    else if (tpProto == "irc")
        result = qtTrId("proto_irc");
    else if ((tpProto == "local-xmpp") || (tpProto == "local_xmpp"))
        result = qtTrId("proto_people_nearby");
    else if (tpProto == "qq")
        result = qtTrId("proto_qq");
    else if (tpProto == "icq")
        result = qtTrId("proto_icq");
    else if (tpProto == "simple")
        result = qtTrId("proto_simple");
    else if (tpProto == "gadugadu")
        result = qtTrId("proto_gadugadu");
    else if (tpProto == "yahoojp")
        result = qtTrId("proto_yahoo_japan");
    else if (tpProto == "yahoo")
        result = qtTrId("proto_yahoo");
    else if (tpProto == "sipe")
        result = qtTrId("proto_sipe");
    else if (tpProto == "myspace")
        result = qtTrId("proto_myspace");
    else if (tpProto == "sametime")
        result = qtTrId("proto_sametime");
    else if (tpProto == "silc")
        result = qtTrId("proto_silc");
    else if (tpProto == "zephyr")
        result = qtTrId("proto_zephyr");
    else
        result = tpProto;
    return result;
}

void TpAcctSetupWidget::getParamsFromLayout(QGraphicsLayout *layout,
                                            QVariantMap *qvm)
{

    QString objName, paramName;
    QGraphicsLayoutItem *child;
    int pos, i;

    for (i = 0; i < layout->count(); ++i)
    {
        child = layout->itemAt(i);
        objName = dynamic_cast<QObject *>(child)->objectName();
        //If the object name starts with param_,
        //then it's an input param field of *some* type...
        if (objName.left(6) == "param_")
        {
            //Chop off the initial param_ so we can then grab the type
            objName = objName.remove(0,6);
            /*
                If it's a properly-formed param input field, it will then have
                an identifier of input type:
                str == MTextEdit, string param
                int == MTextEdit, int param
                uint == MTextEdit, uint param
                bool == MButton (Checkbox view type), bool param
                cbostr == MComboBox, string param
            */

            if ((pos = objName.indexOf("_")) != -1)
            {
                paramName = objName.right(objName.size()-pos-1);
                objName.chop(objName.size()-pos);
                if (objName == "str")
                {
                    qvm->insert(paramName, QVariant(dynamic_cast<MTextEdit *>(child)->text()));

                } else if (objName == "int")
                {
                    qvm->insert(paramName, QVariant(dynamic_cast<MTextEdit *>(child)->text().toInt()));

                } else if (objName == "uint")
                {
                    qvm->insert(paramName, QVariant(dynamic_cast<MTextEdit *>(child)->text().toUInt()));

                } else if (objName == "bool")
                {
                    qvm->insert(paramName, QVariant(dynamic_cast<MButton *>(child)->isChecked()));

                } else if (objName == "cbostr")
                {
                    int idx = dynamic_cast<MComboBox *>(child)->currentIndex();
                    QString paramVal;

                    //Have to special case these, as almost always the
                    //strings used in the combobox will be pretty-ified
                    //versus the actual valid param values...
                    if (paramName == "transport")
                    {
                        /* Mapping:
                        "auto", "Auto"
                        "tls", "SSL/TLS"
                        "tcp", "TCP"
                        "udp", "UDP"
                        */
                        if (idx == 0)
                            paramVal = "auto";
                        else if (idx == 1)
                            paramVal = "tls";
                        else if (idx == 2)
                            paramVal = "tcp";
                        else if (idx == 3)
                            paramVal = "udp";
                        else
                            paramVal = (mProtoParams.contains("transport") ?
                                        mProtoParams.value("transport")->defaultValue().toString() : "");
                        //If idx is -1 or > 3, try to grab default, if it exists...
                    } else
                    {
                        qWarning() << QString("Unrecognized cbostr param input %1").arg(dynamic_cast<QObject *>(child)->objectName());
                        paramVal = "";
                    }
                    qvm->insert(paramName, QVariant(paramVal));

                } else
                {
                    qWarning() << QString("Unrecognized param input object %1/%2").arg(objName, dynamic_cast<QObject *>(child)->objectName());
                }
            } else
            {
                qWarning() << QString("Didn't find 2nd _ in param input object %1/%2").arg(objName, dynamic_cast<QObject *>(child)->objectName());
            }
        }
    }


}

QPair<QVariantMap, QStringList>
        TpAcctSetupWidget::getParams()
{
    QVariantMap setParams, paramInputs;
    QStringList unsetParams;

    this->getParamsFromLayout(this->layout, &paramInputs);
    if (dswAdvanced->layout() && (dswAdvanced->layout()->count() > 0))
        this->getParamsFromLayout(dswAdvanced->layout(), &paramInputs);

    if ((mCurProto == qtTrId("proto_facebook_chat")) && (paramInputs.contains("account"))
        && (!paramInputs.value("account").toString().contains(FACEBOOK_CHAT_ACCOUNT_SUFFIX)))
    {
        QString fbAcct = paramInputs.value("account").toString();
        fbAcct.append(FACEBOOK_CHAT_ACCOUNT_SUFFIX);
        paramInputs.remove("account");
        paramInputs.insert("account", QVariant(fbAcct));
    }


    foreach(QString key, paramInputs.keys())
    {
        //Only look at params that are valid for this proto...
        if (mProtoParams.contains(key))
        {
            //If we've deviated from the default value
            if (paramInputs.value(key) != mProtoParams.value(key)->defaultValue())
            {
                if (mCurParams.contains(key))
                {
                    //If it's currently set in the account params, only add it to the "set" map
                    //if it's different
                    if (mCurParams.value(key) != paramInputs.value(key))
                        setParams.insert(key, paramInputs.value(key));
                } else
                {
                    //If it's not currently set in the account params, add it to the "set" map
                    setParams.insert(key, paramInputs.value(key));
                }

            } else if (mCurParams.contains(key))
            {
                //If we're not deviated from default value, but it is currently set in the account
                //params, then we add it to the "unset" list
                unsetParams.append(key);
            }
        } else
        {
            qWarning() << QString("Trying to manipulate param that isn't valid for this proto: %1/%2!").arg(key, mCurProto);
        }
    }

    return QPair<QVariantMap, QStringList>(setParams, unsetParams);
}

bool TpAcctSetupWidget::validateParams()
{
    //TODO...
    //Maybe on failure this could highlight the invalid params?
    return true;
}

void TpAcctSetupWidget::updateAccount(Acct *acct)
{
    QPair<QVariantMap, QStringList> params = getParams();
    if (params.first.contains("resource"))
    {
        qDebug() << QString("Dropping resource: ") << params.first.value("resource");
        params.first.remove("resource");
    }
    if (edtEditName && !edtEditName->text().isEmpty())
        acct->setDisplayName(edtEditName->text());
    acct->setEnabled(btnEnabled->isChecked());
    acct->updateTpParameters(params.first, params.second);
}

void TpAcctSetupWidget::createAccount()
{
    QPair<QVariantMap, QStringList> params = getParams();
    QString displayName = getDefaultDisplayName(params.first);
    QVariantMap props;
//    qDebug() << "Supported account properties:\n" << tpAM->supportedAccountProperties();
    props.insert("org.freedesktop.Telepathy.Account.Enabled", QVariant(btnEnabled->isChecked()));
    props.insert("org.freedesktop.Telepathy.Account.Icon", QVariant(this->GetProtoTypeIcon(mCurProto)));
    if (params.first.contains("resource"))
    {
        qDebug() << QString("Dropping resource: ") << params.first.value("resource");
        params.first.remove("resource");
    }

    AccountManager::getInstance()->createAccount(mProtoInfo->cmName(), mProtoInfo->name(), displayName, params.first, props);
}

QString TpAcctSetupWidget::getDefaultDisplayName(QVariantMap params)
{
    if (params.contains("account"))
    {
        if (mCurProto == qtTrId("proto_irc"))
        {
            if (params.contains("server")) {
                //% "%1 on %2 - 1 is username, 2 is server"
                return qtTrId("account_name_irc_with_server").arg(params.value("account").toString(), params.value("server").toString());
            } else {
                //% "IRC (%1) - 1 is username"
                return qtTrId("account_name_irc_no_server").arg(params.value("account").toString());
            }
        } else if (mCurProto == qtTrId("proto_facebook_chat"))
        {
            QString fbAcct = params.value("account").toString();
            if (fbAcct.contains(FACEBOOK_CHAT_ACCOUNT_SUFFIX))
                fbAcct.chop(fbAcct.size() - fbAcct.indexOf(FACEBOOK_CHAT_ACCOUNT_SUFFIX));
            //% "Facebook (%1) - 1 is facebook acct name"
            return qtTrId("account_name_facebook").arg(fbAcct);
        } else
        {
            return params.value("account").toString();
        }
    } else if (mCurProto == qtTrId("proto_people_nearby"))
    {
        //% "People Nearby Account"
        return qtTrId("account_name_people_nearby");
    } else
    {
        qWarning("Trying to get chat-tp default display name with no account param!");
        //% "New %1 Account - 1 is protocol"
        return qtTrId("account_name_no_account_param").arg(mCurProto);
    }
}

void TpAcctSetupWidget::onBtnAdvancedClicked()
{
    int idx = layout->indexOf(dswAdvanced);
    if (idx != -1)
    {
        dswAdvanced->hide();
        layout->takeAt(idx);
    } else
    {
        policy->addItem(dswAdvanced, policy->count(), 0, 1, 2, Qt::AlignLeft);
        dswAdvanced->show();
    }
}

void TpAcctSetupWidget::onLinkActivated(QString url)
{
    QDesktopServices::openUrl(url);
}

QVariant TpAcctSetupWidget::safeParamGet(QString param)
{
    if (mCurParams.contains(param))
        return mCurParams.value(param);
    else if (mProtoParams.contains(param))
        return mProtoParams.value(param)->defaultValue();
    else
        return QVariant();
}
