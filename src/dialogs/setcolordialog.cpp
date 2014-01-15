/*******************************************************************

Part of the Fritzing project - http://fritzing.org
Copyright (c) 2007-2012 Fachhochschule Potsdam - http://fh-potsdam.de

Fritzing is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fritzing is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Fritzing.  If not, see <http://www.gnu.org/licenses/>.

********************************************************************

$Revision: 6904 $:
$Author: irascibl@gmail.com $:
$Date: 2013-02-26 16:26:03 +0100 (Di, 26. Feb 2013) $

********************************************************************/

#include "setcolordialog.h"
#include "../debugdialog.h"
#include "../utils/clickablelabel.h"

#include <QLabel>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QPalette>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

static const int MARGIN = 5;
static const int BUTTON_WIDTH = 110;

/////////////////////////////////////

SetColorDialog::SetColorDialog(const QString & message, QColor & currentColor, QColor & standardColor, bool askPrefs, QWidget *parent) : QDialog(parent) 
{
	m_prefsCheckBox = NULL;
	m_message = message;
	m_currentColor = m_selectedColor = currentColor;
	m_standardColor = standardColor;

	this->setWindowTitle(tr("Set %1 Color...").arg(message));
    this->setObjectName("colorDialogWindow");

	QVBoxLayout * vLayout = new QVBoxLayout(this);

	QLabel * label = new QLabel(tr("Choose a new %1 color.").arg(message.toLower()));
	vLayout->addWidget(label);

	QFrame * f1 = new QFrame();
	QHBoxLayout * hLayout1 = new QHBoxLayout(f1);
	QPushButton *button1 = new QPushButton(tr("Choose"));
	button1->setFixedWidth(BUTTON_WIDTH);
	connect(button1, SIGNAL(clicked()), this, SLOT(selectCurrent()));
	hLayout1->addWidget(button1);
	m_currentColorLabel = new ClickableLabel(tr("current %1 color (%2)").arg(message.toLower()).arg(currentColor.name()), this);
	connect(m_currentColorLabel, SIGNAL(clicked()), this, SLOT(selectCurrent()));
	m_currentColorLabel->setPalette(QPalette(currentColor));
	m_currentColorLabel->setAutoFillBackground(true);
	m_currentColorLabel->setMargin(MARGIN);
	hLayout1->addWidget(m_currentColorLabel);
	vLayout->addWidget(f1);

	QFrame * f2 = new QFrame();
	QHBoxLayout * hLayout2 = new QHBoxLayout(f2);
	QPushButton *button2 = new QPushButton(tr("Choose"));
	button2->setFixedWidth(BUTTON_WIDTH);
	connect(button2, SIGNAL(clicked()), this, SLOT(selectStandard()));
	hLayout2->addWidget(button2);
	m_standardColorLabel = new ClickableLabel(tr("standard %1 color (%2)").arg(message.toLower()).arg(standardColor.name()), this);
	connect(m_standardColorLabel, SIGNAL(clicked()), this, SLOT(selectStandard()));
	m_standardColorLabel->setPalette(QPalette(standardColor));
	m_standardColorLabel->setAutoFillBackground(true);
	m_standardColorLabel->setMargin(MARGIN);
	hLayout2->addWidget(m_standardColorLabel);
	vLayout->addWidget(f2);

	QFrame * f3 = new QFrame();
	QHBoxLayout * hLayout3 = new QHBoxLayout(f3);
	QPushButton *button3 = new QPushButton(tr("Custom color ..."));
	button3->setFixedWidth(BUTTON_WIDTH);
	connect(button3, SIGNAL(clicked()), this, SLOT(selectCustom()));
	hLayout3->addWidget(button3);
	m_customColorLabel = new ClickableLabel("", this);
	connect(m_customColorLabel, SIGNAL(clicked()), this, SLOT(selectLastCustom()));
	setCustomColor(currentColor);
	m_customColorLabel->setMargin(MARGIN);
	hLayout3->addWidget(m_customColorLabel);
	vLayout->addWidget(f3);

	QSpacerItem * spacerItem = new QSpacerItem( 0, 10 );
	vLayout->addSpacerItem(spacerItem);

	QFrame * f4 = new QFrame();
	QHBoxLayout * hLayout4 = new QHBoxLayout(f4);
	m_selectedColorLabel = new QLabel();
	setColor(currentColor);
	hLayout4->addWidget(m_selectedColorLabel);
	m_selectedColorLabel->setMargin(4);
	vLayout->addWidget(f4);

	if (askPrefs) {
		QFrame * f5 = new QFrame();
		QHBoxLayout * hLayout5 = new QHBoxLayout(f5);
		m_prefsCheckBox = new QCheckBox(tr("Make this the default %1 color").arg(message.toLower()));
		hLayout5->addWidget(m_prefsCheckBox);
		vLayout->addWidget(f5);
	}

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
	buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	vLayout->addWidget(buttonBox);

	this->setLayout(vLayout);
}

SetColorDialog::~SetColorDialog() {
}

const QColor & SetColorDialog::selectedColor() {
	return m_selectedColor;
}

void SetColorDialog::selectCurrent() {
	setColor(m_currentColor);
}

void SetColorDialog::selectStandard() {
	setColor(m_standardColor);
}

void SetColorDialog::selectLastCustom() {
	setColor(m_customColor);
}

void SetColorDialog::selectCustom() {
	QColor color = QColorDialog::getColor ( m_selectedColor, this, tr("Select custom %1 color").arg(m_message.toLower()));
	if (!color.isValid()) return;

	setColor(color);
	setCustomColor(color);
}

bool SetColorDialog::isPrefsColor() {
	if (m_prefsCheckBox == NULL) return false;

	return m_prefsCheckBox->isChecked();
}

void SetColorDialog::setCustomColor(const QColor & color) {
	m_customColor = color;
	m_customColorLabel->setText(tr("custom color (%1)").arg(color.name()));
	m_customColorLabel->setPalette(QPalette(color));
	m_customColorLabel->setAutoFillBackground(true);
}

void SetColorDialog::setColor(const QColor & color) {
	m_selectedColor = color;
	m_selectedColorLabel->setText(tr("selected color (%1)").arg(color.name()));
	m_selectedColorLabel->setPalette(QPalette(color));
	m_selectedColorLabel->setAutoFillBackground(true);
}

