/*
 *  Copyright (c) 2022 The ByteRtc project authors. All Rights Reserved.
 *  @company ByteDance.Inc
 *  @brief panoramic video interface
 */

#pragma once

namespace bytertc {
/** 
 * @hidden for internal use only on Windows and Android
 * @type keytype
 * @brief Head pose represented in quaternion.<br>
 * q = xi + yj + zk + w，i^2 = j^2 = k^2 = −1
 */
typedef struct Quaternionf {    
    /** 
     * @brief x-coordinate
     */
    float    x;
    /** 
     * @brief y-coordinate
     */
    float    y;
    /** 
     * @brief z-coordinate
     */
    float    z;
    /** 
     * @brief Angle of rotation
     */
    float    w;
} Quaternionf;

/** 
 * @hidden for internal use only on Windows and Android
 * @type api
 * @brief Panoramic video instance
 */
class IPanoramicVideo {
public:

    /** 
     * @type api
     * @brief Update the position of the head for the receiver. The receiver will receive the video tile according to the head position.
     * @param info Refer to Quaternionf{@link #Quaternionf} for details.
     */
    virtual int updateQuaternionf(const Quaternionf& info) = 0;

    ~IPanoramicVideo() {}
};
}
