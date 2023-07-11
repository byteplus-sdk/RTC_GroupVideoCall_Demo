#ifndef BYTERTC_KTV_MANAGER_INTERFACE_H
#define BYTERTC_KTV_MANAGER_INTERFACE_H

#include "bytertc_ktv_defines.h"
namespace bytertc {

/** 
 * @hidden currently not available
 * @type callback
 * @brief KTV manager event handler.
 */
class IKTVManagerEventHandler {

public:
    /**
     * @hidden Constructor/Destructor
     * @brief Destructor
     */
    virtual ~IKTVManagerEventHandler() {
    }

    /** 
     * @type callback
     * @brief The music list callback.
     * @param [in] music_infos Array of the music information. See MusicInfo{@link #MusicInfo}.
     * @param [in] music_count The size of the requested music list.
     * @param [in] total_musics_size The total size of the music list.
     * @param [in] error_code Error code. The value 0 indicates success. For the indications of other values, see KTVErrorCode{@link #KTVErrorCode}.
     */
    virtual void onMusicListResult(MusicInfo* music_infos, int music_count, int total_musics_size, KTVErrorCode error_code) {
        (void)music_infos;
        (void)music_count;
        (void)total_musics_size;
        (void)error_code;
    }

    /** 
     * @type callback
     * @brief The search music callback.
     * @param [in] music_infos Array of the music information. See MusicInfo{@link #MusicInfo}.
     * @param [in] music_count The size of the requested music list.
     * @param [in] total_musics_size The total size of the music list.
     * @param [in] error_code Error code. The value 0 indicates success. For the indications of other values, see KTVErrorCode{@link #KTVErrorCode}.
     */
    virtual void onSearchMusicResult(MusicInfo* music_infos, int music_count, int total_musics_size, KTVErrorCode error_code) {
        (void)music_infos;
        (void)music_count;
        (void)total_musics_size;
        (void)error_code;
    }

    /** 
     * @type callback
     * @brief The hot music callback.
     * @param [in] hot_infos Array of the hot music information. See HotMusicInfo{@link #HotMusicInfo}.
     * @param [in] hot_count The size of the requested hot music list.
     * @param [in] error_code Error code. The value 0 indicates success. For the indications of other values, see KTVErrorCode{@link #KTVErrorCode}.
     */
    virtual void onHotMusicResult(HotMusicInfo* hot_infos, int hot_count, KTVErrorCode error_code) {
        (void)hot_infos;
        (void)hot_count;
        (void)error_code;
    }

    /** 
     * @type callback
     * @brief The music detail callback.
     * @param [in] music_info Music information. See MusicInfo{@link #MusicInfo}.
     * @param [in] error_code Error code. The value 0 indicates success. For the indications of other values, see KTVErrorCode{@link #KTVErrorCode}.
     */
    virtual void onMusicDetailResult(MusicInfo* music_info, KTVErrorCode error_code) {
        (void)music_info;
        (void)error_code;        
    }

    /** 
     * @type callback
     * @brief Download success callback.
     * @param [in] download_id Download task ID.
     * @param [in] download_info Download result. See DownloadResult{@link #DownloadResult}.
     */
    virtual void onDownloadSuccess(int download_id, DownloadResult* download_info) {
        (void)download_id;
        (void)download_info; 
    }

    /** 
     * @type callback
     * @brief Download failure callback.
     * @param download_id Download task ID.
     * @param error_code Error code. See KTVErrorCode{@link #KTVErrorCode}.
     */
    virtual void onDownloadFailed(int download_id, KTVErrorCode error_code) {
        (void)download_id;
        (void)error_code; 
    }

    /** 
     * @type callback
     * @brief Music file download progress callback.
     * @param download_id Download task ID.
     * @param download_percentage The percentage of download progress, in the range of [0,100].
     */
    virtual void onDownloadMusicProgress(int download_id, int download_percentage) {
        (void)download_id;
        (void)download_percentage;
    }
};

/** 
 * @hidden currently not available
 * @type callback
 * @brief KTV player event handler.
 */
class IKTVPlayerEventHandler {
public:
    /**
     * @hidden Constructor/Destructor
     * @brief Destructor
     */
    virtual ~IKTVPlayerEventHandler() {
    }
    
