#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QFileSystemWatcher>
#include <QLabel>
namespace Ui {
class MainWindow;
}
namespace kExtName {

enum
{
    BEGIN,
    jpg = BEGIN,
    png,
    gif,
    bmp,
    END = bmp,
    CNT = END + 1
};

const QString STRING[CNT]=
{
    "*.jpg", "*.png", "*.gif", "*.bmp"
};
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void FilesModelUpdate();
    QIcon GetThumnail(QImage*, int width=128, int height=128);


private slots:
    void DirFilesFilter(QString);
    void EnqueueImgModel(QModelIndex);
    void on_treeView_clicked(const QModelIndex &index);
    void FileViewUpdate(QModelIndex);
    void SelectedItemDelete();
    void MakeImage();
    void chnageDir(QString);
    void on_actionSave_triggered();



    void on_actionSetting_triggered();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;

    //status bar
    QLabel status_text_;
    QString current_dirpath_;
    QFileSystemModel* dir_model_;
    QFileSystemModel* files_;
    QFileSystemWatcher* files_watcher_;
    QStandardItemModel * file_model_;
    QFileInfoList extfilter_imgs_;
    QStringList ext_lists_;
    QImage* result;
    //실제이미지를 저장
    QList<QImage*> img_list_;

    //listwidget에 보여질 모델
    QStandardItemModel* img_queue_model_;


    //Image Setting value
    int img_spacing_;
    int img_margin_;




};

#endif // MAINWINDOW_H
