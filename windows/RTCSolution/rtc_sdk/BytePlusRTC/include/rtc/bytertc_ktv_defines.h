#pragma once

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
    kMusicFilterTypeNone = 0,
    /** 
     * @brief Remove music that does not have lyrics.
     */
    kMusicFilterTypeWithoutLyric     = 1 << 0,
    /** 
     * @brief Remove music that does not support scoring.
     */
    kMusicFilterTypeUnsupportedScore     = 1 << 1,
    /** 
     * @brief Remove music that does not support accompany mode.
     */
    kMusicFilterTypeUnsupportedAccopmay  = 1 << 2,
    /** 
     * @brief Remove music that does not have a climax part.
     */
    kMusicFilterTypeUnsupportedClimx     = 1 << 3,
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
    kMusicHotTypeContentCenter       = 1 << 0,
    /** 
     * @brief Hot music of the project.
     */
    kMusicHotTypeProject          = 1 << 1,
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
    kOriginal = 1,
    /** 
     * @brief Play the instrumental music without vocals.
     */
    kAccompy = 2
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
    kLocal,
    /** 
     * @brief Only play on the remote side.
     */
    kRemote,
    /** 
     * @brief Play on the local and remote side.
     */
    kLocalAndRemote
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
    kPlaying = 1,
    /** 
     * @brief Paused.
     */
    kPaused = 2,
    /** 
     * @brief Stopped.
     */
    kStoped = 3,
    /** 
     * @brief Failed to play.
     */
    kFailed = 4,
    /** 
     * @brief Finished.
     */
    kFinished = 5,
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
    kNone = 0,
    /** 
     * @brief KRC lyrics.
     */
    kKRC = 1,
    /** 
     * @brief LRC lyrics.
     */
    kLRC = 2,
    /** 
     * @brief KRC and LRC lyrics.
     */
    kKRCAndLRC = 3
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
    kKRC,
    /** 
     * @brief LRC lyrics file.
     */
    kLRC
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
    kDownloadFileTypeMusic = 1,
    /** 
     * @brief KRC lyrics file.
     */
    kDownloadFileTypeKRC = 2,
    /** 
     * @brief LRC lyrics file.
     */
    kDownloadFileTypeLRC = 3,
    /** 
     * @brief MIDI file.
     */
    kDownloadFileTypeMIDI = 4

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
 * @type errorcode
 * @brief KTV error code.
 */
enum KTVErrorCode {
    /** 
     * @brief Success.
     */
    kKTVErrorCodeOK = 0,
    /** 
     * @brief Invalid AppID.
     */
    kKTVErrorCodeAppidInValid = -3000,
    /** 
     * @brief Invalid parameter.
     */
    kKTVErrorCodeParasInValid = -3001,
    /** 
     * @brief Failed to get music resources.
     */
    kKTVErrorCodeGetMusicFailed = -3002,
    /** 
     * @brief Failed to get lyrics.
     */
    kKTVErrorCodeGetLyricFailed = -3003,
    /** 
     * @brief The music is removed.
     */
    kKTVErrorCodeMusicTakedown = -3004,
    /** 
     * @brief Failed to download the music file.
     */
    kKTVErrorCodeMusicDownload = -3005,
    /** 
     * @brief Failed to download the MIDI file.
     */
    kKTVErrorCodeMidiDownloadFailed = -3006,
    /** 
     * @brief The system is busy.
     */
    kKTVErrorCodeSystemBusy = -3007,
    /** 
     * @brief Network anomaly.
     */
    kKTVErrorCodeNetwork = -3008,
    /** 
     * @brief The KTV feature is not added to the room.
     */
    kKTVErrorCodeNotJoinRoom = -3009,
    /** 
     * @brief Failed to parse data.
     */
    kKTVErrorCodeParseData = -3010,
    /** 
     * @hidden
     * @deprecated since 353.
     * @brief Failed to download.
     */
    kKTVErrorCodeDownload = -3011,
    /** 
     * @brief Already downloading.
     */
    kKTVErrorCodeDownloading = -3012,
    /** 
     * @brief Internal error. Contact the technical support representatives for help.
     */
    kKTVErrorCodeInternalDomain = -3013,
    /** 
     * @brief Failed to download because of insufficient disk space. Please retry after clearing cache.
     */
    kKTVErrorCodeInsufficientDiskSpace = -3014,
    /** 
     * @brief Failed to download because of music decryption failed. Contact the technical support representatives for help.
     */
    kKTVErrorCodeMusicDecryptionFailed = -3015,
    /** 
     * @brief Failed to download because of music rename failed. Please retry.
     */
    kKTVErrorCodeFileRenameFailed = -3016,
    /** 
     * @brief Failed to download because of network failure. Please retry.
     */
    kKTVErrorCodeDownloadTimeOut = -3017,
    /** 
     * @brief Failed to clear cache because the file is occupied or the system is abnormal. Please retry.
     */
    kKTVErrorCodeClearCacheFailed = -3018,
    /** 
     * @brief Cancel download task.
     */
    kKTVErrorCodeDownloadCanceled = -3019
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
    kKTVPlayerErrorCodeOK = 0,
    /** 
     * @brief Failed to play the music. Download first.
     */
    kKTVPlayerErrorCodeFileNotExist = -3020,
    /** 
     * @brief Failed to play the music. Check the file's format.
     */
    kKTVPlayerErrorCodeFileError = -3021,
    /** 
     * @brief Failed to play the music. Join a room first.
     */
    kKTVPlayerErrorCodeNotJoinRoom = -3022,
    /** 
     * @brief Invalid parameter.
     */
    kKTVPlayerErrorCodeParam = -3023,
    /** 
     * @brief Failed to play the music. Invalid path or failed to open the file.
     */
    kKTVPlayerErrorCodeStartError = -3024,
    /** 
     * @brief Invalid mixing ID.
     */
    kKTVPlayerErrorCodeMixIdError = -3025,
    /** 
     * @brief Invalid position.
     */
    kKTVPlayerErrorCodePositionError = -3026,
    /** 
     * @brief Invalid volume.
     */
    kKTVPlayerErrorCodeAudioVolumeError = -3027,
    /** 
     * @brief Do not support the mix type.
     */
    kKTVPlayerErrorCodeTypeError = -3028,
    /** 
     * @brief Invalid pitch.
     */
    kKTVPlayerErrorCodePitchError = -3029,
    /** 
     * @brief Invalid audio track.
     */
    kKTVPlayerErrorCodeAudioTrackError = -3030,
    /** 
     * @brief Mixing in process.
     */
    kKTVPlayerErrorCodeStartingError = -3031
};


} // namespace bytertc
