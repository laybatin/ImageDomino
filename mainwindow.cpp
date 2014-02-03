#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QImage>
#include <QPixmap>
#include <QImageReader>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QModelIndex>
#include <QDebug>
#include <QShortcut>
#include <QFileDialog>
#include <QFileSystemWatcher>
#include <QLabel>
#include "settingdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    scene(new QGraphicsScene),
    dir_model_(new QFileSystemModel()),
    files_(new QFileSystemModel()),
    files_watcher_(new QFileSystemWatcher),
    file_model_(new QStandardItemModel()),
    img_queue_model_(new QStandardItemModel()),
    result(NULL),
    img_spacing_(0),
    img_margin_(0),
    status_text_("Select")
{
    ui->setupUi(this);
    statusBar()->addWidget(&status_text_);

    //QGraphicsView 관련변수 초기화
    //scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);



    //dir_model_ 초기화
    QString path = "./";
    //dir_model_ = new QFileSystemModel();
    dir_model_->setFilter(QDir::NoDotAndDotDot | QDir::Dirs);
    dir_model_->setRootPath(path);
    ui->treeView->setModel(dir_model_);
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);

    //
    //files_watcher_ = new QFileSystemWatcher;

    //file_model_ 초기화
    for(unsigned int i=0; i<kExtName::CNT; i++) {
        ext_lists_ << kExtName::STRING[i];
    }
    //files_ = new QFileSystemModel();
    //file_model_ = new QStandardItemModel();
    files_->setFilter(QDir::NoDotAndDotDot | QDir::Files);
    files_->setNameFilters(ext_lists_);
    files_->setNameFilterDisables(false);
    ui->fileView->setModel(file_model_);
    ui->fileView->setViewMode(QListView::IconMode);
    ui->fileView->setFlow(QListView::LeftToRight);
    ui->fileView->setGridSize(QSize(96,60));
    ui->fileView->setUniformItemSizes(true);
    ui->fileView->setEditTriggers(QAbstractItemView::NoEditTriggers);


    //qImageListWidget 초기화
    //img_queue_model_ = new QStandardItemModel();
    ui->qImgListWidget->setViewMode(QListView::IconMode);
    ui->qImgListWidget->setGridSize(QSize(170,115));
    ui->qImgListWidget->setFlow(QListView::LeftToRight);
    ui->qImgListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->qImgListWidget->setWrapping(false);

    ui->qImgListWidget->setIconSize(QSize(161,100));
    ui->qImgListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->qImgListWidget->setResizeMode(QListView::Adjust);
    ui->splitter->setStretchFactor(1,2);


    //result = new QImage;

    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->qImgListWidget);
    connect(shortcut, SIGNAL(activated()), this, SLOT(SelectedItemDelete()));    
    connect(files_, SIGNAL(directoryLoaded(QString)), this, SLOT(DirFilesFilter(QString)));
    connect(ui->fileView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EnqueueImgModel(QModelIndex)));
    connect(file_model_, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(FileViewUpdate(QModelIndex)));
    connect(ui->qImgListWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(MakeImage()));
    connect(files_watcher_, SIGNAL(directoryChanged(QString)), this ,SLOT(chnageDir(QString)));
    ui->actionSave->setDisabled(true);


}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * DirFilesFilter
 * QFileSystemModel의 버그(캐시?) 인지 setNameFilter를 해도 계속해서 디렉토리나 파일이 보임
 * 그래서 디렉토리를 조회하여 직접 리스트를 만듬
 */
void MainWindow::DirFilesFilter(QString path) {
    qDebug() << "directoryLoaded";

    QModelIndex fileIdx = files_->index(path);
    int numRows = files_->rowCount(fileIdx);


    QImageReader imgReader;
    for (int index=0; index<numRows; index++) {
        QModelIndex mi = files_->index(index, 0, fileIdx);
        QFileInfo fileInfo = files_->fileInfo(mi);
        imgReader.setFileName(fileInfo.absoluteFilePath());
        if(imgReader.canRead())
            extfilter_imgs_ << fileInfo;
    }


    //extfilter_imgs 파일들을 모델로 만든다.
    FilesModelUpdate();

}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    files_watcher_->removePath(current_dirpath_);
    QString sPath = dir_model_->fileInfo(index).absoluteFilePath();


    qDebug() << "path = " << sPath;
    QModelIndex fileIdx = files_->setRootPath(sPath);        //파일모델 업데이트
    current_dirpath_ = sPath;

    //watcher 업데이트
    files_watcher_->addPath(sPath);
}


/*
 * FilesModelUpdate
 * extfilter_imgs_ 에 들어있는 파일들을 실제로 보여지도록 만든다.
 * 조그만 아이콘으로 만들기 위한 작업
 */
