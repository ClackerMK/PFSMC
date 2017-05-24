#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <map>
#include <array>
#include <string>

#include "monster.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ArrayBox_currentIndexChanged(const QString &arg1);

    void on_continueStep1_released();


    void on_AbilityHigh_currentIndexChanged(int index);

    void on_AbilityMedium_currentIndexChanged(int index);

    void on_AbilityLow_currentIndexChanged(int index);

    void on_CRSelect_valueChanged(int arg1);

private:
    std::map<std::string, std::array<std::array<int, 21>,31>> arrays;

    Monster monster;

    Ui::MainWindow *ui;

    void InitializeStep1();
    void InitializeStep2();
};

#endif // MAINWINDOW_H
