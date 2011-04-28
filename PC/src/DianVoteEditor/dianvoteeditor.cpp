/**
 * dianvoteeditor.h
 *
 * DianVoteEditor main window definition file.
 *
 * Author: tankery.chen@gmail.com
 * Modified:
 *      date        author      action
 *  ------------------------------------------------------------
 *      2011-03-15  tankery     created file
 *
 * Copyright (c) Tankery Chen 2011 @ Dian Group
 */
#include <QtGui>
#include <QUiLoader>
#include <QModelIndex>

#include "../utilities/exceptions.h"
#include "../utilities/slidemodel.h"
#include "slideeditorscene.h"
#include "dianvoteeditor.h"

DianVoteEditor::DianVoteEditor()
        : dianvoteWindow(0)
        , currentSlideScene(0)
        , slidesListModel(0)
{
}

DianVoteEditor::~DianVoteEditor()
{

}

// clean the resources.
void DianVoteEditor::clean() {
    if (slidesListModel) {
        delete slidesListModel;
        slidesListModel = 0;
    }
    foreach (QSlideScene *scene, slideScenes) {
        if (scene) {
            delete scene;
            scene = 0;
        }
    }
    slideScenes.clear();
}

// setup the user interface using uiFile for the parent.
void DianVoteEditor::setupUi(const QString& uiFile, QWidget *parent)
{
    QUiLoader uiLoader(this);
    QFile uif(uiFile);
    if (!uif.open(QIODevice::ReadOnly)) {
        throw new DianVoteException(DianVoteException::UI_FILE_NOTFOUND);
    }
    dianvoteWindow = (QMainWindow*) uiLoader.load(&uif);
    uif.close();
    if (dianvoteWindow == 0) {
        throw new DianVoteException(DianVoteException::UI_LOAD_FAILED);
    }

    setupActions();
    newSlide();

    //set slide view with scene.
    QGraphicsView *view = new QGraphicsView(currentSlideScene, dianvoteWindow);
    view->setFrameShape(QFrame::NoFrame);
    // set the view to this window's central widget.
    dianvoteWindow->setCentralWidget(view);

    // set size.
    dianvoteWindow->setGeometry(50, 50, 800, 600);
    // set parent to the specific one.
    dianvoteWindow->setParent(parent);
}

// setup the main window actions.
void DianVoteEditor::setupActions()
{
    // add a quit action.
    QAction *action = dianvoteWindow->findChild<QAction *>(tr("action_Quit"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupActions(): action_Quit not found...");
    }
    else {
        action->setShortcuts(QKeySequence::Quit);
        action->setStatusTip(tr("Quit DianVote Editor"));
        QObject::connect(action, SIGNAL(triggered()), dianvoteWindow, SLOT(close()));
    }

    // add new action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_New"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupActions(): action_New not found...");
    }
    else {
        action->setShortcuts(QKeySequence::New);
        action->setStatusTip(tr("New a slide file"));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(newSlide()));
    }

    // add open action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_Open"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupActions(): action_Open not found...");
    }
    else {
        action->setShortcuts(QKeySequence::Open);
        action->setStatusTip(tr("Open a slide file"));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(openSlide()));
    }

    // add save action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_Save"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupActions(): action_Save not found...");
    }
    else {
        action->setShortcuts(QKeySequence::Save);
        action->setStatusTip(tr("Save DianVote Slide file."));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(saveSlide()));
    }
}

// Update the slide list by slideScenes.
void DianVoteEditor::updateSlideList() {
    if (slidesListModel == 0) {
        slidesListModel = new QStandardItemModel(dianvoteWindow);
    }
    // clear list.
    slidesListModel->clear();
    // append all slides title to slide list.
    foreach (QSlideScene *scene, slideScenes) {
        slidesListModel->appendRow(new QStandardItem(
                scene->model()->getPlainTopic()));
    }
    // update list view.
    QListView *listView = dianvoteWindow->
                          findChild<QListView*>(tr("slideListView"));
    if (listView) {
        listView->setModel(slidesListModel);
        QObject::connect(listView, SIGNAL(clicked(QModelIndex)),
                         this, SLOT(slideListClicked(QModelIndex)));
    }
    else {
        qDebug("listView not found...");
    }
}

// set application's style using style sheet.
void DianVoteEditor::setStyle(const QString& qssFile)
{
    QFile qss(qssFile);
    if (qss.open(QFile::ReadOnly)) {
        qApp->setStyleSheet(QLatin1String(qss.readAll()));
        qss.close();
    }
    else {
        throw new DianVoteException(DianVoteException::QSS_FILE_NOTFOUND);
    }
}