void MainWindow::FilesModelUpdate() {
    QFileInfoList::const_iterator iter;

    file_model_->clear();

    for (iter=extfilter_imgs_.begin(); iter!=extfilter_imgs_.end(); ++iter) {
        qDebug() << iter->absoluteFilePath();
        QImage img_buffer(iter->absoluteFilePath());
        QIcon icon = GetThumnail(&img_buffer);

        QStandardItem* item;
        if(!icon.isNull()) {
            item = new QStandardItem(icon,iter->fileName());
            file_model_->appendRow(item);
        }
    }


    extfilter_imgs_.clear();;

}

QIcon MainWindow::GetThumnail(QImage* src,int width, int height) {

    QPixmap buffer;
    buffer = QPixmap::fromImage(*src);
    buffer = buffer.scaled(width,height);

    return QIcon(buffer);
}

/*
 * QGraphicsView에 보여질 이미지를 만든다.
 * 더블클릭 시 파일을 읽고 image와 icon화 해서 저장해 놓는다.
 */

void MainWindow::EnqueueImgModel(QModelIndex idx) {



    QString dir_path = current_dirpath_;
    QString file_name = idx.data().toString();
    QString ico_path = dir_path.append("\\").append(file_name);

    QImage* img_buffer = new QImage(ico_path);
    img_list_ << img_buffer;



    int xWidth=150, xHeight=100;

    double src_width = img_buffer->width();
    double src_height = img_buffer->height();

    xHeight = src_height / src_width * xWidth;

    qDebug() << xWidth << "*" << xHeight;


    QIcon icon = GetThumnail(img_buffer, xWidth, xHeight);

    QListWidgetItem* item = new QListWidgetItem(icon, file_name);
    ui->qImgListWidget->addItem(item);


    //이미지 붙이는 작업
    MakeImage();
}

/*
 * FileViewUpdate
 * Item이 하나 들어갈때마다 View가 업데이트 되도록 한다.
 * 쓰레드를 사용하지 않고 할 수 있는 방법
 */

void MainWindow::FileViewUpdate(QModelIndex) {
   ui->fileView->viewport()->update();
   QCoreApplication::processEvents();
}

/*
 * SelectedItemDelete
 * Delete키를 누르면 qImageListWidget의 선택된 아이템이 삭제된다.
 */

void MainWindow::SelectedItemDelete() {
    QListWidgetItem* item = ui->qImgListWidget->currentItem();
    int index = ui->qImgListWidget->currentRow();

    if (item != NULL) {
        QImage* img = img_list_.at(index);
        img_list_.removeAt(index);
        delete img;
        delete item;
    }

    ui->qImgListWidget->clearSelection();

    //이미지를 다시 그린다.
    MakeImage();

}


/*
 * MakeImage
 * 실제 QGraphicsView에 보여질 붙인 이미지를 만든다.
 * 해상도 고려를 하지 않았음
 *
 * Todo
 *  - 해상도가 파일별로 다를시 처리
 */

void MainWindow::MakeImage() {
    if (img_list_.size())
        ui->actionSave->setDisabled(false);
    else {
        ui->actionSave->setDisabled(true);
        scene->clear();
        return;
    }


    scene->clear();
    if (result != NULL)
        delete result;

    int buffer_width = 0;
    int buffer_height = 0;
    QImage* img_buffer;
    for(int k=0; k<img_list_.size(); k++) {
        img_buffer = img_list_.at(k);
        buffer_width = img_buffer->width();
        buffer_height = buffer_height + img_buffer->height() + img_spacing_;
    }

    buffer_height -= img_spacing_;
    buffer_width += img_margin_;

    result = new QImage(buffer_width, buffer_height, QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(result);
    painter.fillRect(result->rect(),Qt::white);

    int temp_height = 0;
    for (int i=0; i<img_list_.size(); i++) {
        img_buffer = img_list_.at(i);
        painter.drawImage(0, temp_height, *img_buffer);
        temp_height += img_buffer->height();
        if (img_spacing_ > 0)
                temp_height += img_spacing_;
    }
    QPixmap buf = QPixmap::fromImage(*result);
    scene->addPixmap(buf);
    scene->setSceneRect(0,0, buffer_width, buffer_height);
    QRectF p = scene->sceneRect();    

}


void MainWindow::on_actionSave_triggered()
{
    QFileDialog dlg;
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    QString fileName = dlg.getSaveFileName();
    qDebug() << fileName;

    result->save(fileName);
}

void MainWindow::chnageDir(QString path) {

    qDebug() << "Change Dir " << path;
}

void MainWindow::on_actionSetting_triggered()
{
    SettingDialog dialog;
    dialog.SetMargin(img_margin_);
    dialog.SetSpacing(img_spacing_);
    int dialogResult = dialog.exec();



    if(dialogResult == QDialog::Accepted) {
        img_spacing_ = dialog.GetSpacing();
        img_margin_ = dialog.GetMargin();
    }



}

