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
        Promise.all([
            Axios.get(`http://${this.url}/gopro/camera/state`),
            Axios.get(`http://${this.url}/gopro/camera/info`)
        ]).then(x => {
            this.state = x[0].data
            this.info = x[1].data
            this.setConnected(true)
        }).catch(() => {
            this.setConnected(false)
        })
    }

    async setConnected(p){
        if(this.connected && !p){
            Axios.post(`http://127.0.0.1:18000/transcode/stop`, {
                url: this.url
            }).then(x => {
                console.log("Stop stream", this.url, x.data)
                Axios.get(`http://${this.url}/gopro/camera/stream/stop?port=${x.data}`).catch(() => {})
                this.connected = p
            })
        }
        else if(!this.connected && p){
            await Axios.get(`http://${this.url}/gopro/webcam/preview`)
            Axios.post(`http://127.0.0.1:18000/transcode/start`, {
                url: this.url
            }).then(x => {
                console.log("Start stream", this.url, x.data)
                Axios.get(`http://${this.url}/gopro/webcam/start?res=12&fov=0&port=${x.data}&protocol=TS`)
                this.connected = p
            }).catch((x) => {
                Axios.post(`http://127.0.0.1:18000/transcode/stop`, {
                    url: this.url
                }).then(() => setTimeout(() => {
                    this.setConnected(p)
                }, 5000))
            })
        }
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
        this.info = {}
        this.state = {}
        this.setConnected(false)
        return Axios.get(`http://${this.url}/gp/gpControl/command/system/reset`).catch(() => {})
    }

    shutdown() {
        this.info = {}
        this.state = {}
        this.setConnected(false)
        return Axios.get(`http://${this.url}/gp/gpControl/command/system/shutdown`).catch(() => {})
    }

    shutter(p){
        return Axios.get(`http://${this.url}/gopro/camera/shutter/${ p ? 'start' : 'stop' }`).catch(() => {})
    }
}