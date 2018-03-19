#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <binfile.h>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMenuBar>
#include "QStandardItemModel"
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Node *first;
    Node *end;

private slots:
    void on_getButton_clicked();

    void on_binButton_clicked();

    void on_exitButton_clicked();

    void on_addButton_clicked();

    void updateTable();

    void on_delButton_clicked();

    void CreateActions();

    void on_EditDelete_valueChanged(int arg1);

    void on_clrButton_clicked();

    void CreateMenu();

    void on_cnfrmButton_clicked();

    void on_repCheckBox_stateChanged(int arg1);

    void on_getStButton_clicked();

    void saveBin();

    void on_saveBin_clicked();

private:
    bool DoDelete;
    QMenu *fileMenu;
    QAction *actionSave;
    QAction *actionBinSt;
    QAction *actionBin;
    QMenu *editMenu;
    QAction *actionDelete;
    QAction *actionConfirm;
    QAction *actionRestore;
    QStandardItemModel *model;
    int targ;
    int NRows;
    Ui::MainWindow *ui;
};


bool deleteRepeated(Node *first, Node **end, int &NRows);

#endif // MAINWINDOW_H
