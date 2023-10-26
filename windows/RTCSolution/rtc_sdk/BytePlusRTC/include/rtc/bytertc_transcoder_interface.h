
/*
 * Copyright 2021 @bytedance
 *  Created on: june 20, 2021
 */

#pragma once

#include "bytertc_defines.h"
#include "bytertc_transcoder_define.h"

namespace bytertc {
/** 
 * @type callback
 * @region Push to CDN
 * @brief Register this observer to receive stream mixing related callbacks from the SDK.
 * Note: Callback functions are thrown synchronously in a non-UI thread within the SDK. Therefore, you must not perform any time-consuming operations or direct UI operations within the callback function, as this may cause the app to crash.
 */
class IMixedStreamObserver {
public:
    /** 
     * @type callback
     * @region Push to CDN
     * @brief Whether your App has the ability to push streams to CDN.   <br>
     *        + True: Yes <br>
     *        + False: No (default value)
     * @notes   <br>
     *       + If you need to use the function of intelligently pushing streams to CDN, you should ensure that your App
     * contains librtmp and has the ability to push streams out. If so, set the callback to True.
     */
    virtual bool isSupportClientPushStream() = 0;
    /** 
     * @hidden(Linux) not available
     * @type callback
     * @region Push to CDN
     * @brief Used for reporting events during pushing streams to CDN
     * @param [in] event Type Stream mixing status, see StreamMixingEvent{@link #StreamMixingEvent}
     * @param [in] task_id Task ID
     * @param [in] error Errors occuring during the pushing process. See StreamMixingErrorCode{@link #StreamMixingErrorCode}
     * @param [in] mix_type Stream mixing and pushing type. See MixedStreamType{@link #MixedStreamType}
     */
    virtual void onMixingEvent(
            StreamMixingEvent event, const char* task_id, StreamMixingErrorCode error, MixedStreamType mix_type) = 0;

    /** 
     * @type callback
     * @region Push to CDN
     * @brief Callback with the video data after stream mixing, running on the video callback thread
     * @param [in] task_id Task ID
     * @param [in] video_frame Video Frame, see IVideoFrame{@link #IVideoFrame}.
     * @notes The interval between callbacks is the same with that between video frames.
     */
    virtual void onMixingVideoFrame(const char* task_id, IVideoFrame* video_frame) = 0;

    /** 
     * @type callback
     * @region Push to CDN
     * @brief Callback with the audio data after stream mixing, running on the audio callback thread
     * @param [in] task_id Task ID.
     * @param [in] audio_frame Audio Frame, see IAudioFrame{@link #IAudioFrame}.
     * @notes You will receive the callback every 10 milliseconds. Each callback carries data collected in the last 10
     * milliseconds.
     */
    virtual void onMixingAudioFrame(const char* task_id, IAudioFrame* audio_frame) = 0;

    /** 
     * @type callback
     * @region Push to CDN
     * @brief This callback carries SEI data, running on the video callback thread
     * @param [in] task_id Task ID
     * @param [in] data_frame SEI data
     */
    virtual void onMixingDataFrame(const char* task_id, IDataFrame* data_frame) = 0;

    /** 
     * @hidden for internal use only
     * @type callback
     * @region Push to CDN
     * @brief Synchronized video frame callback.
     * @param [in] task_id Task ID.
     * @param [in] uids Array of synchronized video frames corresponding to uids.
     * @param [in] video_frames Array of synchronized video frames corresponding to uids.
     * @param [in] data_frame SEI data.
     * @param [in] count The length of the array.
     */
    virtual void onCacheSyncVideoFrames(const char* task_id, const char* uids[], IVideoFrame* video_frames[],
            IDataFrame* data_frame[], int count) = 0;

    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IMixedStreamObserver() = default;
};

/** 
 * @deprecated since 3.52, use IMixedStreamObserver{@link #IMixedStreamObserver} instead.
 * @type callback
 * @region Push to CDN
 * @brief Register this observer to receive stream mixing related callbacks from the SDK.
 * @brief  Custom coded frame callback class.
 * Note: Callback functions are thrown synchronously in a non-UI thread within the SDK. Therefore, you must not perform any time-consuming operations or direct UI operations within the callback function, as this may cause the app to crash.
 */
class ITranscoderObserver {
public:
    /** 
     * @type callback
     * @region Push to CDN
     * @brief Whether your App has the ability to push streams to CDN.   <br>
     *        + true: Yes <br>
     *        + false: No (default value)
     * @notes If you need to use the function of intelligently pushing streams to CDN, you should ensure that your App contains librtmp and has the ability to push streams out. If so, set the callback to True.
     */
    virtual bool isSupportClientPushStream() {
        return false;
    }
    /** 
     * @hidden(Linux) not available
     * @type callback
     * @region Push to CDN
     * @brief Used for reporting events during pushing streams to CDN
     * @param [in] event Type Stream mixing status, see StreamMixingEvent{@link #StreamMixingEvent}
     * @param [in] task_id Task ID
     * @param [in] error Errors occuring during the pushing process. See StreamMixingErrorCode{@link #StreamMixingErrorCode}
     * @param [in] mix_type Stream mixing and pushing type. See StreamMixingType{@link #StreamMixingType}
     */
    virtual void onStreamMixingEvent(
            StreamMixingEvent event, const char* task_id, StreamMixingErrorCode error, StreamMixingType mix_type) = 0;

