<template>
    <div class="w-100 h-100"
        :class="{ 'frame': !selected, 'frame-selected': selected }">
        <!-- Title Display -->
        <span style="
            position: absolute; 
            font-size: 10px;
            transform:translateX(-10px);">
            {{ target?.name }}
        </span>
        <!-- Video Display -->
        <div v-if="target != undefined && target.connected" class="pa-1 w-100"
            :class="{ 'h-0': !isVideoPlaying(), 'h-100': isVideoPlaying() }">
            <video v-show="flag" style="z-index: -10;" ref="myvideo" autoplay></video>
        </div>
        <!-- Resolution -->
        <div class="innerframe tobottom ml-1" 
            v-if="target != undefined && 
                target?.state?.settings?.['2'] != undefined &&
                target?.state?.settings?.['3'] != undefined">

            <span v-if="target.state.settings['2'] == 1">4K</span>
            <span v-else-if="target.state.settings['2'] == 4">2.7K</span>
            <span v-else-if="target.state.settings['2'] == 6">2.7K 4:3</span>
            <span v-else-if="target.state.settings['2'] == 7">1440</span>
            <span v-else-if="target.state.settings['2'] == 9">1080</span>
            <span v-else-if="target.state.settings['2'] == 12">720</span>
            <span v-else-if="target.state.settings['2'] == 18">4K 4:3</span>
            <span v-else-if="target.state.settings['2'] == 21">5.6K</span>
            <span v-else-if="target.state.settings['2'] == 24">5K</span>
            <span v-else-if="target.state.settings['2'] == 25">5K 4:3</span>
            <span v-else-if="target.state.settings['2'] == 26">5.3K 8:7</span>
            <span v-else-if="target.state.settings['2'] == 27">5.3K 4:3</span>
            <span v-else-if="target.state.settings['2'] == 28">4K 8:7</span>
            <span v-else-if="target.state.settings['2'] == 31">8K</span>
            <span v-else-if="target.state.settings['2'] == 35">5.3K 21:9</span>
            <span v-else-if="target.state.settings['2'] == 36">4K 21:9</span>

            <span> | </span>

            <span v-if="target.state.settings['3'] == 0">240</span>
            <span v-else-if="target.state.settings['3'] == 1">120</span>
            <span v-else-if="target.state.settings['3'] == 2">100</span>
            <span v-else-if="target.state.settings['3'] == 3">90</span>
            <span v-else-if="target.state.settings['3'] == 5">60</span>
            <span v-else-if="target.state.settings['3'] == 6">50</span>
            <span v-else-if="target.state.settings['3'] == 8">30</span>
            <span v-else-if="target.state.settings['3'] == 9">25</span>
            <span v-else-if="target.state.settings['3'] == 10">24</span>
            <span v-else-if="target.state.settings['3'] == 13">200</span>
            <span v-else-if="target.state.settings['3'] == 15">400</span>
            <span v-else-if="target.state.settings['3'] == 16">360</span>
            <span v-else-if="target.state.settings['3'] == 17">300</span>

            <span> | </span>

            <span v-if="target.state.settings['175'] == 0">Easy</span>
            <span v-else-if="target.state.settings['175'] == 1">Pro</span>

            <span>{{ target.state.status['115'] }}</span>
        </div>
        <!-- Battery -->
        <div class="innerframe totop"
        style="font-size: 20px;"
        v-if="target != undefined && target?.state?.status?.['1'] == 0 != undefined &&
        target?.state?.status?.['2'] == 0">
            <span v-if="target.state.status['1'] == 0" class="text-red mdi mdi-battery-alert"></span>
            <span v-else-if="target.state.status['2'] == 0" class="text-red mdi mdi-battery-10"></span>
            <span v-else-if="target.state.status['2'] == 1" class="text-yellow mdi mdi-battery-40"></span>
            <span v-else-if="target.state.status['2'] == 2" class="text-green mdi mdi-battery-80"></span>
            <span v-else-if="target.state.status['2'] == 3" class="text-green mdi mdi-battery"></span>
            <span v-else-if="target.state.status['2'] == 4" class="text-warn mdi mdi-battery-charging-40"></span>
        </div>
    </div>
</template>

<script lang="js">
import Hls from 'hls.js'
export default {
    name: "CameraPreview",
    props: ['target', 'selected'],
    data(){
        return {
            hls: undefined,
            attachment: false,
            flag: false
        }
    },
    methods: {
        getURL(){
            const url = this.target?.url
            return `http://127.0.0.1:18000/video/${url}/index.m3u8`
        },
        isVideoPlaying() {
            const videoElement = this.$refs['myvideo']
            try{
                return !videoElement.paused && !videoElement.ended && videoElement.readyState > 2;
            }catch(err){
                return false
            }
        },
        retry(){
            const url = this.target?.url
            if(url != undefined && !this.isVideoPlaying()){
                const p = `http://127.0.0.1:18000/video/${url}/index.m3u8`
                this.$nextTick(() => {
                    this.hls.loadSource(p);
                })
                console.log("preview stream retry attmpt", p)
            }
        }
    },
    mounted(){
        if (Hls.isSupported()) {
            setInterval(() => {
                this.retry()
            }, 2000);
            const video = this.$refs['myvideo'];
            this.hls = new Hls();
            this.hls.attachMedia(video);

            this.hls.on(Hls.Events.MEDIA_ATTACHED, () => {
                console.log("video and hls.js are now bound together !");
                this.hls.loadSource(this.getURL());
                this.attachment = true
            });
        }
        else if (video.canPlayType(this.getURL())){
            video.src = this.getURL();
        }
        else{
            console.error("HLS not supported")
        }
    }
}
</script>

<style scoped>
video {
    width: 100%;
    height: 100%;
}
.frame {
    border-width: 1px;
    border-style: solid;
    border-color: red;
    text-align: center;
}
.frame-selected {
    border-width: 1px;
    border-style: solid;
    border-color: yellow;
    text-align: center;
}
.innerframe {
    font-size: 10px;
    position:fixed;
}
.totop {
    transform: translateY(calc(-16.66vh + 40px));
}
.tobottom {
    transform: translateY(-20px);
}
</style>

