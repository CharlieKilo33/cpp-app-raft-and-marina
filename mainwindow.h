#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void moveRaft(); // Перемещение плота
    void peopleArriveAtPier(); // Прибытие людей на правую пристань
    void peopleBoardRaft(); // Люди садятся на плот
    void peopleDisembark(); // Люди сходят с плота

private:
    Ui::MainWindow *ui;

    // Графические элементы для отображения состояния
    QLabel *raftLabel;
    QLabel *waterLabel;
    QLabel *leftPierLabel;
    QLabel *rightPierLabel;
    QLabel *peopleOnRaftLabel;
    QLabel *leftPierPeopleLabel;
    QLabel *rightPierPeopleLabel;

    // Состояние
    int peopleOnRaft;
    int peopleAtLeftPier;
    int peopleAtRightPier;
    bool raftAtRightPier;
    bool movingLeft;

    // Таймеры для событий
    QTimer *moveTimer;
    QTimer *eventTimer;

    // Обновление интерфейса
    void updateDisplay(); // Обновление интерфейса
    void updateRaftPosition(); // Обновление позиции плота
    QString peopleToString(int count); // Вспомогательная функция для отображения людей
};
#endif // MAINWINDOW_H
