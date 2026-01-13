<template>
    <v-dialog v-model="ison" width="80vw">
        <v-card>
            <p>ssss</p>
        </v-card>
    </v-dialog>
    <p>New Camera</p>
</template>

<script lang="js">

export default {
    model: {
        prop: 'ison',
        event: 'change'
    },
}

</script>