    /** 
     * @type callback
     * @brief Music playing progress callback.
     * @param [in] music_id Music ID.
     * @param [in] progress Music playing progress in milliseconds.
     */
    virtual void onPlayProgress(const char *music_id, int64_t progress) {
        (void)music_id;
        (void)progress;
    }
    
    /** 
     * @type callback
     * @brief Music playing state changed callback.
     * @param [in] music_id Music ID.
     * @param [in] play_state Music playing status. See PlayState{@link #PlayState}.
     * @param [in] error_code Error code. See KTVPlayerErrorCode{@link #KTVPlayerErrorCode}.
     * @notes <br>
     *       You will receive this callback on following events.
     *       + When you successfully start playing music by calling playMusic{@link #IKTVPlayer#playMusic}, you will receive this callback with `PlayStatePlaying` playState. Otherwise the playState will be `PlayStateFailed`.
     *       + If the music with the same music ID is playing when you call playMusic{@link #IKTVPlayer#playMusic} again, the music will restart from the starting position, and you will receive this callback with `PlayStatePlaying` playState to inform the latter music has started.
     *       + When you successfully pause the music by calling pauseMusic{@link #IKTVPlayer#pauseMusic}, you will receive this callback with `PlayStatePaused` playState. Otherwise the playState will be `PlayStateFailed`.
     *       + When you successfully resume the music by calling resumeMusic{@link #IKTVPlayer#resumeMusic}, you will receive this callback with `PlayStatePlaying` playState. Otherwise the playState will be `PlayStateFailed`.
     *       + When you successfully stop the music by calling stopMusic{@link #IKTVPlayer#stopMusic}, you will receive this callback with `PlayStateStoped` playState. Otherwise the playState will be `PlayStateFailed`.
     *       + When the music ends, you will receive this callback with `PlayStateFinished` playState.
     */
    virtual void onPlayStateChanged(const char* music_id, PlayState play_state, bytertc::KTVPlayerErrorCode error_code) {
        (void)music_id;
        (void)play_state;
        (void)error_code;
    }
};

/** 
 * @hidden currently not available
 * @type api
 * @brief KTV player interfaces.
 */
class IKTVPlayer {
public:
    /**
     * @hidden Constructor/Destructor
     * @brief Destructor
     */
    virtual ~IKTVPlayer() = default;
    
    /** 
     * @type api
     * @brief Sets the KTV player event handler.
     * @param [in] handler KTV player event handler. See IKTVPlayerEventHandler{@link #IKTVPlayerEventHandler}.
     */
    virtual void setPlayerEventHandler(IKTVPlayerEventHandler *handler) = 0;

    /** 
     * @type api
     * @brief Plays the music.
     * @param [in] music_id Music ID.
     *        If the song with the same music_id is playing when you call this API, the music will restart from the starting position. An error will be triggered if the audio file corresponding to music_id does not exist.
     * @param [in] track_type Audio track type of the KTV player. See AudioTrackType{@link #AudioTrackType}.
     * @param [in] play_type Audio play type. See AudioPlayType{@link #AudioPlayType}.
     * @notes  <br>
     *       + After calling this API, you will receive the music play state through onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback.
     *       + If the music ID is invalid, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3023 and a play_state of 4.
     *       + If you didn't join the room, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3022 and a play_state of 4.
     *       + If the music file does not exist, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3020 and a play_state of 4.
     */
    virtual void playMusic(const char* music_id, AudioTrackType track_type, AudioPlayType play_type) = 0;

    /** 
     * @type api
     * @brief Pauses the music.
     * @param [in] music_id Music ID.
     * @notes  <br>
     *       + After calling this API, you will receive the music play state through onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback.
     *       + If the music ID is invalid, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3023 and a play_state of 4.
     *       + If you didn't join the room, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3022 and a play_state of 4.
     */
    virtual void pauseMusic(const char *music_id) = 0;

    /** 
     * @type api
     * @brief Resumes playing the music.
     * @param [in] music_id Music ID.
     * @notes  <br>
     *       + After calling this API, you will receive the music play state through onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback.
     *       + If the music ID is invalid, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3023 and a play_state of 4.
     *       + If you didn't join the room, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3022 and a play_state of 4.
     */
    virtual void resumeMusic(const char *music_id) = 0;

