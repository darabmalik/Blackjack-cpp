#include "mainwindow.h"
#include <QScrollArea>
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>
#include <QRandomGenerator>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initializeUI();
    initializeDeck();

    playersQueue.enqueue(&p1);
    playersQueue.enqueue(&p2);
    playersQueue.enqueue(&p3);

    dealInitialCards();
    nextTurn();
}

MainWindow::~MainWindow() {}

void MainWindow::initializeUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);

    // Create Player Sections
    auto createPlayerSection = [&](const QString& title, QHBoxLayout*& layout, QLabel*& scoreLabel, QVector<QLabel*>& cardLabels) {
        QVBoxLayout* vbox = new QVBoxLayout();
        QLabel* titleLabel = new QLabel(title, this);
        scoreLabel = new QLabel("Score: 0", this);

        layout = new QHBoxLayout();
        for (int i = 0; i < 10; ++i) {
            QLabel* cardLabel = new QLabel(this);
            cardLabel->setFixedSize(85, 120);
            cardLabel->setStyleSheet("border: 1px solid black;");
            layout->addWidget(cardLabel);
            cardLabels.append(cardLabel);
        }

        vbox->addWidget(titleLabel);
        vbox->addWidget(scoreLabel);
        vbox->addLayout(layout);
        mainLayout->addLayout(vbox);
    };

    createPlayerSection("Player 1", player1Layout, p1ScoreLabel, p1CardLabels);
    createPlayerSection("Player 2", player2Layout, p2ScoreLabel, p2CardLabels);
    createPlayerSection("Player 3", player3Layout, p3ScoreLabel, p3CardLabels);

    // Turn label and buttons
    turnLabel = new QLabel("Turn: Player 1", this);
    hitButton = new QPushButton("Hit", this);
    standButton = new QPushButton("Stand", this);
    connect(hitButton, &QPushButton::clicked, this, &MainWindow::on_hitButton_clicked);
    connect(standButton, &QPushButton::clicked, this, &MainWindow::on_standButton_clicked);

    buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(turnLabel);
    buttonLayout->addStretch();
    buttonLayout->addWidget(hitButton);
    buttonLayout->addWidget(standButton);

    mainLayout->addLayout(buttonLayout);
}

void MainWindow::initializeDeck()
{
    QVector<QPair<int, int>> tempDeck;
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 1; rank <= 13; ++rank) {
            tempDeck.append(qMakePair(rank, suit));
        }
    }
    std::shuffle(tempDeck.begin(), tempDeck.end(), *QRandomGenerator::global());
    for (auto card : tempDeck) {
        deck.push(card);
    }
}

int MainWindow::getCardValue(int rank)
{
//    if (rank >= 10) return 10;
//    if (rank == 1) return 11;
    return rank;
}

QString MainWindow::getCardImage(int rank, int suit)
{
    const QString suits[] = {"spades", "hearts", "diamonds", "clubs"};
    QString rankStr;

    switch (rank) {
    case 1:  rankStr = "ace"; break;
    case 11: rankStr = "jack"; break;
    case 12: rankStr = "queen"; break;
    case 13: rankStr = "king"; break;
    default: rankStr = QString::number(rank); break;
    }

    QString path = ":/cards/" + rankStr + "of" + suits[suit] + ".png";
    qDebug() << "Card image path:" << path;

    return path;
}


void MainWindow::dealInitialCards()
{
    for (int i = 0; i < 2; ++i) {
        for (Player* p : {&p1, &p2, &p3}) {
            if (deck.isEmpty()) return;
            auto card = deck.pop();
            p->cards.append(card);
            p->score += getCardValue(card.first);
        }
    }
}

void MainWindow::updateUI()
{
    auto updatePlayerCards = [&](Player& player, QVector<QLabel*>& labels, QLabel* scoreLabel) {
        for (QLabel* lbl : labels) {
            lbl->clear();
        }
        for (int i = 0; i < player.cards.size() && i < labels.size(); ++i) {
            auto [rank, suit] = player.cards[i];
            QString path = getCardImage(rank, suit);
            QPixmap pix(path);
            if (!pix.isNull()) {
                labels[i]->setPixmap(pix.scaled(labels[i]->size(), Qt::KeepAspectRatio));
            } else {
                labels[i]->setText("?");
            }
        }
        scoreLabel->setText(QString("Score: %1").arg(player.score));
    };

    updatePlayerCards(p1, p1CardLabels, p1ScoreLabel);
    updatePlayerCards(p2, p2CardLabels, p2ScoreLabel);
    updatePlayerCards(p3, p3CardLabels, p3ScoreLabel);

    if (currentPlayer) {
        turnLabel->setText(QString("Turn: %1").arg(currentPlayer->name));
        hitButton->setEnabled(!currentPlayer->done);
        standButton->setEnabled(!currentPlayer->done);
    }
}

void MainWindow::nextTurn()
{
    if (playersQueue.isEmpty()) return;

    int playersChecked = 0;
    while (playersChecked < 3) {
        currentPlayer = playersQueue.dequeue();

        if (!currentPlayer->done && currentPlayer->score <= 21) {
            playersQueue.enqueue(currentPlayer);  // rotate turn
            updateUI();
            return;
        } else {
            playersChecked++;
            playersQueue.enqueue(currentPlayer);  // keep them in the queue
        }
    }
    Player* winner = &p1;
    int highest = 0;
    for (Player* p : {&p1, &p2, &p3}) {

        if (  p->score <= 21 && p->score > highest)
        {
            highest = p->score;
            winner = p;}

    }
    // qDebug() << winner->name;
    if (winner) {
        QMessageBox::information(this, "Game Over",
                                 QString("Winner is %1 with %2 points!")
                                     .arg(winner->name)
                                     .arg(winner->score));
    } else {
        QMessageBox::information(this, "Game Over", "No winners!");
    }
    hitButton->setEnabled(false);   // ✅ this requires we're inside MainWindow
    standButton->setEnabled(false);
}void MainWindow::on_hitButton_clicked()
{
    if (deck.isEmpty()) return;

    auto card = deck.pop();
    currentPlayer->cards.append(card);
    currentPlayer->score += getCardValue(card.first);

    if (currentPlayer->score > 21) {
        currentPlayer->done = true;
        QMessageBox::information(this, "Bust!", QString("%1 busted!").arg(currentPlayer->name));
    }

    updateUI();
    nextTurn();  // Always call nextTurn to rotate
}

void MainWindow::on_standButton_clicked()
{
    currentPlayer->done = true;
    updateUI();
    nextTurn();  // Always call nextTurn to rotate
}
