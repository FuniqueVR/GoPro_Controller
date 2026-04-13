#include "../preview_popwin.h"
#include <format>
#include <cstdlib>

void PreviewPopup::update_decoder(){
    stream_open = false;
    trying = true;
    int32_t s = -1;
    int32_t model;

    if(first){
        std::cout << "===== OpenCV Build Info =====" << std::endl;
        std::cout << cv::getBuildInformation() << std::endl;
        std::cout << "=============================" << std::endl;
        first = false;
    }

    {
        {
            std::lock_guard<std::mutex> lock(master->camera_mtx);
            s = master->findCamera(state->preview_ip);
        }

        if(s == -1){
            std::cout << "[Preview Decoder] Cannot find camera: " << state->preview_ip << std::endl;
            trying = false;
            return;
        }
        const std::shared_ptr<CameraInfo>& c = master->getCameras().at(s);
        model = _get_current_model(c->hw);
        json buffer_setting = json::object();
        master->getSettingsFromCamera(*c, buffer_setting);
    }

    if(gl_texture != 0){
        glDeleteTextures(1, &gl_texture);
        gl_texture = 0;
    }

    bool g = false;
    const std::shared_ptr<CameraInfo>& c = master->getCameras().at(s);
    if(!g){
        pipeline = 
            "udpsrc port=8554 timeout=1000000000 "
            "! watchdog timeout=1000 "
            "! queue max-size-buffers=0 max-size-bytes=0 max-size-time=1000000000 "
            // Treet incoming UDP byte array as TS format
            "! tsdemux "
            /**
             * We nned to use general decodebin, because GOPRO model might have use
             * H265 or H264, We have no way to find out except fetch info from the incoming camera
             * But i'm too lazy for that. so i just want GStreamer do the detection for me
             */
            "! decodebin "
            // Convert to video feed
            "! videoconvert "
            // Leave video only and drop audio data
            "! video/x-raw,format=BGR "
            // Output to application use sink
            "! appsink sync=false drop=true max-buffers=1";
        //replaceAll(pipeline, "{0}", c->server.c_str());
        cap.open(pipeline, cv::CAP_GSTREAMER);
        std::cout << "[Preview Decoder] Pipeline use:" << std::endl << pipeline << std::endl;
    }
    //g = true;

    if(cap.isOpened()){
        cv::Mat test;
        for(int i = 0; i < MAX_ATTEMPT; i++) {
            std::cout << "[Preview Decoder] Try cap.grab" << std::endl;
            if(cap.grab()) {
                std::cout << "[Preview Decoder] cap.retrieve" << std::endl;
                if(cap.retrieve(test) && !test.empty()){
                    stream_open = true;
                    std::cout << "[Preview Decoder] Pipeline opened successfully! " << test.cols << "x" << test.rows << std::endl;
                    texture_width = test.cols;
                    texture_height = test.rows;
                    std::cout << "[Preview Decoder] Get resolution setting from camera: " << texture_width << ", " << texture_height << std::endl;
                    break;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(33)); // Match ~30fps
        }
        if(!stream_open){
            std::cout << "[Preview Decoder] No valid frames in first " << MAX_ATTEMPT << " attempts, releasing..." << std::endl;
        }
    } else {
        std::cout << "[Preview Decoder] Failed to open pipeline, retrying in 1s..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    if(!stream_open){
        std::cerr << "[Preview Decoder] Could not open pipeline" << std::endl;
        trying = false;
        return;
    }
    
    cv::Mat frame;
    std::cout << "[Preview Decoder] Decoding started!" << std::endl;

    while(stream_open){
        if(!cap.isOpened()){
            std::cerr << "[Preview Decoder] Pipeline closed unexpectedly!" << std::endl;
            trying = false;
            break;
        }

        if(cap.read(frame)){
            if(!frame.empty()){
                std::lock_guard<std::mutex> lock(queue_mutex);
                if(frame_queue.size() >= MAX_QUEUE_SIZE){
                    frame_queue.pop();
                }
                frame_queue.push(frame.clone());
            }
        } else {
            // cap.read failed - stream might have dropped
            std::cerr << "[Preview Decoder] cap.read() failed, stream dropped?" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if(!isopen) break;
    }

    std::cout << "[Preview Decoder] Update decoder end !" << std::endl;
    trying = false;
}
