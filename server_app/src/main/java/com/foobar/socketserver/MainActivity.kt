package com.foobar.socketserver

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log

class MainActivity : AppCompatActivity() {
    private val tag = this.javaClass.simpleName

    lateinit var kServer : KServer
    val nativeThread :Thread

    init {
        nativeThread = Thread {
            try {
                nativeListen()
            } catch (ex: Exception) {
                Log.e(tag, "Error listening!", ex)
            }
        }
        Log.d(tag, "starting native worker thread")
    }

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

        nativeThread.start();
    }

    override fun onDestroy() {
        Log.d(tag, "onDestroy()")
        super.onDestroy()

        try {
            nativeThread.interrupt()
            nativeThread.join(500)
        } catch (ex: Exception) {
            Log.w(tag, "Failed to stop nativeThread", ex)
        }
        kServer.close()
    }

    external fun stringFromJNI(): String
    external fun nativeListen(): Unit

    companion object {
        init {
            System.loadLibrary("csocket")
        }
    }
}