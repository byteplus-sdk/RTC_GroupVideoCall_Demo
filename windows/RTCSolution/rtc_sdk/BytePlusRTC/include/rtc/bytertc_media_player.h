/*
 * Copyright (c) 2023 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Audio Effect Player
*/
#pragma once

#include "bytertc_audio_frame.h"

namespace bytertc {
/** 
 * @type callback
 * @brief observer for the audio frames during local audio file mixing.
 */
class IMediaPlayerAudioFrameObserver {
public:
    /** 
     * @type callback
     * @brief The callback for the audio frames during local audio file mixing.
     * @param player_id
     * @param frame See IAudioFrame{@link #IAudioFrame}.
     */
    virtual void onFrame(int player_id, const IAudioFrame& frame) = 0;
    /**
     * @hidden constructor
     */
    virtual ~IMediaPlayerAudioFrameObserver() {
    }
};
/**  
 * @type keytype
 * @brief Custom audio source mode
 */
enum MediaPlayerCustomSourceMode{
    /**  
     * @brief When you use the local PCM data, use this mode.
     */
    kMediaPlayerCustomSourceModePush = 0,
    /**  
     * @brief When you use the audio data from local memory, use this mode.
     */
    kMediaPlayerCustomSourceModePull,
};

/** 
 * @type keytype
 * @brief The starting position where audio data seeking begins. 
 */
enum MediaPlayerCustomSourceSeekWhence{
    /** 
     * @brief Seeks from the head of the audio data, and the actual data offset after seeking is offset.
     */
    kMediaPlayerCustomSourceSeekWhenceSet = 0,
    /** 
     * @brief Seeks from a specific position of the audio data stream, and the actual data offset after seeking is the current position plus offset.
     */
    kMediaPlayerCustomSourceSeekWhenceCur = 1,
    /** 
     * @brief Seeks from the end of the audio data, and the actual data offset after seeking is the whole data length plus offset.
     */
    kMediaPlayerCustomSourceSeekWhenceEnd = 2,
    /** 
     * @brief Returns the size of audio data.
     */
    kMediaPlayerCustomSourceSeekWhenceSize = 3,
};
/**  
 * @type keytype
 * @brief Custom audio source stream type
 */
enum MediaPlayerCustomSourceStreamType{
    /**  
     * @brief When you use the local PCM data, use this type.
     */
    kMediaPlayerCustomSourceStreamTypeRaw = 0,
    /**  
     * @brief When you use the audio data from local memory, use this type.
     */
    kMediaPlayerCustomSourceStreamTypeEncoded,
};
/** 
 * @type callback
 * @brief  In-memory audio data playing callback interface
 */
class IMediaPlayerCustomSourceProvider {
public:
    /** 
     * @valid since 3.53
     * @type callback
     * @region Music player
     * @brief Callback of getting the path and size of the audio data you input. <br>
     *        When you call openWithCustomSource{@link #IMediaPlayer#openWithCustomSource} API to play the in-memory audio data you input, this callback will be triggered and you need to enter the path and size of the audio data. 
     * @param [in] *buffer The path of the audio data you input. The size of audio data should be equal to or less than the value of bufferSize. The supported formats are: mp3, aac, m4a, 3gp, wav.
     * @param [in] buffer_size The size of the audio data, in bytes. If you want to stop audio playing, you can enter a figure less than or equal to zero into bufferSize and then SDK will stop calling this callback. 
     * @return Returns the size of the audio data that are actually read. 
     * @notes If calling openWithCustomSource{@link #IMediaPlayer#openWithCustomSource} API failed, please enter 0 into buffer and bufferSize. In this situation, SDK will stop calling this callback.  
     */
    virtual int onReadData(uint8_t *buffer, int buffer_size) = 0;
    /** 
     * @valid since 3.53
     * @type callback
     * @region Music player
     * @brief Seeking the audio data based on the set starting position and offset to help SDK read and analyze data. <br>
     *        When you call openWithCustomSource{@link #IMediaPlayer#openWithCustomSource}  API to play the in-memory audio data you input, or when you call setPosition{@link #IMediaPlayer#setPosition} to set the starting position of audio playing, this callback will be trigerred and you need to seek the audio data accorring to the value of offset and whence.  
     * @param [in] offset The offset of the target position relative to the starting position, in bytes. The value can be positive or negative.  <br>
     * @param [in] whence The starting position of the seeking.  Refer to MediaPlayerCustomSourceSeekWhence{@link #MediaPlayerCustomSourceSeekWhence} for more details. 
     * @return <br>
     *         If the seeking succeeded, the information on the final adjusted play position after seeking or the size of the audio data will be returned.<br>
     *         If the seeking failed, -1 will be returned.  
     */
    virtual int64_t onSeek(int64_t offset, MediaPlayerCustomSourceSeekWhence whence) = 0;
    
