#ifndef MUSICWEBMUSICRADIOPLAYWIDGET_H
#define MUSICWEBMUSICRADIOPLAYWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QTimer>
#include "musicabstractmovewidget.h"

class QNetworkCookieJar;
class MusicLrcAnalysis;
class MusicCoreMPlayer;
class MusicRadioSongsThread;
class MusicRadioPlayListThread;

namespace Ui {
class MusicWebMusicRadioPlayWidget;
}

/*! @brief The class of the web music radio widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicWebMusicRadioPlayWidget : public MusicAbstractMoveWidget
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicWebMusicRadioPlayWidget(QWidget *parent = 0);

    virtual ~MusicWebMusicRadioPlayWidget();

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Set network cookie.
     */
    void setNetworkCookie(QNetworkCookieJar *jar);
    /*!
     * Update radio list by given category.
     */
    void updateRadioList(const QString &category);

public Q_SLOTS:
    /*!
     * Set radio to play.
     */
    void radioPlay();
    /*!
     * Set radio to previous.
     */
    void radioPrevious();
    /*!
     * Set radio to next.
     */
    void radioNext();
    /*!
     * Set radio volume.
     */
    void radioVolume(int num);
    /*!
     * Get playList finished.
     */
    void getPlayListFinished();
    /*!
     * Get song information finished.
     */
    void getSongInfoFinished();
    /*!
     * Lrc download state changed.
     */
    void lrcDownloadStateChanged();
    /*!
     * Small pic download state changed.
     */
    void picDownloadStateChanged();
    /*!
     * Current position changed.
     */
    void positionChanged(qint64 position);
    /*!
     * Current duration changed.
     */
    void durationChanged(qint64 duration);
    /*!
     * Override show function.
     */
    void show();

protected:
    /*!
     * Override the widget event.
     */
    virtual void closeEvent(QCloseEvent *event) override;
    /*!
     * Create core module.
     */
    void createCoreModule();
    /*!
     * Start to play music radio.
     */
    void startToPlay();

    Ui::MusicWebMusicRadioPlayWidget *m_ui;
    int m_currentPlayListIndex;
    bool m_isPlaying;
    QTimer m_autoNextTimer;
    MusicLrcAnalysis *m_analysis;
    MusicCoreMPlayer *m_radio;
    MusicRadioPlayListThread *m_playListThread;
    MusicRadioSongsThread *m_songsThread;
    QStringList m_playListIds;

};

#endif // MUSICWEBMUSICRADIOPLAYWIDGET_H
