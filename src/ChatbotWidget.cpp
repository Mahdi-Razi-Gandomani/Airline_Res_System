#include "ChatbotWidget.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QScrollBar>



ChatMessage::ChatMessage(const QString& text, MessageType type, QWidget* parent)
    : QWidget(parent), msgType(type) {
    
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 5, 10, 5);
    
    messageLabel = new QLabel(text);
    messageLabel->setWordWrap(true);
    messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    messageLabel->setMaximumWidth(400);
    
    if (type == User) {
        // User message
        layout->addStretch();
        layout->addWidget(messageLabel);
        messageLabel->setStyleSheet(
            "QLabel {"
            "background: #2e5bff;"
            "color: white;"
            "border-radius: 15px;"
            "padding: 10px 15px;"
            "font-size: 14px;"
            "}"
        );
    } else {
        // Bot message
        layout->addWidget(messageLabel);
        layout->addStretch();
        messageLabel->setStyleSheet(
            "QLabel {"
            "background: #f0f0f0;"
            "color: #1a1a1a;"
            "border-radius: 15px;"
            "padding: 10px 15px;"
            "font-size: 14px;"
            "border: 1px solid #e0e0e0;"
            "}"
        );
    }
    
    setLayout(layout);
}



ChatbotWidget::ChatbotWidget(ChatBot* chatbot, QWidget* parent)
    : QWidget(parent), chatbot(chatbot) {
    
    setupUI();
    
    // Connect to relevant
    connect(chatbot, &ChatBot::responseReady, this, &ChatbotWidget::response);
    connect(chatbot, &ChatBot::errored, this, &ChatbotWidget::handleError);
    

    addMessage("Hello. How can I assist you today?\n", ChatMessage::Bot);
}

void ChatbotWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // Chat area
    scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea {"
        "border: none;"
        "background: white;"
        "}"
    );
    
    chatContainer = new QWidget();
    msgLayout = new QVBoxLayout(chatContainer);
    msgLayout->setAlignment(Qt::AlignTop);
    msgLayout->setSpacing(10);
    msgLayout->setContentsMargins(15, 15, 15, 15);
    
    scrollArea->setWidget(chatContainer);
    mainLayout->addWidget(scrollArea, 1);
    
    // Input area
    QWidget* inputWidget = new QWidget();
    inputWidget->setStyleSheet(
        "QWidget {"
        "background: white;"
        "border-top: 2px solid #e0e0e0;"
        "}"
    );
    
    QHBoxLayout* inputLayout = new QHBoxLayout(inputWidget);
    inputLayout->setSpacing(10);
    inputLayout->setContentsMargins(15, 15, 15, 15);
    
    inputField = new QLineEdit();
    inputField->setPlaceholderText("Type your message here...");
    inputField->setStyleSheet(
        "QLineEdit {"
        "height: 45px;"
        "padding: 0 15px;"
        "border-radius: 22px;"
        "border: 2px solid #e0e0e0;"
        "background: white;"
        "font-size: 14px;"
        "}"
        "QLineEdit:focus {"
        "border: 2px solid #2e5bff;"
        "}"
    );
    
    connect(inputField, &QLineEdit::returnPressed, this, &ChatbotWidget::sendMsg);
    
    sendButton = new QPushButton("Send");
    sendButton->setFixedSize(100, 45);
    sendButton->setStyleSheet(
        "QPushButton {"
        "background: #2e5bff;"
        "color: white;"
        "border-radius: 22px;"
        "font-weight: bold;"
        "font-size: 14px;"
        "border: none;"
        "}"
        "QPushButton:hover {"
        "background: #1a4fd6;"
        "}"
        "QPushButton:disabled {"
        "background: #cccccc;"
        "}"
    );
    
    connect(sendButton, &QPushButton::clicked, this, &ChatbotWidget::sendMsg);
    
    inputLayout->addWidget(inputField);
    inputLayout->addWidget(sendButton);
    
    mainLayout->addWidget(inputWidget);
    
    setLayout(mainLayout);
}

void ChatbotWidget::sendMsg() {
    QString message = inputField->text().trimmed();
    if (message.isEmpty()) {
        return;
    }
    
    // Add user message to chat
    addMessage(message, ChatMessage::User);
    
    inputField->clear();
    sendButton->setEnabled(false);
    inputField->setEnabled(false);

    addMessage("Thinking...", ChatMessage::Bot);
    
    // Process query
    chatbot->process(message);
}

void ChatbotWidget::response(const QString& response) {
    QLayoutItem* lastItem = msgLayout->takeAt(msgLayout->count() - 1);
    if (lastItem) {
        delete lastItem->widget();
        delete lastItem;
    }
    
    // Response
    addMessage(response, ChatMessage::Bot);

    sendButton->setEnabled(true);
    inputField->setEnabled(true);
    inputField->setFocus();
}

void ChatbotWidget::handleError(const QString& error) {
    QLayoutItem* lastItem = msgLayout->takeAt(msgLayout->count() - 1);
    if (lastItem) {
        delete lastItem->widget();
        delete lastItem;
    }
    
    // Show message
    addMessage("Sorry," + error + "\nPlease try again.", ChatMessage::Bot);
    
    sendButton->setEnabled(true);
    inputField->setEnabled(true);
    inputField->setFocus();
}

void ChatbotWidget::addMessage(const QString& text, ChatMessage::MessageType type) {
    ChatMessage* message = new ChatMessage(text, type);
    msgLayout->addWidget(message);
    scroll();
}

void ChatbotWidget::scroll() {
    QScrollBar* scrollBar = scrollArea->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}
