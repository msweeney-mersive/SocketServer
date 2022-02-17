package com.foobar.socketserver

import android.util.Log
import java.net.ServerSocket
import java.net.SocketException
import java.util.concurrent.Executors
import java.util.concurrent.Semaphore
import java.util.concurrent.TimeUnit

class KServer (
    port: Int,
    maxSessions: Int,
) {
    private val tag = this.javaClass.simpleName

    private val listenThread: Thread
    private val semaphore = Semaphore(maxSessions, true)
    private val executor = Executors.newScheduledThreadPool(maxSessions)

    private val serverSocket = ServerSocket(port)

    init {
        listenThread = Thread {
            try {
                generateSequence {
                    semaphore.acquire()
                    serverSocket.accept()
                }.forEach { sock ->
                    try {
                        Log.d(tag, "client isConnected: ${sock.isConnected}")
                        //val session = sessionFactory(sock)
                        executor.submit {
                            try {
                                //session.run()
                            } finally {
                                semaphore.release()
                            }
                        }
                    } catch (ex: Exception) {
                    }
                }
            } catch (ex: SocketException) {
                Log.i(tag, "SocketException caught while waiting for client")
            } catch (ex: Exception) {
                Log.e(tag, "Error listening!", ex)
            }
        }
    }

    fun startListening() {
        Log.d(tag, "startListening()")
        listenThread.start()
    }

    fun close() {
        Log.d(tag, "close()")
        serverSocket.close()

        try {
            listenThread.interrupt()
            listenThread.join(1000)
        } catch (ex: Exception) {
            Log.e(tag, "Failed to stop listenThread", ex)
        }

        executor.shutdownNow()
        executor.awaitTermination(1, TimeUnit.SECONDS)
    }
}