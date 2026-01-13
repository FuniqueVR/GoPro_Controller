<template>
  <v-app>
    <v-main>
      <v-row class="w-100 pa-2">
        <v-col cols="6">
          <v-row style="height:40vh">
            <v-col cols="5">
              <GlobalControl />
            </v-col>
            <v-col cols="7">
              <ActionButtons />
            </v-col>
          </v-row>
          <ControlInspector />
        </v-col>
        <v-col cols="6">
          <CameraList />
        </v-col>
      </v-row>
      <NewCamera v-model="newcam" />
    </v-main>
  </v-app>
</template>

<script>
import ActionButtons from './section/ActionButtons.vue'
import CameraList from './section/CameraList.vue'
import GlobalControl from './section/GlobalControl.vue'
import ControlInspector from './section/ControlInspector.vue'

import NewCamera from './popup/NewCamera.vue'

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
    newcam: false
  }),
}
</script>

<style scoped>

v-main {
  width: 100vw;
  height: 100vh;
}

</style>