    /** 
     * @type api
     * @brief Stops playing the music.
     * @param [in] music_id Music ID.
     * @notes  <br>
     *       + After calling this API, you will receive the music play state through onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback.
     *       + If the music ID is invalid, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3023 and a play_state of 4.
     *       + If you didn't join the room, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3022 and a play_state of 4.
     */
    virtual void stopMusic(const char *music_id) = 0;

    /** 
     * @type api
     * @brief Sets the starting position of the music file.
     * @param [in] music_id Music ID.
     * @param [in] position The starting position of the music file in milliseconds. The value must be less than the total length of the music.
     * @notes  <br>
     *       + The music must be playing when you call this API.
     *       + After calling this API, you will receive the music play state through onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback.
     *       + If the music ID is invalid, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3023 and a play_state of 4.
     *       + If you didn't join the room, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3022 and a play_state of 4.
     */
    virtual void seekMusic(const char *music_id, int position) = 0;

    /** 
     * @type api
     * @brief Sets the volume of the playing music. The music must be playing when you set the volume.
     * @param [in] music_id Music ID.
     * @param [in] volume Volume. Adjustment range: [0,400].
     *         + 0: Mute.
     *         + 100: Original volume.
     *         + 400: 4 times the original volume (with overflow protection).
     * @notes  <br>
     *       + The music must be playing when you call this API.
     *       + If the set volume is greater than 400, it will be adjusted by the maximum value of 400; if the set volume is less than 0, it will be adjusted by the minimum value of 0.
     *       + If the music ID is invalid, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3023 and a play_state of 4.
     *       + If you didn't join the room, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3022 and a play_state of 4.
     */
    virtual void setMusicVolume(const char *music_id, int volume) = 0;

    /** 
     * @type api
     * @brief Switches the audio track type between the original track and the instrumental track.
     * @param [in] music_id Music ID.
     * @notes The music must be playing when you call this API.
     */
    virtual void switchAudioTrackType(const char *music_id) = 0;

    /** 
     * @type api
     * @brief Transposes up/down the music being played.
     * @param music_id Music ID.
     * @param pitch The pitch up/down value relative to the original pitch, in the range of [-12, 12], with the default value of 0.
     *              The difference in pitch between two adjacent values is a semitone. A positive value indicates an increase in pitch, and a negative value indicates a decrease in pitch. A larger absolute value means more pitch increase or decrease.
     * @notes <br>
     *       + The music must be in the playing when you call this API.
     *       + If the set pitch is greater than 12, it will be adjusted by the maximum value of 12; if the set pitch is less than –12, it will be adjusted by the minimum value of –12.
     *       + If the music ID is invalid, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3023 and a play_state of 4.
     *       + If you didn't join the room, you will receive the onPlayStateChanged{@link #IKTVPlayerEventHandler#onPlayStateChanged} callback, with an error_code of -3022 and a play_state of 4.
     */
    virtual void setMusicPitch(const char* music_id, int pitch) = 0;

};

/** 
 * @hidden currently not available
 * @type api
 * @brief KTV manager interfaces.
 */
class IKTVManager {
public:
    /**
     * @hidden Constructor/Destructor
     * @brief Destructor
     */
    virtual ~IKTVManager() = default;
    
    /** 
     * @type api
     * @brief Sets the maximum cache for storing music files.
     * @param [in] max_cache_size_MB The maximum cache to be set in MB.
     *        If the setting value is less than or equal to 0, it will be adjusted to 1,024 MB.
     */
    virtual void setMaxCacheSize(int max_cache_size_MB) = 0;

    /** 
     * @type api
     * @brief Sets the KTV event handler.
     * @param [in] ktvManagerEventHandler KTV event handler. See IKTVManagerEventHandler{@link #IKTVManagerEventHandler}.
     */
    virtual void setKTVManagerEventHandler(IKTVManagerEventHandler *ktvManagerEventHandler) = 0;

