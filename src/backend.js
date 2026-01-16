import express from 'express'
import bodyParser from 'body-parser'
import cors from 'cors'
import os from 'os'
import path from 'path'
import fs from 'fs'
import { spawn } from 'child_process'

const app = express()
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())
app.use(cors())
const port = 18000
let useTranscode = []

export const EventInit = () => {
    const filepath = path.join(os.homedir(), ".gopro_config.json");
    const folderpath = path.join(os.homedir(), "gopro");
    if(!fs.existsSync(folderpath)) fs.mkdirSync(folderpath);

    app.get('/exist', (req, res) => {
        res.json({
            exist: fs.existsSync(filepath)
        })
    })

    app.post('/save', (req, res) => {
        console.log("save data", req.body)
        try{
            fs.writeFileSync(filepath, JSON.stringify(req.body, null, 4))
            res.status(200).send(0)
        }catch(err){
            res.status(500).send(err)
        }
    })

    app.get('/load', (req, res) => {
        res.send(fs.readFileSync(filepath).toString())
    })

    app.post('/transcode/start', (req, res) => {
        const url = req.body.url
        const index = useTranscode.findIndex(x => x.url == url)
        if(url == undefined || index != -1){
            res.status(402).send("transcode is already on")
            return
        }
        let p = 12000
        while(useTranscode.findIndex(x => x.port == p) != -1){
            p = p + 1
        }
        const u = path.join(folderpath, url)
        if(!fs.existsSync(u)) fs.mkdirSync(u)
        const fss = fs.readdirSync(u)
        for(const x of fss){
            fs.rmSync(path.join(u, x), {recursive: true, force: true})
        }
        const a = `-hide_banner -re -i udp://127.0.0.1:${p} -pix_fmt yuv420p -crf 32 -c:v h264_nvenc -an -f hls -hls_time 6 -hls_list_size 5 -hls_flags delete_segments+append_list index.m3u8`
        console.log(a)
        const process = spawn("ffmpeg", a.split(' '), {
            cwd: u
        })
        process.stdout.on('data', (data) => {
            console.log(`ffmpeg: ${data}`);
        });
        process.stderr.on('data', (data) => {
            console.log(`ffmpeg: ${data}`);
        });
        useTranscode.push({
            url: url,
            port: p,
            process: process
        })
        res.status(200).send(p)
    })

    app.post('/transcode/stop', (req, res) => {
        const url = req.body.url
        const index = useTranscode.findIndex(x => x.url == url)
        if(url == undefined || index == -1){
            res.status(402).send("transcode is nevert start yet")
            return
        }
        const data = useTranscode.find(x => x.url == url)
        data.process.stdin.write('q')
        data.process.kill()
        useTranscode.splice(index, 1)
        res.status(200).send(data.port)
    })

    app.use('/video', express.static(folderpath))

    app.listen(port, () => {
        console.log(`Example app listening on port ${port}`)
    })
}
