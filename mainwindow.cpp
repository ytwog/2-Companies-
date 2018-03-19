#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QHeaderView.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DoDelete = true;
    first = NULL;
    model = NULL;
    end = NULL;
    NRows = 0;
    targ = 0;
    ui->setupUi(this);
    ui->EditDelete->setMaximum(NRows);
    CreateActions();
    CreateMenu();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_getButton_clicked()
{
    QString Filename = QFileDialog::getOpenFileName();
    short Err = Readfile(&first, &end, NRows, 0, Filename);
    if(Err == 1)
    {
        ui->ErrorText->setText("Файл не\nудалось открыть");
    }
    else if(Err == 2)
    {
        ui->ErrorText->setText("Некорректные\nданные");
    }
    updateTable();
}





void MainWindow::on_binButton_clicked()
{
    setlocale(0, "");
    saveBin();
    if(!first || !model)
    {
        ui->ErrorText->setText("Нет данных,\nкоторые нужно\nсохранить");
        return;
    }
    deleteRepeated(first, &end, NRows);
    QFile file("Firms.csv");
    Node *tmp = first;
    file.resize("Firms.csv", 0);
    if(file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QDataStream outputStream(&file);
        outputStream.writeRawData(";\"", 2);
        for( int col = 0; col < 4; col++)
        {
            string temp = (ui->tableView->model()->headerData(col, Qt::Horizontal).toString()).toStdString();
            outputStream.writeRawData(temp.c_str(), temp.length());
            if(col + 1 == 4)
            {
                outputStream.writeRawData("\";\n\"" , 4);
            }
            else
            {
                outputStream.writeRawData("\";\"" , 3);
            }
        }
        for( int row = 0; row < ui->tableView->verticalHeader()->count(); row++)
        {
            outputStream << ui->tableView->model()->headerData(row, Qt::Vertical).toString();
            outputStream.writeRawData("\";\"", 3);
            for( int col = 0; col < ui->tableView->horizontalHeader()->count(); col++)
            {
                string temp = (ui->tableView->model()->data(ui->tableView->model()->index(row, col)).toString()).toStdString();
                outputStream.writeRawData(temp.c_str(), temp.length());
                outputStream.writeRawData("\";\"", 3);
            }
            outputStream.writeRawData("\"\n\"", 3);
        }
        outputStream.writeRawData("\"", 1);
    }
    else
    {
        ui->ErrorText->setText("Файл не\nудалось открыть");
        return;
    }
    file.close();
}

bool deleteRepeated(Node *first, Node **end, int &NRows)
{
    bool Success = false;
    Node *obj = first->next;
    while(obj)
    {
        if(!IsIndividual(first, obj->Getname(), obj))
        {
            NRows--;
            if(obj->next)
            {
                (obj->next)->prev = obj->prev;
            }
            else
            {
                *end = obj->prev;
            }
            (obj->prev)->next = obj->next;
            Node *tmp = obj->next;
            delete obj;
            obj = tmp;
            Success = true;
        }
        else
        {
            obj = obj->next;
        }
    }
    return Success;
}