void DianVoteEditor::show()
{
    // show it!
    return dianvoteWindow->show();
}

/**
* @brief manage the file processing.
* @param isSplit split the file to many slide or merge
*       merge many slides to a file.
*/
void DianVoteEditor::fileProcessing(QFile &xmlFile, bool isSplit) {
    QString content;
    if (xmlFile.isOpen()) {
        if (isSplit) {
            content = xmlFile.readAll();

            QDomDocument allSlides;
            QString errorStr("");
            int line, column;
            if (!allSlides.setContent(content, false,
                                      &errorStr, &line, &column)) {
                throw new XmlStreamException(
                        errorStr.toStdString(), line, column);
            }
            // get the slide nodes.
            QDomNodeList nodes =
                    allSlides.documentElement().elementsByTagName(tr("slide"));
            // set each node to the scene list.
            for(int i= 0; i < nodes.size(); ++i) {
                QSlideModel *model = 0;
                QSlideScene *scene = 0;
                QString oneSlide;
                QTextStream slideStream(&oneSlide);
                nodes.at(i).save(slideStream, 1);
                model = new QSlideModel(oneSlide);
                scene = new QSlideEditorScene(dianvoteWindow, model);
                slideScenes.append(scene);
            }
        } // if (isSpliy)
        else {
            // create an empty dom document with root node <dianvote/>
            QDomDocument allSlides;
            QDomNode rootNode = allSlides.createElement(tr("dianvote"));
            allSlides.appendChild(rootNode);

            // append all slides to dom document.
            foreach (QSlideScene *scene, slideScenes) {
                rootNode.appendChild(*scene->model()->getNode());
            }

            // write to xml file.
            xmlFile.write(allSlides.toString().toAscii());

        } // if (isSplit) else
    }
}



void DianVoteEditor::saveSlide() {
    QString fileName
            = QFileDialog::getSaveFileName(dianvoteWindow, tr("Save slide"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    if (file.isOpen()) {
        try {
            fileProcessing(file, false);
            file.close();
        } catch (XmlStreamException *xse) {
            QMessageBox::critical(0, "error", xse->what());
            file.close();
            return;
        }
    }
    else {
        QString msg = tr("QSlideScene::save(): Failed to open %1\n%2")
                      .arg(fileName)
                      .arg(file.errorString());
        qDebug(msg.toAscii().data());
    }
}

void DianVoteEditor::newSlide() {
    // first clean the resources.
    clean();

    // then, new a slide scene.
    // set the slide scene with gray surround the white.
    QFile file(":/slidetemplate/default.xml");
    file.open(QIODevice::ReadOnly);

    QSlideModel *model = file.isOpen() ?
                         new QSlideModel(file.readAll(), dianvoteWindow) :
                         QSlideModel::createModel(dianvoteWindow);
    currentSlideScene = new QSlideEditorScene(dianvoteWindow, model);
    QPixmap pixmap(QSize(800, 600));
    pixmap.fill(Qt::white);
    currentSlideScene->setBackgroundPixmap(pixmap);
    currentSlideScene->setBackgroundBrush(Qt::darkGray);
    slideScenes.append(currentSlideScene);

    updateSlideList();
}

void DianVoteEditor::openSlide() {
    QString fileName
            = QFileDialog::getOpenFileName(dianvoteWindow, tr("Save slide"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    if (!file.isOpen()) {
        QString msg = tr("QSlideScene::open(): Failed to open %1\n%2")
                      .arg(fileName)
                      .arg(file.errorString());
        qDebug(msg.toAscii().data());
        return;
    }

    // clean the resources.
    clean();
    try {
        fileProcessing(file, true);
        file.close();
        currentSlideScene = slideScenes.at(0);
        qobject_cast<QGraphicsView*>(dianvoteWindow->centralWidget())->
                setScene(currentSlideScene);
    } catch (XmlStreamException *xse) {
        QMessageBox::critical(0, "error", xse->what());
        file.close();
        return;
    }

    // update slide list..
    updateSlideList();
}

void DianVoteEditor::slideListClicked(const QModelIndex & index) {
        currentSlideScene = slideScenes.at(index.row());
        qobject_cast<QGraphicsView*>(dianvoteWindow->centralWidget())->
                setScene(slideScenes.at(index.row()));
}
