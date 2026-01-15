<template>
    <div class="text-center">
        <p>全局設定</p>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 0)">新增攝影機</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 1)">移除所有攝影機</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 2)">連結所有</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 3)">斷線所有</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 4)">全部關機</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 5)">錄影</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 6)">停止錄影</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 7)">Tag all</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 8)">設定</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 9)">攝影機模式</v-btn>
            </v-col>
        </v-row>
        <v-row noGutters>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 10)">播放聲音</v-btn>
            </v-col>
            <v-col cols="6">
                <v-btn class="w-100" @click="$emit('update:command', 11)">App 設定</v-btn>
            </v-col>
        </v-row>
    </div>
</template>

<script lang="js">
export default {
    name: "GlobalControl",
    emits: ['update:command'],
}
</script>