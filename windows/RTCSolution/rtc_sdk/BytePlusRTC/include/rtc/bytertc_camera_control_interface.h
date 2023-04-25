/*
 * Copyright 2020 @bytedance
 *
 *  Created on: Dec 16, 2021
 */
#pragma once

#include <stdint.h>

namespace bytertc {
/** 
 * @type keytype
 * @brief Camera fill light status
 */
enum TorchState{

    /**  
     * @brief Camera fill light off
     */
    kTorchStateOff=0,
    /**  
     * @brief Camera fill light on
     */
    kTorchStateOn=1,
};

/** 
 * @hidden(macOS,Windows,Linux)
 * @type api
 * @region video management
 * @brief camera control interface
 * @notes must have been called StartVideoCapture{@link #IRTCVideo#StartVideoCapture} to use this interface when using the SDK internal capture module for video capture.
 */
class ICameraControl {
public:
    /** 
     * @type api
     * @region  video management
     * @brief set the zoom factor of the currently used camera (front/postcondition)
     * @param [in] zoom_val  zoom factor. The value range is [1, < Maximum Zoom Multiplier >]. <br>
     *                  The maximum zoom factor can be obtained by calling getCameraZoomMaxRatio{@link #getCameraZoomMaxRatio}.
     * @return   <br>
     *         + 0: Success. <br>
     *         + < 0: Failure.
     * @notes  <br>
     *         + You must have called StartVideoCapture {@link #IRTCVideo#StartVideoCapture} for video capture using the SDK internal capture module to set the camera zoom factor. <br>
     *         + The setting result fails after calling StopVideoCapture{@link #IRTCVideo#StopVideoCapture} to turn off internal collection.
     */
    virtual int setCameraZoomRatio(float zoom_val) = 0;

    /** 
     * @type api
     * @region Video Management
     * @brief Get the maximum zoom multiple of the currently used camera (front/postcondition)
     * @return Maximum zoom multiple
     * @notes You must have called StartVideoCapture {@link #IRTCVideo#StartVideoCapture} When using the SDK internal capture module for video capture, the camera maximum zoom multiple can be detected.
     */
    virtual float getCameraZoomMaxRatio() = 0;

    /** 
     * @type api
     * @region Video Management
     * @brief Detect whether the currently used camera (front/postcondition) supports zoom (digital/optical zoom).
     * @return   <br>
     *         + true: Support <br>
     *         + false: Not supported
     * @notes  You must have called StartVideoCapture{@link #IRTCVideo#StartVideoCapture} for video capture using the SDK internal capture module to detect camera zoom capability.
     */
    virtual bool isCameraZoomSupported() = 0;

    /** 
     * @type api
     * @region  video management
     * @brief  detect the currently used camera (front/postcondition), whether flash is supported.
     * @return   <br>
     *         + true: Support <br>
     *         + false: Not supported
     * @notes  You must have called StartVideoCapture{@link #IRTCVideo#StartVideoCapture} for video capture using the SDK internal capture module to detect flash capability.
     */
    virtual bool isCameraTorchSupported() = 0;

    /** 
     * @type api
     * @region video management
     * @brief turn on/off the flash of the currently used camera (front/postcondition)
     * @param [in] torch_state turn on/off. See TorchState{@link #TorchState}.
     * @return   <br>
     *         + 0: Success.
     *         + < 0: Failure.
     * @notes  <br>
     *         + You must have called StartVideoCapture{@link #IRTCVideo#StartVideoCapture} for video capture using the SDK internal capture module to set the flash. <br>
     *         + The setting result fails after calling StopVideoCapture{@link #IRTCVideo#StopVideoCapture} to turn off internal collection.
     */
    virtual int setCameraTorch(TorchState torch_state) = 0;

    /** 
     * @type api
     * @region video management
     * @brief Checks if the camera currently in use supports setting the focus point.
     * @return  <br>
     *        + true: Yes.
     *        + false: No.
     * @notes Call StartVideoCapture{@link #IRTCVideo#StartVideoCapture} before calling this API.
     */
    virtual bool isCameraFocusPositionSupported() = 0;
    
    /** 
     * @type api
     * @region video management
     * @brief Sets the focus point of the currently used camera.
     * @param [in] x The x-coordinate of the focus point in range of [0, 1], taking the upper-left corner of the canvas as the origin.
     * @param [in] y The y-coordinate of the focus point in range of [0, 1], taking the upper-left corner of the canvas as the origin.
     * @return  <br>
     *        + 0: Success.
     *        + < 0: Failure
     * @notes <br>
     *        + Call StartVideoCapture{@link #IRTCVideo#StartVideoCapture} before calling this API.
     *        + When the focus point is set to the center of the canvas (i.e., [0.5, 0.5]), the system default value is restored.
     *        + The setting expires after calling StopVideoCapture{@link #IRTCVideo#StopVideoCapture} to disable the internal capture.
     */
    virtual int setCameraFocusPosition(float x, float y) = 0;
    
    /** 
     * @type api
     * @region video management
     * @brief Checks if the camera currently in use supports setting the exposure point.
     * @return  <br>
     *        + true: Yes.
     *        + false: No.
     * @notes Call StartVideoCapture{@link #IRTCVideo#StartVideoCapture} before calling this API.
     */
    virtual bool isCameraExposurePositionSupported() = 0;
    
    /** 
     * @type api
     * @region video management
     * @brief Sets the exposure point of the currently used camera.
     * @param [in] x The x-coordinate of the exposure point in range of [0, 1], taking the upper-left corner of the canvas as the origin.
     * @param [in] y The y-coordinate of the exposure point in range of [0, 1], taking the upper-left corner of the canvas as the origin.
     * @return  <br>
     *        + 0: Success.
     *        + < 0: Failure.
     * @notes <br>
     *        + Call StartVideoCapture{@link #IRTCVideo#StartVideoCapture} before calling this API.
     *        + When the exposure point is set to the center of the canvas (i.e., [0.5, 0.5]), the system default value is restored.
     *        + The setting expires after calling StopVideoCapture{@link #IRTCVideo#StopVideoCapture}.
     */
    virtual int setCameraExposurePosition(float x, float y) = 0;
    
    /** 
     * @type api
     * @region video management
     * @brief Sets the exposure compensation for the currently used camera.
     * @param [in] val Exposure compensation in range of [-1, 1]. Default to 0, which means no exposure compensation.
     * @return  <br>
     *        + 0: Success.
     *        + < 0: Failure.
     * @notes <br>
     *        + Call StartVideoCapture{@link #IRTCVideo#StartVideoCapture} before calling this API.
     *        + The setting expires after calling StopVideoCapture{@link #IRTCVideo#StopVideoCapture}.
     */
    virtual int setCameraExposureCompensation(float val) = 0;
};

}  // namespace bytertc
