<template>
  <v-app>
    <v-main>
      <v-row class="w-100 pa-2">
        <v-col cols="6">
          <v-row style="height:40vh">
            <v-col cols="5">
              <GlobalControl @update:command="p => globalCommand(p)" />
            </v-col>
            <v-col cols="7">
              <ActionButtons />
            </v-col>
          </v-row>
          <ControlInspector />
        </v-col>
        <v-col cols="6">
          <CameraList :cameras="controller.cameras" />
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
import Axios from 'axios'

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
    newcam: false
  }),

  mounted(){
    this.mainloop = setInterval(() => {
      this.controller.render()
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