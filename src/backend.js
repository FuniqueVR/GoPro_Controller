import express from 'express'
import bodyParser from 'body-parser'
import cors from 'cors'
import os from 'os'
import path from 'path'
import fs from 'fs'

const app = express()
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())
app.use(cors({ origin: ['http://127.0.0.1:18000', 'http://localhost:5173'] }))
const port = 18000

export const EventInit = () => {
    const filepath = path.join(os.homedir(), ".gopro_config.json");

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

    app.listen(port, () => {
        console.log(`Example app listening on port ${port}`)
    })
}
