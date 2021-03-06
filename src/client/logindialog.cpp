#include <QDebug>
/*
   DrawPile - a collaborative drawing program.

   Copyright (C) 2007-2008 Calle Laakkonen

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include <QInputDialog>
#include "logindialog.h"

#include "ui_logindialog.h"

namespace dialogs {

LoginDialog::LoginDialog(QWidget *parent)
	: QDialog(parent), appenddisconnect_(false)
{
	ui_ = new Ui_LoginDialog;
	ui_->setupUi(this);
	ui_->progress->setMaximum(103);
}

LoginDialog::~LoginDialog()
{
	delete ui_;
}

/**
 * The message is set to the title message label and the window title
 * @param message title message to set
 */
void LoginDialog::setTitleMessage(const QString& message)
{
	setWindowTitle(message);
	ui_->titlemessage->setText(message);
}

/**
 * Show the dialog and display the connecting message.
 * The address is displayed
 * @param address remote host address
 * @param host if true, expecting to host a session
 */
void LoginDialog::connecting(const QString& address, bool host)
{
	host_ = host;
	if(host)
		setTitleMessage(tr("Hosting a drawing session"));
	else
		setTitleMessage(tr("Joining a drawing session"));
	ui_->statustext->setText(tr("Connecting to %1...").arg(address));
	ui_->progress->setValue(0);
	ui_->buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
	show();
}

/**
 * Connection established. Next step is the login handshake.
 */
void LoginDialog::connected()
{
	ui_->statustext->setText(tr("Logging in..."));
	ui_->progress->setValue(1);
}

/**
 * User is now logged in.
 */
void LoginDialog::loggedin()
{
	if(host_) {
		// When hosting, we don't need to download any raster data
		hide();
	} else {
		ui_->statustext->setText(tr("Downloading board contents..."));
		ui_->progress->setValue(2);
	}
}

void LoginDialog::error(const QString& message)
{
	qDebug() << "LoginDialog::error(" << message << ")";
	ui_->statustext->setText(message);
	appenddisconnect_ = true;
}

/**
 * If the dialog is still visible when this is shown, it means
 * the login/join/host sequence has failed.
 */
void LoginDialog::disconnected(const QString& message)
{
	if(appenddisconnect_) {
		ui_->statustext->setText(ui_->statustext->text() + "\n" + message);
		appenddisconnect_ = false;
	} else {
		ui_->statustext->setText(message);
	}
	ui_->progress->setValue(0);
	setTitleMessage(tr("Couldn't join a session"));
	disconnect(SIGNAL(rejected()));
	ui_->buttonBox->setStandardButtons(QDialogButtonBox::Close);
}

/**
 * Raster data download progresses. When progress hits 100, the download
 * sequence is complete and the dialog is hidden.
 * @param p progress percentage.
 */
void LoginDialog::raster(int p)
{
	ui_->progress->setValue(3+p);
	if(p>=100)
		hide();
}

/**
 * Request a password.
 */
void LoginDialog::getPassword()
{
	bool ok;
	const QString passwd = QInputDialog::getText(this,
			tr("Password required"),
			tr("Password:"),
			QLineEdit::Password,
			QString(),
			&ok);
	if(ok) {
		ui_->statustext->setText(tr("Sending password..."));
		emit password(passwd);
	} else {
		reject();
	}
}

}

