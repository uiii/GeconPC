#ifndef GECON_MAINWINDOW_HPP
#define GECON_MAINWINDOW_HPP

#include <QMainWindow>

namespace Gecon {

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
    Ui::MainWindow *ui;
};


} // namespace Gecon
#endif // GECON_MAINWINDOW_HPP
