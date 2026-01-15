<template>
    <div class="frame">
        <div class="w-100 h-100" v-if="target != undefined">
            <p>{{ target.state.status['2'] }}</p>
        </div>
    </div>
</template>

<script lang="js">
export default {
    name: "CameraPreview",
    props: ['target']
}
</script>

<style scoped>
.frame {
    border-width: 5px;
    border-style: solid;
    border-color: red;
}
</style>

