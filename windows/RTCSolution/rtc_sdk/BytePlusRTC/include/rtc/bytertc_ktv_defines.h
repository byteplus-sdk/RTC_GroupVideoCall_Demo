#ifndef BYTERTC_KTV_DEFINES_H
#define BYTERTC_KTV_DEFINES_H
#include <string>

namespace bytertc {

/** 
 * @hidden currently not available currently not available
 * @type keytype
 * @brief The filter type of the music list.
 */
enum MusicFilterType {
    /** 
     * @brief No filter.
     */
    kFilterNone = 0,
    /** 
     * @brief Remove music that does not have lyrics.
     */
    kFilterWithoutLyric     = 1 << 0,
    /** 
     * @brief Remove music that does not support scoring.
     */
    kFilterUnsupportedScore     = 1 << 1,
    /** 
     * @brief Remove music that does not support accompany mode.
     */
    kFilterUnsupportedAccopmay  = 1 << 2,
    /** 
     * @brief Remove music that does not have a climax part.
     */
    kFilterUnsupportedClimx     = 1 << 3,
};

/** 
 * @hidden currently not available currently not available
 * @type keytype
 * @brief Hot music type.
 */
enum MusicHotType {
    /** 
     * @brief Hot music in the content center.
     */
    kHotContentCenter       = 1 << 0,
    /** 
     * @brief Hot music of the project.
     */
    kHotProject          = 1 << 1,
};

/** 
 * @hidden currently not available currently not available
 * @type keytype
 * @brief Audio track type of the KTV player.
 */
enum class AudioTrackType {
    /** 
     * @brief Play the original music with vocals.
     */
    kAudioTrackTypeOriginal = 1,
    /** 
     * @brief Play the instrumental music without vocals.
     */
    kAudioTrackTypeAccompy = 2
};

/** 
 * @hidden currently not available
 * @type keytype
 * @brief Audio play type.
 */
enum class AudioPlayType {
    /** 
     * @brief Only play on the local side.
     */
    kAudioPlayTypeLocal,
    /** 
     * @brief Only play on the remote side.
     */
    kAudioPlayTypeRemote,
    /** 
     * @brief Play on the local and remote side.
     */
    kAudioPlayTypeLocalAndRemote
};

/** 
 * @hidden currently not available
 * @type keytype
 * @brief Music playing status.
 */
enum class PlayState {
    /** 
     * @brief Playing.
     */
    kPlayStatePlaying = 1,
    /** 
     * @brief Paused.
     */
    kPlayStatePaused = 2,
    /** 
     * @brief Stopped.
     */
    kPlayStateStoped = 3,
    /** 
     * @brief Failed to play.
     */
    kPlayStateFailed = 4,
    /** 
     * @brief Finished.
     */
    kPlayStateFinished = 5,
};

/** 
 * @hidden currently not available
 * @type keytype
 * @brief The lyrics type.
 */
enum class LyricStatus {
    /** 
     * @brief No lyrics.
     */
    kLyricStatusNone = 0,
    /** 
     * @brief KRC lyrics.
     */
    kLyricStatusKRC = 1,
    /** 
     * @brief LRC lyrics.
     */
    kLyricStatusLRC = 2,
    /** 
     * @brief KRC and LRC lyrics.
     */
    kLyricStatusKRCAndLRC = 3
};

/** 
 * @hidden currently not available
 * @type keytype
 * @brief The lyrics file's format.
 */
enum class DownloadLyricType {
    /** 
     * @brief KRC lyrics file.
     */
    kDownKrc,
    /** 
     * @brief LRC lyrics file.
     */
    kDownLrc
};



/** 
 * @hidden currently not available
 * @type keytype
 * @brief Music information.
 */
struct MusicInfo
{
    /** 
     * @brief Music ID.
     */
    const char*  music_id;
    /** 
     * @brief Music name.
     */
    const char*  music_name;
    /** 
     * @brief Singer.
     */
    const char*  singer;
    /** 
     * @brief Vendor ID.
     */
    const char*  vendor_id;
    /** 
     * @brief Vendor name.
     */
    const char* vendor_name;
    /** 
     * @brief Latest update timestamp in milliseconds.
     */
    int64_t update_timestamp;
    /** 
     * @brief The URL of the music cover.
     */
    const char*  poster_url;
    /** 
     * @brief The lyrics type. See LyricStatus{@link #LyricStatus}.
     */
    LyricStatus lyric_status;
    /** 
     * @brief The length of the song in milliseconds.
     */
    int duration;
    /** 
     * @brief Whether the song supports scoring.
     */
    bool enable_score;
    /** 
     * @brief The starting time of the climax part in milliseconds.
     */
    int climax_start_time;
    /** 
     * @brief The ending time of the climax part in milliseconds.
     */
     int climax_stop_time;

};

/** 
 * @hidden currently not available
 * @type keytype
 * @brief Hot music information.
 */
struct HotMusicInfo {
    /** 
     * @brief Hot music type. See MusicHotType{@link #MusicHotType}.
     */
    MusicHotType hot_type;
    /** 
     * @brief Hot list name.
     */
    const char* hot_name;
    /** 
     * @brief Music information. See MusicInfo{@link #MusicInfo}.
     */
    MusicInfo* musics;
    /** 
     * @brief The number of music in the list.
     */
    int music_count;
};

/** 
 * @hidden currently not available
 * @type keytype
 * @brief Download file type.
 */
enum DownloadFileType {
    /** 
     * @brief Audio file.
     */
    kDownloadMusic = 1,
    /** 
     * @brief KRC lyrics file.
     */
    kDownloadKRC = 2,
    /** 
     * @brief LRC lyrics file.
     */
    kDownloadLRC = 3,
    /** 
     * @brief MIDI file.
     */
    kDownloadMIDI = 4

} ;

/** 
 * @hidden currently not available
 * @type keytype
 * @brief Download music information.
 */
struct DownloadResult {
    /** 
     * @brief Download file path.
     */
    const char* local_file_path;
    /** 
     * @brief Music ID.
     */
    const char* music_id;
    /** 
     * @brief Download file type. See DownloadFileType{@link #DownloadFileType}.
     */
    DownloadFileType type;
};


/** 
 * @hidden currently not available
 * @type keytype
 * @brief KTV error code.
 */
enum KTVErrorCode {
    /** 
     * @brief Success.
     */
    kErrorCodeKTVOK = 0,
    /** 
     * @brief Invalid AppID.
     */
    kErrorCodeKTVAppidInValid = -3000,
    /** 
     * @brief Invalid parameter.
     */
    kErrorCodeKTVParasInValid = -3001,
    /** 
     * @brief Failed to get music resources.
     */
    kErrorCodeKTVGetMusicFailed = -3002,
    /** 
     * @brief Failed to get lyrics.
     */
    kErrorCodeKTVGetLyricFailed = -3003,
    /** 
     * @brief The music is removed.
     */
    kErrorCodeKTVMusicTakedown = -3004,
    /** 
     * @brief Failed to download the music file.
     */
    kErrorCodeKTVMusicDownload = -3005,
    /** 
     * @brief Failed to download the MIDI file.
     */
    kErrorCodeKTVMidiDownloadFailed = -3006,
    /** 
     * @brief The system is busy.
     */
    kErrorCodeKTVSystemBusy = -3007,
    /** 
     * @brief Network anomaly.
     */
    kErrorCodeKTVNetwork = -3008,
    /** 
     * @brief The KTV feature is not added to the room.
     */
    kErrorCodeKTVNotJoinRoom = -3009,
    /** 
     * @brief Failed to parse data.
     */
    kErrorCodeKTVParseData = -3010,
    /** 
     * @brief Failed to download.
     */
    kErrorCodeKTVDownload = -3011,
    /** 
     * @brief Already downloading.
     */
    kErrorCodeKTVDownloading = -3012,
    /** 
     * @brief Internal error. Contact the technical support representatives for help.
     */
    kErrorCodeKTVInternalDomain = -3013
};

/** 
 * @hidden currently not available
 * @type keytype
 * @brief KTV player error code.
 */
enum KTVPlayerErrorCode {
    /** 
     * @brief Success.
     */
    kErrorCodeKTVPlayOK = 0,
    /** 
     * @brief Failed to play the music. Download first.
     */
    kErrorCodeKTVPlayFileNotExist = -3020,
    /** 
     * @brief Failed to play the music. Check the file's format.
     */
    kErrorCodeKTVPlayFileError = -3021,
    /** 
     * @brief Failed to play the music. Join a room first.
     */
    kErrorCodeKTVPlayNotJoinRoom = -3022,
    /** 
     * @brief Invalid parameter.
     */
    kErrorCodeKTVPlayParam = -3023,
    /** 
     * @brief Failed to play the music. Invalid path or failed to open the file.
     */
    kErrorCodeKTVPlayStartError = -3024,
    /** 
     * @brief Invalid mixing ID.
     */
    kErrorCodeKTVPlayMixIdError = -3025,
    /** 
     * @brief Invalid position.
     */
    kErrorCodeKTVPlayPositionError = -3026,
    /** 
     * @brief Invalid volume.
     */
    kErrorCodeKTVPlayAudioVolumeError = -3027,
    /** 
     * @brief Do not support the mix type.
     */
    kErrorCodeKTVPlayTypeError = -3028,
    /** 
     * @brief Invalid pitch.
     */
    kErrorCodeKTVPlayPitchError = -3029,
    /** 
     * @brief Invalid audio track.
     */
    kErrorCodeKTVPlayAudioTrackError = -3030,
    /** 
     * @brief Mixing in process.
     */
    kErrorCodeKTVPlayStartingError = -3031
};


} // namespace bytertc
#endif // BYTERTC_KTV_DEFINES_H
