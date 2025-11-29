#ifndef CHATBOTWIDGET_H
#define CHATBOTWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include "ChatBot.h"


class ChatMessage : public QWidget {
    Q_OBJECT

public:
    enum MessageType {User, Bot};
    ChatMessage(const QString& text, MessageType type, QWidget* parent = nullptr);

private:
    QLabel* messageLabel;
    MessageType msgType;
};

class ChatbotWidget : public QWidget {
    Q_OBJECT

public:
    explicit ChatbotWidget(ChatBot* chatbot, QWidget* parent = nullptr);

private slots:
    void sendMsg();
    void response(const QString& response);
    void handleError(const QString& error);

private:
    void setupUI();
    void addMessage(const QString& text, ChatMessage::MessageType type);
    void scroll();

    ChatBot* chatbot;
    QVBoxLayout* chatLayout;
    QScrollArea* scrollArea;
    QWidget* chatContainer;
    QLineEdit* inputField;
    QPushButton* sendButton;
    QVBoxLayout* msgLayout;
};

#endif
