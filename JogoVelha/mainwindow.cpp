#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(bt1()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(bt2()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(bt3()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(bt4()));
    connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(bt5()));
    connect(ui->pushButton_6,SIGNAL(clicked()),this,SLOT(bt6()));
    connect(ui->pushButton_7,SIGNAL(clicked()),this,SLOT(bt7()));
    connect(ui->pushButton_8,SIGNAL(clicked()),this,SLOT(bt8()));
    connect(ui->pushButton_9,SIGNAL(clicked()),this,SLOT(bt9()));
    connect(ui->pushButton_10,SIGNAL(clicked()),this,SLOT(bt10()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow :: bt1()
{
    if(i == true){
            ui->pushButton->setText("X");
            i = false;
            x[0] = true;
            y[0] = false;
    }else{
            ui->pushButton->setText("0");
            i=true;
            y[0] = true;
            x[0] = false;
    };
    vitoria();
}

void MainWindow :: bt2()
{
    if(i == true){
            ui->pushButton_2->setText("X");
            i = false;
            x[1] = true;
            y[1] = false;
    }else{
            ui->pushButton_2->setText("0");
            i = true;
            y[1] = true;
            x[1] = false;
    };
    vitoria();
}

void MainWindow :: bt3()
{
    if(i == true){
            ui->pushButton_3->setText("X");
            i = false;
            x[2] = true;
            y[2] = false;
    }else{
            ui->pushButton_3->setText("0");
            i = true;
            y[2] = true;
            x[2] = false;
    };
    vitoria();
}

void MainWindow :: bt4()
{
    if(i == true){
            ui->pushButton_4->setText("X");
            i = false;
            x[3] = true;
            y[3] = false;
    }else{
            ui->pushButton_4->setText("0");
            i = true;
            y[3] = true;
            x[3] = false;
    };
    vitoria();
}

void MainWindow :: bt5()
{
    if(i == true){
            ui->pushButton_5->setText("X");
            i = false;
            x[4] = true;
            y[4] = false;
    }else{
            ui->pushButton_5->setText("0");
            i = true;
            y[4] = true;
            x[4] = false;
    };
    vitoria();
}

void MainWindow :: bt6()
{
    if(i == true){
            ui->pushButton_6->setText("X");
            i = false;
            x[5] = true;
            y[5] = false;
    }else{
            ui->pushButton_6->setText("0");
            i = true;
            y[5] = true;
            x[5] = false;
    };
    vitoria();
}

void MainWindow :: bt7()
{
    if(i == true){
            ui->pushButton_7->setText("X");
            i = false;
            x[6] = true;
            y[6] = false;
    }else{
            ui->pushButton_7->setText("0");
            i = true;
            y[6] = true;
            x[6] = false;
    };
    vitoria();
}

void MainWindow :: bt8()
{
    if(i == true){
            ui->pushButton_8->setText("X");
            i = false;
            x[7] = true;
            y[7] = false;
    }else{
            ui->pushButton_8->setText("0");
            i = true;
            x[7] = false;
            y[7] = true;
    };
    vitoria();
}

void MainWindow :: bt9()
{
    if(i == true){
            ui->pushButton_9->setText("X");
            i = false;
            x[8] = true;
            y[8] = false;
    }else{
            ui->pushButton_9->setText("0");
            i = true;
            x[8] = false;
            y[8] = true;
    };
    vitoria();
}

void MainWindow :: bt10()
{
    ui->pushButton->setText("");
    ui->pushButton_2->setText("");
    ui->pushButton_3->setText("");
    ui->pushButton_4->setText("");
    ui->pushButton_5->setText("");
    ui->pushButton_6->setText("");
    ui->pushButton_7->setText("");
    ui->pushButton_8->setText("");
    ui->pushButton_9->setText("");
    jogadas = 0;
    for(int i=0;i<10;i++){
        x[i] = {false};
        y[i] = {false};
    };
    ui->textoEsquerda->setText("");
    ui->textoDireita->setText("");
}

void MainWindow :: vitoria(){
    jogadas = jogadas +1;
    if(x[0] == true and x[1] == true and x[2] == true){
        ui->textoEsquerda->setText("O jogador X venceu");
        ui->textoDireita->setText("O jogador 0 perdeu");
        mens();
    }else if(x[3] == true and x[4] == true and x[5] == true){
        ui->textoEsquerda->setText("O jogador X venceu");
        ui->textoDireita->setText("O jogador 0 perdeu");
        mens();
    }else if(x[6] == true and x[7] == true and x[8] == true){
        ui->textoEsquerda->setText("O jogador X venceu");
        ui->textoDireita->setText("O jogador 0 perdeu");
        mens();
    }else if(x[0] == true and x[4] == true and x[8] == true){
        ui->textoEsquerda->setText("O jogador X venceu");
        ui->textoDireita->setText("O jogador 0 perdeu");
        mens();
    }else if(x[2] == true and x[4] == true and x[6] == true){
        ui->textoEsquerda->setText("O jogador X venceu");
        ui->textoDireita->setText("O jogador 0 perdeu");
        mens();
    }else if(x[0] == true and x[3] == true and x[6] == true){
        ui->textoEsquerda->setText("O jogador X venceu");
        ui->textoDireita->setText("O jogador 0 perdeu");
        mens();
    }else if(x[1] == true and x[4] == true and x[7] == true){
        ui->textoEsquerda->setText("O jogador X venceu");
        ui->textoDireita->setText("O jogador 0 perdeu");
        mens();
    }else if(x[2] == true and x[5] == true and x[8] == true){
        ui->textoEsquerda->setText("O jogador X venceu");
        ui->textoDireita->setText("O jogador 0 perdeu");
        mens();
    }else if(y[0] == true and y[1] == true and y[2] == true){
        ui->textoEsquerda->setText("O jogador 0 venceu");
        ui->textoDireita->setText("O jogador X perdeu");
        mens();
    }else if(y[3] == true and y[4] == true and y[5] == true){
        ui->textoEsquerda->setText("O jogador 0 venceu");
        ui->textoDireita->setText("O jogador X perdeu");
        mens();
    }else if(y[6] == true and y[7] == true and y[8] == true){
        ui->textoEsquerda->setText("O jogador 0 venceu");
        ui->textoDireita->setText("O jogador X perdeu");
        mens();
    }else if(y[0] == true and y[4] == true and y[8] == true){
        ui->textoEsquerda->setText("O jogador 0 venceu");
        ui->textoDireita->setText("O jogador X perdeu");
        mens();
    }else if(y[2] == true and y[4] == true and y[6] == true){
        ui->textoEsquerda->setText("O jogador 0 venceu");
        ui->textoDireita->setText("O jogador X perdeu");
        mens();
    }else if(y[0] == true and y[3] == true and y[6] == true){
        ui->textoEsquerda->setText("O jogador 0 venceu");
        ui->textoDireita->setText("O jogador X perdeu");
        mens();
    }else if(y[1] == true and y[4] == true and y[7] == true){
        ui->textoEsquerda->setText("O jogador 0 venceu");
        ui->textoDireita->setText("O jogador X perdeu");
        mens();
    }else if(y[2] == true and y[5] == true and y[8] == true){
        ui->textoEsquerda->setText("O jogador 0 venceu");
        ui->textoDireita->setText("O jogador X perdeu");
        mens();
    }
}

void MainWindow :: mens(){
    QMessageBox msgBox;
    msgBox.setText("O JOGO ACABOU");
    msgBox.exec();
}
