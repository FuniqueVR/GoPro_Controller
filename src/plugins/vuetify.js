import { VApp, VToolbar, VRow, VCol, VBtn } from 'vuetify/components'
// Styles
import '@mdi/font/css/materialdesignicons.css'
import 'vuetify/dist/vuetify.min.css'
import 'vuetify/styles'

// Vuetify
import { createVuetify } from 'vuetify'

export default createVuetify({
  theme: {
    defaultTheme: "dark",
  },
  defaults: {
    global: {
      ripple: false,
    },
    VBtn: {
      elevation: 4,
      prependIcon: "$vuetify",
    },
  },
  components: {
    VApp,
    VToolbar,
    VRow, VCol, VBtn,
  },
})
