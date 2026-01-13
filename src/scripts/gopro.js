import Axios from 'axios'

export class GoProCameraStatus {

}

export class GoProCamera {
    name = ""
    url = ""

    constructor(name, url){
        this.name = name
        this.url = url
    }

    enable_control(p = true) {
        return Axios.get(`http://${this.url}/gopro/camera/control/wired_usb`, {
            params: {
                p: p ? '1' : '0'
            }
        }).catch(() => {})
    }

    keep_alive() {
        return Axios.get(`http://${this.url}/gopro/camera/keep_alive`).catch(() => {})
    }

    reboot() {
        return Axios.get(`http://${this.url}/gp/gpControl/command/system/reset`).catch(() => {})
    }
}

export class GoProController {
    cameras = []


}