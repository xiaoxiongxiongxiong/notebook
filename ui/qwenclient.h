// qwenclient.h
#ifndef QWENCLIENT_H
#define QWENCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>

class QwenClient : public QObject
{
    Q_OBJECT

public:
    explicit QwenClient(const QString & apiKey, QObject * parent = nullptr);
    virtual ~QwenClient();
    void askQuestion(const QString & question);

signals:
    void answerReceived(const QString & answer);
    void requestError(const QString & errorMessage);

private slots:
    void onReplyFinished(QNetworkReply * reply);

private:
    QNetworkAccessManager * m_manager = nullptr;
    QString m_apiKey;
};

#endif // QWENCLIENT_H