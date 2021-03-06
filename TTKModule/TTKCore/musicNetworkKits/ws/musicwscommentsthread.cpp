#include "musicwscommentsthread.h"
#include "musicsemaphoreloop.h"
#include "musicdownloadquerywsthread.h"
#include "musicnumberdefine.h"
#///QJson import
#include "qjson/parser.h"

#include <QTimer>

MusicWSSongCommentsThread::MusicWSSongCommentsThread(const QString &type, QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
    m_querySearchType = type;
}

QString MusicWSSongCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWSSongCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));
    MusicSemaphoreLoop loop;
    MusicDownLoadQueryWSThread *query = new MusicDownLoadQueryWSThread(m_querySearchType, this);
    query->setQueryAllRecords(false);
    query->setQuerySimplify(true);
    query->startToSearch(MusicDownLoadQueryThreadAbstract::MusicQuery, name);
    connect(query, SIGNAL(downLoadDataChanged(QString)), &loop, SLOT(quit()));
    loop.exec();

    m_rawData["songID"] = 0;
    if(!query->getMusicSongInfos().isEmpty())
    {
        m_rawData["songID"] = query->getMusicSongInfos().first().m_songId.toInt();
        startToPage(0);
    }
}

void MusicWSSongCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();
    m_pageTotal = 0;
    m_interrupt = true;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_SG_COMMIT_URL, false)
                    .arg(m_querySearchType).arg(m_rawData["songID"].toInt()).arg(m_pageSize).arg(offset + 1);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWSSongCommentsThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantMap page = value["page"].toMap();
                m_pageTotal = page["totalCount"].toInt();

                QVariantList datas = value["comments"].toList();
                foreach(const QVariant &data, datas)
                {
                    if(m_interrupt) return;

                    if(data.isNull())
                    {
                        continue;
                    }

                    value = data.toMap();
                    MusicResultsItem comment;
                    QVariantMap user = value["user"].toMap();
                    comment.m_nickName = user["nickname"].toString();
                    comment.m_coverUrl = user["img"].toString();

                    comment.m_playCount = value["repliesCount"].toString();
                    comment.m_updateTime = QString::number(QDateTime(QDate::fromString(value["createTime"].toString(),
                                                                     "yyyy-MM-dd")).toMSecsSinceEpoch());
                    comment.m_description = value["content"].toString();

                    emit createSearchedItem(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}



MusicWSPlaylistCommentsThread::MusicWSPlaylistCommentsThread(QObject *parent)
    : MusicDownLoadCommentsThread(parent)
{
    m_pageSize = 20;
}

QString MusicWSPlaylistCommentsThread::getClassName()
{
    return staticMetaObject.className();
}

void MusicWSPlaylistCommentsThread::startToSearch(const QString &name)
{
    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(name));

    m_rawData["songID"] = name;
    startToPage(0);
}

void MusicWSPlaylistCommentsThread::startToPage(int offset)
{
    if(!m_manager)
    {
        return;
    }

    M_LOGGER_INFO(QString("%1 startToSearch %2").arg(getClassName()).arg(offset));
    deleteAll();
    m_pageTotal = 0;
    m_interrupt = true;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(WS_PL_COMMIT_URL, false)
                    .arg(m_rawData["songID"].toString()).arg(m_pageSize).arg(offset + 1);

    QNetworkRequest request;
    request.setUrl(musicUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("User-Agent", MusicUtils::Algorithm::mdII(WS_UA_URL_1, ALG_UA_KEY, false).toUtf8());
    setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}

void MusicWSPlaylistCommentsThread::downLoadFinished()
{
    if(!m_reply)
    {
        deleteAll();
        return;
    }

    M_LOGGER_INFO(QString("%1 downLoadFinished").arg(getClassName()));
    m_interrupt = false;

    if(m_reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = m_reply->readAll();

        QJson::Parser parser;
        bool ok;
        QVariant data = parser.parse(bytes, &ok);
        if(ok)
        {
            QVariantMap value = data.toMap();
            if(value.contains("data"))
            {
                value = value["data"].toMap();
                QVariantMap page = value["page"].toMap();
                m_pageTotal = page["totalCount"].toInt();

                QVariantList datas = value["comments"].toList();
                foreach(const QVariant &data, datas)
                {
                    if(m_interrupt) return;

                    if(data.isNull())
                    {
                        continue;
                    }

                    value = data.toMap();
                    MusicResultsItem comment;
                    QVariantMap user = value["user"].toMap();
                    comment.m_nickName = user["nickname"].toString();
                    comment.m_coverUrl = user["img"].toString();

                    comment.m_playCount = value["repliesCount"].toString();
                    comment.m_updateTime = QString::number(QDateTime(QDate::fromString(value["createTime"].toString(),
                                                                     "yyyy-MM-dd")).toMSecsSinceEpoch());
                    comment.m_description = value["content"].toString();

                    emit createSearchedItem(comment);
                }
            }
        }
    }

    emit downLoadDataChanged(QString());
    deleteAll();
    M_LOGGER_INFO(QString("%1 downLoadFinished deleteAll").arg(getClassName()));
}
