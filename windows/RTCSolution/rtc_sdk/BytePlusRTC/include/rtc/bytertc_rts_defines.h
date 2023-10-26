/*
 * Copyright (c) 2022 The BytePlusRTC project authors. All Rights Reserved.
 * @brief BytePlusRTC Common Defines
 */

#pragma once

#ifdef WIN32
#define BYTERTC_API extern "C" __declspec(dllexport)
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#if TARGET_OS_MAC && !(TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR)
#define BYTERTC_API __attribute__((__visibility__("default"))) extern "C"
#else
#define BYTERTC_API __attribute__((__visibility__("default")))
#endif
#else
#define BYTERTC_API __attribute__((__visibility__("default")))
#endif

#ifdef __GNUC__
#    define GCC_VERSION_AT_LEAST(x,y) (__GNUC__ > (x) || __GNUC__ == (x) && __GNUC_MINOR__ >= (y))
#else
#    define GCC_VERSION_AT_LEAST(x,y) 0
#endif

#if GCC_VERSION_AT_LEAST(3,1)
#    define BYTERTC_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#    define BYTERTC_DEPRECATED __declspec(deprecated)
#else
#    define BYTERTC_DEPRECATED
#endif

#include <stdint.h>
#include <stddef.h>

namespace bytertc {


/**
 * @hidden currently not available
 */
enum RtsReturnStatus {

    kRtsReturnStatusSuccess = 0,

    kRtsReturnStatusFailure = -1,

    kRtsReturnStatusParameterErr = -2,

    kRtsReturnStatusWrongState = -3,

    kRtsReturnStatusHasInRoom = -4,

    kRtsReturnStatusHasInLogin = -5,