void MainWindow::updateTable()
{
    Node *tmp = first;
    ui->ErrorText->setText("");
    ui->richText->setText("");
    if(model)
    {
        delete model;
        model = NULL;
    }
    if(tmp && NRows)
    {
        string RichString;
        while(tmp)
        {
            if(tmp->Getbudget() > 1000000)
            {
                RichString.append(tmp->Getname());
                RichString += '\n';
            }
            tmp = tmp->next;
        }
        ui->richText->setText(QString::fromStdString(RichString));
        tmp = first;
        if(DoDelete)
        {
            if(deleteRepeated(first, &end, NRows))
            {
                ui->ErrorText->setText("Повторяющиеся\nфирмы были\nудалены!");
            }
        }
        ui->EditDelete->setMaximum(NRows);
        model = new QStandardItemModel(NRows, 4, this);
        ui->tableView->setModel(model);
        for(int col = 0; col < model->columnCount(); col++)
        {
            int _w = ui->tableView->width() - ui->tableView->verticalHeader()->size().rwidth();
            if(col == 0)
            {
                ui->tableView->horizontalHeader()->resizeSection(col, _w * 5 / 16);
            }
            else if(col == 1)
            {
                ui->tableView->horizontalHeader()->resizeSection(col, _w * 3 / 16);
            }
            else if(col == 2)
            {
                ui->tableView->horizontalHeader()->resizeSection(col, _w * 3 / 16);
            }
            else if(col == 3)
            {
                ui->tableView->horizontalHeader()->resizeSection(col, _w * 5 / 16);
            }
            ui->tableView->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Fixed);
        }
        for(int row = 0; ((row < model->rowCount()) && (tmp)); row++)
        {
            for(int col = 0; col < model->columnCount(); col++)
            {
                QModelIndex index = model->index(row, col);
                switch(col)
                {
                case 0:
                    model->setData(index, QString::fromStdString(tmp->Getname()));
                    break;
                case 1:
                    model->setData(index, tmp->Getbudget());
                    break;
                case 2:
                    model->setData(index, tmp->Getmembers());
                    break;
                case 3:
                    model->setData(index, QString::fromStdString(tmp->Gettax()));
                    break;
                default:
                    break;
                }
            }
            tmp = tmp->next;
        }
        model->setHeaderData(0, Qt::Horizontal, "Name");
        model->setHeaderData(1, Qt::Horizontal, "$");
        model->setHeaderData(2, Qt::Horizontal, "Members");
        model->setHeaderData(3, Qt::Horizontal, "Tax");
        ui->tableView->setModel(model);
    }
}

void MainWindow::on_exitButton_clicked()
{
    this->close();
}

void MainWindow::on_addButton_clicked()
{
    if((ui->NameEdit->text() == "") || (ui->TaxEdit->text() == ""))
    {
        ui->ErrorText->setText("Одно или\nнесколько значений\nне были указаны");
        return;
    }
    else if(!IsInt(ui->TaxEdit->text()))
    {
        ui->ErrorText->setText("ИНН должно\nсостоять только\nиз цифр");
        return;
    }
    else
    {
        ui->ErrorText->setText("");
        add(&first, end, &end, ui->NameEdit->text().toStdString(), ui->BudgetEdit->value(), ui->MembersEdit->value(), ui->TaxEdit->text().toStdString());
        NRows++;
        updateTable();
    }
}

void MainWindow::on_delButton_clicked()
{
    targ = ui->EditDelete->value();
    if((targ <= NRows) && (targ > 0))
    {
        delNode(&first, &end, targ);
        NRows--;
        if(NRows < targ)
        {
            //targ--;
        }
    }
    updateTable();
}


void MainWindow::CreateActions()
{
    actionConfirm = new QAction(tr("Применить изменения"), this);
    actionConfirm->setShortcut(Qt::CTRL + Qt::Key_E);
    connect(actionConfirm, SIGNAL(triggered()), this, SLOT(on_cnfrmButton_clicked()));

    actionRestore = new QAction(tr("Сбросить изменения"), this);
    actionRestore->setShortcut(Qt::CTRL + Qt::Key_R);
    connect(actionRestore, SIGNAL(triggered()), this, SLOT(on_clrButton_clicked()));

    actionDelete = new QAction(tr("Удалить фирму"), this);
    actionDelete->setShortcut(Qt::CTRL + Qt::Key_Delete);
    connect(actionDelete, SIGNAL(triggered()), this, SLOT(on_delButton_clicked()));

    actionSave = new QAction(tr("Сохранить"), this);
    actionSave->setShortcut(QKeySequence::Save);
    connect(actionSave, SIGNAL(triggered()), this, SLOT(on_binButton_clicked()));

    actionBinSt = new QAction(tr("Открыть бинарный файл"), this);
    actionBinSt->setShortcut(QKeySequence::Italic);
    connect(actionBinSt, SIGNAL(triggered()), this, SLOT(on_getStButton_clicked()));

    actionBin = new QAction(tr("Открыть выбранный бинарный файл"), this);
    actionBin->setShortcut(QKeySequence::Open);
    connect(actionBin, SIGNAL(triggered()), this, SLOT(on_getButton_clicked()));
}

