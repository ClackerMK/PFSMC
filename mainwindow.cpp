#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <dirent.h>
#include <exception>

#include "mainwindow.h"
#include "ui_mainwindow.h"

std::array<std::array<int, 21>,31> readArrayFromCSV(std::istream& str)
{
    std::array<std::array<int, 21>,31> result;
    std::string line;

    int i = 0;
    while (std::getline(str, line))
    {
        std::stringstream          lineStream(line);
        std::string                cell;

        result[i] = std::array<int, 21>();

        int y = 0;
        while(std::getline(lineStream,cell, ';'))
        {
            result[i][y] = std::stoi(cell);
            y++;
        }
        i++;
    }

    return result;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    InitializeStep1();
    InitializeStep2();
}

void MainWindow::InitializeStep1()
{
    DIR* dir;
    struct dirent *ent;

    QStringList ArrayHorizontalHeader;
    ArrayHorizontalHeader << "AC" << "AC (t)" << "AC (f)" << \
                   "Fort" << "Ref" << "Will" << \
                   "CMD" << "hp" << \
                   "DC (Abillity)" << "DC (Spell)" << \
                   "Ability Mod" << \
                   "Skill (Master)" << "Skill (Good)" << \
                   "Options";
    QStringList ArrayVerticalHeader;
    for (int i = 0; i < 31; i++)
    {
        if (i == 0)
            ArrayVerticalHeader << "1/2 (0)";
        else
            ArrayVerticalHeader << QString::number(i);
    }

    ui->ArrayTable->setRowCount(31);
    ui->ArrayTable->setColumnCount(14);
    ui->ArrayTable->setHorizontalHeaderLabels(ArrayHorizontalHeader);
    ui->ArrayTable->setVerticalHeaderLabels(ArrayVerticalHeader);
    ui->ArrayTable->resizeColumnsToContents();

    QStringList list;
    list << "Strength" << "Dexterity" << "Constitution" << "Intelligence" << "Wisdom" << "Charisma";
    ui->AbilityHigh->addItems(list);
    ui->AbilityMedium->addItems(list);
    ui->AbilityLow->addItems(list);
    ui->AbilityHigh->setCurrentIndex(-1);
    ui->AbilityMedium->setCurrentIndex(-1);
    ui->AbilityLow->setCurrentIndex(-1);

    for (int i = 1; i < ui->tabWidget->count(); i++)
    {
        ui->tabWidget->setTabEnabled(i, false);
    }

    if ((dir = opendir(".\\Arrays")) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            std::string name(ent->d_name);
            std::string path(".\\Arrays\\" + name);
            size_t found  = name.find(".csv");
            if (found != std::string::npos)
            {
               std::ifstream file;
               file.open(path.c_str());
               if (file.is_open())
               {
                   std::cout << "Starts reading Arrays" << name << std::endl;
                   ui->ArrayBox->addItem(QString::fromStdString(name.substr(0,found)));
                   ui->ArrayBox->setCurrentIndex(-1);
                   arrays[name.substr(0, found)] = readArrayFromCSV(file);
               }
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }

}


void MainWindow::InitializeStep2()
{

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ArrayBox_currentIndexChanged(const QString &arg1)
{
    for (size_t i = 0; i < arrays[arg1.toStdString()].size(); i++)
    {
        for (size_t y = 0; y < ui->ArrayTable->columnCount(); y++)
        {
            QString str;
            if (y < 10)
            {
                str = QString::number(arrays[arg1.toStdString()][i][y]);
            } else if (y == 10)
            {

                str = "+" + QString::number(arrays[arg1.toStdString()][i][y]) + ",";
                str += "+" + QString::number(arrays[arg1.toStdString()][i][y+1]) +",";
                str += "+" + QString::number(arrays[arg1.toStdString()][i][y+2]);
            } else if (y == 11)
            {
                str = "+" + QString::number(arrays[arg1.toStdString()][i][y+2]) + " x " + QString::number(arrays[arg1.toStdString()][i][y+3]);
            } else if (y == 12)
            {
                str = "+" + QString::number(arrays[arg1.toStdString()][i][y+3]) + " x " + QString::number(arrays[arg1.toStdString()][i][y+4]);
            } else
            {
                str = QString::number(arrays[arg1.toStdString()][i][y+4]) + "c, ";
                str += QString::number(arrays[arg1.toStdString()][i][y+5]) + "s, ";
                str += QString::number(arrays[arg1.toStdString()][i][y+6]) + "m, ";
                str += QString::number(arrays[arg1.toStdString()][i][y+7]) + "a, ";
            }
            ui->ArrayTable->setItem(i,y, new QTableWidgetItem(str));
        }
    }
    ui->ArrayTable->resizeColumnsToContents();
    ui->AbilityHigh->setEnabled(arg1 != "");
    ui->AbilityMedium->setEnabled(arg1 != "");
    ui->AbilityLow->setEnabled(arg1 != "");

    ui->ModHigh->setText("+" + QString::number(arrays[arg1.toStdString()][ui->CRSelect->value()][10]));
    ui->ModMedium->setText("+" + QString::number(arrays[arg1.toStdString()][ui->CRSelect->value()][11]));
    ui->ModLow->setText("+" + QString::number(arrays[arg1.toStdString()][ui->CRSelect->value()][12]));

    if (arg1 == "")
    {
        ui->continueStep1->setEnabled(false);
    }
}


void MainWindow::on_continueStep1_released()
{
    ui->tabWidget->setTabEnabled(1,true);
    ui->tabWidget->setCurrentIndex(1);

    std::array<int, 21> &arr = arrays[ui->ArrayBox->currentText().toStdString()][ui->CRSelect->value()];
    Statistics stats;
    stats.AC = arr[0];
    stats.AC_touch = arr[1];
    stats.AC_ff = arr[2];
    stats.Fort = arr[3];
    stats.Ref = arr[4];
    stats.Will = arr[5];
    stats.CMD = arr[6];
    stats.HP = arr[7];
    stats.AbillityDC = arr[9];
    stats.SpellDC = arr[10];
    switch (ui->AbilityHigh->currentIndex())
    {
        case 0:
            stats.Str = arr[11];
            break;
        case 1:
            stats.Dex = arr[11];
            break;
        case 2:
            stats.Con = arr[11];
            break;
        case 3:
            stats.Int = arr[11];
            break;
        case 4:
            stats.Wis = arr[11];
            break;
        case 5:
            stats.Cha = arr[11];
        break;
        default:
            throw new std::runtime_error("Ability Score (High) not valid");
        break;
    }
    switch (ui->AbilityMedium->currentIndex())
    {
        case 0:
            stats.Str = arr[12];
            break;
        case 1:
            stats.Dex = arr[12];
            break;
        case 2:
            stats.Con = arr[12];
            break;
        case 3:
            stats.Int = arr[12];
            break;
        case 4:
            stats.Wis = arr[12];
            break;
        case 5:
            stats.Cha = arr[12];
        break;
        default:
            throw new std::runtime_error("Ability Score (Medium) not valid");
        break;
    }
    switch (ui->AbilityLow->currentIndex())
    {
        case 0:
            stats.Str = arr[13];
            break;
        case 1:
            stats.Dex = arr[13];
            break;
        case 2:
            stats.Con = arr[13];
            break;
        case 3:
            stats.Int = arr[13];
            break;
        case 4:
            stats.Wis = arr[13];
            break;
        case 5:
            stats.Cha = arr[13];
        break;
        default:
            throw new std::runtime_error("Ability Score (Low) not valid");
        break;
    }

    monster.setBaseStats(stats);
    monster.setCR(ui->CRSelect->value());
}


void MainWindow::on_AbilityHigh_currentIndexChanged(int index)
{
    static int index_old = -1;
    if (index != -1)
    {
        if (ui->AbilityMedium->currentIndex() == index)
        {
            ui->AbilityMedium->setCurrentIndex(index_old);
        } else if (ui->AbilityLow->currentIndex() == index)
        {
            ui->AbilityLow->setCurrentIndex(index_old);
        }
    }

    index_old = index;

    if (ui->AbilityHigh->currentIndex() != -1 && ui->AbilityMedium->currentIndex() != -1 && ui->AbilityLow->currentIndex() != -1)
    {
        ui->continueStep1->setEnabled(true);
    } else
    {
        ui->continueStep1->setEnabled(false);
    }
}

void MainWindow::on_AbilityMedium_currentIndexChanged(int index)
{
    static int index_old = -1;
    if (index != -1)
        {
        if (ui->AbilityHigh->currentIndex() == index)
        {
            ui->AbilityHigh->setCurrentIndex(index_old);
        } else if (ui->AbilityLow->currentIndex() == index)
        {
            ui->AbilityLow->setCurrentIndex(index_old);
        }
    }

    index_old = index;

    if (ui->AbilityHigh->currentIndex() != -1 && ui->AbilityMedium->currentIndex() != -1 && ui->AbilityLow->currentIndex() != -1)
    {
        ui->continueStep1->setEnabled(true);
    } else
    {
        ui->continueStep1->setEnabled(false);
    }
}

void MainWindow::on_AbilityLow_currentIndexChanged(int index)
{
    static int index_old = -1;
    if (index != -1)
    {
        if (ui->AbilityHigh->currentIndex() == index)
        {
            ui->AbilityHigh->setCurrentIndex(index_old);
        } else if (ui->AbilityMedium->currentIndex() == index)
        {
            ui->AbilityMedium->setCurrentIndex(index_old);
        }
    }

    index_old = index;

    if (ui->AbilityHigh->currentIndex() != -1 && ui->AbilityMedium->currentIndex() != -1 && ui->AbilityLow->currentIndex() != -1)
    {
        ui->continueStep1->setEnabled(true);
    } else
    {
        ui->continueStep1->setEnabled(false);
    }
}

void MainWindow::on_CRSelect_valueChanged(int arg1)
{
    if (ui->ArrayBox->currentIndex() != -1)
    {
        ui->ModHigh->setText("+" + QString::number(arrays[ui->ArrayBox->currentText().toStdString()][arg1][10]));
        ui->ModMedium->setText("+" + QString::number(arrays[ui->ArrayBox->currentText().toStdString()][arg1][11]));
        ui->ModLow->setText("+" + QString::number(arrays[ui->ArrayBox->currentText().toStdString()][arg1][12]));
    }
}
