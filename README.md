# GoPro Controller

Tool for control multiple GoPro Cameras, The design is for above 100 cameras connection.

## System Requirement

* Operating System: Debine / Windows


## 協定

可以參考 GoPro Http API 協定的 [Docs](https://gopro.github.io/OpenGoPro/http#tag/Webcam/operation/GPCAMERA_WEBCAM_START_OGP)

你的 GoPro 必須要用 USB 連著 (沒錯, GoPro 可以用 Http 控制, 當用 USB 接著)

透過 {IP}:9090/ 進入 websocket server

接著透過這個方式傳輸訊息, websocket server 會有 analysis header 的 key, 把訊息丟到對的 processer.

需求物件結構
```json
{
    "key": "command | query | webcam | media | preview | preset",
    "value": {
        "name": "label"
    }
}
```
需求樹狀圖

* key: command
  * value-name: reboot
  * value-name: shutdown
  * value-name: keep_alive
  * value-name: usb_on
  * value-name: usb_off
  * value-name: datetime
  * value-name: zoom
  * value-name: shutter_on
  * value-name: shutter_off
  * value-name: ip
  * value-name: scan
  * value-name: clean
  * value-name: add
  * value-name: delete
  * value-name: rename
* key: query
  * value-name: get
  * value-name: getall
  * value-name: set
  * value-name: setall
* key: webcam
  * value-name: preview
  * value-name: exit
  * value-name: start
  * value-name: stop
  * value-name: status
  * value-name: version
* key: media
  * value-name: lastmedia
* key: preview
  * value-name: start
  * value-name: stop
* key: preset
  * value-name: set
