#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , peopleOnRaft(0)
    , peopleAtLeftPier(0)
    , peopleAtRightPier(0)
    , raftAtRightPier(true)
    , movingLeft(false)
{
    ui->setupUi(this);

    // Создаем основной контейнер
    QWidget *mainWidget = new QWidget(this);
    this->setCentralWidget(mainWidget);

    // Основной вертикальный макет
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);

    // Горизонтальный макет для водного пространства и пристаней
    QHBoxLayout *waterLayout = new QHBoxLayout();

    leftPierLabel = new QLabel(this);
    leftPierLabel->setStyleSheet("background-color: black;");
    leftPierLabel->setFixedSize(50, 250);  // Левая пристань

    waterLabel = new QLabel("Водное пространство", this);
    waterLabel->setStyleSheet("background-color: blue;");
    waterLabel->setAlignment(Qt::AlignCenter);
    waterLabel->setFixedSize(700, 250);  // Размер водного пространства

    rightPierLabel = new QLabel(this);
    rightPierLabel->setStyleSheet("background-color: black;");
    rightPierLabel->setFixedSize(50, 250); // Правая пристань

    raftLabel = new QLabel("ПЛОТ", this);
    raftLabel->setStyleSheet("background-color: gray;");
    raftLabel->setFixedSize(100, 50); // Размер плота

    // Добавляем пристани и водное пространство в горизонтальный макет
    waterLayout->addWidget(leftPierLabel);
    waterLayout->addWidget(waterLabel);
    waterLayout->addWidget(rightPierLabel);

    // Добавляем горизонтальный макет с водным пространством в основной вертикальный макет
    mainLayout->addLayout(waterLayout);

    // Добавляем плот поверх водного пространства
    raftLabel->move(600, 150);
    raftLabel->setParent(waterLabel);

    // Макет для информации о людях на плоту и пристанях
    QHBoxLayout *infoLayout = new QHBoxLayout();

    peopleOnRaftLabel = new QLabel("Люди на плоту: 0", this);
    leftPierPeopleLabel = new QLabel("Люди на левой пристани: 0", this);
    rightPierPeopleLabel = new QLabel("Люди на правой пристани: 0", this);

    // Добавляем метки с информацией в горизонтальный макет
    infoLayout->addWidget(leftPierPeopleLabel);
    infoLayout->addWidget(peopleOnRaftLabel);
    infoLayout->addWidget(rightPierPeopleLabel);

    // Добавляем информацию о людях в основной макет
    mainLayout->addLayout(infoLayout);

    // Таймеры для перемещения плота и событий
    moveTimer = new QTimer(this);
    eventTimer = new QTimer(this);

    connect(moveTimer, &QTimer::timeout, this, &MainWindow::moveRaft);
    connect(eventTimer, &QTimer::timeout, this, &MainWindow::peopleArriveAtPier);

    // Запуск событий
    eventTimer->start(2000); // Люди прибывают каждые 2 секунды
    moveTimer->start(1000);  // Обновляем каждые 1 секунду
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::moveRaft()
{
    if (peopleOnRaft > 0) {
        if (raftAtRightPier) {
            movingLeft = true;
            raftAtRightPier = false;
            raftLabel->move(raftLabel->x() - 50, raftLabel->y());  // Плот двигается влево
        } else if (movingLeft) {
            // Плот достиг левой пристани
            raftLabel->move(100, 150);  // Двигаем плот к левой пристани
            peopleDisembark();
        }
    } else {
        if (!raftAtRightPier) {
            movingLeft = false;
            raftAtRightPier = true;
            raftLabel->move(raftLabel->x() + 50, raftLabel->y());  // Плот двигается вправо
        } else if (!movingLeft) {
            // Плот достиг правой пристани
            raftLabel->move(600, 150);  // Двигаем плот к правой пристани
        }
    }
    updateDisplay();
}

void MainWindow::peopleArriveAtPier()
{
    if (raftAtRightPier) {
        int peopleArriving = QRandomGenerator::global()->bounded(1, 5);  // До 5 человек могут прибыть
        peopleAtRightPier += peopleArriving;
        if (peopleAtRightPier > 100) {
            peopleAtRightPier = 100;  // Ограничиваем количество людей на пристани до 100
        }

        // Люди пересаживаются на плот
        peopleBoardRaft();
    }
    updateDisplay();
}

void MainWindow::peopleBoardRaft()
{
    if (raftAtRightPier && peopleAtRightPier > 0) {
        int peopleBoarding = QRandomGenerator::global()->bounded(1, 4);  // До 4 человек могут сесть на плот
        peopleOnRaft = qMin(peopleBoarding, peopleAtRightPier);  // Ограничиваем количество людей на плоту
        peopleAtRightPier -= peopleOnRaft;
    }
    updateDisplay();
}

void MainWindow::peopleDisembark()
{
    if (!raftAtRightPier && movingLeft && peopleOnRaft > 0) {
        peopleAtLeftPier += peopleOnRaft;
        peopleOnRaft = 0;
    }
    updateDisplay();
}

void MainWindow::updateDisplay()
{
    // Обновляем информацию на экране
    peopleOnRaftLabel->setText("Люди на плоту: " + QString::number(peopleOnRaft));
    leftPierPeopleLabel->setText("Люди на левой пристани: " + QString::number(peopleAtLeftPier));
    rightPierPeopleLabel->setText("Люди на правой пристани: " + QString::number(peopleAtRightPier));
}