    kRtsReturnStatusRoomIdInUse = -8,

};

/** 
 * @type keytype
 * @brief  The reason why the user left the room.   <br>
 */
enum UserOfflineReason {
    /** 
     * @brief The remote user calls `leaveRoom` to exit the room. <br>
     */
    kUserOfflineReasonQuit = 0,
    /** 
     * @brief The remote user is offline.   <br>
     */
    kUserOfflineReasonDropped = 1,
    /** 
     * @brief The remote user switches to invisible.   <br>
     */
    kUserOfflineReasonSwitchToInvisible = 2,
    /** 
     * @brief The remote user is kicked out of the room.
     *        Is kicked out of the room due to calling the kicked out user's OpenAPI.
     */
    kUserOfflineReasonKickedByAdmin = 3,
};

/** 
 * @type keytype
 * @brief The type of joining the room.   <br>
 */
enum JoinRoomType {
    /** 
     * @brief Join the room for the first time. The user calls `joinRoom` to join the room. <br>
     */
    kJoinRoomTypeFirst = 0,
    /** 
     * @brief Rejoin the room. The user loses connection to the server due to poor network, and rejoins the room successfully.   <br>
     */
    kJoinRoomTypeReconnected = 1,
};

/** 
 * @type keytype
 * @brief  The type of user login
 */
enum LoginType {
    /** 
     * @brief login for the first time. The user manually called `login` and received a successful login.
     */
    kLoginTypeFirst = 0,
    /** 
     * @brief Reconnect. User network is poor, lost connection with the server, SDK  automatically reconnect login success.
     */
    kLoginTypeReconnected = 1,
};

/** 
 * @type errorcode
 * @brief login result   <br>
 *        The result of calling `login` will notify the user through the `onLoginResult` callback.
 */
enum LoginErrorCode {
    /** 
     * @brief Call the `login` method to log in successfully
     */
    kLoginErrorCodeSuccess = 0,
    /** 
     * @brief The Token used when calling the `login` method is invalid or expired, requiring the user to reacquire the Token.
     */
    kLoginErrorCodeInvalidToken = -1000,
    /** 
     * @brief login error   <br>
     *        An unknown error occurred when calling the `login` method, which caused the login to fail and requires re-login.
     */
    kLoginErrorCodeLoginFailed = -1001,
    /** 
     * @brief There was a problem with the user ID passed in when the `login` method was called.
     */
    kLoginErrorCodeInvalidUserId = -1002,
    /** 
     * @brief An error occurred at the server level when calling `login` to log in.
     */
    kLoginErrorCodeServerError = -1003,
};

/** 
 * @type keytype
 * @brief The reason why users log out
 */
enum LogoutReason {
    /** 
     * @brief Users choose to logout by calling the `logout` to log out or terminating the engine to log out.
     */
    kLogoutReasonLogout = 0,
    /** 
     * @brief The current user is kicked out as another user logs in with the same UserId.
     */
    kLogoutReasonDuplicateLogin = 1,
};

/** 
 * @type keytype
 * @brief User online status.
 */
enum UserOnlineStatus {
    /** 
     * @brief The peer user is offline   <br>
     *         The peer user has called `logout`, or has not called `login` to log in.
     */
    kUserOnlineStatusOffline = 0,
    /** 
     * @brief The peer user is online   <br>
     *         The peer user calls `login` to log in, and the connection status is normal.
     */
    kUserOnlineStatusOnline = 1,
    /** 
     * @brief Unable to get the online state of the peer user   <br>
     *        Returned when a cascade error occurs and the online state of the peer user is abnormal.
     */
    kUserOnlineStatusUnreachable = 2,
};

/** 
 * @type keytype
 * @brief  Server message send result content
 */
struct ServerACKMsg {
    /** 
     * @brief length
     */
    int length;
    /** 
     * @brief content of the message
     */
    char* ack_msg;
};

/** 
 * @type keytype
 * @brief Message type
 */
enum MessageConfig {
    /** 
     * @brief Low latency reliable and orderly message
     */
    kMessageConfigReliableOrdered = 0,
    /** 
     * @brief Ultra-low latency ordered message
     */
    kMessageConfigUnreliableOrdered = 1,
    /** 
     * @brief Ultra-low latency unordered message
     */
    kMessageConfigUnreliableUnordered = 2
};

/** 
 * @type errorcode
 * @brief Result of sending messages and the reason of failure if it fails.
 */
enum UserMessageSendResult {
    /** 
     * @brief The message has been sent successfully.
     */
    kUserMessageSendResultSuccess = 0,
    /** 
     * @brief Failure. Sending timeout.
     */
    kUserMessageSendResultTimeout = 1,
    /** 
     * @brief Failure. Channel disconnected.
     */
    kUserMessageSendResultNetworkDisconnected = 2,
    /** 
     * @brief Failure. Recipient not found.
     */
    kUserMessageSendResultNoReceiver = 3,
    /** 
     * @brief Failure. The remote user did not logged in or joining the room.
     */
    kUserMessageSendResultNoRelayPath = 4,
    /** 
     * @brief Failure. Exceeds QPS limit.
     */
    kUserMessageSendResultExceedQPS = 5,
    /** 
     * @brief Failure. The app server failed to receive the message sent by the client.<br>
     *        Triggered by calling `sendServerMessage` or `sendServerBinaryMessage`, and carried by `onServerMessageSendResult` callback.
     */
    kUserMessageSendResultE2BSSendFailed = 17,
    /** 
     * @brief Failure. The app server received the message sent by the client, but failed to response.<br>
     *        Triggered by calling `sendServerMessage` or `sendServerBinaryMessage`, and carried by `onServerMessageSendResult` callback.
     */
    kUserMessageSendResultE2BSReturnFailed = 18,
    /** 
     * @brief Failure. The sender of the message did not join the room
     */
    kUserMessageSendResultNotJoin = 100,
    /** 
     * @brief Failure. Connection initialization not completed
     */
    kUserMessageSendResultInit = 101,
    /** 
     * @brief Failure. No data transmission channel connection available
     */
    kUserMessageSendResultNoConnection = 102,
    /** 
     * @brief Failure. Message exceeds the range of the permitted size, 64 KB.
     */
    kUserMessageSendResultExceedMaxLength = 103,
    /** 
     * @brief Failure. The id of the recipient is empty
     */
    kUserMessageSendResultEmptyUser = 104,
    /** 
     * @brief Failure. The sender of the message did not log in.
     */
    kUserMessageSendResultNotLogin = 105,
    /** 
     * @brief Failure. No parameters were set before sending the message to the server
     */
    kUserMessageSendResultServerParamsNotSet = 106,
    /** 
     * @brief Failure. Unknown error
     */
    kUserMessageSendResultUnknown = 1000,
};

/** 
 * @type keytype
 * @brief Result of broadcasting messages in the room
 */
enum RoomMessageSendResult {
    /** 
     * @brief Message sent successfully.
     */
    kRoomMessageSendResultSuccess = 200,
    /** 
     * @brief Failure. Sending timeout.
     */
    kRoomMessageSendResultTimeout = 1,
    /** 
     * @brief Failure. Channel disconnected.
     */
    kRoomMessageSendResultNetworkDisconnected = 2,
    /** 
     * @brief Failure. Exceeds QPS limit.
     */
    kRoomMessageSendResultExceedQPS = 5,
    /** 
     * @brief Failure. The sender has not joined a room.
     */
    kRoomMessageSendResultNotJoin = 100,
    /** 
     * @brief Failure. No data transmission channel connection available
     */
    kRoomMessageSendResultNoConnection = 102,
    /** 
     * @brief Failure. Message exceeds the range of the permitted size, 64 KB.
     */
    kRoomMessageSendResultExceedMaxLength = 103,
    /** 
     * @brief Failure. Unknown error
     */
    kRoomMessageSendResultUnknown = 1000,
};

/** 
 * @type keytype
 * @brief SDK  Connection status with the signaling server.
 */
enum ConnectionState {
    /** 
     * @brief Disconnected for 12s, SDK will try to reconnect automatically.
     */
    kConnectionStateDisconnected = 1,
    /** 
     * @brief The first request to connect to the server. Connecting.
     */
    kConnectionStateConnecting = 2,
    /** 
     * @brief The first connection was successful.
     */
    kConnectionStateConnected = 3,
    /** 
     * @brief The code includes the following status: <br>
     *        + The first connection is not successful for 10s. <br>
     *        + The connection has been lost for 10s. Auto reconnecting.
     */
    kConnectionStateReconnecting = 4,
    /** 
     * @brief Successful reconnection after disconnection.
     */
    kConnectionStateReconnected = 5,
    /** 
     * @brief In status `kConnectionStateDisconnected` for more than 10s without a successful reconnection. SDK will still continue to try to reconnect.
     */
    kConnectionStateLost = 6,
    /** 
     * @brief Connection failure caused by abnormal state of the server. SDK will not automatically reconnect, please re-enter the room or contact technical support.
     */
    kConnectionStateFailed = 7,
};

/** 
 * @type keytype
 * @brief Media streaming network quality.
 */
enum NetworkQuality {
    /** 
     * @brief Network quality unknown.
     */
    kNetworkQualityUnknown = 0,
    /** 
     * @brief The network quality is excellent.
     */
    kNetworkQualityExcellent,
    /** 
     * @brief The subjective feeling is similar to kNetworkQualityExcellent, but the bit rate may be slightly lower.
     */
    kNetworkQualityGood,
    /** 
     * @brief Subjective feelings are flawed but do not affect communication.
     */
    kNetworkQualityPoor,
    /** 
     * @brief Can barely communicate but not smoothly.
     */
    kNetworkQualityBad,
    /** 
     * @brief The quality of the network is very poor and communication is basically impossible.
     */
    kNetworkQualityVbad,
    /** 
     * @brief The network is down. It may be down due to no answer within 12s, airplane mode on, disconnected cable, etc.
     *        Refer to [Get connection state](https://docs.byteplus.com/byteplus-rtc/docs/95376) for more details about getting connection state.
     */
    kNetworkQualityDown,
};

/** 
 * @type keytype
 * @brief SDK  Network connection type.
 */
enum NetworkType {
    /** 
     * @brief Network connection type unknown.
     */
    kNetworkTypeUnknown = -1,
    /** 
     * @brief The network connection has been disconnected.
     */
    kNetworkTypeDisconnected = 0,
    /** 
     * @brief The network connection type is LAN.
     */
    kNetworkTypeLAN = 1,
    /** 
     * @brief The network connection type is Wi-Fi (including hotspots).
     */
    kNetworkTypeWIFI = 2,
    /** 
     * @brief The network connection type is 2G mobile network.
     */
    kNetworkTypeMobile2G = 3,
    /** 
     * @brief The network connection type is 3G mobile network.
     */
    kNetworkTypeMobile3G = 4,
    /** 
     * @brief The network connection type is 4G mobile network.
     */
    kNetworkTypeMobile4G = 5,
    /** 
     * @brief The network connection type is 5G mobile network.
     */
    kNetworkTypeMobile5G = 6,
};

/** 
 * @type errorcode
 * @brief Callback error code.   <br>
 *        When an unrecoverable error is encountered inside the SDK, the user is notified via the `onError` callback.
 */
enum RtsErrorCode {
    /** 
     * @brief Token  is invalid.
     *        The Token used when joining the room is invalid or expired. The user is required to retrieve the token and call the `updateToken` to update the token.
     */
    kRtsErrorCodeInvalidToken = -1000,
    /** 
     * @brief Join room error.
     *        An unknown error occurred while joining the room, which caused the joining room to fail. Users are required to rejoin the room.
     */
    kRtsErrorCodeJoinRoom = -1001,
    /** 
     * @brief The user has been removed from the room because the same user joined the room on the other client.
     */
    kRtsErrorCodeDuplicateLogin = -1004,
    /** 
     * @brief The user has been removed from the room by the administrator via a OpenAPI call.
     */
    kRtsErrorCodeKickedOut = -1006,
    /** 
     * @brief When calling `createRTCRoom`, if the roomid is illegal, it will return null and throw the error
     */
    kRtsRoomErrorCodeRoomIdIllegal = -1007,
    /** 
     * @brief Token expired. Call `joinRoom` to rejoin with a valid Token.
     */
    kRtsRoomErrorTokenExpired = -1009,
    /** 
     * @brief The Token you provided when calling `updateToken` is invalid.
     */
    kRtsRoomErrorUpdateTokenWithInvalidToken = -1010,
    /** 
     * @brief Users have been removed from the room because the administrator dismissed the room by calling OpenAPI.
     */
    kRtsErrorCodeRoomDismiss = -1011,
    /** 
     * @brief there is a room with the same roomId，whose room id is the same with echo test
     */
    kRtsErrorCodeRoomAlreadyExist = -1013,
    /** 
     * @brief The user name set by the same engine instance when entering multiple rooms must be the same
     */
    kRtsErrorCodeUserIDDifferent = -1014,
    /** 
     * @brief The user has been removed from the room due to the abnormal status of server. <br>
     *        SDK  is disconnected with the signaling server. It will not reconnect automatically. Please contact technical support.<br>
     */
    kRtsErrorCodeAbnormalServerStatus = -1084,
    /**
     * @hidden for internal use only
     */
    kRtsErrorCodeInternalDeadLockNotify = -1111,
};

/** 
 * @type errorcode
 * @brief Callback warning code. The warning code indicates that there is a problem within the SDK and is trying to recover. Warning codes only serve as notifications.
 */
enum RtsWarningCode {
    /** 
     * @deprecated since 3.46 and will be deleted in 3.51.
     */
    kRtsWarningCodeGetRoomFailed = -2000,
    /** 
     * @brief Failed to enter the room.   <br>
     *        When you call the first time to join the room or disconnect and reconnect due to poor network conditions, the room entry fails due to a server error. The SDK automatically retries the room.
     */
    kRtsWarningCodeJoinRoomFailed = -2001,
    /** 
     * @brief Release audio & video stream failed.   <br>
     *        When you publish audio & video streams in your room, the publication fails due to a server error. The SDK automatically retries the release.
     */
    kRtsWarningCodePublishStreamFailed = -2002,
    /** 
     * @hidden currently not available
     * @brief  The function call order is wrong.
     */
    kRtsWarningCodeInvokeError = -2005,
    /** 
     * @hidden for internal use only
     * @brief Scheduling exception, the media server address returned by the server is unavailable.
     */
    kRtsWarningCodeInvalidExpectMediaServerAddress = -2007,
    /** 
     * @brief Sending a custom broadcast message failed, you are not currently in the room.
     */
    kRtsWarningCodeSendCustomMessage = -2011,
    /** 
     * @brief When the number of people in the room exceeds 500, stop sending `onUserJoined` and `onUserLeave` callbacks to existing users in the room, and prompt all users in the room via broadcast.
     */
    kRtsWarningCodeUserNotifyStop = -2013,


