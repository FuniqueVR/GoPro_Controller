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
                    const yo = y.data.data
                    console.log("YO", yo)
                    if(yo.camera != undefined){
                        for(const cam of yo.camera){
                            this.cameras.push(new GoProCamera(cam.name, cam.url))
                            console.log(`Load Camera: ${cam.name} ${cam.url}`)
                        }
                    }
                    this.setting = yo.setting
                    console.log(this.cameras)
                })
            }else{
                Axios.post('http://127.0.0.1:18000/save', this.toJson())
            }
        })
    }

    render(){
        for(const x of this.cameras){
            x.render();
        }
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
        this.update()
    }

    enable_control_all(p = true){
        for(const x of this.cameras){
            x.enable_control(p);
        }
    }

    reboot_all(){
        for(const x of this.cameras){
            x.reboot();
        }
    }

    shutdown_all(){
        for(const x of this.cameras){
            x.shutdown();
        }
    }

    shutter(p){
        for(const x of this.cameras){
            x.shutter(p);
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