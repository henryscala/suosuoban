#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}

void ConfigDialog::setText(QString &text)
{
    ui->plainTextEdit->setPlainText(text);
}

QString ConfigDialog::getText()
{
    return ui->plainTextEdit->toPlainText();
}
