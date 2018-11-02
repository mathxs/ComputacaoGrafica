#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMessageBox>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool i = true;
    bool x[9] = {false};
    bool y[9] = {false};
    int jogadas = 0;
    ~MainWindow();

private:
    Ui::MainWindow *ui;


public slots :
    void bt1();
    void bt2();
    void bt3();
    void bt4();
    void bt5();
    void bt6();
    void bt7();
    void bt8();
    void bt9();
    void bt10();
    void vitoria();
    void mens();
};

#endif // MAINWINDOW_H
