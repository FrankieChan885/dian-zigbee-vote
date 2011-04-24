/**
* @file slidefonteditor.cpp
* @brief the QSlideFontEditor implementation file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-23
*/
#include <QtGui>

#include "slidefonteditor.h"
#include "slidetextitem.h"

QSlideFontEditor::QSlideFontEditor(QObject *text, QWidget *parent,
                                   Qt::WindowFlags f)
        : QFrame(parent, f)
{
    textItem = qobject_cast<QSlideTextItem *>(text);
    createCombos();
    createButtons();
    mergeWidgets();
}

/**
* @brief this function create the buttons in this widget.
*       and thanks for QT Nokia, cause this is copied from theirs.
*/
void QSlideFontEditor::createButtons() {
    boldButton = new QPushButton(this);
    boldButton->setToolTip(tr("Bold"));
    boldButton->setCheckable(true);
    boldButton->setIcon(QIcon(":/res/bold.png"));
    boldButton->setShortcut(tr("Ctrl+B"));
    connect(boldButton, SIGNAL(clicked()),
            this, SLOT(handleFontChange()));

    italicButton = new QPushButton(this);
    italicButton->setToolTip(tr("Italic"));
    italicButton->setCheckable(true);
    italicButton->setIcon(QIcon(":/res/italic.png"));
    italicButton->setShortcut(tr("Ctrl+I"));
    connect(italicButton, SIGNAL(clicked()),
            this, SLOT(handleFontChange()));

    underlineButton = new QPushButton(this);
    underlineButton->setCheckable(true);
    underlineButton->setIcon(QIcon(":/res/underline.png"));
    underlineButton->setToolTip(tr("Underline"));
    underlineButton->setShortcut(tr("Ctrl+U"));
    connect(underlineButton, SIGNAL(clicked()),
            this, SLOT(handleFontChange()));
}

/**
* @brief this function create the combo box in this widget.
*       and thanks for QT Nokia, cause this is copied from theirs.
*/
void QSlideFontEditor::createCombos() {
    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()),
                                                 Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(
    ":/res/text.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textButtonTriggered()));
}

void QSlideFontEditor::mergeWidgets() {
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(boldButton);
    buttonLayout->addWidget(underlineButton);
    buttonLayout->addWidget(italicButton);
    buttonLayout->addWidget(fontColorToolButton);
    buttonLayout->addWidget(fontSizeCombo);

    QHBoxLayout *comboLayout = new QHBoxLayout;
    comboLayout->addWidget(fontCombo);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(buttonLayout);
    layout->addLayout(comboLayout);

    setLayout(layout);
}

/**
 * @brief when font combo box selected an font, this
 *      function will be call.
 */
void QSlideFontEditor::currentFontChanged(const QFont &) {
    handleFontChange();
}

/**
 * @brief this is the font size edit function.
 */
void QSlideFontEditor::fontSizeChanged(const QString &) {
    handleFontChange();
}

/**
 * @brief this is the actual font edit function.
 */
void QSlideFontEditor::handleFontChange() {
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldButton->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicButton->isChecked());
    font.setUnderline(underlineButton->isChecked());

    textItem->setFont(font);
    textItem->update();
}

/**
 * @brief this is the font color edit function.
 *       and thanks for QT Nokia, cause this is copied from qtdemo.
 */
void QSlideFontEditor::textColorChanged() {
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                ":/res/text.png",
                qVariantValue<QColor>(textAction->data())));
    textButtonTriggered();
}
void QSlideFontEditor::textButtonTriggered()
{
    textItem->setDefaultTextColor(qVariantValue<QColor>(textAction->data()));
    textItem->update();
}

/**
* @brief this function create the color menu for color tool button.
*       and thanks for QT Nokia, cause this is copied from qtdemo.
*/
QMenu *QSlideFontEditor::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue")
          << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setIcon(createColorIcon(colors.at(i)));
        action->setData(colors.at(i));
        connect(action, SIGNAL(triggered()),
                this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }

    return colorMenu;
}

/**
* @brief this function create the color icon for color tool button.
*       and thanks for QT Nokia, cause this is copied from qtdemo.
*/
QIcon QSlideFontEditor::createColorToolButtonIcon(const QString &imageFile,
                        QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    QRect target(0, 0, 50, 60);
    QRect source(0, 0, 42, 42);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);

    return QIcon(pixmap);
}

/**
* @brief this function create the color icon for color menu.
*       and thanks for QT Nokia, cause this is copied from qtdemo.
*/
QIcon QSlideFontEditor::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    pixmap.fill(color);

    return QIcon(pixmap);
}

/* Copyright (C) 
* 2011 - Tankery Chen @ Dian Group
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/

