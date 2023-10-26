/*
 * Copyright (c) 2023 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Audio Effect Player
*/
#pragma once

#include "bytertc_audio_frame.h"

namespace bytertc {
/** 
 * @type callback
 * @brief Event handler for IAudioEffectPlayer{@link #IAudioEffectPlayer}. You must call setEventHandler{@link #IAudioEffectPlayer#setEventHandler} to set the corresponding event handler to get the events.
 */
class IAudioEffectPlayerEventHandler {
public:
    virtual ~IAudioEffectPlayerEventHandler() {}
    /** 
    * @type callback
    * @brief Callback for audio mixing status change.
    * @param effect_id The ID of IAudioEffectPlayer{@link #IAudioEffectPlayer}. Set by getAudioEffectPlayer{@link #IRTCVideo#getAudioEffectPlayer}.
    * @param state See PlayerState{@link #PlayerState}.
    * @param error See PlayerError{@link #PlayerError}.
    */
    virtual void onAudioEffectPlayerStateChanged(int effect_id, PlayerState state, PlayerError error) = 0;
};
/** 
 * @valid since 3.53
 * @type api
 * @brief Audio effect player<br>
 *        Call setEventHandler{@link #IAudioEffectPlayer#setEventHandler} to set the callback handler to receive related callbacks.
 */
class IAudioEffectPlayer {
public:
    IAudioEffectPlayer(){}
    virtual ~IAudioEffectPlayer() {}
    /** 
     * @type api
     * @brief Starts to play the audio effect file. <br>
     *        This API can be called multiple times with different IDs and filepaths for multiple effects at the same time.
     * @param effect_id Audio effect ID. Used for identifying the audio effect, please ensure that the audio effect ID is unique.  <br>
     *        If this API is called repeatedly with the same ID, the previous effect will stop and the next effect will start, and you'll receive onAudioEffectPlayerStateChanged{@link #IAudioEffectPlayerEventHandler#onAudioEffectPlayerStateChanged}.
     * @param file_path Audio effect file paths.
     *        URL of online file, URI of local file, or full path to the local file are supported. For URL of online file, only the https protocol is supported.
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
     * @param config See AudioEffectPlayerConfig{@link #AudioEffectPlayerConfig}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes  <br>
     *       + If the file has been loaded into memory via preload{@link #IAudioEffectPlayer#preload}, make sure that the ID here is the same as the ID set by preload{@link #IAudioEffectPlayer#preload}.
     *       + After starting to play an audio effect file, you can call the stop{@link #IAudioEffectPlayer#stop} API to stop playing the audio effect file.
     */
    virtual int start(int effect_id, const char* file_path, const AudioEffectPlayerConfig& config) = 0;
    /**  
     * @type api
     * @brief Stops the playback of audio effect files.
     * @param effect_id Audio effect ID
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes  <br>
     *       + After calling the start{@link #IAudioEffectPlayer#start} API to start playing the audio effect file, you can call this API to stop playing the audio effect file.
     *       + After calling this API to stop playing an audio effect file, the audio effect file will be unloaded automatically.
     */
    virtual int stop(int effect_id) = 0;
    /** 
     * @type api
     * @brief Stops playback of all audio effect files.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes  <br>
     *       + After calling start{@link #IAudioEffectPlayer#start} to start playing audio effect files, you can call this API to stop playing all audio effect files.
     *       + After calling this API to stop playing all audio effect files, the audio effect files will be unloaded automatically.
     */
    virtual int stopAll() = 0;
    /** 
     * @type api
     * @brief Preloads specified music files into memory to avoid repeated loading when playing the same file frequently and reduce CPU usage.
     * @param effect_id Audio effect ID。Used for identifying the audio effect. Please ensure that the audio effect ID is unique.
     *        If this API is called repeatedly with the same ID, the later one will overwrite the previous one.
     *        If you call start{@link #IAudioEffectPlayer#start} first and then call this API with the same ID, the SDK will stop the previous effect and then load the next one, and you will receive onAudioEffectPlayerStateChanged{@link #IAudioEffectPlayerEventHandler#onAudioEffectPlayerStateChanged}.
     *        After calling this API to preload A.mp3, if you need to call start{@link #IAudioEffectPlayer#start} to play B.mp3 with the same ID, please call unload{@link #IAudioEffectPlayer#unload} to unload A.mp3 first, otherwise SDK will report an error AUDIO_MIXING_ERROR_LOAD_CONFLICT.
     * @param file_path The filepath of effect file. URL of online file, URI of local file, or full path to local file. For URL of online file, only the https protocol is supported. 
     *        The length of the pre-loaded file must not exceed 20s.
     *        Audio effect file formats supported are the same as start{@link #IAudioEffectPlayer#start}.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes  <br>
     *       + This API just preloads the specified audio effect file, and only calls the start{@link #IAudioEffectPlayer#start} API to start playing the specified audio effect file.
     *       + The specified audio effect file preloaded by calling this API can be unloaded by unload{@link #IAudioEffectPlayer#unload}.
     */
    virtual int preload(int effect_id, const char* file_path) = 0;
    /** 
     * @type api
     * @brief Unloads the specified audio effect file.
     * @param effect_id Audio effect ID
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes Call this API after start{@link #IAudioEffectPlayer#start} or preload{@link #IAudioEffectPlayer#preload}.
     */
    virtual int unload(int effect_id) = 0;
    /** 
     * @type api
     * @brief Unloads all audio effect files.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     */
    virtual int unloadAll() = 0;
   /** 
     * @type api
     * @brief Pauses the playback of audio effect files.
     * @param effect_id Audio effect ID
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes
     *       + After calling the start{@link #IAudioEffectPlayer#start} API to start playing the audio effect file, you can pause the audio effect file by calling this API.
     *       + After calling this API to pause the audio effect file, you can call the resume{@link #IAudioEffectPlayer#resume} API to resume playback.
     */
    virtual int pause(int effect_id) = 0;
    /** 
     * @type api
     * @brief Pauses the Playback of all audio effect files.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes
     *       + After calling the start{@link #IAudioEffectPlayer#start} API to start playing audio effect files, you can pause playing all audio effect files by calling this API.
     *       + After calling this API to pause the playback of all audio effect files, you can call the resumeAll{@link #IAudioEffectPlayer#resumeAll} API to resume all playback.
     */
    virtual int pauseAll() = 0;
    /** 
     * @type api
     * @brief Resumes the playback of audio effect files.
     * @param effect_id Audio effect ID
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes After calling the pause{@link #IAudioEffectPlayer#pause} API to pause the audio effect file, you can resume playback by calling this API.
     */
    virtual int resume(int effect_id) = 0;
    /** 
     * @type api
     * @brief Resumes the playback of all audio effect files.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes After calling the pauseAll{@link #IAudioEffectPlayer#pauseAll} API to pause all the audio effect files being played, you can resume playback by calling this API.
     */
    virtual int resumeAll() = 0;
    /** 
     * @type api
     * @brief Sets the start position of the audio effect file.
     * @param effect_id Audio effect ID
     * @param pos The starting playback position of the audio effect file in milliseconds.
     *        You can get the total duration of the audio effect file by calling getDuration{@link #IAudioEffectPlayer#getDuration}, the value of pos should be less than the total duration of the audio effect file.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes <br>
     *        + When playing online files, calling this API may cause a delay in playback.
     *        + Call this API after start{@link #IAudioEffectPlayer#start}.
     */
    virtual int setPosition(int effect_id, int pos) = 0;
    /** 
     * @type api
     * @brief Gets the current position of audio effect file playback.
     * @param effect_id  Audio effect ID
     * @return  <br>
     *        + >0: Success, the current progress of audio effect file playback in milliseconds.
     *        + < 0: Failure.
     * @notes <br>
     *        + When playing online files, calling this API may cause a delay in playback.
     *        + Call this API after start{@link #IAudioEffectPlayer#start}.
     */
    virtual int getPosition(int effect_id) = 0;
    /** 
     * @type api
     * @brief Adjusts the volume level of a specified audio effect, including audio effect file and PCM effect.
     * @param effect_id Audio effect ID
     * @param volume The ratio of the volume to the original volume in % with overflow protection. The range is `[0, 400]` and the recommended range is `[0, 100]`.
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes Call this API after start{@link #IAudioEffectPlayer#start}.
     */
    virtual int setVolume(int effect_id, int volume) = 0;
    /** 
     * @type api
     * @brief Sets the volume of all audio effect, including audio effect files and PCM effects.
     * @param volume The ratio of the volume to the original volume in % with overflow protection. The range is `[0, 400]` and the recommended range is `[0, 100]`.  
     * @return  <br>
     *        + 0: Success.
     *        + < 0 : Fail. See ReturnStatus{@link #ReturnStatus} for more details.
     * @notes This API has a lower priority than setVolume{@link #IAudioEffectPlayer#setVolume}, i.e. the volume of the audio effect set by setVolume{@link #IAudioEffectPlayer#setVolume} is not affected by this API.
     */
    virtual int setVolumeAll(int volume) = 0;
    /** 
     * @type api
     * @brief Gets the current volume.
     * @param effect_id  Audio effect ID
     * @return  <br>
     *        + >0: Success, the current volume value.  <br>
     *        + < 0: Failed.
     * @notes Call this API after start{@link #IAudioEffectPlayer#start}.
     */
    virtual int getVolume(int effect_id) = 0;
    /** 
     * @type api
     * @brief Get the duration of the audio effect file.
     * @param effect_id  Audio effect ID
     * @return  <br>
     *        + >0: Success, the duration of the audio effect file in milliseconds.  <br>
     *        + < 0: failed.
     * @notes Call this API after start{@link #IAudioEffectPlayer#start}.
     */
    virtual int getDuration(int effect_id) = 0;
    /** 
     * @type api
     * @brief Set the event handler.
     * @param handler See IAudioEffectPlayerEventHandler{@link #IAudioEffectPlayerEventHandler}.
     * @return  <br>
     *        + 0: Success.  <br>
     *        + < 0: Failed.
     */
    virtual int setEventHandler(IAudioEffectPlayerEventHandler* handler) = 0;
};

}
