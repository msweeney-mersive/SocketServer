package com.foobar.socketserver

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log

class MainActivity : AppCompatActivity() {
    private val tag = this.javaClass.simpleName

    lateinit var kServer : KServer

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)


        val kport =  resources.getInteger(R.integer.kotlin_server_port)
        val kmaxClients =  resources.getInteger(R.integer.kotlin_server_max_clients)
        Log.d(tag, "Kotlin server, max number of connected clients: ${kmaxClients}")
        kServer = KServer(kport, kmaxClients)

        Log.d(tag, "Starting kotlin server on port: ${kport}")
        kServer.startListening()
    }

    override fun onResume() {
        super.onResume()

        Log.d(tag, "onResume(), calling native function ...")
        var s = stringFromJNI()
        Log.d(tag, "native function returned: ${s}")
    }

    override fun onDestroy() {
        super.onDestroy()
        kServer.close()
    }

    external fun stringFromJNI(): String

    companion object {
        init {
            System.loadLibrary("csocket")
        }
    }
}