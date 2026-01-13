<template>
  <v-app>
    <v-main>
      <v-row class="w-100">
        <v-col cols="6">
          <v-row style="height:40vh">
            <v-col cols="4">
              <GlobalControl />
            </v-col>
            <v-col cols="8">
              <ActionButtons />
            </v-col>
          </v-row>
          <ControlInspector />
        </v-col>
        <v-col cols="6">
          <CameraList />
        </v-col>
      </v-row>
    </v-main>
  </v-app>
</template>

<script>
import ActionButtons from './section/ActionButtons.vue'
import CameraList from './section/CameraList.vue'
import GlobalControl from './section/GlobalControl.vue'
import ControlInspector from './section/ControlInspector.vue'

export default {
  name: 'App',

  components: {
    ActionButtons,
    CameraList,
    GlobalControl,
    ControlInspector,
  },

  data: () => ({
    //
  }),
}
</script>

<style scoped>

v-main {
  width: 100vw;
  height: 100vh;
}

</style>