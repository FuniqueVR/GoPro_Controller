<template>
    <div class="text-center">
        <div style="width: 100%; height:calc(90vh - 50px);">
            <v-row class="pa-1" v-for="i in 6" :key="`row-${i}`" style="height:calc((90vh - 50px) / 6)" no-gutters>
                <v-col class="pa-1" v-for="j in 6" :key="`row${i}-col-${j}`" cols="2">
                    <p style="font-size: 10px;">{{ (((i - 1) * 6) + j).toString().padStart(4, '0') }}</p>
                    <CameraPreview 
                        class="w-100 h-100"
                        :selected="
                            cameras[(((i - 1) * 6) + j) - 1] != undefined &&
                            cameras[(((i - 1) * 6) + j) - 1].url == selectURL
                        "
                        style="cursor: pointer;"
                        :target="cameras[(((i - 1) * 6) + j) - 1]" 
                        @click="clickCam((((i - 1) * 6) + j) - 1)"/>
                </v-col>
            </v-row>
        </div>
    </div>
</template>

<script lang="js">
import CameraPreview from '../components/CameraPreview.vue'

export default {
    name: "CameraList",
    props: ['cameras'],
    emits:['select:camera'],
    components: {
        CameraPreview
    },
    data(){
        return {
            selectURL: undefined
        }
    },
    methods: {
        clickCam(index){
            if(this.cameras[index] != undefined){
                if(this.selectURL == this.cameras[index].url){
                    this.selectURL = undefined
                    this.$emit('select:camera', undefined)
                    console.log("unselect: ", undefined)
                }else{
                    this.selectURL = this.cameras[index].url
                    this.$emit('select:camera', this.selectURL)
                    console.log("select: ", this.selectURL)
                }
            }
        }
    }
}
</script>