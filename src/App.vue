<template>
  <v-app>
    <v-main>
      <v-row class="w-100 py-2 px-0 ma-0">
        <v-col cols="6">
          <v-row style="height:40vh">
            <v-col cols="5">
              <GlobalControl @update:command="p => globalCommand(p)" />
            </v-col>
            <v-col cols="7">
              <ActionButtons 
                :cameras="cameras"
                :selected="selectedCam" />
            </v-col>
          </v-row>
          <ControlInspector />
        </v-col>
        <v-col cols="6">
          <CameraList 
            :cameras="cameras"
            @select:camera="v => clickCam(v)" />
        </v-col>
      </v-row>
      <NewCamera v-model="newcam" @submit="(v1, v2) => controller.add_camera(v1, v2)" />
    </v-main>
  </v-app>
</template>

<script>
import ActionButtons from './section/ActionButtons.vue'
import CameraList from './section/CameraList.vue'
import GlobalControl from './section/GlobalControl.vue'
import ControlInspector from './section/ControlInspector.vue'

import NewCamera from './popup/NewCamera.vue'

import { GoPro } from './scripts/gopro'

export default {
  name: 'App',

  components: {
    ActionButtons,
    CameraList,
    GlobalControl,
    ControlInspector,

    NewCamera,
  },

  data: () => ({
    mainloop: undefined,
    controller: new GoPro(),
    cameras: [],
    newcam: false,
    selectedCam: undefined,
  }),

  mounted(){
    this.mainloop = setInterval(() => {
      this.controller.render()
      this.cameras = this.controller.cameras
    }, 500);
  },
  unmounted(){
    clearInterval(this.mainloop)
  },

  methods: {
    globalCommand(p) {
      switch(p){
        case 0:
          {
            this.newcam = true
            break;
          }
        case 1:
          {
            this.controller.cameras = []
            break;
          }
        case 2:
          {
            this.controller.enable_control_all(true)
            break;
          }
        case 3:
          {
            this.controller.enable_control_all(false)
            break;
          }
        case 4:
          {
            this.controller.shutdown_all()
            break;
          }
        case 5:
          {
            this.controller.shutter(true)
            break;
          }
        case 6:
          {
            this.controller.shutter(false)
            break;
          }
        case 7:
          {
            break;
          }
      }
    },
    clickCam(url){
        this.selectedCam = url
    }
  }
}
</script>

<style scoped>

v-main {
  width: 100vw;
  height: 100vh;
}

</style>