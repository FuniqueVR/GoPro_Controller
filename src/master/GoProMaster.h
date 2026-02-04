/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <mutex>
#include <thread>
#include <functional>
#include "hv/WebSocketClient.h"
#include <nlohmann/json.hpp>
#include "../common/iphelper.h"
#include "../common/camera_code.h"

using json = nlohmann::json;

/**
 * The setting data for the camera
 * Should be generate by GoProMaster::getSettingsFromCamera method
 */
struct ConvertSetting {
    int32_t values[GOPRO_SETTING_SIZE];
};

/**
 * The status data for the camera
 * Should be generate by GoProMaster::getstatusFromCamera method
 */
struct ConvertStatus {
    int32_t battery_present;
    int32_t internal_battery_bars;
    int32_t overheating;
    int32_t busy;
    int32_t quick_capture;
    int32_t encoding;
    int32_t lcd_lock;
    int32_t video_encoding_duration;
    int32_t wireless_connections_enable;
    int32_t pairing_state;
    int32_t last_pairing_type;
    int32_t last_paring_success;
    int32_t wifi_scan_state;
    int32_t last_wifi_scan_success;
    int32_t wifi_provisioning_state;
    int32_t remote_version;
    int32_t remote_connected;
    std::string connected_wifi_ssid;
    std::string ap_ssid;
    int32_t connected_devices;
    int32_t preview_stream;
    int32_t primary_storage;
    int32_t remaining_photos;
    int32_t remaining_video_time;
    int32_t photos;
    int32_t videos;
    int32_t ota;
    int32_t pending_fw_update_cancel;
    int32_t locate;
    int32_t timelapse_interval_countdown;
    int32_t sd_card_remaining;
    int32_t preview_stream_available;
    int32_t wifi_bar;
    int32_t active_hilight;
    int32_t time_since_last_hilight;
    int32_t minimum_status_poll_period;
    int32_t liveview_exposure_select_mode;
    int32_t liveview_y;
    int32_t liveview_x;
    int32_t gps_lock;
    int32_t ap_mode;
    int32_t inhernal_battery_percentage;
    int32_t microphone_accessory;
    int32_t zoom_level;
    int32_t wireless_band;
    int32_t zoom_available;
    int32_t mobile_friendly;
    int32_t ftu;
    int32_t v5ghz_availiable;
    int32_t ready;
    int32_t ota_changed;
    int32_t cold;
    int32_t rotation;
    int32_t zoom_while_encoding;
    int32_t flatmode;
    int32_t video_preset;
    int32_t photo_preset;
    int32_t timelapse_preset;
    int32_t preset_group;
    int32_t preset;
    int32_t preset_modified;
    int32_t remaining_live_bursts;
    int32_t live_bursts;
    int32_t capture_delay_active;
    int32_t media_mode_state;
    int32_t time_warp_speed;
    int32_t linux_core;
    int32_t lens_type;
    int32_t hindsignt;
    int32_t schedule_capture_preset_id;
    int32_t scheduled_capture;
    int32_t display_mod_status;
    int32_t sd_card_write_speed_error;
    int32_t sd_card_error;
    int32_t turbo_transfer;
    int32_t camera_control_id;
    int32_t usb_connected;
    int32_t usb_controlled;
    int32_t sd_card_capacity;
    int32_t photo_interval_capture_count;
    std::string camera_name;
};

/**
 * Basically holds the information of the camera
 * And which websocket server its from
 */
struct CameraInfo {
    /**
     * Camera IP
     */
    std::string ip;
    /**
     * Websocket server ip
     */
    std::string server;
    /**
     * The json states
     */
    json state;
};

typedef void (*camera_setting_feedback)(ConvertSetting setting);
typedef void (*camera_status_feedback)(ConvertStatus status);

/**
 * Basically holds the Websocket instance
 */
struct ServerConnection {
    /**
     * Websocket IP
     */
    std::string ip;
    /**
     * The libhv websocket client
     */
    hv::WebSocketClient client;
    /**
     * Current connection state
     */
    bool connected = false;
    /**
     * last message received from the server
     */
    std::string last_message;
};


/**
 * GoPro Master Worker
 * Use this hub stuff to control multiple websocket server or camera
 * And handles the message sender and process
 * It also use multithread to decode the message from the websocket instancess
 */
class GoProMaster {
public:
    GoProMaster();
    /**
     * Destroy the threadings and release the resource under.
     */
    ~GoProMaster();

    std::string addServer(const std::string& ip);
    void reconnectAll();
    void disconnectAll();
    void cleanAll();

    /**
     * 
     */
    void reconnect(const std::string& ip);
    void disconnect(const std::string& ip);
    void clean(const std::string& ip);

    // Commands
    void command_only(const std::string command, std::string target = "");
    void command_only(const std::string server, const std::string command, std::string target = "");
    void query_only(const std::string command, std::string target = "");
    void query_only(const std::string server, const std::string command, std::string target = "");
    void webcam_only(const std::string command, std::string target = "");
    void webcam_only(const std::string server, const std::string command, std::string target = "");
    void startRecordingAll();
    void stopRecordingAll();
    void setModePhotoAll();
    void setModeVideoAll();

    bool applyAll(const std::string& ip, const ConvertSetting& res);

    void registerCameraSettingFeedback(camera_setting_feedback v);
    void registerCameraStatusFeedback(camera_status_feedback v);

    std::mutex camera_mtx;

    // Data for UI
    const std::vector<std::shared_ptr<CameraInfo>>& getCameras() const;
    const std::vector<std::shared_ptr<ServerConnection>>& getServers() const;
private:
    std::vector<std::shared_ptr<CameraInfo>> cameras;
    std::vector<std::shared_ptr<ServerConnection>> servers;
    std::thread t1;
    std::unordered_map<std::string, bool> ipQueryFinish = std::unordered_map<std::string, bool>();
    camera_setting_feedback _camera_setting_feedback = NULL;
    camera_status_feedback _camera_status_feedback = NULL;

    bool done = false;

    void update();
    void processMessage(const std::string& ip, const std::string& msg);
    void sendToAll(const std::string& msg);
    void cleanCameraFromServer(const std::string ip);

public:
    void setdone();
    /**
     * We will need to convert the Value ID to index here
     * It's easier for me to display stuff on the gui this way
     */
    bool getSettingsFromCamera(CameraInfo target, ConvertSetting& res);
    bool getStatusFromCamera(CameraInfo target, ConvertStatus&& res);

    void getSettingFromJsonData(json data, ConvertSetting& res);
    void getStatusFromJsonData(json data, ConvertStatus& res);

    int32_t findCamera(const std::string ip);
    int32_t findServer(const std::string ip);
    int32_t findCamera(const std::string server, const std::string ip);
};