    /** 
     * @brief The old room has been replaced by new room with the same roomId
     */
    kRtsWarningCodeOldRoomBeenReplaced = -2016
};

/** 
 * @type keytype
 * @brief  Business check code   <br>
 *         The error code that the API `setBusinessId` sends back. <br>
 */
enum BusinessCheckCode {
    /** 
     * @brief The user is already in the room.   <br>
     *        You must set the Business Id before joining room, otherwise it will be invalid. <br>
     */
    kBusinessCheckCodeAlreadyInRoom = -6001,

    /** 
     * @brief  The input is invalid.   <br>
     *         For the legal characters, see `setBusinessId`. <br>
     */
    kBusinessCheckCodeInputInvalidate = -6002,
};

/** 
 * @type keytype
 * @brief The CPU and memory information used by App. <br>
 *        You will periodically(2s) receive `onSysStats` of the information.
 */
struct SysStats {
    /** 
     * @brief The CPU cores of your system.
     */
    unsigned int cpu_cores;
    /** 
     * @brief The CPU usage rate of the current application. The value range is [0,1].
     */
    double cpu_app_usage;
    /** 
     * @hidden currently not available
     * @brief The cpu usage rate of the current system, the value range is [0,1].
     */
    double cpu_total_usage;
    /** 
     * @brief The memory usage of the current application(/MB).
     */
    double memory_usage;
    /** 
     * @brief Total memory(/MB).
     */
    unsigned long long full_memory;
    /** 
     * @brief Used system memory(/MB).
     */
    unsigned long long total_memory_usage;
    /** 
     * @brief Free system memory(/MB).
     */
    unsigned long long free_memory;
    /** 
     * @brief The memory usage rate of the current application(/%).
     */
    double memory_ratio;
    /** 
     * @brief The system memory usage rate(/%).
     */
    double total_memory_ratio;
};

/** 
 * @type keytype
 * @brief HTTP/HTTPS  proxy status
 */
enum HttpProxyState {
    /** 
     * @brief HTTP/HTTPS  initialization state
     */
    kHttpProxyStateInit,
    /** 
     * @brief HTTP/HTTPS  Connection successful
     */
    kHttpProxyStateConnected,
    /** 
     * @brief HTTP/HTTPS  connection failed
     */
    kHttpProxyStateError
};

/** 
 * @type keytype
 * @brief SOCKS5  proxy connection status
 */
enum Socks5ProxyState {
    /** 
     * @brief SOCKS5  connection initialization
     */
    kSocks5Init,
    /** 
     * @brief SOCKS5  Connection successful
     */
    kSocks5Connected,
    /** 
     * @brief SOCKS5  connection error
     */
    kSocks5Error,
    /** 
     * @brief TCP  connection failed
     */
    kSocks5TcpConnectFail,
    /** 
     * @brief TCP  connection closed
     */
    kSocks5TcpClose,
    /** 
     * @brief SOCKS  protocol TCP negotiation phase failed
     */
    kSocks5ProtocolTcpFail,
    /** 
     * @brief UDP  connection failed
     */
    kSocks5ProtocolUdpFail,
    /** 
     * @brief Authorization failed
     */
    kSocks5AuthFail,
    /** 
     * @brief Unknown state
     */
    kSocks5Unknown
};

/** 
 * @type keytype
 * @brief Cloud Proxy Information
 */
struct CloudProxyInfo {
    /** 
     * @type keytype
     * @brief Cloud proxy IP
     */
    const char* cloud_proxy_ip;
    /** 
     * @type keytype
     * @brief Cloud Proxy Port
     */
    int cloud_proxy_port;
};

/** 
 * @type keytype
 * @brief Configuration of Cloud Proxy
 */
struct CloudProxyConfiguration {
    /** 
     * @brief Information of cloud proxy.<br>
     */
    CloudProxyInfo* cloud_proxies = nullptr;
    /** 
     * @brief Number of cloud proxy.<br>
     */
    int cloud_proxy_count = 0;
};

/** 
 * @type keytype
 * @brief The maximum length of an ID.
 */
const unsigned int MAX_DEVICE_ID_LENGTH = 512;

/** 
 * @type keytype
 * @brief Types of local proxies.
 */
enum LocalProxyType {
    /** 
     * @brief Socks5 proxy. If you chose Socks5 as the local proxy, you need to make sure all requirements listed in the Socks5 document are satisfied. 
     */
    kLocalProxyTypeSocks5 = 1,
    /** 
     * @brief Http tunnel proxy.
     */
    kLocalProxyTypeHttpTunnel = 2
};

/** 
 * @type keytype
 * @brief Detailed information of local proxy configurations.
 */
struct LocalProxyConfiguration {
    /** 
     * @brief Types of local proxies. Refer to LocalProxyType{@link #LocalProxyType} for details.
     */
    LocalProxyType local_proxy_type;
    /** 
     * @type keytype
     * @brief Local proxy IP.
     */
    const char* local_proxy_ip;
    /** 
     * @type keytype
     * @brief Local proxy port.
     */
    int local_proxy_port;
    /** 
     * @type keytype
     * @brief The username of the local proxy.
     */
    const char* local_proxy_username;
    /** 
     * @type keytype
     * @brief The password of the local proxy.
     */
    const char* local_proxy_password;
};

/** 
 * @type keytype
 * @brief The states of local proxy connection.
 */
enum LocalProxyState {
    /** 
     * @brief TCP proxy server is connected.
     */
    kLocalProxyStateInited = 0,