void MainWindow::CreateMenu()
{
    fileMenu = menuBar()->addMenu(tr("Файл"));
    fileMenu->addAction(actionSave);
    fileMenu->addAction(actionBinSt);
    fileMenu->addAction(actionBin);

    editMenu = menuBar()->addMenu(tr("Правка"));
    editMenu->addAction(actionConfirm);
    editMenu->addAction(actionRestore);
    editMenu->addAction(actionDelete);
}


void MainWindow::on_EditDelete_valueChanged(int arg1)
{
    targ = arg1;
}

void MainWindow::on_clrButton_clicked()
{
    updateTable();
}

void MainWindow::on_cnfrmButton_clicked()
{
    if(model)
    {
        Node *tmp = first;
        for(int row = 0; tmp; row++)
        {
            tmp->Setname(model->data(model->index(row, 0)).toString().toStdString());
            tmp->Setbudget(model->data(model->index(row, 1)).toInt());
            tmp->Setmembers(model->data(model->index(row, 2)).toString().toInt());
            tmp->Settax(model->data(model->index(row, 3)).toString().toStdString());
            tmp = tmp->next;
        }
    }
    updateTable();
}

void MainWindow::on_repCheckBox_stateChanged(int arg1)
{
    DoDelete = (!DoDelete);
    updateTable();
}

void MainWindow::on_getStButton_clicked()
{
    short Err = Readfile(&first, &end, NRows, 0);
    if(Err == 1)
    {
        ui->ErrorText->setText("Файл не\nудалось открыть");
    }
    else if(Err == 2)
    {
        ui->ErrorText->setText("Некорректные\nданные");
    }
    updateTable();
}

void MainWindow::saveBin()
{
    if(!first || !model)
    {
        ui->ErrorText->setText("Нет данных,\nкоторые нужно\nсохранить");
        return;
    }
    deleteRepeated(first, &end, NRows);
    FILE *fout = fopen("Firms.dat", "wb");
    Node *tmp = first;
    string temp = "";
    if(fout)
    {

        temp += ";\"";
        for(int col = 0; col < 4; col++)
        {
            temp += (ui->tableView->model()->headerData(col, Qt::Horizontal).toString()).toStdString();
            if(col + 1 == 4)
            {
                temp += "\";\n\"";
            }
            else
            {
                temp += "\";\"";
            }
        }
        for( int row = 0; row < ui->tableView->verticalHeader()->count(); row++)
        {
            temp += ui->tableView->model()->headerData(row, Qt::Vertical).toString().toStdString();
            temp += "\";\"";
            for( int col = 0; col < ui->tableView->horizontalHeader()->count(); col++)
            {
                temp += ui->tableView->model()->data(ui->tableView->model()->index(row, col)).toString().toStdString();
                temp += "\";\"";
            }
            temp += "\"\n\"";
        }
        temp += "\"";
        for(int i = 0; i < temp.length(); i++)
        {
            fwrite(&temp[i], sizeof(char), 1, fout);
        }
    }
    else
    {
        ui->ErrorText->setText("Файл не\nудалось открыть");
        return;
    }
    fclose(fout);

}

void MainWindow::on_saveBin_clicked()
{
    short Err = Readfile(&first, &end, NRows, 1, "Firms.dat");
    ui->richText->setText(QString::fromStdString(first->Getname()));
    if(Err == 1)
    {
        ui->ErrorText->setText("Файл не\nудалось открыть");
    }
    else if(Err == 2)
    {
        ui->ErrorText->setText("Некорректные\nданные");
    }
    updateTable();
}
