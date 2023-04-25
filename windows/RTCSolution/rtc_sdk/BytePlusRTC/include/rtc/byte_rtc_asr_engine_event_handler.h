//
// Created by shenpengliang on 2021/8/23.
//

#pragma once

namespace bytertc {
/** 
 * @hidden(Windows,Linux,macOS)
 * @type callback
 * @brief Speech recognition service usage status callback
 */
class IRTCASREngineEventHandler {
public:
    /**  
     * @hidden constructor/destructor
     * @brief Destructor
     */
    virtual ~IRTCASREngineEventHandler() {
    }
    /** 
     * @type callback
     * @brief  Speech recognition service starts successful callback
     */
    virtual void onSuccess() = 0;
    /** 
     * @type callback
     * @brief Callback of successful voice to text conversion. All the recognized text is returned through this callback.
     * @param [in] message Text message obtained after the completion of the recognition
     * @notes If the network connection is interrupted during the recognition process, the callback after the reconnection carries only the text message recognized after the reconnection.
     */
    virtual void onMessage(const char* message) = 0;
    /** 
     * @type callback
     * @brief This callback is triggered when an error event occurs within the speech recognition service.
     * @param  [in] error_code Error code <br>
     *         + < 0: parameter error or API call order error. See RTCASRErrorCode{@link #RTCASRErrorCode}; <br>
     *         + > 0: see [speech recognition service error code](https://docs.byteplus.com/en/speech/docs/real-time-speech-recog#bbbf89f6)  
     * @param  [in] error_message Detailed error message
     */
    virtual void onError(int error_code, const char* error_message) = 0;
};
}  // namespace bytertc
