/*
 * Copyright (c) 2022 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Audio Defines
 */

#pragma once

#include "bytertc_media_defines.h"
#include <string.h>
#include <math.h>
// use fft size 512 to calculate spectrum, so spectrum size is 512 / 2 + 1 = 257
#define SPECTRUM_SIZE 257

namespace bytertc {
/** 
 * @type keytype
 * @brief Audio sample rate in Hz.
 */
enum AudioSampleRate {
    /** 
     * @brief Default value. 48000Hz.
     */
    kAudioSampleRateAuto = -1,
    /** 
     * @brief 8000Hz
     */
    kAudioSampleRate8000 = 8000,
    /** 
     * @brief 16000Hz
     */
    kAudioSampleRate16000 = 16000,
    /** 
     * @brief 32000Hz
     */
    kAudioSampleRate32000 = 32000,
    /** 
     * @brief 44100Hz
     */
    kAudioSampleRate44100 = 44100,
    /** 
     * @brief 48000Hz
     */
    kAudioSampleRate48000 = 48000
};
/** 
 * @type keytype
 * @brief Audio channel
 */
enum AudioChannel {
    /** 
     * @brief Default value.
     */
    kAudioChannelAuto = -1,
    /** 
     * @brief Mono channel.
     */
    kAudioChannelMono = 1,
    /** 
     * @brief Dual channels.
     */
    kAudioChannelStereo = 2
};

/** 
 * @type keytype
 * @brief Audio format
 */
struct AudioFormat {
    /** 
     * @brief Audio sample rate. See AudioSampleRate{@link #AudioSampleRate}
     */
    AudioSampleRate sample_rate;
    /** 
     * @brief Audio channels. See AudioChannel{@link #AudioChannel}
     */
    AudioChannel channel;
};
/** 
 * @type keytype
 * @brief The audio input for the audio processor.
 */
enum AudioProcessorMethod{
    /** 
     * @brief Locally captured audio frame.
     */
    kAudioFrameProcessorRecord = 0,
    /** 
     * @brief The mixed remote audio.
     */
    kAudioFrameProcessorPlayback = 1,
    /** 
     * @brief The audio streams from remote users.
     */
    kAudioFrameProcessorRemoteUser = 2,
    /** 
     * @hidden(Windows,Linux,macOS)
     * @brief The SDK-level in-ear monitoring.
     */
    kAudioFrameProcessorEarMonitor = 3,
    /** 
     * @hidden(Linux)
     * @brief The shared-screen audio.
     */
    kAudioFrameProcessorScreen = 4,
};
/** 
 * @type keytype
 * @brief Type of audio device
 */
enum RTCAudioDeviceType {
    /** 
     * @brief Unknown device
     */
    kRTCAudioDeviceTypeUnknown = -1,
    /** 
     * @brief Speaker or headphone
     */
    kRTCAudioDeviceTypeRenderDevice = 0,
    /** 
     * @brief Microphone
     */
    kRTCAudioDeviceTypeCaptureDevice = 1,
    /** 
     * @brief Screen capturing audio device
     */
    kRTCAudioDeviceTypeScreenCaptureDevice = 2,
};

/** 
 * @type keytype
 * @brief Audio playback device
 */
enum AudioRoute {
    /** 
     * @brief Default devices
     */
    kAudioRouteDefault = -1,
    /** 
     * @brief Wired earphones
     */
    kAudioRouteHeadset = 1,
    /** 
     * @brief Earpiece
     */
    kAudioRouteEarpiece = 2,
    /** 
     * @brief Speaker
     */
    kAudioRouteSpeakerphone = 3,
    /** 
     * @brief Bluetooth earphones
     */
    kAudioRouteHeadsetBluetooth = 4,
    /** 
     * @brief USB Device
     */
    kAudioRouteHeadsetUSB = 5
};

/** 
 * @type keytype
 * @brief  Audio playback device
 */
enum AudioPlaybackDevice {
    /** 
     * @brief Wired earphones
     */
    kAudioPlaybackDeviceHeadset = 1,
    /** 
     * @brief Earpiece
     */
    kAudioPlaybackDeviceEarpiece = 2,
    /** 
     * @brief Speaker
     */
    kAudioPlaybackDeviceSpeakerphone = 3,
    /** 
     * @brief Bluetooth earphones
     */
    kAudioPlaybackDeviceHeadsetBluetooth = 4,
    /** 
     * @brief USB Device
     */
    kAudioPlaybackDeviceHeadsetUSB = 5
};

/** 
 * @type keytype
 * @brief Audio scene type. <br>
 *        After selecting an audio scene, RTC will automatically apply call volume/media volume based on the client's audio capture playback device and status. <br>
 *        If the preset audio scene type cannot meet your business needs, please contact technical support classmate for customization.
 */
enum AudioScenarioType {
    /** 
     * @brief Music scene. Default setting. <br>
     *        This scene is suitable for scenes that require musical expression. Such as live music, etc. <br>
     *         Audio capture playback device and capture playback status, the mapping to the volume type is as follows: <br>
     *         <table>
     *            <tr><th></th><th>Mic Off</th><th>Mic On</th><th>Remarks</th><tr>
     *            <tr><td>Build-in microphone and speaker/earpiece</td><td>Media volume</td><td>Call volume</td><td>/</td></tr>
     *            <tr><td>Wired headset/ USB headset/ external sound card</td><td>Media volume</td><td> Media Volume</td><td>/</td></tr>
     *            <tr><td>Bluetooth Headset</td><td>Media Volume</td><td>Media Volume</td><td>Even if the Bluetooth headset has a microphone, you can only use the build-in microphone for local audio capture.</td></tr>
     *         </table>
     */
    kAudioScenarioTypeMusic = 0,
    /** 
     * @brief High-quality call scenarios. <br>
     *        Media volume is perferred in this scenario. The preference may introduce switch of volume type when you switch on/off the microphone. To avoid the switch, use `AUDIO_SCENARIO_HIGHQUALITY_CHAT`.
     *        This scenario can provide decent audio performance using the speaker/bluetooth earphones. It can also avoid sudden changes of volume type when switching to bluetooth earphones.
     *         Audio capture playback device and capture playback status, the mapping to the volume type is as follows: <br>
     *         <table>
     *            <tr><th></th><th>Mic Off</th><th>Mic On</th><th>Remarks</th><tr>
     *            <tr><td>Build-in microphone and speaker/earpiece</td><td> Media volume </td><td> call volume </td><td>/</td></tr>
     *            <tr><td>Wired headset/ USB headset/ external sound card</td><td> Media volume </td><td> Media Volume </td><td>/</td></tr>
     *            <tr><td> Bluetooth Headset </td><td> Call Volume </td><td> Call Volume </td><td>The microphone on the Bluetooth headset can be used for audio capture. </td></tr>
     *         </table>
     */
    kAudioScenarioTypeHighQualityCommunication = 1,
    /** 
     * @brief Call volume-only scene.<br>
     *        In this scenario, regardless of the client side audio acquisition and playback device and the acquisition and playback status, the call volume is used throughout the process.<br>
     *        Suitable for calls or meeting scenarios that require frequent access. <br>
     *        This scene maintains a uniform audio pattern without sudden volume changes in listening; echoes are eliminated to the greatest extent possible for optimum call clarity.<br>
     *        When using a Bluetooth headset, you can use the microphone that comes with the Bluetooth headset for audio capture. <br>
     *        However, this scene will depress the volume of other audio being played back using the media volume and the sound quality will deteriorate.
     */
    kAudioScenarioTypeCommunication = 2,
    /** 
     * @brief Pure media scene. It is generally not recommended.
     *        In this scenario, regardless of the client side audio acquisition and playback device and the acquisition and playback status, the media volume is used throughout the process.
     *        If the audio quality is unsatisfactory when you put on speaker, please contact our technical support team.
     */
    kAudioScenarioTypeMedia = 3,
    /** 
     * @brief Game media scene. Only suitable for game scenes.
     *        If the audio quality is unsatisfactory when you put on speaker, please contact our technical support team.
     *        For different audio device and status, the selected volume type is as follows:
     *         <table>
     *            <tr><th></th><th>Mic Off</th><th>Mic On</th><th>Remarks</th><tr>
     *            <tr><td>Build-in microphone and speaker/earpiece</td><td> Media volume </td><td> Media volume </td><td>/</td></tr>
     *            <tr><td>Wired headset/ USB headset/ external sound card</td><td> Media volume </td><td> Media Volume </td><td>/</td></tr>
     *            <tr><td> Bluetooth Headset </td><td> Call Volume </td><td> Call Volume </td><td> Ability to use the microphone included in the Bluetooth headset for audio capture. </td></tr>
     *         </table>
     */
    kAudioScenarioTypeGameStreaming = 4,
    /** 
     * @brief High-quality chatting scenario <br>
     *        This scenerio is the same as `kAudioScenarioTypeHighQualityCommunication`, but in one case: in this scenerio, switching on/off the microphone does not change the volume type. When you talk with the built-in microphone and speaker/earpiece, the call volume is used all the time. <br>
     *        For different audio device and status, the selected volume type is as follows:
     *         <table>
     *            <tr><th></th><th>Mic Off</th><th>Mic On</th><th>Remarks</th><tr>
     *            <tr><td>Build-in microphone and speaker/earpiece</td><td> Call volume </td><td> call volume </td><td>/</td></tr>
     *            <tr><td>Wired headset/ USB headset/ external sound card</td><td> Media volume </td><td> Media Volume </td><td>/</td></tr>
     *            <tr><td> Bluetooth Headset </td><td> Call Volume </td><td> Call Volume </td><td> Ability to use the microphone included in the Bluetooth headset for audio capture. </td></tr>
     *         </table>
     */
    kAudioScenarioTypeHighQualityChat = 5,
};

/** 
 * @type keytype
 * @brief Private method. Change sound effect type
 */
enum VoiceChangerType {
    /** 
     * @brief Acoustic, no special effects
     */
    kVoiceChangerTypeOriginal = 0,
    /** 
     * @brief Giant
     */
    kVoiceChangerTypeGiant = 1,
    /** 
     * @brief Chipmunk
     */
    kVoiceChangerTypeChipmunk = 2,
    /** 
     * @brief Little yellow man
     */
    kVoiceChangerTypeMinionst = 3,
    /** 
     * @brief Trill
     */
    kVoiceChangerTypeVibrato = 4,
    /** 
     * @brief Robot
     */
    kVoiceChangerTypeRobot = 5,
};

/** 
 * @type keytype
 * @brief Private method.  Reverb effect type
 */
enum VoiceReverbType {
    /** 
     * @brief Acoustic, no special effects
     */
    kVoiceReverbTypeOriginal = 0,
    /** 
     * @brief Echo
     */
    kVoiceReverbTypeEcho = 1,
    /** 
     * @brief Concert
     */
    kVoiceReverbTypeConcert = 2,
    /** 
     * @brief Ethereal
     */
    kVoiceReverbTypeEthereal = 3,
    /** 
     * @brief Karaoke
     */
    kVoiceReverbTypeKTV = 4,
    /** 
     * @brief Recording studio
     */
    kVoiceReverbTypeStudio = 5,
    /** 
     * @brief Virtual Stereo
     */
    kVoiceReverbTypeVirtualStereo = 6,
    /** 
     * @brief Spacious
     */
    kVoiceReverbTypeSpacious = 7,
    /** 
     * @brief 3D vocal
     */
    kVoiceReverbType3D = 8,
    /** 
     * @hidden for internal use only
     * @brief Pop
     */
    kVoiceReverbTypePop = 9,
    /** 
     * @hidden for internal use only
     * @brief Disco
     */
    kVoiceReverbTypeDisco = 10,
    /** 
     * @hidden for internal use only
     * @brief Old Record
     */
    kVoiceReverbTypeOldRecord = 11,
    /** 
     * @hidden for internal use only
     * @brief Harmony
     */
    kVoiceReverbTypeHarmony = 12,
    /** 
     * @hidden for internal use only
     * @brief Rock
     */
    kVoiceReverbTypeRock = 13,
    /** 
     * @hidden for internal use only
     * @brief Blues
     */
    kVoiceReverbTypeBlues = 14,
    /** 
     * @hidden for internal use only
     * @brief Jazz
     */
    kVoiceReverbTypeJazz = 15,
    /** 
     * @hidden for internal use only
     * @brief Electronic
     */
    kVoiceReverbTypeElectronic = 16,
    /** 
     * @hidden for internal use only
     * @brief Vinyl
     */
    kVoiceReverbTypeVinyl = 17,
    /** 
     * @hidden for internal use only
     * @brief Chamber
     */
    kVoiceReverbTypeChamber = 18,
};
/** 
 * @type keytype
 * @brief Audio Equalization effect.
 */
enum VoiceEqualizationBandFrequency {
    /** 
     * @brief The frequency band with a center frequency of 31Hz.
     */
    kVoiceEqualizationBandFrequency31 = 0,
    /** 
     * @brief The frequency band with a center frequency of 62Hz.
     */
    kVoiceEqualizationBandFrequency62 = 1,
    /** 
     * @brief The frequency band with a center frequency of 125Hz.
     */
    kVoiceEqualizationBandFrequency125 = 2,
        /** 
     * @brief The frequency band with a center frequency of 250Hz.
     */
    kVoiceEqualizationBandFrequency250 = 3,
        /** 
     * @brief The frequency band with a center frequency of 500Hz.
     */
    kVoiceEqualizationBandFrequency500 = 4,
        /** 
     * @brief The frequency band with a center frequency of 1kHz.
     */
    kVoiceEqualizationBandFrequency1k = 5,
    /** 
     * @brief The frequency band with a center frequency of 2kHz.
     */
    kVoiceEqualizationBandFrequency2k = 6,
    /** 
     * @brief The frequency band with a center frequency of 4kHz.
     */
    kVoiceEqualizationBandFrequency4k = 7,
    /** 
     * @brief The frequency band with a center frequency of 8kHz.
     */
    kVoiceEqualizationBandFrequency8k = 8,
    /** 
     * @brief The frequency band with a center frequency of 16kHz.
     */
    kVoiceEqualizationBandFrequency16k = 9,
};
/** 
 * @type keytype
 * @brief 
 */
struct VoiceEqualizationConfig {
    /** 
     * @brief Frequency band. See VoiceEqualizationBandFrequency{@link #VoiceEqualizationBandFrequency}.
     */
    VoiceEqualizationBandFrequency frequency;
    /** 
     * @brief Gain of the frequency band in dB. The range is `[-15, 15]`.
     */
    int gain;
};
/** 
 * @type keytype
 * @brief Voice reverb effect.
 */
struct VoiceReverbConfig {
    /** 
     * @brief The room size for reverb simulation. The range is `[0.0, 100.0]`. The default value is `50.0f`. The larger the room, the stronger the reverberation.
     */
    float room_size = 50.0f;
    /** 
     * @brief The decay time of the reverb effect. The range is `[0.0, 100.0]`. The default value is `50.0f`. 
     */
    float decay_time = 50.0f;
    /** 
     * @brief The damping index of the reverb effect. The range is `[0.0, 100.0]`. The default value is `50.0f`. 
     */
    float damping = 50.0f;
    /** 
     * @brief The Intensity of the wet signal in dB. The range is `[-20.0, 10.0]`. The default value is `0.0f`. 
     */
    float wet_gain = 0.0f;
    /** 
     * @brief The Intensity of the dry signal in dB. The range is `[-20.0, 10.0]`. The default value is `0.0f`. 
     */
    float dry_gain = 0.0f;
    /** 
     * @brief The delay of the wet signal in ms. The range is `[0.0, 200.0]`. The default value is `0.0f`. 
     */
    float pre_delay = 0.0f;
};

/** 
 * @type keytype
 * @brief User ID of the source of the audio stream and the corresponding volume.
 */
struct AudioVolumeInfo {
    /** 
     * @brief Linear volume, the value range is: [0,255], which has a linear relationship with the original volume.
     * The larger the value, the greater the volume. Silence is below 25 (absolutely silent is 0, below 25 there is basically no sound, which can be regarded as silent),
     * low volume is 25~76, medium volume is 76~204, and high volume is above 204
     */
    int linear_volume;
    /** 
     * @brief Non-linear volume, the value range is: [-127, 0], the unit is dB. This volume range is a logarithmic conversion of the original volume,
     * and the resolution is more sensitive at medium and low volume, which is suitable for identification as an Active Speaker (the most active user in the room).
     * The high volume is 0~-20dB, the medium volume is -20~-40dB, and the low volume is -40~-60dB. If it is lower than -60dB, it is silent.
     */
    int nonlinear_volume;
    /** 
     * @brief User of the audio stream source ID
     */
    const char* uid;
};

/** 
 * @type keytype
 * @brief  Audio mix file playback status.
 */
enum AudioMixingState {
    /** 
     * @brief Mix loaded
     */
    kAudioMixingStatePreloaded = 0,
    /** 
     * @brief Mix is playing
     */
    kAudioMixingStatePlaying,
    /** 
     * @brief Mix Pause
     */
    kAudioMixingStatePaused,
    /** 
     * @brief Mixing stopped
     */
    kAudioMixingStateStopped,
    /** 
     * @brief Mix playback failed
     */
    kAudioMixingStateFailed,
    /** 
     * @brief End of mixing
     */
    kAudioMixingStateFinished,
    /** 
     * @brief Prepare PCM Mix
     */
    kAudioMixingStatePCMEnabled,
    /** 
     * @brief End of PCM mix playback
     */
    kAudioMixingStatePCMDisabled,
};
/** 
 * @type keytype
 * @brief Error code for audio mixing
 */
enum AudioMixingError {
    /** 
     * @brief OK
     */
    kAudioMixingErrorOk = 0,
    /** 
     * @brief Preload failed. Invalid path or the length exceeds 20s.
     */
    kAudioMixingErrorPreloadFailed,
    /** 
     * @brief Mixing failed. Invalid path or fail to open the file.
     */
    kAudioMixingErrorStartFailed,
    /** 
     * @brief Invalid mixID
     */
    kAudioMixingErrorIdNotFound,
    /** 
     * @brief Invalid position
     */
    kAudioMixingErrorSetPositionFailed,
    /** 
     * @brief Invalid volume. The range is [0, 400].
     */
    kAudioMixingErrorInValidVolume,
    /** 
     * @brief Another file was preloaded for mixing. Call unloadAudioMixing{@link #IAudioMixingManager#unloadAudioMixing} first.
     */
    kAudioMixingErrorLoadConflict,
    /** 
     * @brief Do not support the mix type.
     */
    kAudioMixingErrorIdTypeNotMatch,
    /** 
     * @brief Invalid pitch value.
     */
    kAudioMixingErrorInValidPitch,
    /** 
     * @brief Invalid audio track.
     */
    kAudioMixingErrorInValidAudioTrack,
    /** 
     * @brief Mixing starting
     */
    kAudioMixingErrorIsStarting,
    /** 
     * @brief Invalid playback speed
     */
    kAudioMixingErrorInValidPlaybackSpeed,
    /** 
     * @deprecated since 3.45 and will be deleted in 3.51
     * @brief Audio mixing error code. Deprecated.
     */
    kAudioMixingErrorCanNotOpen = 701,
};

/** 
 * @type keytype
 * @brief  Whether to turn on the earphone monitoring function
 */
enum EarMonitorMode {
    /** 
     * @brief Not open
     */
    kEarMonitorModeOff = 0,
    /** 
     * @brief Open
     */
    kEarMonitorModeOn = 1,
};

/** 
 * @hidden currently not available
 * @type keytype
 * @region audio management
 * @brief Audio encoding type
 */
enum AudioCodecType {
    /** 
     * @brief Unknown encoding type
     */
    kAudioCodecTypeNone = 0,
    /** 
     * @brief Opus  encoding type
     */
    kAudioCodecTypeOpus,
    /** 
     * @brief AAC  encoding type
     */
    kAudioCodecTypeAac,
};

/** 
 * @type keytype
 * @brief Audio input format type
 */
enum class AudioFormatType {
    /** 
     *  PCM_S16
     */
    kRawPCMs16 = 0,
    /** 
     *  PCM_S32
     */
    kRawPCMs32 = 1,
};

/** 
 * @type keytype
 * @brief bluetooth mode, it works only in media mode.
 */
enum BluetoothMode {
    /** 
     * @brief select bluetooth mode automatically.
     */
    kBluetoothModeAuto = 0,
    /** 
     * @brief use A2DP mode
     */
    kBluetoothModeA2DP,
    /** 
     * @brief use HFP mode
     */
    kBluetoothModeHFP
};

/** 
 * @type keytype
 * @brief  Audio input source type
 */
enum AudioSourceType {
    /** 
     *  Custom Capture Audio Source
     */
    kAudioSourceTypeExternal = 0,
    /** 
     *  RTC SDK internal acquisition audio source
     */
    kAudioSourceTypeInternal,
};

/** 
 * @type keytype
 * @brief Audio output type
 */
enum AudioRenderType {
    /** 
     *  Custom Render
     */
    kAudioRenderTypeExternal = 0,
    /** 
     *  RTC SDK internal render
     */
    kAudioRenderTypeInternal,
};

/** 
 * @type keytype
 * @brief Speech recognition service authentication method, please consult the speech recognition service related classmates for details
 */
enum ASRAuthorizationType {
    /** 
     * @brief Token authentication
     */
    kASRAuthorizationTypeToken = 0,
    /** 
     * @brief Signature authentication
     */
    kASRAuthorizationTypeSignature = 1,
};

/** 
 * @type keytype
 * @brief Verification information required to use automatic speech recognition services
 */
struct RTCASRConfig {
    /** 
     * @brief Application ID
     */
    const char* app_id;
    /** 
     * @brief User ID
     */
    const char* user_id;
    /** 
     * @brief For authentication methods. See ASRAuthorizationType{@link #ASRAuthorizationType}
     */
    ASRAuthorizationType authorization_type;
    /** 
     * @brief Access token
     */
    const char* access_token;
    /** 
     * @brief Private key. Signature  cannot be empty in authentication mode, and it is empty in token authentication mode. See [Authentication Method](https://docs.byteplus.com/speech/docs/authentication-method).
     */
    const char* secret_key;
    /** 
     * @brief For scenario information. See Business Clusters (https://docs.byteplus.com/en/speech/docs/real-time-speech-recog)
     */
    const char* cluster;
};

/** 
 * @hidden currently not available
 * @type keytype
 * @brief Speech recognition service error code.   <br>
 *        Except for errors caused by network reasons, the speech recognition service will retry itself, other errors will stop the service. At this time, it is recommended to call `startASR` again to restart the speech recognition function.
 */
enum RTCASRErrorCode {
    /** 
     * @brief The network connection is broken, the service is unavailable, and internal reconnection will be carried out
     */
    kRTCASRErrorNetworkInterrupted = -1,
    /** 
     * @brief Repeatedly call `startASR`. After starting the speech recognition service, you need to call `stopASR` to stop the speech recognition service before you can call `startASR` again to start the service.
     */
    kRTCASRErrorAlreadyStarted = -2,
    /** 
     * @brief The token required by the speech recognition service is empty
     */
    kRTCASRErrorTokenEmpty = -3,
    /** 
     * @brief Signature  secret_key empty in authentication mode
     */
    kRTCErrorSignatureKeyEmpty = -4,
    /** 
     * @brief User ID is empty
     */
    kRTCASRErrorUserIdNull = -5,
    /** 
     * @brief Application ID is empty
     */
    kRTCASRErrorAPPIDNull = -6,
    /** 
     * @brief Speech recognition service cluster is empty
     */
    kRTCASRErrorClusterNull = -7,
    /** 
     * @brief The speech recognition service connection failed, this version does not have speech recognition function, please contact RTC technical support.
     */
    kRTCASRErrorOperationDenied = -8
};

/** 
 * @type keytype
 * @brief Mixing type
 */
enum AudioMixingType {
    /** 
     * @brief Played at the local device only
     */
    kAudioMixingTypePlayout,
    /** 
     * @brief Sent to the remote devices only
     */
    kAudioMixingTypePublish,
    /** 
     * @brief Played at the local device and sent to the remote devices.
     */
    kAudioMixingTypePlayoutAndPublish
};

/** 
 * @type keytype
 * @brief  Mixing configuration
 */
struct AudioMixingConfig {
    /** 
     * @brief For mixing playback types. See AudioMixingType{@link #AudioMixingType}
     */
    AudioMixingType type;
    /** 
     * @brief Mix playback times,
     *        + Play_count < = 0: Infinite loop <br>
     *        + Play_count == 1: Play once (default) <br>
     *        + Play_count > 1: Play play_count times
     */
    int play_count;
     /** 
      * @brief The position of the audio file playback progress bar during audio mixing, the parameter should be an integer, in milliseconds.
      */
     int position;
     /** 
      * @brief Set the time interval (ms) for the audio file playing progress callback. The `onAudioMixingPlayingProgress` callback then will be triggered according to the set value, no callback by default.  <br>
      *       + The value of interval is a multiple of 10 greater than 0. When the value set is not divisible by 10, the default is rounded up by 10. For example, if the value is set to 52ms, it will be automatically adjusted to 60ms, then the SDK will trigger `onAudioMixingPlayingProgress` callback at the set interval.  <br>
      *       + If the value is less than or equals to 0, the callback will not be triggered.  <br>
      */
    int64_t callback_on_progress_interval = 0;
    /** 
     * @brief Attach the process information of local audio file mixing to the captured audio data. Enable the function to enhance the synchronicity of the remote audio mixing.
     * @notes <br>
     *        + The function is effective when mixing a single audio file. <br>
     *        + Use `true` for enabling the function and `false` for disable the function. The default is `false`.
     */
    bool sync_progress_to_record_frame = false;
};

/** 
 * @type keytype
 * @brief  Mix playback channel type
 */
enum AudioMixingDualMonoMode{
    /** 
     * @brief Consistent with audio files
     */
    kAudioMixingDualMonoModeAuto,
    /** 
     * @brief Only the left channel audio in the audio file can be heard
     */
    kAudioMixingDualMonoModeL,
    /** 
     * @brief Only the right channel audio in the audio file can be heard
     */
    kAudioMixingDualMonoModeR,
    /** 
     * @brief Can hear the left and right audio channels in the audio file at the same time
     */
    kAudioMixingDualMonoModeMix
};

/** 
 * @type keytype
 * @brief The volume callback modes.
 */
enum AudioReportMode {
    /** 
     * @brief Always-on(Default).
     */
    kAudioReportModeNormal,
   /** 
     * @brief After visibly joining a room and unpublish your streams, disable the volume callback.
     */
    kAudioReportModeDisconnect,
    /** 
     * @brief After visibly joining a room and unpublish your streams, enable the volume callback. The volume is reset to 0.
     */
    kAudioReportModeReset,
};

/** 
 * @type keytype
 * @brief Configuration of whether including locally mixed audio info in the audio properties report.
 */
enum AudioPropertiesMode {
    /** 
     * @brief Only locally captured microphone audio info and locally captured screen audio info are included in the audio properties report.
     */
    kAudioPropertiesModeMicrophone,
    /** 
     * @brief Locally mixing audio info is included in the audio properties report, in addition to locally captured microphone audio info and locally captured screen audio info.
     */
    kAudioPropertiesModeAudioMixing
};

/** 
 * @type keytype
 * @brief Configuration for audio property prompt.
 */
struct AudioPropertiesConfig {
    /** 
     * @brief Prompt interval in ms <br>
     *        + `<= 0`: Turn off prompt <br>
     *        + `(0,100]`: Invalid interval value, and will be automatically reset to 100ms. <br>
     *        + `> 100`: the actual value of interval
     */
    int interval = 0;