    virtual ~IMediaPlayerCustomSourceProvider() {}
};
/** 
 * @type keytype
 * @brief The audio source for media player.
 */
struct MediaPlayerCustomSource {
    IMediaPlayerCustomSourceProvider* provider = nullptr;
    /**  
     * @type keytype
     * @brief See MediaPlayerCustomSourceMode{@link #MediaPlayerCustomSourceMode}.
     */
    MediaPlayerCustomSourceMode mode = kMediaPlayerCustomSourceModePush;
    /**  
     * @type keytype
     * @brief See MediaPlayerCustomSourceStreamType{@link #MediaPlayerCustomSourceStreamType}.
     */
    MediaPlayerCustomSourceStreamType type = kMediaPlayerCustomSourceStreamTypeRaw;
};
/** 
 * @type callback
 * @brief Event handler for IMediaPlayer{@link #IMediaPlayer}. You must call setEventHandler{@link #IMediaPlayer#setEventHandler} to set the corresponding event handler to get the events.
 */
class IMediaPlayerEventHandler {
public:
    virtual ~IMediaPlayerEventHandler() {}
    /** 
    * @type callback
    * @brief Callback for audio mixing status change.
    * @param player_id The ID of IMediaPlayer{@link #IMediaPlayer}. Set by getMediaPlayer{@link #IRTCVideo#getMediaPlayer}.
    * @param state See PlayerState{@link #PlayerState}.
    * @param error See PlayerError{@link #PlayerError}.
    */
    virtual void onMediaPlayerStateChanged(int player_id, PlayerState state, PlayerError error) = 0;
   /** 
    * @type callback
    * @brief Periodic callback for audio mixing progress. The period is set by setProgressInterval{@link #IMediaPlayer#setProgressInterval}.
    * @param player_id The ID of IMediaPlayer{@link #IMediaPlayer}. Set by getMediaPlayer{@link #IRTCVideo#getMediaPlayer}.
    * @param progress Mixing progress in ms.
    */
    virtual void onMediaPlayerPlayingProgress(int player_id, int64_t progress) = 0;
};
/** 
 * @valid since 3.53
 * @type api
 * @brief Media player<br>
 *        Call setEventHandler{@link #IMediaPlayer#setEventHandler} to set the callback handler to receive related callbacks.
 */
class IMediaPlayer {
public:
    IMediaPlayer(){}
    virtual ~IMediaPlayer() {}
    /** 
     * @type api
     * @brief Open the audio file. <br>
     *        You can only open one audio file with one player instance at the same time. For multiple audio files at the same time, create multiple player instances.
     *        For audio file in PCM format, see openWithCustomSource{@link #IMediaPlayer#openWithCustomSource}. `openWithCustomSource` and this API are mutually exclusive.
     * @param file_path Audio file paths.
     *        URL of online file, URI of local file, or full path to local file. For URL of online file, only the https protocol is supported.
     *        Recommended sample rate for audio effect files: 8KHz、16KHz、22.05KHz、44.1KHz、48KHz.
     *        Local audio effect file formats supported by different platforms:
     *        <table>
     *           <tr><th></th><th>mp3</th><th>mp4</th><th>aac</th><th>m4a</th><th>3gp</th><th>wav</th><th>ogg</th><th>ts</th><th>wma</th></tr>
     *           <tr><td>Android</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td></td></tr>
     *           <tr><td>iOS/macOS</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td></td><td></td></tr>
     *           <tr><td>Windows</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td>Y</td><td>Y</td></tr>
     *        </table>
     *        Online audio effect file formats supported by different platforms.
     *        <table>
     *           <tr><th></th><th>mp3</th><th>mp4</th><th>aac</th><th>m4a</th><th>3gp</th><th>wav</th><th>ogg</th><th>ts</th><th>wma</th></tr>
     *           <tr><td>Android</td><td>Y</td><td></td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td></td><td></td></tr>
     *           <tr><td>iOS/macOS</td><td>Y</td><td></td><td>Y</td><td>Y</td><td></td><td>Y</td><td></td><td></td><td></td></tr>
     *           <tr><td>Windows</td><td>Y</td><td></td><td>Y</td><td>Y</td><td>Y</td><td>Y</td><td></td><td>Y</td><td>Y</td></tr>
     *        </table>
     * @param config See MediaPlayerConfig{@link #MediaPlayerConfig}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     */
    virtual int open(const char* file_path, const MediaPlayerConfig& config) = 0;
    /** 
     * @type api
     * @brief Start playing the audio. Call this API when you call open{@link #IMediaPlayer#open} and set `AutoPlay=False`.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes  <br>
     * + For audio file in PCM format, see openWithCustomSource{@link #IMediaPlayer#openWithCustomSource}. `openWithCustomSource` and this API are mutually exclusive.
     * + After calling this API, call stop{@link #IMediaPlayer#stop} to stop playing the audio file.
     */
    virtual int start() = 0;
    /** 
     * @type api
     * @brief Enable audio mixing with audio raw data.
     *        To open the audio file, see open{@link #IMediaPlayer#open}. `open` and this API are mutually exclusive.
     * @param source See MediaPlayerCustomSource{@link #MediaPlayerCustomSource}.
     * @param config See MediaPlayerConfig{@link #MediaPlayerConfig}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes  <br>
     *       + After calling this API, you must call pushExternalAudioFrame{@link #IMediaPlayer#pushExternalAudioFrame} to push audio data and start the audio mixing.
     *       + To stop the raw data audio mixing, call stop{@link #IMediaPlayer#stop}.
     */
    virtual int openWithCustomSource(const MediaPlayerCustomSource& source, const MediaPlayerConfig& config) = 0;
    /** 
     * @type api
     * @brief After calling open{@link #IMediaPlayer#open}, start{@link #IMediaPlayer#start}, or openWithCustomSource{@link #IMediaPlayer#openWithCustomSource} to start audio mixing, call this method to stop audio mixing.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     */
    virtual int stop() = 0;
    /** 
     * @type api
     * @brief After calling open{@link #IMediaPlayer#open}, or start{@link #IMediaPlayer#start} to start audio mixing, call this API to pause audio mixing.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *        + After calling this API to pause audio mixing, call resume{@link #IMediaPlayer#resume} to resume audio mixing.<br>
     *        + The API is valid for audio file, not PCM data.
     */
    virtual int pause() = 0;
    /** 
     * @type api
     * @brief After calling pause{@link #IMediaPlayer#pause} to pause audio mixing, call this API to resume audio mixing.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *        The API is valid for audio file, not PCM data.
     */
    virtual int resume() = 0;
    /** 
     * @type api
     * @brief Adjusts the volume of the specified audio mixing, including media file mixing and PCM mixing.
     * @param volume The ratio of the volume to the original volume in % with overflow protection. The range is `[0, 400]` and the recommended range is `[0, 100]`.
     * @param type See AudioMixingType{@link #AudioMixingType}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes Call this API only when audio is mixing.
     */
    virtual int setVolume(int volume, AudioMixingType type) = 0;
    /** 
     * @type api
     * @brief Gets the current volume.
     * @param type See AudioMixingType{@link #AudioMixingType}.
     * @return  <br>
     *        + >0: Success, the current volume. <br>
     *        + < 0: Failed.
     * @notes Call this API only when audio is mixing, including media file mixing and PCM mixing.
     */
    virtual int getVolume(AudioMixingType type) = 0;
    /** 
     * @type api
     * @brief Gets the duration of the media file.
     * @return  <br>
     *        + >0: Success, the duration of the media file in milliseconds.  <br>
     *        + < 0: Failed.
     * @notes <br>
     *        + Call this API only when audio is mixing.
     *        + The API is valid for audio file, not PCM data.
     */
    virtual int getTotalDuration() = 0;
    /** 
     * @type api
     * @brief Gets the actual playback duration of the mixed media file, in milliseconds.
     * @return  <br>
     *        + >0: Success, the actual playback time.
     *        + < 0: Failed.
     * @notes <br>
     *        + The actual playback time refers to the playback time of the song without being affected by stop, jump, double speed, and freeze. For example, if the song stops playing for 30 seconds at 1:30 or skips to 2:00, and then continues to play normally for 2 minutes, the actual playing time is 3 minutes and 30 seconds.
     *        + Call this API only when audio is mixing and the interval set by setProgressInterval{@link #IMediaPlayer#setProgressInterval} is above `0`.
     *        + The API is valid for audio file, not PCM data.
     */
    virtual int getPlaybackDuration() = 0;
    /** 
     * @type api
     * @brief Gets the playback progress of the media file.
     * @return  <br>
     *        + >0: Success, the playback progress of media file in ms.
     *        + < 0: Failed.
     * @notes <br>
     *        + Call this API only when audio is mixing.
     *        + The API is valid for audio file, not PCM data.
     */
    virtual int getPosition() = 0;
    /** 
     * @type api
     * @brief Set the pitch of the local audio file mixing. Usually used in karaoke scenes.
     * @param pitch The increase or decrease value compared with the original pitch of the music file. The range is `[-12, 12]`. The default value is 0. The pitch distance between two adjacent values is half a step. A positive value indicates a rising pitch, and a negative value indicates a falling pitch.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *        + Call this API only when audio is mixing.
     *        + Support audio file and PCM data.
     */
    virtual int setAudioPitch(int pitch) = 0;
    /** 
     * @type api
     * @brief Sets the starting playback position of the media file.
     * @param position The starting position of the media file in milliseconds.
     *        You can get the total duration of the media file through getTotalDuration{@link #IMediaPlayer#getTotalDuration}. The value of position should be less than the total duration of the media file.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes  <br>
     *        + The API is valid for audio file, not PCM data.
     *        + When playing online files, calling this API may cause playback delay.
     */
    virtual int setPosition(int position) = 0;
    /** 
     * @type api
     * @brief Sets the channel mode of the mixing of the media file.
     * @param mode The mode of channel. The default channel mode is the same as the source file. See AudioMixingDualMonoMode{@link #AudioMixingDualMonoMode}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *       + Call this API only when audio is mixing.
     *       + Audio file is supported, but not PCM data.
     */
    virtual int setAudioDualMonoMode(AudioMixingDualMonoMode mode) = 0;
    /** 
     * @type api
     * @brief Gets the track count of the current media file.
     * @return + >= 0：Success. Return the track count of the current media file.
     *         + < 0：Failed.
     * @notes <br>
     *       + Call this API only when audio is mixing.
     *       + This API is valid for audio file, not PCM data.
     */
    virtual int getAudioTrackCount() = 0;
    /** 
     * @type api
     * @brief Specifies the playback track of the current media file.
     * @param index The specified playback audio track, starting from 0, and the range is `[0, getAudioTrackCount()-1]`. The value must be less than the return value of getAudioTrackCount{@link #IMediaPlayer#getAudioTrackCount}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *       + Call this API only when audio is mixing.
     *       + This API is valid for audio file, not PCM data.
     */
    virtual int selectAudioTrack(int index) = 0;
    /** 
     * @type api
     * @brief Set the playback speed of the audio file.
     * @param speed The ratio of the actual playback speed than that of the original speed of the audio file in %. The range is `[50,200]`. The default value is 100.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *       + Call this API only when audio is mixing.
     *       + The API is valid for audio file and PCM data.
     */
    virtual int setPlaybackSpeed(int speed) = 0;
    /** 
     * @type api
     * @brief Set the interval of the periodic callback onMediaPlayerPlayingProgress{@link #IMediaPlayerEventHandler#onMediaPlayerPlayingProgress} during audio mixing.
     * @param interval interval in ms.
     *       + interval > 0: The callback is enabled. The actual interval is `10*(mod(10)+1)`.
     *       + interval <= 0: The callback is disabled.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *       + Call this API only when audio is mixing.
     *       + This API is valid for audio file, not PCM data.
     */
    virtual int setProgressInterval(int64_t interval) = 0;
    /** 
     * @type api
     * @brief To call enableVocalInstrumentBalance{@link #IRTCVideo#enableVocalInstrumentBalance} to adjust the volume of the mixed media file or the PCM audio data, you must pass in its original loudness through this API.
     * @param loudness Original loudness in lufs. The range is `[-70.0, 0.0]`.
     *       When the value is less than -70.0lufs, it will be adjusted to -70.0lufs by default, and if it is more than 0.0lufs, the loudness will not be equalized. The default value is 1.0lufs, which means no processing.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *       + Call this API only when audio is mixing.
     *       + The API is valid for audio file and PCM data.
     */
    virtual int setLoudness(float loudness) = 0;
    /** 
     * @type api
     * @brief Register an observer to receive related callbacks when the local media file is mixing.
     * @param observer See IMediaPlayerAudioFrameObserver{@link #IMediaPlayerAudioFrameObserver}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *        The API is valid for audio file, not PCM data.
     */
    virtual int registerAudioFrameObserver(IMediaPlayerAudioFrameObserver* observer) = 0;
    /** 
     * @type api
     * @brief Push PCM audio frame data for mixing.
     * @param audio_frame See IAudioFrame{@link #IAudioFrame}.
     * @return  <br>
     *       + 0: Success.  <br>
     *       + < 0: Failed.
     * @notes
     *      + Before calling this method, the raw audio data mixing must be enabled through openWithCustomSource{@link #IMediaPlayer#openWithCustomSource}.
     *      + Suggestions: Before pushing data for the first time, please cache a certain amount of data (like 200 ms) on the application side, and then push it at once. Schedule subsequent push operation every 10 ms with audio data of 10 ms.
     *      + To pause the playback, just pause the push.
     */
    virtual int pushExternalAudioFrame(IAudioFrame* audio_frame) = 0;
    /** 
     * @type api
     * @brief Set the event handler.
     * @param handler See IMediaPlayerEventHandler{@link #IMediaPlayerEventHandler}.
     * @return  <br>
     *        + 0: Success.  <br>
     *        + < 0: Failed.
     */
    virtual int setEventHandler(IMediaPlayerEventHandler* handler) = 0;
 };


}
