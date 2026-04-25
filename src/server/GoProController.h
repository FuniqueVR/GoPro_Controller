/*
 * Copyright (c) [2026] [Elly/Funique]
 *
 * This software is licensed under the [MIT License].
 * See the LICENSE file in the project root for more information.
*/
#pragma once

#include <utility>
#include <vector>
#include <list>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include "mdns_cpp/logger.hpp"
#include "mdns_cpp/mdns.hpp"
#include "hv/requests.h"
#include <nlohmann/json.hpp>
#include "../common/iphelper.h"
#include "../common/camera_code.h"

using json = nlohmann::json;
std::string getPacket(std::string key, json data);

typedef std::pair<std::string,std::string> SingleResponse;

///
/// The middleware server worker, the hero
/// The cpp files break into two folder, and base on implementation detail to seperate cpp files
/// - controller (public methods)
/// - private (private methods)
///
class GoProController {
public:
    GoProController();
    GoProController(const GoProController&) = delete;
    GoProController& operator=(const GoProController&) = delete;
    virtual ~GoProController();
    ///
    /// Spawn a background worker for
    /// - Check camera alive or not
    /// - - Base on the state, put it in the "camera_alive_ips" variable
    ///
    void update();

#pragma region Camera part of calls
    ///
    /// Using mdns feature to catch all gopro services and store the ip addresses
    ///
    void scanCameras();
    ///
    /// Clear the array of IP address
    ///
    void cleanCameras();
    ///
    /// Rename the camera by ip input
    /// It will change the variable "camera_name", and store to the record and keep it in the disk
    ///
    void renameCameras(std::string ip, std::string name);
    ///
    /// Adding camera base on serial number
    /// The serial string must satisfied rules below
    /// - Must be number charachers
    /// - Must at least 3 length long (This will took last 3 charachers anyway)
    ///
    void addCameras(std::string serial);
    ///
    /// Delete camera by IP address
    ///
    void deleteCameras(std::string ip);
#pragma endregion

#pragma region Control part of calls
    void setPreset(std::string target, int32_t mode);
    void reboot(std::string target);
    void shutdown(std::string target);
    void keep_alive(std::string target);
    ///
    /// Send the usb control command to the camera
    /// The most obvious command when send off
    /// If ison is off the camera will go to black screen
    /// If ison is on the camera will go to normal camera control screen
    ///
    /// Args:
    /// - target: Camera IP address
    /// - ison: usb control turn on or off
    ///
    void usb(std::string target, bool ison);
    ///
    /// Send the datetime command to the camera
    /// The datetime value will base on the machine where it run
    /// Better sync it with network before doing yeah ?
    ///
    /// Args:
    /// - target: Camera IP address
    ///
    void datetime(std::string target);
    ///
    /// Send the zoom command to the camera
    ///
    /// Args:
    /// - target: Camera IP address
    /// - value: The zoom value, should be in the range [0...100]
    ///
    void zoom(std::string target, int32_t value);
    ///
    /// Send the shutter command to the camera
    ///
    /// Args:
    /// - target: Camera IP address
    /// - isstart: shutter turn on or off
    ///
    void shutter(std::string target, bool isstart);
    ///
    /// Send the locate function to the camera
    ///
    /// Args:
    /// - target: Camera IP address
    /// - ison: locate turn on or off
    ///
    void locate(std::string target, bool ison);
    ///
    /// Get the current ip list record, It looks up variable "camera_ips"
    ///
    /// Return: 
    /// serialized json
    /// ["IP1", "IP2", "IP3"]
    ///
    std::string getAllIP();
#pragma endregion

#pragma region Status part of calls
    std::string queryStatus(std::string target);
    std::string setSetting(std::string target, int32_t ID, std::string value);
    ///
    /// If source is null, it does not matter anyway, it just for ignore apply reason.
    /// If target is null, it will the apply range to all of the clients.
    ///
    std::string setSettingAll(const std::string source, const std::string target, int32_t preset, json value);
#pragma endregion

#pragma region Webcam part of calls
    void webcamMode(std::string target);
    void webcamUnMode(std::string target);
    void webcamOn(std::string target, int32_t startPort, int32_t res, int32_t fov, bool TS);
    void webcamOff(std::string target);
    std::string webcamStatus(std::string target);
    std::string webcamVersion(std::string target);
#pragma endregion

#pragma region Preview part of calls
    ///
    /// Open preview on the camera
    ///
    /// Args:
    /// - target: The Camera IP
    /// - port: The open port
    ///
    void previewOn(std::string target, int32_t port);
    ///
    /// Close preview on the camera
    ///
    /// Args:
    /// - target: The Camera IP
    ///
    void previewOff(std::string target);
#pragma endregion

#pragma region Media part of calls
    ///
    /// Received target camera media list, basically the whole list
    ///
    /// Args:
    /// - target: Camera ip
    ///
    std::string getMediaList(std::string target);
    ///
    /// Received target camera last store media
    ///
    /// Args:
    /// - target: Camera ip
    ///
    std::string getLastMedia(std::string target);
    ///
    /// Get the download link for the last media resource
    ///
    /// Notice:
    /// ! If master is not at the same machine as where this server hosting,
    /// ! It will trying to download to the /res folder as temp, and using HTTP static router as a download link
    /// ! This might be slower than you think, since you have to wait it to finish
    /// ? But if they are the same machine, the download link will go straight to the GoPro camera provide http URL
    /// ? Which means no buffer waiting time.
    ///
    /// TODO: 
    /// - Make pipe download possible. Reduce time to download the media
    /// 
    /// Args:
    /// - target
    ///
    std::string getFetchURL(std::string target_ip, bool is_local);
#pragma endregion

protected:
#pragma region IO
    ///
    /// Loading record from disk
    ///
    void _loadRecord();
    ///
    /// Store record to disk
    ///
    void _updateRecord();
#pragma endregion
    // Control part of calls
    void _setAllPreset(std::vector<std::string> targets, int32_t mode);
    void _setPreset(std::string target, int32_t mode);
    void _rebootAll(std::vector<std::string> targets);
    void _reboot(std::string target);
    void _shutdownAll(std::vector<std::string> targets);
    void _shutdown(std::string target);
    void _keepAliveAll(std::vector<std::string> targets);
    void _keepAlive(std::string target);
    void _usbAll(std::vector<std::string> targets, bool ison);
    void _usb(std::string target, bool ison);
    void _datetimeAll(std::vector<std::string> targets);
    void _datetime(std::string target);
    void _zoomAll(std::vector<std::string> targets, int32_t value);
    void _zoom(std::string target, int32_t value);
    void _shutterAll(std::vector<std::string> targets, bool isstart);
    void _shutter(std::string target, bool isstart);
    void _locate(std::string target, bool ison);
    // Status part of calls
    std::vector<SingleResponse> _queryAllStatus(std::vector<std::string> targets);
    SingleResponse _queryStatus(std::string target);
    std::vector<SingleResponse> _queryAllHW(std::vector<std::string> targets);
    SingleResponse _queryHW(std::string target);
    std::vector<SingleResponse> _setAllSetting(std::vector<std::string> targets, int32_t ID, std::string value);
    SingleResponse _setSetting(std::string target, int32_t ID, std::string value);
    std::vector<SingleResponse> _setAllSetting(std::vector<std::string> targets, int32_t preset, json res);
    std::vector<SingleResponse> _setSetting(std::string target, int32_t preset, json res);
    std::vector<SingleResponse> _setSetting_utility(std::string target, json res, std::vector<int32_t> setting_ids);
    // Webcam part of calls
    void _webcamAllMode(std::vector<std::string> targets);
    void _webcamMode(std::string target);
    void _webcamAllUnMode(std::vector<std::string> targets);
    void _webcamUnMode(std::string target);
    void _webcamAllOn(std::vector<std::string> targets, int32_t startPort, int32_t res, int32_t fov, bool TS);
    void _webcamOn(std::string target, int32_t startPort, int32_t res, int32_t fov, bool TS);
    void _webcamAllOff(std::vector<std::string> targets);
    void _webcamOff(std::string target);
    SingleResponse _webcamStatus(std::string target);
    SingleResponse _webcamVersion(std::string target);
    // Preview part of calls
    void _previewAllOn(std::vector<std::string> targets, int32_t port);
    void _previewOn(std::string target, int32_t port);
    void _previewAllOff(std::vector<std::string> targets);
    void _previewOff(std::string target);
    // Media part of calls
    std::vector<SingleResponse> _getAllMediaList(std::vector<std::string> targets);
    SingleResponse _getMediaList(std::string target);
    std::vector<SingleResponse> _getAllLastMedia(std::vector<std::string> targets);
    SingleResponse _getLastMedia(std::string target);
    // Utility calls
    SingleResponse _getSingleResponse(std::string target, std::string suffix);
    std::vector<SingleResponse> _getAllResponse(std::vector<std::string> targets, std::string suffix);

    int32_t _get_current_model(json hwinfo);
private:
    mdns_cpp::mDNS mdns;
    bool mdns_scaned = false;
    std::vector<std::thread> scan_workers;
    /**
     * @brief IP address record
     */
    std::vector<std::string> camera_ips;
    /**
     * @brief IP address record (Alive)
     */
    std::vector<std::string> camera_alive_ips;
    /**
     * @brief IP address record (Thread Guard)
     */
    std::mutex ips_mutex;
    /**
     * @brief IP address record (Alive) (Thread Guard)
     */
    std::mutex ips_alive_mutex;
    /**
     * @brief IP : Hardware info json data
     */
    std::unordered_map<std::string, json> camera_hw;
    /**
     * @brief IP : Name
     */
    std::unordered_map<std::string, std::string> camera_name;
    std::atomic<bool> scanning{false}; // To track scanning state if needed
};
