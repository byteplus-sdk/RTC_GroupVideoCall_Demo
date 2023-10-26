/*
 * Copyright (c) 2021 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC SAMI SDK
*/

#pragma once

#include "bytertc_audio_defines.h"

namespace bytertc {
/** 
 * @type callback
 * @brief Karaoke scoring event handler.
 * Note: Callback functions are thrown synchronously in a non-UI thread within the SDK. Therefore, you must not perform any time-consuming operations or direct UI operations within the callback function, as this may cause the app to crash.
 */
class ISingScoringEventHandler {
public:
    /** 
     * @type callback
     * @brief The callback for real-time scoring data. This callback is triggered after startSingScoring{@link #ISingScoringManager#startSingScoring} is called.
     * @param info Real-time scoring data.
     */
    virtual void onCurrentScoringInfo(
                 const SingScoringRealtimeInfo& info) = 0;
};
/** 
 * @hidden(Linux)
 * @type api
 * @brief Standard Pitch Data.
 */
class IStandardPitchCollection {
public:
    /** 
     * @type api
     * @brief Get the total number of lyric lines
     */
    virtual int getCount() = 0;
    /** 
     * @type api
     * @brief Get the standard pitch information of each lyric.
     * @param index Number of lines, of which the range is from 0 to the total number of lines obtained by calling getCount{@link #IStandardPitchCollection#getCount} minus 1.
     * @return See StandardPitchInfo{@link #StandardPitchInfo}.
     */
    virtual StandardPitchInfo getStandardPitchInfo(int index) = 0;
    /** 
     * @type api
     * @brief Destroy the IStandardPitchCollection class and release the resources.
     */
    virtual void release() = 0;
};
/** 
 * @hidden(Linux)
 * @type api
 * @brief Karaoke scoring management interface.
 */
class ISingScoringManager {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    ISingScoringManager() {
    }
    /** 
     * @type api
     * @brief Initialize karaoke scoring feature.
     * @param sing_scoring_appkey The key for karaoke scoring, used to authenticate whether the karaoke scoring is enabled.
     * @param sing_scoring_token The key for karaoke scoring, used to authenticate whether the karaoke scoring is enabled.
     * @param handler Karaoke scoring event handler，see ISingScoringEventHandler{@link #ISingScoringEventHandler}.
     * @notes Enter two keys to enable karaoke scoring. Authentication is done offline, bind the Appkey and Token according to the package name (bundleID). Please contact technical support to apply for the key.
     * @return  <br>
     *        + 0：Success.
     *        + -1：Interface call failed.
     *        + -2： Karaoke scoring module not integrated.
     *        + >0： Other error. For details, see[Error code].
     */
    virtual int initSingScoring(
                     const char* sing_scoring_appkey,
                     const char* sing_scoring_token,
                     ISingScoringEventHandler* handler) = 0;
    /** 
     * @type api
     * @brief Set the configuration of karaoke scoring.
     * @param config The parameters of karaoke scoring. See SingScoringConfig{@link #SingScoringConfig}.
     * @return  <br>
     *        + 0：Success.
     *        + -1：Interface call failed.
     *        + -2： Karaoke scoring module not integrated.
     *        + >0： Other errors. For details, see[Error code].
     */
    virtual int setSingScoringConfig(
                 const SingScoringConfig& config) = 0;
    /** 
     * @type api
     * @brief Get standard pitch. 
     * @param midi_filepath Midi file path of the song.
     * @return IStandardPitchCollection{@link #IStandardPitchCollection} Standard pitch data array.
     * @notes Please make sure the same midi file path is passed in this API and setSingScoringConfig{@link #ISingScoringManager#setSingScoringConfig}.
     */
    virtual IStandardPitchCollection* getStandardPitchInfoCollection(const char* midi_filepath) = 0;
    /** 
     * @hidden(Linux)
     * @type api
     * @brief Start karaoke scoring. 
     * @param position You can get the playback position where you start karaoke scoring. Unit: ms.
     * @param scoring_info_interval Time interval between two real-time callbacks. Unit: ms; Default interval: 50 ms. Minimum interval: 20 ms.
     * @return  <br>
     *        + 0：Success.
     *        + -1：Interface call failed.
     *        + -2： Karaoke scoring module not integrated.
     *        + >0： Other error. For details, see[Error code].
     * @notes <br>
     *        + You can call this API after calling initSingScoring{@link #initSingScoring} to initialize karaoke scoring.
     *        + After this interface is called, you will receive the scoring result onCurrentScoringInfo{@link #ISingScoringEventHandler#onCurrentScoringInfo} at set interval.
     *        + If you call the startAudioMixing{@link #startAudioMixing} to play an audio file, call this interface after you receive onAudioMixingStateChanged{@link #IRTCVideoEventHandler#onAudioMixingStateChanged}(AUDIO_MIXING_STATE_PLAYING(1)).
     */    
    virtual int startSingScoring(int position,
                                 int scoring_info_interval) = 0;
    /** 
     * @type api
     * @brief Stop karaoke scoring. 
     * @return  <br>
     *        + 0：Success.
     *        + <0：Failure.
     */   
    virtual int stopSingScoring() = 0;
    /** 
     * @type api
     * @brief Get the score for the previous lyric. You can call this API after startSingScoring{@link #ISingScoringManager#startSingScoring} is called.
     * @return  <br>
     *        + <0：Failed to get the score for the previous lyric.
     *        + >=0：The score for the previous lyric.
     */   
    virtual int getLastSentenceScore() = 0;
    /** 
     * @type api
     * @brief Get the total score for the user's current performance.You can call this API after startSingScoring{@link #ISingScoringManager#startSingScoring} is called.
     * @return  <br>
     *        + <0：Failed to get the total score.
     *        + >=0：The current total score.
     */    
    virtual int getTotalScore() = 0;
    /** 
     * @type api
     * @brief Get the average score for the user's current performance.
     * @return  <br>
     *        + <0：Failed to get the average score.
     *        + >=0：The average score.
     */   
    virtual int getAverageScore() = 0;
    /**
     * @hidden constructor/destructor
     */
    virtual ~ISingScoringManager() {
    }
};

}  // namespace bytertc
