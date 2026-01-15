<template>
    <div class="text-center">
        <div style="width: 100%; height:calc(90vh - 50px);">
            <v-row class="pa-2" v-for="i in 6" :key="`row-${i}`" style="height:calc((90vh - 50px) / 6)" no-gutters>
                <v-col class="pa-2" v-for="j in 6" :key="`row${i}-col-${j}`" cols="2">
                    <p>{{ (((i - 1) * 6) + j).toString().padStart(4, '0') }}</p>
                    <CameraPreview :target="cameras[(((i - 1) * 6) + j) - 1]" class="w-100 h-100" />
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
    components: {
        CameraPreview
    }
}
</script>