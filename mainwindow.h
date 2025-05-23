#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QListWidget;
class QLabel;
class QPushButton;
class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchClicked();
    void onImageSelected(QListWidgetItem* item);

private:
    void loadSavedImages();
    void saveImages();
    void searchImages(const QString &directory);

    QListWidget *listWidget;
    QLabel      *imageLabel;
    QPushButton *searchButton;

    QStringList imagePaths;
};

#endif // MAINWINDOW_H
