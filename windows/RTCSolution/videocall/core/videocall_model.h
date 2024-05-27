#pragma once
#include <unordered_map>
#include <vector>
#include <string>

namespace videocall {
    struct VideoResolution {
        int width = 640;
        int height = 360;
    };

    enum AudioQuality {
        kAudioQualityFluent = 0,
        kAudioQualityStandard = 1,
        kAudioQualityHD = 2,
    };

    struct VideoConfiger {
        VideoResolution resolution;
        int fps = 15;
        int kbps = 64;
    };

    struct VideoCallSettingModel {
        VideoConfiger camera{ {1280, 720}, 15, 500 };
        AudioQuality audio_quality{ kAudioQualityStandard };
        bool enable_camera_mirror = true;
    };

    struct User {
        std::string user_id;
        std::string user_name;
        // UTC/GMT join call time
        int64_t created_at{ 0 };
        bool is_sharing{ false };
        bool is_mic_on{ false };
        bool is_camera_on{ false };
        int audio_volume{ 0 };
    };

    struct VideoCallRoom {
      std::string room_id;
      std::string screen_shared_uid;
      // UTC/GMT, call duration time
      int64_t duration;
    };

    struct StreamInfo {
        std::string user_id;
        std::string user_name;
        // Resolution width value
        int width;
        // Resolution height value
        int height;
        // video frame rate
        int video_fps;
        // Video bit rate
        int video_kbitrate;
        // audio bit rate
        int audio_kbitrate;
        // audio delay
        int audio_delay;
        // video delay
        int video_delay;
        // audio packet loss rate
        float audio_loss_rate;
        // Video packet loss rate
        float video_loss_rate;
        // network quality
        int natwork_quality;
    };
}


