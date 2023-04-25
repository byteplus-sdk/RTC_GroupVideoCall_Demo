/*
 * Copyright (c) 2020 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC audio Frame
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <memory>
#include "bytertc_audio_defines.h"

namespace bytertc {

/** 
 * @type keytype
 * @region Audio Management
 * @brief Audio frame type
 */
enum AudioFrameType {
    /** 
     * @brief PCM 16bit
     */
    kFrameTypePCM16 = 0
};

/** 
 * @type keytype
 * @region audio management
 * @brief Audio frame construction class
 */
typedef struct AudioFrameBuilder {
    /** 
     * @brief Audio Sampling Rate
     */
    AudioSampleRate sample_rate;

    /** 
     * @brief Number of audio channels
     */
    AudioChannel channel;

    /** 
     * @brief Audio frame timestamp in microseconds
     */
    int64_t timestamp_us = 0;

    /** 
     * @brief Audio frame data
     */
    uint8_t* data;

    /** 
     * @brief Audio frame data size
     */
    int64_t data_size = 0;

    /** 
     * @brief Is deep-copy or not
     */
    bool deep_copy = true;
} AudioFrameBuilder;
/** 
 * @type keytype
 * @brief Audio frame
 */
class IAudioFrame {
public:
    /** 
     * @type api
     * @region Audio Management
     * @brief Gets audio frame timestamp.
     * @return Audio frame timestamp in microseconds
     */
    virtual int64_t timestampUs() const = 0;
    /** 
     * @type api
     * @region Audio Management
     * @brief Gets audio sample rate. See AudioSampleRate{@link #AudioSampleRate}
     * @return Audio sample rate in Hz
     */
    virtual AudioSampleRate sampleRate() const = 0;
    /** 
     * @type api
     * @region Audio Management
     * @brief Gets the number of audio channels. See AudioChannel{@link #AudioChannel}
     * @return Number of audio channels
     * @notes For dual channels, the audio frames are interleaved.
     */
    virtual AudioChannel channel() const = 0;
    /** 
     * @type api
     * @region Audio Management
     * @brief Gets audio frame memory address
     * @return Audio frame memory address
     */
    virtual uint8_t* data() const = 0;
    /** 
     * @type api
     * @region Audio Management
     * @brief Getd audio frame data size
     * @return Audio frame data size in bytes.
     */
    virtual int dataSize() const = 0;
    /** 
     * @type api
     * @region Audio Management
     * @brief Gets audio frame type
     * @return Audio frame type, support PCM only. See AudioFrameType{@link #AudioFrameType}
     */
    virtual AudioFrameType frameType() const = 0;
    /** 
     * @type api
     * @region Audio Management
     * @brief Release audio frames
     */
    virtual void release() = 0;
    /** 
     * @type api
     * @region Audio Management
     * @brief Gets audio mute state identifier
     * @return Is the data muted:  <br>
     *         + true: Yes <br>
     *         + false: No
     */
    virtual bool isMutedData() const = 0;
    /**
     * @hidden constructor/destructor
     */
protected:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IAudioFrame() = default;
};

/** 
 * @type api
 * @region Audio Management
 * @brief Create IAudioFrame
 * @param [in] builder Audio frame build instance. See AudioFrameBuilder{@link #AudioFrameBuilder}
 * @return Refer to IAudioFrame{@link #IAudioFrame} for more details.
 */
BYTERTC_API IAudioFrame* buildAudioFrame(const AudioFrameBuilder& builder);

/** 
 * @type keytype
 * @brief Audio data callback method
 */
enum class AudioFrameCallbackMethod{
    /** 
     * @brief The callback of the audio data recorded by local microphone.
     */
    kAudioFrameCallbackRecord,
    /** 
     * @brief The callback of the mixed audio data of all remote users subscribed by the local user.
     */
    kAudioFrameCallbackPlayback,
    /** 
     * @brief The callback of the mixed audio data including the data recorded by local microphone and that of all remote users subscribed by the local user.
     */
    kAudioFrameCallbackMixed,
    /** 
     * @brief The callback of the audio data before mixing of each remote user subscribed by the local user.
     */
    kAudioFrameCallbackRemoteUser,
    /** 
     * @brief The callback of screen audio data captured locally.
     */
    kAudioFrameCallbackRecordScreen,
};

/** 
 * @type callback
 * @region Audio Data Callback
 * @brief Audio data callback observer
 */
class IAudioFrameObserver {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IAudioFrameObserver() {
    }
    /**
     * @hidden for internal use only
     * @valid since 3.50
     */
    virtual void onRecordAudioFrameOriginal(const IAudioFrame& audio_frame) = 0;
    
    /** 
     * @type callback
     * @region Audio Data Callback
     * @brief Returns audio data recorded by microphone
     * @param [in] audio_frame Audio data. See IAudioFrame{@link #IAudioFrame}
     */
    virtual void onRecordAudioFrame(const IAudioFrame& audio_frame) = 0;

    /** 
     * @type callback
     * @region Audio Data Callback
     * @brief Returns the mixed audio data of all subscribed remote users
     * @param [in] audio_frame Audio data. See IAudioFrame{@link #IAudioFrame}
     */
    virtual void onPlaybackAudioFrame(const IAudioFrame& audio_frame) = 0;
    /** 
     * @type callback
     * @region Audio Data Callback
     * @brief Returns the audio data of one remote user.
     * @param [in] stream_info Remote stream information. See RemoteStreamKey{@link #RemoteStreamKey}.
     * @param [in] audio_frame Audio data. See IAudioFrame{@link #IAudioFrame}
     * @notes This callback works on the playback thread. Don't do anything time-consuming in this callback, or it may affect the entire audio playback chain.
     */
    virtual void onRemoteUserAudioFrame(const RemoteStreamKey& stream_info, const IAudioFrame& audio_frame) = 0;

