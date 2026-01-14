import Axios from 'axios'
import { GoProCamera } from './gopro_camera'

export class GoProCameraStatus {

}

export class GoPro {
    cameras = []
    setting = {

    }

    constructor(){
        Axios.get('http://127.0.0.1:18000/exist').then(x => {
            if(x.data.exist){
                Axios.get('http://127.0.0.1:18000/load').then(y => {
                    const yo = JSON.parse(y.data)
                    this.cameras = yo.cameras
                    this.setting = yo.setting
                })
            }else{
                Axios.post('http://127.0.0.1:18000/save', this.toJson())
            }
        })
    }

    update(){
        Axios.post('http://127.0.0.1:18000/save', this.toJson())
    }

    add_camera(name, str){
        console.log("add_camera", name, str)
        if(str.length < 3){
            return false
        }
        const v = []
        v.push(str[str.length - 3])
        v.push(str[str.length - 2])
        v.push(str[str.length - 1])

        this.cameras.push(new GoProCamera(name, `172.2${v[0]}.1${v[1]}${v[2]}.51:8080`))
    }

    enable_control_all(p = true){
        for(const x of this.cameras){
            x.enable_control(p);
        }
    }

    reboot_all(p){
        for(const x of this.cameras){
            x.reboot(p);
        }
    }

    shutdown_all(p){
        for(const x of this.cameras){
            x.shutdown(p);
        }
    }

    toJson(){
        const data = {
            camera: this.cameras.map(x => ({ name: x.name, url: x.url }))
        }
        const setting = {

        }
        const v = {
            data: data,
            setting: setting
        }
        return v
    }
}