    /** 
     * @brief Whether to enable audio spectrum detection.
     */
    bool enable_spectrum = false;

    /** 
     * @brief Whether to enable Voice Activity Detection.
     */
    bool enable_vad = false;

    /** 
     * @brief The volume callback modes. See AudioReportMode{@link #AudioReportMode}.
     */
    AudioReportMode local_main_report_mode = kAudioReportModeNormal;

    /** 
     * @brief The smoothing coefficient for audio attribute information prompt. The range is `(0.0, 1.0]`. <br>
     *        The default value is `1.0`, which means the smoothing effect is off by default. Smaller the value, smoother the audio volume prompt. If you want to enable the smooth effect, the recommended value is `0.3`.
     */
    float smooth = 1.0f;   
        
    /** 
     * @brief Configuration of whether to include locally mixed audio info in `onLocalAudioPropertiesReport`. See AudioPropertiesMode{@link #AudioPropertiesMode}. <br>
     *        Locally captured microphone audio info and locally captured screen audio info are included by default.
     */
    AudioPropertiesMode audio_report_mode = kAudioPropertiesModeMicrophone;
};
/** 
 * @type keytype
 * @brief Audio properties   <br>
 */
struct AudioPropertiesInfo {
    /** 
     * @brief linear volume. The value is in linear relation to the original volume. The higher the value, the higher the volume. The range is [0,255]. <br>
     *        - [0, 25]: Silence <br>
     *        - [26, 75]: Low volume <br>
     *        - [76, 204]: Medium volume <br>
     *        - [205, 255]: High volume <br>
     */
    int linear_volume = 0;
    /** 
     * @brief non-linear volume in dB. The value is in proportion to the log value of the original volume. You can use the value to recognize the Active Speaker in the room. The range is [-127, 0]. <br>
     *        - [-127, -60]: Silence <br>
     *        - [-59, -40]: Low volume <br>
     *        - [-39, -20]: Medium volume <br>
     *        - [-19, 0]: High volume <br>
     */
    int nonlinear_volume = 0;
    /** 
     * @brief Spectrum array. The default length is 257.
     */
    float spectrum[SPECTRUM_SIZE] = {0};
    /** 
     * @brief Voice Activity Detection (VAD) result
     *        + 1: Voice activity detected.<br>
     *        + 0: No voice activity detected.<br>
     *        + -1: VAD not activated.<br>
     */
    int vad = -1;
};

/** 
 * @type keytype
 * @brief Remote audio properties
 */
struct RemoteAudioPropertiesInfo {
    /** 
     * @type keytype
     * @brief Remote stream information. See RemoteStreamKey{@link #RemoteStreamKey}.
     */
    RemoteStreamKey stream_key;
    /** 
     * @type keytype
     * @brief See AudioPropertiesInfo{@link #AudioPropertiesInfo}.
     */
    AudioPropertiesInfo audio_properties_info;
};

/** 
 * @type keytype
 * @brief Local audio properties
 */
struct LocalAudioPropertiesInfo {
    /** 
     * @brief See StreamIndex{@link #StreamIndex}.
     */
    StreamIndex stream_index = StreamIndex::kStreamIndexMain;
    /** 
     * @type keytype
     * @brief See AudioPropertiesInfo{@link #AudioPropertiesInfo}.
     */
    AudioPropertiesInfo audio_properties_info;
};

/** 
 * @type keytype
 * @brief  Sound quality
 */
enum AudioProfileType {
    /** 
     * @brief Default sound quality<br>
     *        The sound quality configuration of RoomProfileType{@link #RoomProfileType} set by the server or client.
     */
    kAudioProfileTypeDefault = 0,
    /** 
     * @brief Smooth  <br>
     *        Sample rate: 16 KHz<br>
     *        Mono-channel<br>
     *        Encoding bitrate: 32 Kpbs <br>
     *        Low resource consumption, and small network packets guarantees a smooth call. It is suitable for most game scenarios, such as team-wide voice chat, group-wide voice chat, nation-wide voice chat.
     */
    kAudioProfileTypeFluent = 1,
    /** 
     * @brief Mono-channel standard  <br>
     *        Sample rate: 24 KHz<br>
     *        Encoding bitrate: 48 Kpbs<br>
     *        For scenarios requiring distinct voice, you can choose this mode, which achieves balanced latency, consumption, and network packets. It is suitable for educational Apps and the online Mafia Games.
     */
    kAudioProfileTypeStandard = 2,
    /** 
     * @brief Dual-channel music  <br>
     *        Sample rate: 48 KHz<br>
     *        Encoding bitrate: 128 Kpbs<br>
     *        This mode provides high-resolution audio at a cost of high latency, consumption, and large network packets. It is suitable for music Apps such as co-hosting and online talent contests. <br>
     *        Not recommended for game Apps.
     */
    kAudioProfileTypeHD = 3,
    /** 
     * @brief Dual-channel standard  <br>
     *        Sample rate: 48 KHz<br>
     *        Encoding bitrate: 80 Kpbs
     */
    kAudioProfileTypeStandardStereo = 4,
    /** 
     * @brief Mono-channel music  <br>
     *        Sample rate: 48 KHz<br>
     *        Encoding bitrate: 64 Kpbs
     */
    kAudioProfileTypeHDMono = 5,
};

/** 
 * @type keytype
 * @brief ANC modes.The ANC algorithm is determined by the room profile you set when entering the room.
 */
enum AnsMode {
   /** 
     * @brief Disable ANC.
     */
    kAnsModeDisable = 0,
     /** 
     * @brief Cancel subtle background noise.
     */
    kAnsModeLow = 1,
    /** 
     * @brief Cancel medium-level, continuous noise, such as the sound of fans or air conditioners.
     */
    kAnsModeMedium = 2,
    /** 
     * @brief Cancel loud, impulsive, and intermittent noise, such as keyboard clicking noise, a crash/clash, a bark, and chair scraping noise.
     */
    kAnsModeHigh = 3,
    /** 
     * @brief Enable automatic ANC. The ANC algorithm is dynamically determined by RTC.
     */
    kAnsModeAutomatic = 4,
};

/** 
 * @type keytype
 * @brief Coordinate value of the local user's position in the rectangular coordinate system in the RTC room.
 */
struct Position {
    /** 
     * @brief X-coordinate
     */
    float x;
    /** 
     * @brief Y-coordinate
     */
    float y;
    /** 
     * @brief Z-coordinate
     */
    float z;
    /**
     * @hidden constructor/destructor
     */
    bool isEqual(const Position& pos) const {
        return (fabs(x - pos.x) < 1e-2) &&
        (fabs(y - pos.y) < 1e-2) &&
        (fabs(z - pos.z) < 1e-2);
    }
};

/** 
 * @type keytype
 * @brief Direction Orientation Information
 */
struct Orientation {
    /** 
     * @brief X-coordinate
     */
    float x;
    /** 
     * @brief Y-coordinate
     */
    float y;
    /** 
     * @brief Z-coordinate
     */
    float z;
    /**
     * @hidden constructor/destructor
     */
    bool isEqual(const Orientation& orientation) const {
        return x == orientation.x && y == orientation.y && z == orientation.z;
    }
};

/** 
 * @type keytype
 * @brief Three-dimensional orientation information, each pair of vectors need to be perpendicular. See Orientation{@link #Orientation}.
 */
struct HumanOrientation {
    /** 
     * @brief Forward orientation, the default value is {1,0,0}, i.e., the forward orientation is in the positive direction of x-axis.
     */
    Orientation forward =  { 1, 0, 0 };
    /** 
     * @brief Rightward orientation, the default value is {0,1,0}, i.e., the rightward orientation is in the positive direction of y-axis.
     */
    Orientation right = { 0, 1, 0 };
    /** 
     * @brief Upward orientation, the default value is {0,0,1}, i.e., the upward orientation is in the positive direction of z-axis.
     */
    Orientation up = { 0, 0, 1 };
    /**
     * @hidden constructor/destructor
     */
    bool isEqual(const HumanOrientation& human_orientation) const {
        return forward.isEqual(human_orientation.forward) && right.isEqual(human_orientation.right) && up.isEqual(human_orientation.up);
    }
};

/** 
 * @type keytype
 * @brief Accessibility to volume setting
 */
enum AudioAbilityType {
    /** 
     * @brief Unknown
     */
    kAudioAbilityTypeUnknown = -1,
    /** 
     * @brief The volume setting is accessible.
     */
    kAudioAbilityAble = 0,
    /** 
     * @brief The volume setting is inaccessible.
     */
    kAudioAbilityUnable = 1,
};
/** 
 * @type keytype
 * @brief The alignment mode of remote audio streams
 */
enum AudioAlignmentMode {
    /** 
     * @brief Disabled
     */
    kAudioAlighmentModeOff,
    /** 
     * @brief All subscribed audio streams are aligned based on the process of the background music.
     */
    kAudioAlighmentModeAudioMixing,
};

/** 
 * @type keytype
 * @brief Audio device information
 */
struct AudioDeviceInfo {
    /** 
     * @brief Device ID
     */
    char device_id[MAX_DEVICE_ID_LENGTH];
    /** 
     * @brief Friendly name of the audio adapter to which the endpoint device is attached. For example, "Speakers (XYZ Audio Adapter)"
     */
    char device_name[MAX_DEVICE_ID_LENGTH];
    /** 
     * @brief Friendly name of the endpoint device. For example, "XYZ Audio Adapter"
     */
    char device_short_name[MAX_DEVICE_ID_LENGTH];
    /** 
     * @brief ID of the sound card connected to the audio device. With this variable, you can choose the speaker and microphone connected to the same sound card with ease.
     */
    char device_container_id[MAX_DEVICE_ID_LENGTH];
    /** 
     * @brief ID of the device vendor
     */
    int64_t device_vid;
    /** 
     * @brief Product number of the device
     */
    int64_t device_pid;
    /** 
     * @brief Connection type of the device
     */
    DeviceTransportType transport_type;
    /** 
     * @brief Accessibility to the volume setting
     */
    AudioAbilityType volume_settable;
    /** 
     * @brief Whether the device is the default device
     */
    bool is_system_default;
    /**
     * @hidden constructor/destructor
     */
    AudioDeviceInfo() {
        memset(device_id, 0, MAX_DEVICE_ID_LENGTH);
        memset(device_name, 0, MAX_DEVICE_ID_LENGTH);
        memset(device_short_name, 0, MAX_DEVICE_ID_LENGTH);
        memset(device_container_id, 0, MAX_DEVICE_ID_LENGTH);
        this->device_vid = 0;
        this->device_pid = 0;
        this->transport_type = DeviceTransportType::kDeviceTransportTypeUnknown;
        this->volume_settable = AudioAbilityType::kAudioAbilityTypeUnknown;
        this->is_system_default = false;
    }
    /**
     * @hidden constructor/destructor
     */
    AudioDeviceInfo& operator=(const AudioDeviceInfo& src) {
        if (this != &src) {
            strncpy(device_id, src.device_id, MAX_DEVICE_ID_LENGTH - 1);
            strncpy(device_name, src.device_name, MAX_DEVICE_ID_LENGTH - 1);
            strncpy(device_short_name, src.device_short_name, MAX_DEVICE_ID_LENGTH - 1);
            strncpy(device_container_id, src.device_container_id, MAX_DEVICE_ID_LENGTH - 1);
            device_id[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_name[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_short_name[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_container_id[MAX_DEVICE_ID_LENGTH - 1] = '\0';
            device_vid = src.device_vid;
            device_pid = src.device_pid;
            transport_type = src.transport_type;
            volume_settable = src.volume_settable;
            is_system_default = src.is_system_default;
        }

        return *this;
    }
};
/** 
 * @type keytype
 * @brief Standard pitch data
 */
struct StandardPitchInfo {
    /** 
     * @brief Starting time, unit: ms.
     */
    int start_time;
    /** 
     * @brief Duration, unit: ms.
     */
    int duration;
    /** 
     * @brief pitch.
     */
    int pitch;
};
/** 
 * @type keytype
 * @brief Karaoke scoring mode.
 */
enum MulDimSingScoringMode {
    /** 
     * @brief The score is provided based on the pitch.
     */
    kMulDimSingScoringModeNote = 0
};
/** 
 * @type keytype
 * @brief Karaoke scoring configuration.
 */
struct SingScoringConfig {
    /** 
     * @brief Sampling rate. Only 44,100 Hz and 48,000 Hz are supported.
     */
    AudioSampleRate sample_rate;
    /** 
     * @brief Scoring mode, see MulDimSingScoringMode{@link #MulDimSingScoringMode}.
     */
    MulDimSingScoringMode mode;
    /** 
     * @brief The file path of the lyrics. The scoring feature only supports KRC lyrics file.
     */
    const char* lyrics_filepath;
    /** 
     * @brief The path of the midi file.
     */
    const char* midi_filepath;
};
/** 
 * @type keytype
 * @brief Real-time scoring data.
 */
struct SingScoringRealtimeInfo {
    /** 
     * @brief Current playback position.
     */
    int current_position;
    /** 
     * @brief The user's pitch.
     */
    int user_pitch;
    /** 
     * @brief Standard pitch.
     */
    int standard_pitch;
    /** 
     * @brief Lyric index.
     */
    int sentence_index;
    /** 
     * @brief The score for the previous lyric.
     */
    int sentence_score;
    /** 
     * @brief The total score for the user's current performance.
     */
    int total_score;
    /** 
     * @brief The average score for the user's current performance.
     */
    int average_score;
};
/** 
 * @type keytype
 * @brief Audio file recording source type.
 */
enum AudioFrameSource {
    /** 
     * @brief The audio captured by the local microphone.
     */
    kAudioFrameSourceMic = 0,
    /** 
     * @brief The audio got by mixing all remote user's audio.
     */
    kAudioFrameSourcePlayback = 1,
    /** 
     * @brief The audio got by mixing the local captured audio and all remote user's audio.
     */
    kAudioFrameSourceMixed = 2,
};
/** 
 * @type keytype
 * @brief Audio quality.
 */
enum AudioQuality {
    /** 
     * @brief low quality
     */
    kAudioQualityLow = 0,
    /** 
     * @brief medium quality
     */
    kAudioQualityMedium = 1,
    /** 
     * @brief high quality
     */
    kAudioQualityHigh = 2,
    /** 
     * @brief ultra high quality
     */
    kAudioQualityUltraHigh = 3,
};
/** 
 * @type keytype
 * @brief Audio recording config
 */
struct AudioRecordingConfig {
    /**
     * @hidden Constructor/Destructor
     */
    AudioRecordingConfig() {
        absolute_file_name = nullptr;
        frame_source = kAudioFrameSourceMixed;
        sample_rate = kAudioSampleRateAuto;
        channel = kAudioChannelAuto;
        quality = kAudioQualityMedium;
    }
    /** 
     * @brief Absolute path of the recorded file, file name included. The App must have the write and read permission of the path.
     * @notes The files format is restricted to .aac and .wav.
     */
    const char* absolute_file_name;
    /** 
     * @brief The source of the recording. See AudioFrameSource{@link #AudioFrameSource}.
     */
    AudioFrameSource frame_source;
    /** 
     * @brief See AudioSampleRate{@link #AudioSampleRate}.
     */
    AudioSampleRate sample_rate;
    /** 
     * @brief Number of audio channels. See AudioChannel{@link #AudioChannel}.
     * @notes If number of audio channels of recording is different than that of audio capture, the behavior is: <br>
     *        + If the number of capture is 1, and the number of recording is 2, the recorded audio is two-channel data after copying mono-channel data. <br>
     *        + If the number of capture is 2, and the number of recording is 1, the recorded audio is recorded by mixing the audio of the two channels.
     */
    AudioChannel channel;
    /** 
     * @brief Recording quality. Only valid for .aac file. See AudioQuality{@link #AudioQuality}.
     * @notes When the sample rate is 32kHz, the file (10min) size for different qualities are: <br>
     *        + low: 1.2MB; <br>
     *        + medium: 2MB; <br>
     *        + high: 3.75MB; <br>
     *        + ultra high: 7.5MB.
     */
    AudioQuality quality;
};
/** 
 * @type keytype
 * @brief Audio recording config
 */
enum AudioRecordingState {
    /** 
     * @brief Recording exception
     */
    kAudioRecordingStateError = 0,
    /** 
     * @brief Recording in progress
     */
    kAudioRecordingStateProcessing = 1,
    /** 
     * @brief The recording task ends, and the file is saved.
     */    
    kAudioRecordingStateSuccess = 2,
};
/** 
 * @type errorcode
 * @brief Error code for audio recording.
 */
enum AudioRecordingErrorCode {
    /** 
     * @brief OK
     */
    kAudioRecordingErrorCodeOk = 0,
    /** 
     * @brief No file write permissions.
     */
    kAudioRecordingErrorCodeNoPermission = -1,
    /** 
     * @brief Not in the room.
     */
    kAudioRecordingErrorNotInRoom = -2,
    /** 
     * @brief Started.
     */
    kAudioRecordingAlreadyStarted = -3,
    /** 
     * @brief Not started.
     */
    kAudioRecordingNotStarted = -4,
    /** 
     * @brief Failure. Invalid file format.
     */
    kAudioRecordingErrorCodeNotSupport = -5,
    /** 
     * @brief Other error.
     */
    kAudioRecordingErrorCodeOther = -6,
};

}  // namespace bytertc
