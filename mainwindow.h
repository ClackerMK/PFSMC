#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <map>
#include <array>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    std::map<std::string, std::array<std::map<std::string, int>,30>> arrays;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
