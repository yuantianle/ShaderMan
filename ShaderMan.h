#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ShaderMan.h"

class ShaderMan : public QMainWindow
{
    Q_OBJECT

public:
    ShaderMan(QWidget *parent = Q_NULLPTR);

private:
    Ui::ShaderManClass ui;


};
