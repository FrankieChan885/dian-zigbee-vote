/**
* @file slidefontbar.cpp
* @brief the QSlideFontBar implementation file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-23
*/
#include <QtGui>

#include "slidefontbar.h"

QSlideFontBar::QSlideFontBar(QWidget *parent)
        : QObject(parent)
{
    createCombos();
    createActions();
}

/**
 * @brief add the font, color toolbar or menubar to main window.
 */
QToolBar *QSlideFontBar::addToolBar(const QString& title) {
    QToolBar * ToolBar = new QToolBar(title.isEmpty() ?
                                      tr("font") : title);
    ToolBar->addWidget(fontCombo);
    ToolBar->addWidget(fontSizeCombo);
    ToolBar->addAction(boldAction);
    ToolBar->addAction(italicAction);
    ToolBar->addAction(underlineAction);
    ToolBar->addWidget(fontColorToolButton);

    return ToolBar;
}
QMenu *QSlideFontBar::addMenu(const QString& title) {
    QMenu *menu = new QMenu(title.isEmpty() ?
                                      tr("font") : title);
    menu->addAction(boldAction);
    menu->addAction(italicAction);
    menu->addAction(underlineAction);
    // TODO: add font size and font family menu actions..

    return menu;
}

/**
* @brief set font to font bar
*/
void QSlideFontBar::setFont(const QFont &font) {
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    fontCombo->setCurrentFont(font);
}

/**
* @brief this function create the buttons in this widget.
*       and thanks for QT Nokia, cause this is copied from theirs.
*/
void QSlideFontBar::createActions() {
    boldAction = new QAction(this);
    boldAction->setToolTip(tr("Bold"));
    boldAction->setCheckable(true);
    boldAction->setIcon(QIcon(":/res/bold.png"));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()),
            this, SLOT(handleFontChange()));

    italicAction = new QAction(this);
    italicAction->setToolTip(tr("Italic"));
    italicAction->setCheckable(true);
    italicAction->setIcon(QIcon(":/res/italic.png"));
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()),
            this, SLOT(handleFontChange()));

    underlineAction = new QAction(this);
    underlineAction->setCheckable(true);
    underlineAction->setIcon(QIcon(":/res/underline.png"));
    underlineAction->setToolTip(tr("Underline"));
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()),
            this, SLOT(handleFontChange()));
}

/**
* @brief this function create the combo box in this widget.
*       and thanks for QT Nokia, cause this is copied from theirs.
*/
void QSlideFontBar::createCombos() {
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
    colorAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(
    ":/res/text.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()),
            this, SLOT(textActionTriggered()));
}

/**
 * @brief when font combo box selected an font, this
 *      function will be call.
 */
void QSlideFontBar::currentFontChanged(const QFont &) {
    handleFontChange();
}

/**
 * @brief this is the font size edit function.
 */
void QSlideFontBar::fontSizeChanged(const QString &) {
    handleFontChange();
}

/**
 * @brief this is the actual font edit function.
 */
void QSlideFontBar::handleFontChange() {
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    emit fontChanged(font);
}

/**
 * @brief this is the font color edit function.
 *       and thanks for QT Nokia, cause this is copied from qtdemo.
 */
void QSlideFontBar::textColorChanged() {
    colorAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(
                ":/res/text.png",
                qVariantValue<QColor>(colorAction->data())));
    textActionTriggered();
}

void QSlideFontBar::textActionTriggered()
{
    emit colorChanged(qVariantValue<QColor>(colorAction->data()));
}

/**
* @brief this function create the color menu for color tool button.
*       and thanks for QT Nokia, cause this is copied from qtdemo.
*/
QMenu *QSlideFontBar::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::gray << Qt::red << Qt::green
            << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("gray") << tr("red") << tr("green")
            << tr("blue") << tr("yellow");

    QMenu *colorMenu = new QMenu(fontColorToolButton);
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
QIcon QSlideFontBar::createColorToolButtonIcon(const QString &imageFile,
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
QIcon QSlideFontBar::createColorIcon(QColor color)
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

