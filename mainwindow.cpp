#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <dirent.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    DIR* dir;
    struct dirent *ent;

    ui->setupUi(this);

    for (int i = 1; i < ui->tabWidget->count(); i++)
    {
        ui->tabWidget->setTabEnabled(i, false);
    }

    if ((dir = opendir(".\\Arrays")) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            std::string name(ent->d_name);
            size_t found  = name.find(".csv");
            if (found != std::string::npos)
            {
               printf ("Starts reading Arrays");
               printf ("%s\n", name.c_str());
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
