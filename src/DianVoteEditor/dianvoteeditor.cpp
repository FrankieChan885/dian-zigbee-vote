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

#include "../utilities/exceptions.h"
#include "../utilities/slidemodel.h"
#include "slideeditorscene.h"
#include "dianvoteeditor.h"

DianVoteEditor::DianVoteEditor()
        : dianvoteWindow(0)
        , isUntitled(true)
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

// setup the user interface using uiFile.
// the dianvoteWindow will be set to this window's
// central window.
void DianVoteEditor::setupUi(const QString& uiFile)
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

    //set slide view with scene.
    QGraphicsView *view = new QGraphicsView(dianvoteWindow);
    view->setFrameShape(QFrame::NoFrame);
    // set the view to this window's central widget.
    dianvoteWindow->setCentralWidget(view);

    setCentralWidget(dianvoteWindow);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("[*] DianVote Editor"));
    setCurrentFile(tr(""));

    isUntitled = true;

    setupActions();
    setupToolBars();
    setupStatusBar();

    readSettings();

    newFile();
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
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(close()));
    }

    // add new action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_New"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupActions(): action_New not found...");
    }
    else {
        action->setShortcuts(QKeySequence::New);
        action->setStatusTip(tr("New a slide file"));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(newFile()));
    }

    // add open action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_Open"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupActions(): action_Open not found...");
    }
    else {
        action->setShortcuts(QKeySequence::Open);
        action->setStatusTip(tr("Open a slide file"));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(open()));
    }

    // add save action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_Save"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupActions(): action_Save not found...");
    }
    else {
        action->setShortcuts(QKeySequence::Save);
        action->setStatusTip(tr("Save DianVote Slide file."));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(save()));
    }

    // add save as action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_SaveAs"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupActions(): action_SaveAs not found...");
    }
    else {
        action->setShortcuts(QKeySequence::SaveAs);
        action->setStatusTip(tr("Save as a new DianVote Slide file."));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(saveAs()));
    }

    // add about software action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_AboutSoftware"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupActions(): action_AboutSoftware not found...");
    }
    else {
        action->setStatusTip(tr("About this software"));
        QObject::connect(action, SIGNAL(triggered()), this, SLOT(aboutSoftware()));
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
        QObject::connect(scene, SIGNAL(itemsModified()),
                         this, SLOT(documentWasModified()));
    }
    // update list view.
    QListView *listView = dianvoteWindow->
                          findChild<QListView*>(tr("slideListView"));
    if (listView) {
        listView->setModel(slidesListModel);
        QObject::connect(listView, SIGNAL(activated(QModelIndex)),
                         this, SLOT(slideListActivated(QModelIndex)));
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

// the slides may be save at close.
void DianVoteEditor::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void DianVoteEditor::newFile()
{
    // if modified and cancel to save, cancel open.
    if (isWindowModified() && !maybeSave()) {
        return;
    }

    // first clean the resources.
    clean();

    // then, new a slide scene.
    // and append it to slide list.
    currentSlideScene = newSlide();
    slideScenes.append(currentSlideScene);

    qobject_cast<QGraphicsView*>(dianvoteWindow->centralWidget())->
            setScene(currentSlideScene);

    updateSlideList();
}

void DianVoteEditor::open()
{
    // if modified and cancel to save, cancel open.
    if (isWindowModified() && !maybeSave()) {
        return;
    }
    // start open new file.
    QString fileName = QFileDialog::getOpenFileName(dianvoteWindow);
    if (!fileName.isEmpty()) {
        loadFile(fileName);
        updateSlideList();
    }
}

bool DianVoteEditor::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool DianVoteEditor::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(dianvoteWindow, tr("Save As"),
                                                    curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void DianVoteEditor::aboutSoftware()
{
   QMessageBox::about(dianvoteWindow, tr("About DianVote Editor"),
            tr("The <b>DianVote Editor</b> is a application to edit the"
               " DianVote slide file, which is for the vote in class."
               " The DianVote slide file can stores many topic with some"
               " selections, this selections can be vote when play."
               " If you need more information, please contact <a http="
               "\"mailto:tankery.chen@gmail.com\">Tankery Chen</a> @"
               "<a http=\"www.dian.org.cn\">Dian Group</a>"));
}

void DianVoteEditor::documentWasModified()
{
    setWindowModified(true);
}

void DianVoteEditor::sceneAreaChanged(const QList<QRectF> &) {
    qDebug("scene area changed..");
    documentWasModified();
}

void DianVoteEditor::setupToolBars()
{
    QToolBar *fileToolBar = dianvoteWindow->addToolBar(tr("File"));

    QAction *action = 0;
    // add new action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_New"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupToolBars(): action_New not found...");
    }
    else {
        action->setIcon(QIcon(":/res/new.png"));
        fileToolBar->addAction(action);
    }

    // add open action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_Open"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupToolBars(): action_Open not found...");
    }
    else {
        action->setIcon(QIcon(":/res/open.png"));
        fileToolBar->addAction(action);
    }

    // add save action.
    action = dianvoteWindow->findChild<QAction *>(tr("action_Save"));
    if (action == 0) {
        qDebug("DianVoteEditor::setupToolBars(): action_Save not found...");
    }
    else {
        action->setIcon(QIcon(":/res/save.png"));
        fileToolBar->addAction(action);
    }

//    // add a edit toolbar.
//    QToolBar *editToolBar = addToolBar(tr("Edit"));
//    editToolBar->addAction(cutAct);
//    editToolBar->addAction(copyAct);
//    editToolBar->addAction(pasteAct);
}

