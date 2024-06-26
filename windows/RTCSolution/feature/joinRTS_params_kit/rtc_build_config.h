#pragma once  

namespace vrd {

    /**
    * Required when using RTS as a business server
    * When the official service is launched, it needs to be deployed on its own server
    */
	static const std::string URL = "";

    /**
    * Required when using RTC
    * The unique identifier of each application is obtained through the application of the RTC console. 
    * The instances generated by different AppIds are completely independent in the audio and video calls in RTC and cannot communicate with each other.
    * https://console.byteplus.com/rtc/listRTC
    */
    static const std::string APPID = "";

    /**
    * When using RTC, AppKey is used to generate rtc token and authenticate rtc token
    * https://console.byteplus.com/rtc/listRTC
    */
    static const std::string APPKey= "";

    /**
    * It is required when using RTS, and is used for authentication when the server calls RTS openapi.
    * Used to confirm that the server has the ownership of this APPID
    * https://console.byteplus.com/iam/keymanage/
    */
    static const std::string AccessKeyID = "";

    /**
    * It is required when using RTS, and is used for authentication when the server calls RTS openapi.
    * Used to confirm that the server has the ownership of this APPID
    * https://console.byteplus.com/iam/keymanage/
    */
    static const std::string SecretAccessKey = "";
}