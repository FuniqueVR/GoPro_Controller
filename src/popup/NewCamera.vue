<template>
    <v-dialog v-model="showDialog" width="80vw">
        <v-card>
            <v-text-field v-model="name" label="名稱"></v-text-field>
            <v-text-field v-model="serial" label="後三碼序號"></v-text-field>
            <v-btn @click="confirm">確定</v-btn>
        </v-card>
    </v-dialog>
    <p>New Camera</p>
</template>

<script lang="js">

export default {
    props: ['modelValue'],
    emits: ['update:modelValue', 'submit'],
    computed: { 
      showDialog: { 
        get () { return this.modelValue },
        set (value) { this.$emit('update:modelValue', value) }     
      }
    },
    watch: {
        showDialog(){
            this.name = ""
            this.serial = ""
        }
    },
    data(){
        return {
            name: "",
            serial: ""
        }
    },
    methods: {
        confirm(){
            this.$emit('submit', this.name, this.serial)
            this.$emit('update:modelValue', false)
        }
    }
}

</script>