<template>
    <v-dialog v-model="showDialog" width="80vw">
        <v-card>
            <p>ssss</p>
        </v-card>
    </v-dialog>
    <p>New Camera</p>
</template>

<script lang="js">

export default {
    props: ['modelValue'],
    emits: ['update:modelValue'],
    computed: { 
      showDialog: { 
        get () { return this.modelValue },
        set (value) { this.$emit('update:modelValue', value) }     
      }
    }
}

</script>