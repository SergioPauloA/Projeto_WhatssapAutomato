#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStringList>
#include <QMessageBox>
#include <QThread>
#include <QtNetwork>

class WhatsAppSender : public QWidget {
    Q_OBJECT

public:
    WhatsAppSender(QWidget* parent = nullptr);

private:
    QLineEdit* transmitterNumber;
    QLineEdit* recipientNumbers;
    QTextEdit* messageText;
    QProgressBar* progressBar;
    QLabel* progressLabel;
    QPushButton* useTransmitterBtn;
    QPushButton* readyBtn;
    QPushButton* sendBtn;

    void onUseTransmitter();
    void onReady();
    void onSendMessage();
    bool validatePhoneNumber(const QString& phone);
    void sendWhatsAppMessage(const QStringList& recipientList, const QString& message);
    void sendRequestToTwilio(const QString& to, const QString& message, int index);
};

WhatsAppSender::WhatsAppSender(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout;

    // Número do transmissor
    transmitterNumber = new QLineEdit(this);
    transmitterNumber->setPlaceholderText("Digite o número do transmissor");
    mainLayout->addWidget(transmitterNumber);

    useTransmitterBtn = new QPushButton("Usar esse número", this);
    connect(useTransmitterBtn, &QPushButton::clicked, this, &WhatsAppSender::onUseTransmitter);
    mainLayout->addWidget(useTransmitterBtn);

    // Número(s) do destinatário
    recipientNumbers = new QLineEdit(this);
    recipientNumbers->setPlaceholderText("Digite os números dos destinatários");
    mainLayout->addWidget(recipientNumbers);

    readyBtn = new QPushButton("Pronto", this);
    connect(readyBtn, &QPushButton::clicked, this, &WhatsAppSender::onReady);
    mainLayout->addWidget(readyBtn);

    // Mensagem
    messageText = new QTextEdit(this);
    messageText->setPlaceholderText("Escreva a mensagem do coração");
    mainLayout->addWidget(messageText);

    sendBtn = new QPushButton("Enviar a todos", this);
    connect(sendBtn, &QPushButton::clicked, this, &WhatsAppSender::onSendMessage);
    mainLayout->addWidget(sendBtn);

    // Barra de progresso
    progressBar = new QProgressBar(this);
    progressLabel = new QLabel("Enviando", this);
    QHBoxLayout* progressLayout = new QHBoxLayout;
    progressLayout->addWidget(progressLabel);
    progressLayout->addWidget(progressBar);
    mainLayout->addLayout(progressLayout);

    setLayout(mainLayout);
    setWindowTitle("Envio de Mensagens via WhatsApp");
}

bool WhatsAppSender::validatePhoneNumber(const QString& phone) {
    // Expressão regular para validar o número de telefone no formato "(xx) xxxxxxxxx"
    QRegExp regex("^\\(\\d{2}\\) \\d{9}$");
    return regex.exactMatch(phone);
}

void WhatsAppSender::onUseTransmitter() {
    QString phone = transmitterNumber->text().trimmed();
    if (phone.isEmpty()) {
        QMessageBox::warning(this, "Erro", "O número do transmissor não pode ser vazio.");
        return;
    }

    // Validação do número do transmissor
    if (!validatePhoneNumber(phone)) {
        QMessageBox::warning(this, "Erro", "Número de telefone do transmissor inválido. O formato correto é: (xx) xxxxxxxxx");
        return;
    }

    QMessageBox::information(this, "Sucesso", "Número do transmissor confirmado!");
}

void WhatsAppSender::onReady() {
    QString recipients = recipientNumbers->text().trimmed();
    QStringList recipientList = recipients.split("\n", QString::SkipEmptyParts);

    // Validação dos números dos destinatários
    for (const QString& phone : recipientList) {
        if (!validatePhoneNumber(phone)) {
            QMessageBox::warning(this, "Erro", "Um ou mais números de destinatário estão no formato incorreto.");
            return;
        }
    }

    // Mostra a tela de mensagem
    QMessageBox::information(this, "Sucesso", "Destinatários confirmados. Escreva sua mensagem.");
}

void WhatsAppSender::onSendMessage() {
    QString message = messageText->toPlainText().trimmed();
    if (message.isEmpty()) {
        QMessageBox::warning(this, "Erro", "A mensagem não pode ser vazia.");
        return;
    }

    QString recipients = recipientNumbers->text().trimmed();
    QStringList recipientList = recipients.split("\n", QString::SkipEmptyParts);

    // Inicia a simulação de envio de mensagens
    sendWhatsAppMessage(recipientList, message);
}

void WhatsAppSender::sendWhatsAppMessage(const QStringList& recipientList, const QString& message) {
    progressBar->setRange(0, recipientList.size());
    progressLabel->setText("Enviando");

    for (int i = 0; i < recipientList.size(); ++i) {
        // Simulando envio de WhatsApp
        QString recipient = recipientList[i];
        progressBar->setValue(i + 1);

        // Chama o método para enviar a mensagem via WhatsApp pela API Twilio
        sendRequestToTwilio(recipient, message, i);

        // Atualiza a label após cada envio
        progressLabel->setText("Enviado para " + recipient);

        // Simulando atraso de envio
        QThread::sleep(1);
    }

    progressLabel->setText("Pronto");
    QMessageBox::information(this, "Sucesso", "Todas as mensagens foram enviadas!");
}

void WhatsAppSender::sendRequestToTwilio(const QString& to, const QString& message, int index) {
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    // Modifique esta URL para enviar mensagens via WhatsApp
    QNetworkRequest request(QUrl("https://api.twilio.com/2010-04-01/Accounts/YOUR_ACCOUNT_SID/Messages.json"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray postData;
    // Use o formato whatsapp:+1234567890
    postData.append("To=whatsapp:" + to.replace("(", "").replace(")", "").replace(" ", "").toUtf8());
    postData.append("&From=whatsapp:YOUR_TWILIO_WHATSAPP_NUMBER");
    postData.append("&Body=" + message.toUtf8());

    // Autenticação básica com SID e Token da Twilio
    QString auth = "Basic " + QByteArray("YOUR_ACCOUNT_SID:YOUR_AUTH_TOKEN").toBase64();
    request.setRawHeader("Authorization", auth.toUtf8());

    // Enviar requisição
    QNetworkReply* reply = manager->post(request, postData);

    // Processar a resposta
    connect(reply, &QNetworkReply::finished, this, [reply, index]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Mensagem enviada com sucesso para " << index;
        } else {
            qDebug() << "Erro ao enviar mensagem:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    WhatsAppSender window;
    window.show();
    return app.exec();
}
