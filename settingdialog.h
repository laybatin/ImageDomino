#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();
    int GetSpacing();
    int GetMargin();
    void SetSpacing(int spacing);
    void SetMargin(int margin);

private slots:
    void on_pushButton_OK_clicked();
    void on_spinBox_spacing_editingFinished();
    void on_spinBox_spacing_valueChanged(int arg1);
    void on_spinBox_margin_valueChanged(int arg1);

private:
    Ui::SettingDialog *ui;
    int spacing_;
    int margin_;
};

#endif // SETTINGDIALOG_H
