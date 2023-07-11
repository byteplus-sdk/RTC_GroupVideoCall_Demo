/*
 *  Copyright (c) 2022 The ByteRtc project authors. All Rights Reserved.
 *  @company ByteDance.Inc
 *  @brief spatial audio interface
 */

#pragma once

#include "bytertc_audio_defines.h"

namespace bytertc {
/** 
 * @type keytype
 * @brief Information on the user's position in the rectangular coordinate system for the spatial audio.
 */
struct PositionInfo {
    /** 
     * @brief 3D coordinate values of the user's position in the rectangular coordinate system for the spatial audio. You need to build your own rectangular coordinate system. Refer to Position{@link #Position} for details.
     */
    Position position;
    /** 
     * @brief Information on the three-dimensional orientation of the user in the rectangular coordinate system for the spatial audio. Any two of the 3D coordinate vectors of the user's position need to be perpendicular to each other. Refer to HumanOrientation{@link #HumanOrientation} for details.
     */
    HumanOrientation orientation;
};

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
    BYTERTC_DEPRECATED virtual int updatePosition(const Position& pos) = 0;

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
    BYTERTC_DEPRECATED virtual int updateSelfOrientation(const HumanOrientation& orientation) = 0;

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
    BYTERTC_DEPRECATED virtual int updateListenerPosition(const Position &pos) = 0;
    
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
    BYTERTC_DEPRECATED virtual int updateListenerOrientation(const HumanOrientation& orientation) = 0;
    /** 
     * @valid since 3.52.
     * @type api
     * @hidden(Linux)
     * @region Audio management
     * @brief Sets the coordinate and orientation of the local user as a listener in the rectangular coordinate system the local user built to achieve expected spatial audio effects. 
     * @param positionInfo  Information on the local user's position. Refer to  PositionInfo{@link #PositionInfo} for details. 
     * @return  <br>
     *        + 0: Success.  <br>
     *        + <0: Failure.  <br>
     *        + -2: Failure. The reason is that any two of the 3D coordinate vectors of your position are not perpendicular to each other. 
     * @notes <br>
     *        Call this API after joining the room. Before calling this API, you should call enableSpatialAudio{@link #ISpatialAudio#enableSpatialAudio} first to enable the spatial audio function. <br>
     *        The settings made locally will not influence other users' spatial audio experience.
     */
    virtual int updateSelfPosition(const PositionInfo& position_info) = 0;
    /** 
     * @valid since 3.52.
     * @type api
     * @hidden(Linux)
     * @region Audio management
     * @brief Sets the coordinate and orientation of the remote user as a speaker in the rectangular coordinate system of the local user. In this case, the local user hears from the remote user with the expected spatial audio effects.
     * @param uid User ID
     * @param positionInfo Information on the remote user's position. Refer to PositionInfo{@link #PositionInfo} for details. 
     * @return   <br>
     *        + 0: Success.  <br>
     *        + <0: Failure.  <br>
     *        + -2: Failure. The reason is that any two of the 3D coordinate vectors of the position of the remote user are not perpendicular to each other. 
     * @notes <br>
     *        Call this API after creating the room. <br>
     *        The settings made locally will not influence other users' spatial audio experience.
     */
    virtual int updateRemotePosition(const char* uid, const PositionInfo& position_info) = 0;
    /** 
     * @valid since 3.52.
     * @type api
     * @hidden(Linux)
     * @region Audio management
     * @brief Disables all spatial audio effects set by calling updateRemotePosition{@link #ISpatialAudio#updateRemotePosition} for a certain remote user.
     * @param uid User ID of the remote user.
     * @return  <br>
     *        + 0: Success.  <br>
     *        + <0: Failure.
     */
    virtual int removeRemotePosition(const char* uid) = 0;
    /** 
     * @valid since 3.52.
     * @type api
     * @hidden(Linux)
     * @region Audio management
     * @brief Disables all spatial audio effects set by calling updateRemotePosition{@link #ISpatialAudio#updateRemotePosition} for all remote users.
     * @return <br>
     *        + 0: Success.  <br>
     *        + <0: Failure.
     */
    virtual int removeAllRemotePosition() = 0;

    /** 
     * @hidden constructor/destructor
     * @brief  Destructor
     */
    virtual ~ISpatialAudio() = default;
};

}  // namespace bytertc
