/*
 *  Copyright (c) 2022 The ByteRtc project authors. All Rights Reserved.
 *  @company ByteDance.Inc
 *  @brief range audio interface
 */

#pragma once

#include "bytertc_audio_defines.h"

namespace bytertc {
/** 
 * @type keytype
 * @brief The range where local user can hear audio with attenuation effect.
 */
struct ReceiveRange {
    /** 
     * @brief The minimum distance at which the local user can hear the attenuated audio from remote users.  <br>
     *        The value must be ≥ 0, but ≤ max.  <br>
     *        No attenuation effect for audio from distances less than this value you set.
     */
    int min;
    /** 
     * @brief The maximum distance at which the local user can hear audio from remote users.  <br>
     *        The value must be ≥ min, and > 0.  <br>
     *        Audio from distances larger than the value you set cannot be heard.
     */
    int max;
};

/** 
 * @type keytype
 * @brief Volume Roll-off modes that a sound has in an audio source
 */
enum AttenuationType {
    /** 
     * @brief Disable Volume Attenuation
     */
    kAttenuationTypeNone = 0,
    /** 
     * @brief Linear roll-off mode which lowers the volume of the sound over the distance
     */
    kAttenuationTypeLinear = 1,
    /** 
     * @brief Exponential roll-off mode which exponentially decreases the volume of the sound with the distance raising
     */
    kAttenuationTypeExponential = 2,
};

/** 
 * @hidden(Linux)
 * @type api
 * @brief Range audio instance
 */
class IRangeAudio {
public:
    /** 
     * @type api
     * @region Range Audio
     * @brief Enable/disable the range audio function.  <br>
     *        Range audio means that within a certain range in a same RTC room, the audio volume of the remote user received by the local user will be amplified/attenuated as the remote user moves closer/away. The audio coming from out of the range cannot be heard. See updateReceiveRange{@link #IRangeAudio#updateReceiveRange} to set audio receiving range.
     * @param [in] enable Whether to enable audio range function：  <br>
     *        + true: Enable  <br>
     *        + false: Disable（Defaulting setting）
     * @notes You can call this API anytime before or after entering a room. To ensure a smooth switch to the range audio mode after entering the room, you need to call updatePosition{@link #IRangeAudio#updatePosition} before this API to set your own position coordinates, and then enable the range audio function.
     */
    virtual void enableRangeAudio(bool enable) = 0;
    /** 
     * @type api
     * @region Range Audio
     * @brief Updates the audio receiving range for the local user.
     * @param [in] range Audio receiving range, see ReceiveRange{@link #ReceiveRange}.
     * @return API call result:  <br>
     *        + 0: Success  <br>
     *        + !0: Failure
     */
    virtual int updateReceiveRange(const ReceiveRange &range) = 0;
    /** 
     * @type api
     * @region Range Audio
     * @brief Updates the coordinate of the local user's position in the rectangular coordinate system in the current room.
     * @param [in] pos 3D coordinate values, the default value is [0, 0, 0], see [Position](70098#position-2).
     * @return API call result: <br>
     *        + 0: Success  <br>
     *        + !0: Failure
     * @notes  <br>
     *        + After calling this API, you should call enableRangeAudio{@link #IRangeAudio#enableRangeAudio} to enable range audio function to actually enjoy the range audio effect.  <br>
     */
    virtual int updatePosition(const Position &pos) = 0;

    /** 
     * @type api
     * @region Range Audio
     * @brief Set the volume roll-off mode that a 3D sound has in an audio source when using the Range Audio feature.<br>
     * @param type Volume roll-off mode. It is linear roll-off mode by default. Refer to AttenuationType{@link #AttenuationType} for more details.
     * @param coefficient Coefficient for the exponential roll-off mode. The default value is 1. It ranges [0.1,100]. We recommended to set it to `50`. The volume roll-off speed gets faster as this value increases.
     * @return Result of the call<br>
     *         + `0`: Success<br>
     *         + `-1`: Failure because of calling this API before the user has joined a room or before enabling the Range Audio feature by calling enableRangeAudio{@link #IRangeAudio#enableRangeAudio}.
     * @notes Call updateReceiveRange{@link #IRangeAudio#updateReceiveRange} to set the range outside which the volume of the sound does not attenuate.
     */
    virtual int setAttenuationModel(AttenuationType type, float coefficient) = 0;

    /** 
     * @type api
     * @region Range Audio
     * @brief Set the flags to mark the user groups, within which the users talk without attenuation. <br>
     *        In the RTC room, if the flags of the users intersects with each other, the users talk without attenuation. <br>
     *        For example, the user is a member of multiple teams, and teammates of the same team talks without attentuation. You can set the flag for each team, and includes the flags of the user's teams in the user's flags. 
     * @param flags Array of flags.
     */
    virtual void setNoAttenuationFlags(const char** flags, int len) = 0;

    /**
     * @hidden constructor/destructor
     */
    virtual ~IRangeAudio() {}
};
}  // namespace bytertc
