// qwenclient.cpp
#include "qwenclient.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

QwenClient::QwenClient(const QString & apiKey, QObject * parent)
    : QObject(parent), m_apiKey(apiKey)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &QwenClient::onReplyFinished);
}

QwenClient::~QwenClient()
{
    if (nullptr != m_manager)
    {
        delete m_manager;
        m_manager = nullptr;
    }
}

void QwenClient::askQuestion(const QString & question)
{
    QUrl url("https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + m_apiKey).toUtf8());

    // 构造请求体
    QJsonObject root;
    root["model"] = "qwen3-max"; // 可换为 qwen-turbo（更快更便宜）

    QJsonObject input;
    QJsonArray messages;
    QJsonObject msg;
    msg["role"] = "user";
    msg["content"] = question;
    messages.append(msg);
    input["messages"] = messages;
    root["input"] = input;

    // 可选：设置参数
    QJsonObject parameters;
    parameters["seed"] = 1234;
    parameters["result_format"] = "message";
    root["parameters"] = parameters;

    QJsonDocument doc(root);
    QByteArray jsonData = doc.toJson();

    m_manager->post(request, jsonData);
}

void QwenClient::onReplyFinished(QNetworkReply * reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        emit requestError(QStringLiteral("网络错误: %1").arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument responseDoc = QJsonDocument::fromJson(data);
    QJsonObject obj = responseDoc.object();

    // 检查是否调用成功
    if (obj.contains("output") && obj["output"].isObject())
    {
        QJsonObject output = obj["output"].toObject();
        if (output.contains("choices") && output["choices"].isArray())
        {
            QJsonArray choices = output["choices"].toArray();
            if (!choices.isEmpty())
            {
                QJsonObject firstChoice = choices[0].toObject();
                if (firstChoice.contains("message"))
                {
                    QJsonObject message = firstChoice["message"].toObject();
                    QString answer = message["content"].toString();
                    emit answerReceived(answer);
                    reply->deleteLater();
                    return;
                }
            }
        }
    }

    // 处理错误（如 quota 超限、认证失败等）
    QString errorMsg = "未知错误";
    if (obj.contains("code"))
    {
        errorMsg = QString("错误 %1: %2")
            .arg(obj["code"].toString())
            .arg(obj["message"].toString());
    }
    emit requestError(errorMsg);
    reply->deleteLater();
}