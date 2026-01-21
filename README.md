# GoPro Controller

這是一個透過 GUI 控制 GoPro 的專案

## 架構圖

```mermaid
---
title: 結構
---
graph LR
    subgraph 攝影機群A
        CAMA1[Go Pro 攝影機];
        CAMA2[Go Pro 攝影機];
        CAMA3[Go Pro 攝影機];
        CAMAH[USB HUB];
        CAMAR[樹莓派];

        CAMAH-->CAMA1;
        CAMAH-->CAMA2;
        CAMAH-->CAMA3;
        CAMAR-->CAMAH;
    end
    subgraph 攝影機群B
        CAMB1[Go Pro 攝影機];
        CAMB2[Go Pro 攝影機];
        CAMB3[Go Pro 攝影機];
        CAMBH[USB HUB];
        CAMBR[樹莓派];

        CAMBH-->CAMB1;
        CAMBH-->CAMB2;
        CAMBH-->CAMB3;
        CAMBR-->CAMBH;
    end
    subgraph 攝影機群C
        CAMC1[Go Pro 攝影機];
        CAMC2[Go Pro 攝影機];
        CAMC3[Go Pro 攝影機];
        CAMCH[USB HUB];
        CAMCR[樹莓派];

        CAMCH-->CAMC1;
        CAMCH-->CAMC2;
        CAMCH-->CAMC3;
        CAMCR-->CAMCH;
    end
    L[筆電控制];
    L-->|Websocket|CAMAR;
    L-->|Websocket|CAMBR;
    L-->|Websocket|CAMCR;
```

```mermaid
---
title: 建置流程, 包含機器
---
```

## 協定

透過 {IP}:3000/api 進入 websocket server

接著透過這個方式傳輸訊息, websocket server 會有 analysis header 的 key, 把訊息丟到對的 processer.
```json
{
    "key": "string",
    "value": "object"
}
```

#### KEY: fetch

抓到所有狀態

需求物件結構
```json
{}
```

回傳物件結構
```json
{
    "cameras": [
        {
            "serial": "string"
        }
    ]
}
```

#### KEY: command

抓到所有狀態

需求物件結構
```json
{
    "name": [
        "keep_alive", "", ""
    ]
}
```

回傳物件結構
```json
{

}
```