    /** 
     * @type api
     * @brief Gets the music list.
     * @param [in] page_num Page number. The default value is 1.
     * @param [in] page_size The number of the music that displays on one page.
     * @param [in] filters The filter type of the music list. See MusicFilterType{@link #MusicFilterType}.
     * @notes After calling this API, you will receive the music list through onMusicListResult{@link #IKTVManagerEventHandler#onMusicListResult} callback.
     */
    virtual void getMusicList(int page_num, int page_size,  int filters) = 0;

    /** 
     * @type api
     * @brief Search music by keywords.
     * @param [in] key_word Keyword. The string should be no more than 20 characters.
     * @param [in] page_num Page number. The default value is 1.
     * @param [in] page_size The number of the music that displays on one page.
     * @param [in] filters The filter type of the music list. See MusicFilterType{@link #MusicFilterType}.
     * @notes After calling this API, you will receive the music list through onSearchMusicResult{@link #IKTVManagerEventHandler#onSearchMusicResult} callback.
     */
    virtual void searchMusic(const char* key_word, int page_num, int page_size, int filters) = 0;

    /** 
     * @type api
     * @brief Gets hot music according to music types.
     * @param [in] hot_types Hot music type. See MusicHotType{@link #MusicHotType}.
     * @param [in] filters The filter type of the music list. See MusicFilterType{@link #MusicFilterType}.
     * @notes After calling this API, you will receive the music list through onHotMusicResult{@link #IKTVManagerEventHandler#onHotMusicResult} callback.
     */
    virtual void getHotMusic(int hot_types, int filters) = 0;


    /** 
     * @type api
     * @brief Gets music detail.
     * @param [in] music_id Music ID.
     * @notes After calling this API, you will receive the music detial through onMusicDetailResult{@link #IKTVManagerEventHandler#onMusicDetailResult} callback.
     */
    virtual void getMusicDetail(const char* music_id) = 0;

    /** 
     * @type api
     * @brief Download music.
     * @param [in] music_id Music ID.
     * @return Download task ID.
     * @notes  <br>
     *       + If the music is successfully downloaded, you will receive onDownloadSuccess{@link #IKTVManagerEventHandler#onDownloadSuccess}.
     *       + If the music fails to download, you will receive onDownloadFailed{@link #IKTVManagerEventHandler#onDownloadFailed}.
     *       + When the music download progress is updated, you will receive onDownloadMusicProgress{@link #IKTVManagerEventHandler#onDownloadMusicProgress}.
     */
    virtual int downloadMusic(const char* music_id) = 0;

    /** 
     * @type api
     * @brief Download lyrics.
     * @param [in] music_id Music ID.
     * @param [in] type The lyrics file's format. See DownloadLyricType{@link #DownloadLyricType}.
     * @return Download task ID.
     * @notes  <br>
     *       + If the lyrics are successfully downloaded, you will receive onDownloadSuccess{@link #IKTVManagerEventHandler#onDownloadSuccess}.
     *       + If the lyrics fail to download, you will receive onDownloadFailed{@link #IKTVManagerEventHandler#onDownloadFailed}.
     */
    virtual int downloadLyric(const char* music_id, DownloadLyricType type) = 0;

    /** 
     * @type api
     * @brief Download MIDI files.
     * @param [in] music_id Music ID.
     * @return Download task ID.
     * @notes  <br>
     *       + If the file is successfully downloaded, you will receive onDownloadSuccess{@link #IKTVManagerEventHandler#onDownloadSuccess}.
     *       + If the file fails to download, you will receive onDownloadFailed{@link #IKTVManagerEventHandler#onDownloadFailed}.
     */
    virtual int downloadMidi(const char* music_id) = 0;

    /** 
     * @type api
     * @brief Cancels download task.
     * @param [in] download_id Download task ID.
     */
    virtual void cancelDownload(int download_id) = 0;


    /** 
     * @type api
     * @brief Clear music cache, including music and lyrics.
     */
    virtual void clearCache() = 0;

    /** 
     * @type api
     * @brief Gets the KTV player.
     * @return KTV player interfaces. See IKTVPlayer{@link #IKTVPlayer}.
     */
    virtual IKTVPlayer *getKTVPlayer() = 0;

};

} // namespace bytertc
#endif // BYTERTC_KTV_MANAGER_INTERFACE_H
