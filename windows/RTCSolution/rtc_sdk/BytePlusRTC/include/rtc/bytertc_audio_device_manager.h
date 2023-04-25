/*
 * Copyright (c) 2020 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Audio Device Manager
*/

#pragma once

#include "bytertc_audio_defines.h"

namespace bytertc {

/** 
 * @deprecated since 3.45 and will be deleted in 3.51, use IAudioDeviceCollection{@link #IAudioDeviceCollection} and IVideoDeviceCollection{@link #IVideoDeviceCollection} instead.
 * @type api
 * @region engine management
 * @brief Audio & video equipment related information
 */
class IDeviceCollection {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    IDeviceCollection() {
    }
    /** 
     * @type api
     * @region engine management
     * @brief Get the number of audio & video devices in the current system
     * @return Number of audio & video devices
     */
    virtual int getCount() = 0;
    /** 
     * @type api
     * @region Engine management
     * @brief According to the index number, get device information
     * @param [in] index Device index number, starting from 0, note that it must be less than the return value of getCount{@link #IAudioDeviceCollection#getCount}.
     * @param [out] device_name device name
     * @param [out] device_id device ID
     * @return  <br>
     *         + 0: Success. <br>
     *         +! 0: failure <br>
     */
    virtual int getDevice(int index, char device_name[MAX_DEVICE_ID_LENGTH], char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /** 
     * @type api
     * @region engine management
     * @brief Releases the resources occupied by the current IDeviceCollection{@link #IAudioDeviceCollection} object.
     * @notes This method should be called to release related resources when you do not need to return a list of audio & video device related information.
     */
    virtual void release() = 0;
/**
 * @hidden constructor/destructor
 */
protected:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IDeviceCollection() {
    }
};

/** 
 * @hidden(Android,iOS)
 * @type api
 * @region  engine management
 * @brief Audio equipment related information
 */
class IAudioDeviceCollection : public IDeviceCollection {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    IAudioDeviceCollection() {};
    /** 
     * @type api
     * @region engine management
     * @brief Get the number of audio & video devices in the current system
     * @return Number of audio devices
     */
    virtual int getCount() override {return 0;};
    /** 
     * @type api
     * @region Engine management
     * @brief According to the index number, get device information
     * @param  [in] index Device index number, starting from 0, note that it must be less than the return value of getCount{@link #IAudioDeviceCollection#getCount}.
     * @param  [out] device_name Device name
     * @param  [out] device_id Device ID
     * @return   <br>
     *         + 0: Success. <br>
     *         +! 0: failure <br>
     */
    virtual int getDevice(int index, char device_name[MAX_DEVICE_ID_LENGTH], char device_id[MAX_DEVICE_ID_LENGTH]) override {return 0;};
    /** 
     * @type api
     * @region  engine management
     * @brief  Releases the resources occupied by the current IAudioDeviceCollection{@link #IAudioDeviceCollection} object.
     * @notes This method should be called to release related resources when you do not need to return a list of audio device related information.
     */
    virtual void release()override {};
    /** 
     * @type api
     * @region Engine management
     * @brief According to the index number, get device information
     * @param  [in] index Device index number, starting from 0, note that it must be less than the return value of GetCount{@link #IAudioDeviceCollection#getCount}.
     * @param  [out] audio_device_info device info. Refer to AudioDeviceInfo{@link #AudioDeviceInfo} for details.
     * @return   <br>
     *         + 0: Success. <br>
     *         +! 0: failure <br>
     */
    virtual int getDevice(int index, AudioDeviceInfo* audio_device_info) = 0;
protected:
    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IAudioDeviceCollection() {};
};

/** 
 * @hidden(Android,iOS)
 * @type api
 * @brief Audio Facility Management
 */
class IAudioDeviceManager {
public:
    /** 
     * @hidden constructor/destructor
     * @brief Constructor
     */
    IAudioDeviceManager() {
    }
    /** 
     * @type api
     * @region  Audio device management
     * @brief  Get a list of the audio playback device. When the audio playback device changes, you will receive `onAudioMediaDeviceStateChanged` and you need to call this API again to get the new device list.
     * @return  A list of all audio playback devices. See IAudioDeviceCollection{@link #IAudioDeviceCollection}.
     */
    virtual IAudioDeviceCollection* enumerateAudioPlaybackDevices() = 0;
    /** 
     * @type api
     * @region  Audio Facility Management
     * @brief  Get a list of audio acquisition devices in the current system. If there are subsequent device changes, you need to call this interface again to get a new device list.
     * @return An object that contains a list of all audio capture devices in the system. See IAudioDeviceCollection{@link #IAudioDeviceCollection}.
     */
    virtual IAudioDeviceCollection* enumerateAudioCaptureDevices() = 0;

    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Set the audio playback device to follow the OS setting or not.
     * @param [in] followed <br>
     *        + true: follow the OS setting. You can not call setAudioPlaybackDevice{@link #IAudioDeviceManager#setAudioPlaybackDevice} at the same time. The default value.
     *        + false: do not follow the OS setting. You can call setAudioPlaybackDevice{@link #IAudioDeviceManager#setAudioPlaybackDevice} to set the audio playback device.
     */
    virtual void followSystemPlaybackDevice(bool followed) = 0;

    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Set the audio capture device to follow the OS setting or not.
     * @param [in] followed <br>
     *        + true: follow the OS setting. You can not call setAudioCaptureDevice{@link #IAudioDeviceManager#setAudioCaptureDevice} at the same time. The default value.
     *        + false: do not follow the OS setting. You can call setAudioCaptureDevice{@link #IAudioDeviceManager#setAudioCaptureDevice} to set the audio capture device.
     */
    virtual void followSystemCaptureDevice(bool followed) = 0;
    /** 
     * @type api
     * @region Audio device management
     * @brief Sets the audio playback device.
     * @param [in] device_id Audio playback device's ID. You can get the ID by calling
     * enumerateAudioPlaybackDevices{@link #enumerateAudioPlaybackDevices}.
     * @return    <br>
     *         + 0: Success <br>
     *         + < 0: Failure <br>
     * @notes After you call followSystemPlaybackDevice{@link #IAudioDeviceManager#followSystemPlaybackDevice} to set the audio playback device to follow the system setting, you cannot call this API to set the audio playback device.
     */
    virtual int setAudioPlaybackDevice(const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Set up audio capture devices.
     * @param [in] device_id Audio capture device ID, available through enumerateAudioCaptureDevices{@link #enumerateAudioCaptureDevices}.
     * @return  Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     * @notes After you call followSystemCaptureDevice{@link #IAudioDeviceManager#followSystemCaptureDevice} to set the audio playback device to follow the system setting, you cannot call this API to set the audio capture device.
     */
    virtual int setAudioCaptureDevice(const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /** 
     * @type api
     * @region  Audio Facility Management
     * @brief Set the current audio playback device volume
     * @param [in] volume Audio playback device volume, the value range is [0,255], the setting beyond this range is invalid.
     *        + [0,25] Is nearly silent; <br>
     *        + [25,75] Is low volume; <br>
     *        + [76,204] Is medium volume; <br>
     *        + [205,255] Is high volume. <br>
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int setAudioPlaybackDeviceVolume(unsigned int volume) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Get the current audio playback device volume.
     * @param [out] volume Audio playback device volume, the range should be within [0,255].
     *        + [0,25] Is nearly silent; <br>
     *        + [25,75] Is low volume; <br>
     *        + [76,204] Is medium volume; <br>
     *        + [205,255] Is high volume. <br>
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int getAudioPlaybackDeviceVolume(unsigned int* volume) = 0;
    /** 
     * @type api
     * @region  Audio Facility Management
     * @brief Set the current audio capture volume
     * @param [in] volume Audio capture volume, the value range is [0,255], the setting beyond this range is invalid.
     *        + [0,25] Is nearly silent; <br>
     *        + [25,75] Is low volume; <br>
     *        + [76,204] Is medium volume; <br>
     *        + [205,255] Is high volume. <br>
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int setAudioCaptureDeviceVolume(unsigned int volume) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Get the current audio capture volume.
     * @param [out] volume Audio capture volume, the range is within [0,255].
     *        + [0,25] Is nearly silent; <br>
     *        + [25,75] Is low volume; <br>
     *        + [76,204] Is medium volume; <br>
     *        + [205,255] Is high volume. <br>
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int getAudioCaptureDeviceVolume(unsigned int* volume) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Mute or Unmute the current audio playback device. The default state is speaking.
     * @param  [in] mute <br>
     *        + true: Mute <br>
     *        + false: Speaking <br>
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int setAudioPlaybackDeviceMute(bool mute) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Get the mute state of the current audio playback device.
     * @param [out] mute <br>
     *        + true: Mute <br>
     *        + false: Speaking <br>
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int getAudioPlaybackDeviceMute(bool* mute) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Mute or Unmute the current audio capture device. The default state is speaking.
     * @param [in] mute <br>
     *        + true: Mute <br>
     *        + false: Speaking <br>
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int setAudioCaptureDeviceMute(bool mute) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Get state of the audio capture device.
     * @param [out] mute <br>
     *        + true: Mute <br>
     *        + false: Speaking <br>
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int getAudioCaptureDeviceMute(bool* mute) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Gets the current audio playback device ID.
     * @param [out] device_id Audio playback device ID
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int getAudioPlaybackDevice(char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Gets the current audio capture device ID.
     * @param [out] device_id audio capture device ID. You should allocate and free memory according to the MAX_DEVICE_ID_LENGTH size.
     * @return   Method call result   <br>
     *         + 0: Success. <br>
     *         + < 0: failure <br>
     */
    virtual int getAudioCaptureDevice(char device_id[MAX_DEVICE_ID_LENGTH]) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Try to initialize the audio playback device, and you can detect abnormal problems such as the device does not exist, permissions are denied/disabled, etc.
     * @param [in] deviceId Device index number
     * @return Device status error code
     *         + 0: device detection result is normal
     *         + -1: The interface status is incorrect, for example, the interface is called for detection after the acquisition is started normally
     *         + -2: The acquisition device does not have microphone permission, and attempts to initialize the device fail
     *         + -3: The device does not exist, there is currently no device or return when the device is removed
     *         + -4: Device audio format not supported
     *         + -5: Error for other reasons
     * @notes 1. This interface needs to be called before entering the room; <br>
     *        2. Successful detection does not necessarily mean that the device can be started successfully. It may also fail to start due to the device being monopolized by other application processes, or CPU/memory shortage.
     */
    virtual int initAudioPlaybackDeviceForTest(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;
    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Try to initialize the audio capture device, which can detect abnormal problems such as the device does not exist, permissions are denied/disabled, etc.
     * @param [in] deviceId Device index
     * @return Device status error code
     *         + 0: device detection result is normal
     *         + -1: The interface status is incorrect, for example, the interface is called for detection after the acquisition is started normally
     *         + -2: The acquisition device does not have microphone permission, and attempts to initialize the device fail
     *         + -3: The device does not exist, there is currently no device or return when the device is removed
     *         + -4: Device audio format not supported
     *         + -5: Error for other reasons
     * @notes 1. This interface needs to be called before entering the room; <br>
     *        2. Successful detection does not necessarily mean that the device can be started successfully. It may also fail to start due to the device being monopolized by other application processes, or CPU/memory shortage.
     */
    virtual int initAudioCaptureDeviceForTest(const char deviceId[MAX_DEVICE_ID_LENGTH]) = 0;

    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Start the playback test for the local audio device. RTC will start playing the audio file specified. And RTC will notify the audio volume via the `onAudioPlaybackDeviceTestVolume` periodically.
     * @param [in] test_audio_file_path  Specify the path of the audio file for the playback test, including *.mp3, *.aac, *.m4a, *.3gp, and *.wav.
     * @param [in] indication_interval The time interval between each callback in milliseconds. We recommend setting it to 200 ms. The minimal value is 10 ms.
     * @return <br>
     *         + 0: Success  <br>
     *         + < 0: Failure
     * @notes <br>
     *       + You can call this API whether the user is in the room.   <br>
     *       + Call stopAudioPlaybackDeviceTest{@link #IAudioDeviceManager#stopAudioPlaybackDeviceTest} to stop the playback test before moving on to the other device tests.
     */
     virtual int startAudioPlaybackDeviceTest(const char* test_audio_file_path, int indication_interval) = 0;

    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Stop the playback test for the local audio device.
     * @return Result <br>
     *         + 0: Success  <br>
     *         + < 0: Failure
     * @notes  Call this API to stop the playback test started by calling startAudioPlaybackDeviceTest{@link #IAudioDeviceManager#startAudioPlaybackDeviceTest} before moving on to the other device tests.
     */
    virtual int stopAudioPlaybackDeviceTest() = 0;

    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Start the capture and playback test for local audio devices.
     * @param [in] indication_interval During the test, you'll receive `onLocalAudioPropertiesReport` periodically. Set the period in ms with this parameter. Recommended value is 200 ms; the minimal value is 10 ms.
     * @return  result
     *         + 0: success  <br>
     *         + < 0: failure  <br>
     * @notes  <br>
     *       + The audio capturing stops in 30s after calling this API and begins to play the recording audio. Before that, you can call stopAudioDeviceRecordAndPlayTest{@link #IAudioDeviceManager#stopAudioDeviceRecordAndPlayTest} to stop audio capturing and start playing the recording audio. <br>
     *       + Call stopAudioDevicePlayTest{@link #IAudioDeviceManager#stopAudioDevicePlayTest} to stop the test, including capturing and playing the recording. <br>
     *       + You must stop the test before starting another test for audio devices. <br>
     *       + You must stop the test before calling `enableAudioPropertiesReport`. <br>
     *       + This test performs locally and does not involve network connection testing. <br>
     */
    virtual int startAudioDeviceRecordTest(int indication_interval) = 0;

    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Call this API to stop recording in the test and start to play the recording in 30 s after calling startAudioDeviceRecordTest{@link #IAudioDeviceManager#startAudioDeviceRecordTest}.
     * @return result
     *         + 0: Success  <br>
     *         + < 0: Failure
     * @notes After calling this API, the recording starts playing during which you can call stopAudioDevicePlayTest{@link #IAudioDeviceManager#stopAudioDevicePlayTest} to stop playing.
     */
    virtual int stopAudioDeviceRecordAndPlayTest() = 0;

    /** 
     * @type api
     * @region Audio Facility Management
     * @brief Stop the capture and playback test for local audio devices which is started by calling startAudioDeviceRecordTest{@link #IAudioDeviceManager#startAudioDeviceRecordTest}.<br>
     * Before the test ends by itself, you can call this API to stop the recording or playing.
     * @return <br>
     *         + 0: Success  <br>
     *         + < 0: failure
     */
    virtual int stopAudioDevicePlayTest() = 0;

    /** 
     * @type api
     * @hidden(Android,iOS,Linux)
     * @region Audio Facility Management
     * @brief Enables/disables the silent device filter function.
     * @param [in] enable Whether to enable the silent device filter function:
     *        + 1: true
     *        + 0: false
     * @return + 0: Success.
     */
    virtual int enableFilterSilentDevice(bool enable) = 0;

    /** 
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IAudioDeviceManager() {
    }
};

}  // namespace bytertc
