/**
* @file slidefontbar.h
* @brief the QSlideFontBar definition file.
* @author Tankery Chen @ Dian Group
* @version 1.0.0
* @date 2011-04-27
*/
#ifndef __SLIDEFONTBAR_H_
#define __SLIDEFONTBAR_H_

#include <QIcon>
#include <QWidget>

class QToolBar;
class QToolButton;
class QComboBox;
class QFontComboBox;
class QFont;
class QAction;
class QColor;
class QString;
class QMenu;

/**
* @brief this class is a manager of text font (including color).
*       the action of bar will be created in constructer.
*       after instantiated this class, you can use \getFontToolBar
*       and other similar function to get the toolbar or menubar
*       with the defined font & color actions.
*/
class QSlideFontBar : public QObject
{
    Q_OBJECT
public:
    QSlideFontBar(QWidget *parent = 0);

    /**
     * @brief add the font, color toolbar or menubar to main window.
     */
    QToolBar *addToolBar(const QString& title = QString());
    QMenu *addMenu(const QString& title = QString());

    /**
    * @brief set font to font bar
    */
    void setFont(const QFont &font);

signals:
    /**
     * @brief emit when any font property have changed.
     */
    void fontChanged(const QFont &font);
    /**
     * @brief emit when any color property have changed.
     */
    void colorChanged(const QColor &color);

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
    void textActionTriggered();

private:
    void createActions();
    void createCombos();

    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(const QString &imageFile,
                        QColor color);
    QIcon createColorIcon(QColor color);

    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QToolButton *fontColorToolButton;
    QAction *colorAction;

    QFontComboBox *fontCombo;
    QComboBox *fontSizeCombo;
};

#endif // __SLIDEFONTBAR_H_

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