    /** 
     * @type callback
     * @region Audio Data Callback
     * @brief Returns mixed audio data including both data recorded by the local microphone and data from all subscribed remote users
     * @param [in] audio_frame Audio data. See IAudioFrame{@link #IAudioFrame}
     */
    virtual void onMixedAudioFrame(const IAudioFrame& audio_frame) = 0;

    /** 
     * @type callback
     * @region Screen audio data callback
     * @brief Returns the audio data played locally
     * @param [in] audio_frame Audio data. See IAudioFrame{@link #IAudioFrame}
     */
    virtual void onRecordScreenAudioFrame(const IAudioFrame& audio_frame) {
    }
};
/** 
 * @deprecated since 3.45 and will be deleted in 3.51, use IAudioFrameProcessor{@link #IAudioFrameProcessor} instead.
 * @type callback
 * @region audio processing
 * @brief Custom audio processor
 */
class IAudioProcessor{
public:
    /** 
     * @type callback
     * @region Audio processing
     * @brief Get the audio frames collected by the RTC SDK, and perform custom processing, and finally give the processed audio frames to the RTC SDK for encoding and transmission.
     * @param  [in] audioFrame The audio frame collected by the RTC SDK can be customized to directly modify the data in the audio data. See IAudioFrame{@link #IAudioFrame}.
     * @return   <br>
     *        0: Unprocessed <br>
     *         > 0: Processing succeeded <br>
     *         < 0: Processing failed
     * @notes Before audio custom processing, you need to call `registerLocalAudioProcessor` to set up the audio custom processor.
     */
    virtual int processAudioFrame(const IAudioFrame& audioFrame) = 0;
};
/** 
 * @hidden(Linux)
 * @type callback
 * @brief The custom audio processor.
 */
class IAudioFrameProcessor{
public:
    /** 
     * @type callback
     * @brief Returns the address of the locally captured audio frame for custom processing.
     * @param [in] audioFrame The address of the audio frame. See IAudioFrame{@link #IAudioFrame}
     * @notes <br>
     *        + After custom processing, SDK will encode the processed audio frames and transmit to the remote user.  <br>
     *        + After calling `enableAudioProcessor` with locally captured audio frame specified, you will receive this callback.
     */
    virtual int onProcessRecordAudioFrame(IAudioFrame& audioFrame) = 0;
    /** 
     * @type callback
     * @brief Returns the address of the locally captured audio frame for custom processing.
     * @param [in] audioFrame The address of the audio frame. See IAudioFrame{@link #IAudioFrame}
     * @notes After calling `enableAudioProcessor` with mixed remote audio, you will receive this callback.
     */
    virtual int onProcessPlayBackAudioFrame(IAudioFrame& audioFrame) = 0;
    /** 
     * @type callback
     * @brief Returns the address of the locally captured audio frame for custom processing.
     * @param [in] stream_info Information of the audio stream. See RemoteStreamKey{@link #RemoteStreamKey}
     * @param [in] audioFrame The address of the audio frame. See IAudioFrame{@link #IAudioFrame}
     * @notes After calling `enableAudioProcessor` with audio streams of the remote users, you will receive this callback.
     */
    virtual int onProcessRemoteUserAudioFrame(const RemoteStreamKey& stream_info, IAudioFrame& audioFrame) = 0;
    /** 
     * @hidden(Windows,Linux)
     * @type callback
     * @brief You will receive the address of SDK-level in-ear monitoring audio frames for custom processing.
     * @param [in] audioFrame The address of the in-ear monitoring audio frames. See IAudioFrame{@link #IAudioFrame}.
     * @notes <br>
     *        + You will process only in-ear monitoring audio.  <br>
     *        + After calling enableAudioProcessor{@link #RTCVideo#enableAudioProcessor} to set the audio input to in-ear monitoring, you will receive this callback.
     */
    virtual int onProcessEarMonitorAudioFrame(IAudioFrame& audioFrame) = 0;
    /** 
     * @type callback
     * @brief Returns the address of the shared-screen audio frames for custom processing.
     * @param [in] audioFrame The address of audio frames. See IAudioFrame{@link #IAudioFrame}.
     * @notes After calling `enableAudioProcessor` to set the audio input to the shared-screen audio, you will receive this callback.
     */
    virtual int onProcessScreenAudioFrame(IAudioFrame& audioFrame) = 0;
};

/** 
 * @deprecated since 3.45 and will be deleted in 3.51.
 * @type callback
 * @region audio data callback
 * @brief Audio data callback observer
 */
class IRemoteAudioFrameObserver {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IRemoteAudioFrameObserver() {
    }

    /** 
     * @type callback
     * @region Audio data callback
     * @brief Get audio data for a single stream, this callback is triggered by calling registerRemoteAudioFrameObserver {@link #IRTCVideo#registerRemoteAudioFrameObserver}.
     * @param  [in] audio_frame Audio data, see: IAudioFrame{@link #IAudioFrame}
     * @param  [in] stream_info The audio stream business information, see: RemoteStreamKey{@link #RemoteStreamKey}
     */
    virtual void onRemoteAudioFrame(const IAudioFrame& audio_frame, const RemoteStreamKey& stream_info) = 0;
};



}  // namespace bytertc
