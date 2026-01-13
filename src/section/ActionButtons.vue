<template>
    <div class="text-center">
        <p>攝影機控制</p>
        <v-row no-gutters>
            <v-col cols="4">
                <v-btn class="w-100">錄影</v-btn>
            </v-col>
            <v-col cols="4">
                <v-btn class="w-100">停止</v-btn>
            </v-col>
            <v-col cols="4">
                <v-btn class="w-100">套用所有設定</v-btn>
            </v-col>
        </v-row>
        <v-row no-gutters>
            <v-col cols="3">
                <v-btn class="w-100">連結</v-btn>
            </v-col>
            <v-col cols="3">
                <v-btn class="w-100">斷線</v-btn>
            </v-col>
            <v-col cols="3">
                <v-btn class="w-100">關機</v-btn>
            </v-col>
            <v-col cols="3">
                <v-btn class="w-100">叫</v-btn>
            </v-col>
        </v-row>
        <v-select label="Mode" density="compact" hide-details>
            
        </v-select>
        <v-select label="Preset" density="compact" hide-details>
            
        </v-select>
    </div>
</template>

<script lang="js">

export default {
    data(){
        return {
            modes: [
                {
                    id: 0
                }
            ]
        }
    }
}

</script>