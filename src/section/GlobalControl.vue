<template>
    <div class="text-center">
        <p>全局設定</p>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100">新增攝影機</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100">移除所有攝影機</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100" @click="cam.enable_control(true)">連結所有</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100" @click="cam.enable_control(false)">斷線所有</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100">全部關機</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100">錄影</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100">停止錄影</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100">Tag all</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100">設定</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100">攝影機模式</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100">播放聲音</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100">App 設定</v-btn>
            </v-col>
        </v-row>
    </div>
</template>

<script lang="js">
import { GoProCamera } from '../scripts/gopro'

export default {

    data(){
        return {
            cam: new GoProCamera("TEST", "172.22.122.51:8080")
        }
    },
    methods: {

    }
}

</script>