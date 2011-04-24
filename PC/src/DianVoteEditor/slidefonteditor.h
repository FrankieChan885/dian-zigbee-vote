/**
* @file slidefonteditor.h
* @brief the QSlideFontEditor definition file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-23
*/
#ifndef SLIDEFONTEDITOR_H
#define SLIDEFONTEDITOR_H

#include <QFrame>
#include <QIcon>

class QPushButton;
class QToolButton;
class QComboBox;
class QFontComboBox;
class QSlideTextItem;
class QAction;
class QColor;
class QString;
class QMenu;

class QSlideFontEditor : public QFrame
{
    Q_OBJECT
public:
    QSlideFontEditor(QObject *text, QWidget *parent = 0,
                     Qt::WindowFlags f = 0);

signals:
    /**
     * @brief emit when any font property have changed.
     */
    void fontChanged();

public slots:
    /**
     * @brief when font combo box selected an font, this
     *      function will be call.
     */
    void currentFontChanged(const QFont &font);
    /**
     * @brief this is the actual font edit function.
     */
    void handleFontChange();
    /**
     * @brief this is the font size edit function.
     */
    void fontSizeChanged(const QString &size);
    /**
     * @brief this is the font color edit function.
     */
    void textColorChanged();
    void textButtonTriggered();

    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &imageFile,
                        QColor color);
    QIcon createColorIcon(QColor color);

private:
    void createButtons();
    void createCombos();
    void mergeWidgets();

    QPushButton *boldButton;
    QPushButton *underlineButton;
    QPushButton *italicButton;
    QToolButton *fontColorToolButton;
    QAction *textAction;

    QFontComboBox *fontCombo;
    QComboBox *fontSizeCombo;

    QSlideTextItem *textItem;
};

#endif // SLIDEFONTEDITOR_H

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