    /** 
     * @type callback
     * @region Push to CDN
     * @brief Callback with the video data after stream mixing, running on the video callback thread
     * @param [in] task_id Task ID
     * @param [in] video_frame Video Frame, see IVideoFrame{@link #IVideoFrame}.
     * @notes The interval between callbacks is the same with that between video frames.
     */
    virtual void onMixingVideoFrame(const char* task_id, IVideoFrame* video_frame) = 0;

    /** 
     * @type callback
     * @region Push to CDN
     * @brief Callback with the audio data after stream mixing, running on the audio callback thread
     * @param [in] task_id Task ID.
     * @param [in] audio_frame Audio Frame, see IAudioFrame{@link #IAudioFrame}.
     * @notes You will receive the callback every 10 milliseconds. Each callback carries data collected in the last 10 milliseconds.
     */
    virtual void onMixingAudioFrame(const char* task_id, IAudioFrame* audio_frame) = 0;

    /** 
     * @type callback
     * @region Push to CDN
     * @brief This callback carries SEI data, running on the video callback thread
     * @param [in] task_id Task ID
     * @param [in] data_frame SEI data
     */
    virtual void onMixingDataFrame(const char* task_id, IDataFrame* data_frame) = 0;
    /** 
     * @hidden for internal use only
     * @type callback
     * @valid since 3.50
     * @brief Synchronized video frame callback. Receive this callback when you enable simplex chorus mode by calling setSyncControlparam{@link #ITranscoderParam#setSyncControlparam} and set `sync_client_video_need_mix=False`. 
     * @param [in] video_frames Array of synchronized video frames. See IVideoFrame{@link #IVideoFrame}.
     * @param [in] data_frame SEI data. See IDataFrame{@link #IDataFrame}.
     * @param [in] uids Array of uids corresponding to the video frames.
     * @param [in] task_id Task ID.
     * @param [in] count length of `video_frames`.
     */
    virtual void onCacheSyncVideoFrames(const char* task_id, const char* uids[], IVideoFrame* video_frames[], IDataFrame* data_frame[], int count) = 0;

    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~ITranscoderObserver() = default;
};
/** 
 * @hidden(Linux)
 * @type callback
 * @region Push to CDN
 * @brief Register this observer to receive the callbacks about pushing a single stream to CDN.
 * Note: Callback functions are thrown synchronously in a non-UI thread within the SDK. Therefore, you must not perform any time-consuming operations or direct UI operations within the callback function, as this may cause the app to crash.
 */
class IPushSingleStreamToCDNObserver {
public:
     /** 
     * @type callback
     * @region Push to CDN
     * @brief Used for reporting events during pushing a single stream to CDN.
     * @param event Stream mixing and pushing status, see SingleStreamPushEvent{@link #SingleStreamPushEvent}.
     * @param task_id Task ID
     * @param error Errors occurring during the pushing process. See StreamMixingErrorCode{@link #StreamMixingErrorCode}
     */
    virtual void onStreamPushEvent(SingleStreamPushEvent event, const char *task_id, int error) = 0;
    /**
     * @hidden constructor/destructor
     */
    virtual ~IPushSingleStreamToCDNObserver() = default;
};

/** 
 * @type callback
 * @brief Observer
 */
class IChorusCacheSyncObserver {
public:
    /**
     * @hidden constructor/destructor
    */
    virtual ~IChorusCacheSyncObserver() {
    }
    /** 
     * @type callback
     * @brief The user who calls startChorusCacheSync{@link #IRTCVideo#startChorusCacheSync} as `consumer` receives the callback with synced video frames. The interval of receiving the callback is set by `fps`.
     * @param count length of `uids`
     * @param uids[] The list of participants as `producer` and `retransmitter`. The participants not sending media data are excluded.
     * @param video_frames[] Video data frame corresponding to `uids`. See IVideoFrame{@link #IVideoFrame}.
     */    
    virtual void onSyncedVideoFrames(int count, const char* uids[], bytertc::IVideoFrame* video_frames[]) = 0;
    /** 
     * @type callback
     * @brief Receives the callback when the `producer` or `retransmitter` changes.
     * @param count Current length of `uids`
     * @param uids Current list of the participants
     * @notes You may receive the callback is the following cases:
     *        + The user calls startChorusCacheSync{@link #IRTCVideo#startChorusCacheSync} or stopChorusCacheSync{@link #IRTCVideo#stopChorusCacheSync};
     *        + The cache syncing participant quits abnormally. 
     */
    virtual void onSyncedUsersChanged(int count, const char* uids[]) = 0;
    /** 
     * @type callback
     * @brief Chorus cache sync event callback
     * @param event See ChorusCacheSyncEvent{@link #ChorusCacheSyncEvent}.
     * @param error See ChorusCacheSyncError{@link #ChorusCacheSyncError}.
     */
    virtual void onSyncEvent(ChorusCacheSyncEvent event, ChorusCacheSyncError error) = 0;
};

/** 
 * @type api
 * @brief Create an instance with stream mixing configurations
 */
BYTERTC_API ITranscoderParam* createTranscoderParam();
/** 
 * @type api
 * @brief Create an instance with stream mixing configurations
 */
BYTERTC_API IMixedStreamConfig* createMixedStreamConfig();
} /* namespace bytertc */
