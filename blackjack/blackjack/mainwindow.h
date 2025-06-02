#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include <QQueue>
#include <QVector>
#include <QPair>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Player {
    QString name;
    int score = 0;
    bool done = false;
    QVector<QPair<int, int>> cards;  // rank, suit
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_hitButton_clicked();
    void on_standButton_clicked();

private:
    void initializeUI();              // Layout & widget setup
    void initializeDeck();           // Create and shuffle deck
    void dealInitialCards();         // Deal 2 cards each
    void updateUI();                 // Refresh card displays & score
    void nextTurn();                 // Move to next player
    int getCardValue(int rank);      // Blackjack logic
    QString getCardImage(int rank, int suit);  // Image path

    // Game Data
    QStack<QPair<int, int>> deck;           // Stack: Deck
    QQueue<Player*> playersQueue;           // Queue: Turns
    Player p1{"Player 1"}, p2{"Player 2"}, p3{"Player 3"};
    Player* currentPlayer = nullptr;

    // Card Display Labels
    QVector<QLabel*> p1CardLabels;
    QVector<QLabel*> p2CardLabels;
    QVector<QLabel*> p3CardLabels;

    // UI Widgets
    QWidget* centralWidget;
    QLabel* turnLabel;
    QLabel* p1ScoreLabel;
    QLabel* p2ScoreLabel;
    QLabel* p3ScoreLabel;
    QPushButton* hitButton;
    QPushButton* standButton;
    QVBoxLayout* mainLayout;
    QHBoxLayout* buttonLayout;
    QHBoxLayout* player1Layout;
    QHBoxLayout* player2Layout;
    QHBoxLayout* player3Layout;
};

#endif // MAINWINDOW_H
