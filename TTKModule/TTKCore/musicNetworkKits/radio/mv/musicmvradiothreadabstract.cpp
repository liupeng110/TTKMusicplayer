#include "musicmvradiothreadabstract.h"

MusicMVRadioThreadAbstract::MusicMVRadioThreadAbstract(QObject *parent)
    : MusicDownLoadQueryThreadAbstract(parent)
{
    m_searchText = "1";
}

QString MusicMVRadioThreadAbstract::getClassName()
{
    return staticMetaObject.className();
}

void MusicMVRadioThreadAbstract::startToSearch(QueryType type, const QString &text)
{
    Q_UNUSED(type);

    deleteAll();
    m_searchText = text.isEmpty() ? "1" : text;
    m_interrupt = true;

    QNetworkRequest request;
    QUrl musicUrl = MusicUtils::Algorithm::mdII(MV_CATEGORY_URL, false);
    request.setUrl(musicUrl);
    setSslConfiguration(&request);

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(finished()), SLOT(downLoadFinished()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
}
