/*
 *  Copyright (c) 2022 The ByteRtc project authors. All Rights Reserved.
 *  @company ByteDance.Inc
 *  @brief spatial audio interface
 */

#pragma once

#include "bytertc_audio_defines.h"

namespace bytertc {

/** 
 * @hidden(Linux)
 * @type api
 * @brief Spatial audio instance
 */
class ISpatialAudio {
public:
    /** 
     * @type api
     * @region Spatial Audio
     * @brief Enable/disable spatial audio function.  <br>
     * @param [in] enable Whether to enable spatial audio function:  <br>
     *        + true：Enable  <br>
     *        + false：Disable(Default setting)
     * @notes You need to call updatePosition{@link #ISpatialAudio#updatePosition} as well to really enjoy the spatial audio effect.
     */
    virtual void enableSpatialAudio(bool enable) = 0;

    /** 
     * @type api
     * @brief Updates the coordinate of the local user's position as a sound source in the rectangular coordinate system in the current room. <br>
     *        Without setting the position as a listener by calling updateListenerPosition{@link #ISpatialAudio#updateListenerPosition}, the position as a listener is the same as the position as a sound source.
     * @param [in] pos 3D coordinate values, the default value is [0, 0, 0], see Position{@link #Position}.
     * @return API call result: <br>
     *        + 0: Success  <br>
     *        + !0: Failure
     * @notes Before calling this API, you should call enableSpatialAudio{@link #ISpatialAudio#enableSpatialAudio} first to enable spatial audio function.
     */
    virtual int updatePosition(const Position& pos) = 0;

    /** 
     * @type api
     * @region Audio management
     * @brief After the local user joins the room, call this API to update the orientation of the local user as a sound source in the 3D coordinates for the spatial audio.  <br>
     *        Without setting the orientation as a listener by calling updateListenerOrientation{@link #ISpatialAudio#updateListenerOrientation}, the orientation as a listener is the same as the orientation as a sound source.
     * @param [in] orientation Refer to HumanOrientation{@link #HumanOrientation} for more information.
     * @return API call result:  <br>
     *        + 0: Success  <br>
     *        + !0: Failure
     * @notes  <br>
     *        +  Before calling this API, you should call enableSpatialAudio{@link #ISpatialAudio#enableSpatialAudio} first to enable spatial audio function, and then call UpdatePosition{@link #ISpatialAudio#updatePosition} to update the position of the local user.  <br>
     *        +  Call disableRemoteOrientation{@link #ISpatialAudio#disableRemoteOrientation} to disable the orientation effect as the sound source.
     */
    virtual int updateSelfOrientation(const HumanOrientation& orientation) = 0;

    /** 
     * @type api
     * @region Audio management
     * @brief Turn off the effect of the orientation of the local user as the sound source. <br>
     *        After the effect is off, all the other users in the room listen to the local user as if the local user is in right front of each of them.
     * @notes <br>
     *        + After the orientation effect as the sound source is disabled, you cannot enable it during the lifetime of the `SpatialAudio` instance. <br>
     *        + Calling this API does not affect the orientation effect of the local user as a listener. See updateSelfOrientation{@link #ISpatialAudio#updateSelfOrientation} and updateListenerOrientation{@link #ISpatialAudio#updateListenerOrientation}.
     */
    virtual void disableRemoteOrientation() = 0;
    
    /** 
     * @type api
     * @brief Updates the local user's position as a listener in the RTC room. <br>
     *        By calling this API, you can set the position of the local user as a listener different from the position as a sound source.
     * @param [in] pos 3D coordinate values. See Position{@link #Position}.<br>
     *                 If the API is not called, the position as a listener is the same as that set by calling updatePosition{@link #ISpatialAudio#updatePosition}.
     * @return <br>
     *        + 0: Success  <br>
     *        + !0: Failure
     * @notes <br>
     *        + Before calling this API, you should call enableSpatialAudio{@link #ISpatialAudio#enableSpatialAudio} first to enable the spatial audio function. <br>
     */
    virtual int updateListenerPosition(const Position &pos) = 0;
    
    /** 
     * @type api
     * @brief Updates the local user's orientation as a listener in the RTC room. <br>
     *        By calling this API, you can set the orientation of the local user as a listener different from the orientation as a sound source.
     * @param [in] orientation See HumanOrientation{@link #HumanOrientation}. <br>
     *                 If the API is not called, the orientation as a listener is the same as that set by calling updateSelfOrientation{@link #ISpatialAudio#updateSelfOrientation}.
     * @return API call result:  <br>
     *        + 0: Success  <br>
     *        + !0: Failure
     * @notes Before calling this API, you should call enableSpatialAudio{@link #ISpatialAudio#enableSpatialAudio} first to enable spatial audio function, and then call updatePosition{@link #ISpatialAudio#updatePosition} to update the position of the local user.  <br>
     */
    virtual int updateListenerOrientation(const HumanOrientation& orientation) = 0;

    /** 
     * @hidden constructor/destructor
     * @brief  Destructor
     */
    virtual ~ISpatialAudio() = default;
};

}  // namespace bytertc
