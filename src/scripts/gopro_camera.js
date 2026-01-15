import Axios from 'axios'

export class GoProCamera {
    name = ""
    url = ""
    connected = false
    state = {}
    info = {}

    constructor(name, url){
        this.name = name
        this.url = url
    }

    render(){
        Axios.get(`http://${this.url}/gopro/camera/state`).then(x => {
            this.state = x.data
        }).catch(() => {})
        Axios.get(`http://${this.url}/gopro/camera/info`).then(x => {
            this.info = x.data
        }).catch(() => {})
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

    shutdown() {
        return Axios.get(`http://${this.url}/gp/gpControl/command/system/shutdown`).catch(() => {})
    }

    shutter(p){
        return Axios.get(`http://${this.url}/gopro/camera/shutter/${ p ? 'start' : 'stop' }`).catch(() => {})
    }
}