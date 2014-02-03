#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_pushButton_OK_clicked()
{
    //close();
    accept();       //return 1
}

void SettingDialog::on_spinBox_spacing_editingFinished()
{

}

void SettingDialog::on_spinBox_spacing_valueChanged(int arg1)
{
    spacing_ = arg1;
}

void SettingDialog::on_spinBox_margin_valueChanged(int arg1)
{
    margin_ = arg1;
}


int SettingDialog::GetSpacing() {
    return spacing_;
}

int SettingDialog::GetMargin() {
    return margin_;
}

void SettingDialog::SetSpacing(int spacing) {
    spacing_ = spacing;
    ui->spinBox_spacing->setValue(spacing_);
}

void SettingDialog::SetMargin(int margin) {
    margin_ = margin;
    ui->spinBox_margin->setValue(margin_);
}
