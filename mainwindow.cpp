#include "mainwindow.h"
#include "smart_ptr.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDirIterator>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidgetItem>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Создаем центральный виджет и устанавливаем его
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Инициализируем основные элементы интерфейса
    listWidget = new QListWidget(this);
    imageLabel = new QLabel(this);
    imageLabel->setFixedSize(400, 400);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setText("Выберите изображение для предпросмотра");

    searchButton = new QPushButton("Поиск изображений", this);

    // Создаем лэйаут слева: кнопка + список изображений
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(searchButton);
    leftLayout->addWidget(listWidget);

    // Основной горизонтальный лэйаут: слева элементы интерфейса, справа — предпросмотр изображения
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addLayout(leftLayout);
    mainLayout->addWidget(imageLabel);

    // Подключаем сигналы к слотам
    connect(searchButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(listWidget, &QListWidget::itemClicked, this, &MainWindow::onImageSelected);

    // При запуске загружаем сохранённые ссылки на файлы
    loadSavedImages();

    setWindowTitle("Просмотр изображений");
}

MainWindow::~MainWindow()
{
    // При закрытии сохраняем найденные (или использованные) ссылки в файл
    saveImages();
}

void MainWindow::onSearchClicked()
{
    // Позволяем пользователю выбрать папку для поиска изображений
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку для поиска изображений");
    if (dir.isEmpty())
        return;

    // Очищаем предыдущий список найденных изображений
    listWidget->clear();
    imagePaths.clear();

    // Запускаем поиск изображений в выбранной папке
    searchImages(dir);

    // Добавляем найденные пути в список
    for (const QString &path : imagePaths)
        listWidget->addItem(path);

    // Оповещаем пользователя о результате поиска
    QMessageBox::information(this, "Поиск завершён", QString("Найдено %1 изображений.").arg(imagePaths.size()));
}

void MainWindow::searchImages(const QString &directory)
{
    // Используем QDirIterator для рекурсивного перебора файлов с нужными расширениями
    QDirIterator it(directory, QStringList() << "*.jpeg" << "*.jpg" << "*.bmp" << "*.png",
                    QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();
        imagePaths << filePath;
    }
}

void MainWindow::onImageSelected(QListWidgetItem *item)
{
    QString path = item->text();
    QPixmap pix(path);
    if (!pix.isNull()) {
        pix = pix.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLabel->setPixmap(pix);
    } else {
        imageLabel->setText("Не удалось загрузить изображение");
    }
}

void MainWindow::saveImages()
{
    SmartPtr<QFile> file(new QFile("images.txt"));
    if (file->open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(file.get());
        for (const QString &path : imagePaths)
            out << path << "\n";
        file->close();
    }
}

void MainWindow::loadSavedImages()
{
    SmartPtr<QFile> file(new QFile("images.txt"));
    if (!file->exists())
        return;

    if (file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        imagePaths.clear();
        QTextStream in(file.get());
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty() && QFile::exists(line)) {
                imagePaths << line;
                listWidget->addItem(line);
            }
        }
        file->close();
    }
}
