package com.example.administrator.cartel;

import android.bluetooth.*;
import android.util.*;
import java.io.*;
import java.util.*;

public class BlueCon {
    private BluetoothSocket mmSocket = null;
    private InputStream mmInStream = null;
    private OutputStream mmOutStream = null;
    private final String TAG = "BlueLED";

    public BlueCon(BluetoothDevice device) {

        BluetoothSocket tmp = null;
        try
        {
            tmp = device.createRfcommSocketToServiceRecord(UUID.fromString("00001101-0000-1000-8000-00805F9B34FB"));
        }
        catch (IOException e)
        {
            Log.d(TAG,"Error con el UUID");
        }
        mmSocket = tmp;
    }

    public void conect() {
        InputStream tmpIn = null;
        OutputStream tmpOut = null;

        try
        {
            mmSocket.connect();

            tmpIn = mmSocket.getInputStream();
            tmpOut = mmSocket.getOutputStream();
        }
        catch (IOException connectException)
        {
            Log.d(TAG,"Error conectando el socket");
            try
            {
                mmSocket.close();
            }
            catch (IOException closeException)
            {
            }
            return;
        }

        mmInStream = tmpIn;
        mmOutStream = tmpOut;
    }

    public boolean isCon()
    {
        return mmSocket.isConnected();
    }

    public byte[] read() {
        byte[] buffer = new byte[1024];  // buffer store for the stream
        int bytes; // bytes returned from read()

        byte[] result = null;

        try
        {
            bytes = mmInStream.read(buffer);

            result = new byte[bytes];

            for (int i = 0; i < bytes; i++) {
                Log.d("RECIBIDO", String.valueOf(buffer[i]));
                result[i] = buffer[i];
            }
        }
        catch (IOException e)
        {
            return null;
        }
        return result;
    }

    public void write(byte[] bytes)
    {
        try
        {
            mmOutStream.write(bytes);
        }
        catch (IOException e)
        {
            Log.d(TAG,"Error escribiendo al socket.");
        }
    }

    public void cancel() {
        try
        {
            mmSocket.close();
        }
        catch (IOException e)
        {

        }
    }
}