    /** 
     * @brief The local proxy is connected.
     */
    kLocalProxyStateConnected = 1,

    /** 
     * @brief Errors occurred when connecting to the local proxy.  
     */
    kLocalProxyStateError = 2,
};

/** 
 * @type keytype
 * @brief The errors of local proxy connection.
 */
enum LocalProxyError {
    /** 
     * @brief There are no errors in local proxies. 
     */
    kLocalProxyErrorOK = 0,

    /** 
     * @brief The connection to Socks5 proxy failed because the proxy server replies wrong version numbers which don't accord with the Socks5 document. Please check the proxy server. 
     */
    kLocalProxyErrorSocks5VersionError = 1,

    /** 
     * @brief The connection to Socks5 proxy failed because the format of the proxy's replies doesn't accord with the Socks5 document. Please check the proxy server. 
     */
    kLocalProxyErrorSocks5FormatError = 2,

    /** 
     * @brief The connection to Socks5 proxy failed because the proxy replies wrong information which doesn't accord with the Socks5 document. Please check the proxy server. 
     */
    kLocalProxyErrorSocks5InvalidValue = 3,

    /** 
     * @brief The connection to Socks5 proxy failed because the username and password of the local proxy are not provided. Please call `setLocalProxy` and enter your username and password. 
     */
    kLocalProxyErrorSocks5UserPassNotGiven = 4,