void DianVoteEditor::setupStatusBar()
{
    dianvoteWindow->statusBar()->showMessage(tr("Ready"));
}

void DianVoteEditor::readSettings()
{
    QSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    move(pos);
    resize(size);
}

void DianVoteEditor::writeSettings()
{
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool DianVoteEditor::maybeSave()
{
    if (isWindowModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(dianvoteWindow, tr("DianVote"),
                     tr("The slides has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard
                     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool DianVoteEditor::loadFile(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(dianvoteWindow, tr("DianVote"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    // clear old slides.
    clean();

    try {
        // start processing the file.
        fileProcessing(file, true);
        file.close();
    } catch (XmlStreamException *xse) {
        QMessageBox::critical(0, "error", xse->what());
        file.close();
        return false;
    }
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    dianvoteWindow->statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

bool DianVoteEditor::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(dianvoteWindow, tr("DianVote"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    try {
        fileProcessing(file, false);
    } catch (XmlStreamException *xse) {
        QMessageBox::critical(0, "error", xse->what());
        file.close();
        return false;
    }
    QApplication::restoreOverrideCursor();

    file.close();
    setCurrentFile(fileName);
    dianvoteWindow->statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void DianVoteEditor::setCurrentFile(const QString &fileName)
{
    static int sequenceNumber = 1;

    isUntitled = fileName.isEmpty();
    if (isUntitled) {
        curFile = tr("slides%1.txt").arg(sequenceNumber++);
    } else {
        curFile = QFileInfo(fileName).canonicalFilePath();
        isUntitled = false;
    }

    setWindowModified(false);
    setWindowFilePath(curFile);
}

QString DianVoteEditor::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
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
                scene = new QSlideEditorScene(model, dianvoteWindow);
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



QSlideScene* DianVoteEditor::newSlide() {
    // set the slide scene with gray surround the white.
    QFile file(":/slidetemplate/default.xml");
    file.open(QIODevice::ReadOnly);

    // the model will be delete by scene, no need to set parent.
    QSlideModel *model = file.isOpen() ?
                         new QSlideModel(file.readAll()) :
                         QSlideModel::createModel();

    QSlideScene *scene = new QSlideEditorScene(model, dianvoteWindow);
    QPixmap pixmap(QSize(800, 600));
    pixmap.fill(Qt::white);
    scene->setBackgroundPixmap(pixmap);
    scene->setBackgroundBrush(Qt::darkGray);

    return scene;
}

void DianVoteEditor::slideListActivated(const QModelIndex & index) {
        currentSlideScene = slideScenes.at(index.row());
        qobject_cast<QGraphicsView*>(dianvoteWindow->centralWidget())->
                setScene(currentSlideScene);
}
