<template>
    <div class="text-center">
        <div style="width: 100%; height:calc(90vh - 100px);">
            <v-row v-for="i in 6" :key="`row-${i}`" style="height:calc((90vh - 100px) / 6)" no-gutters>
                <v-col v-for="j in 6" :key="`row${i}-col-${j}`" cols="2">
                    <p>{{ (((i - 1) * 6) + j).toString().padStart(4, '0') }}</p>
                </v-col>
            </v-row>
        </div>
    </div>
</template>

<script lang="js">
export default {
    name: "CameraList",
    props: ['cameras']
}
</script>