    /** 
     * @brief The connection to Socks5 proxy failed because TCP is closed. Please check the proxy server and your network connection status.
     */
    kLocalProxyErrorSocks5TcpClosed = 5,

    /** 
     * @brief Errors in Http tunnel proxy. Please check Http tunnel proxy and your network connection status.
     */
    kLocalProxyErrorHttpTunnelFailed = 6,
};

/**  
 * @type keytype
 * @brief Local log parameters.
 */
enum class LocalLogLevel {
    /** 
     * @brief Info level.
     */
    kInfo     = 0,
    /** 
     * @brief (Default) Warning level.
     */
    kWarning  = 1,
    /** 
     * @brief Error level.
     */
    kError    = 2,
    /** 
     * @brief Turn off logging.
     */
    kNone     = 3
};
 
/** 
 * @type keytype
 * @brief Local log parameters.
 */
struct LogConfig {
    /** 
     * @brief (Required) Local log directory.
     */
    const char* log_path = nullptr;
    /** 
     * @brief (Optional) The logging level. See LocalLogLevel{@link #LocalLogLevel}. The default is warning level.
     */
    LocalLogLevel log_level = LocalLogLevel::kWarning;

    /** 
     * @brief (Optional) The limits for total log file size in MB. The range is 1 to 100 MB, and the default value is 10 MB.
     *        If `log_file_size` < 1, it will be set to 1 MB. If `log_file_size` > 100, it will be set to 100 MB. <br>
     *        The maximum size for a single log file is 2 MB.
     *        If 1 ≤ `log_file_size` ≤ 2, one log file will be generated. If `log_file_size` > 2, the first `⌊log_file_size/2⌋` files will be filled with 2 MB each, the `⌊log_file_size/2⌋+1` fill will be filled with `log_file_size mod 2`.
     *        If the size exceeds the remaining space, the oldest file will be deleted.
     */
    uint32_t log_file_size = 10;

    /** 
     * @brief (Optional) Local log file name prefix. It should follow the regular expression pattern of `[a-zA-Z0-9_@-.]{1,128}`.
     *        The final file name will be the prefix followed by "_" and the file creation time, and "_rtclog.log" at the end. For example, `logPrefix_2023-05-25_172324_rtclog.log`.
     */
    const char* log_filename_prefix = nullptr;
};

}  // namespace bytertc
