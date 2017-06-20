package com.example.administrator.cartel;

import android.util.Log;

import java.util.Date;


public class Cartel {

    private byte [] frame = null;
    private int len = 0;
    private byte CR = 0x0D, LF = 0x0A;
    final private BlueCon conexion;

    final private boolean DEBUG = true;
    private final String TAG = "BlueLED";

    public Cartel(BlueCon con)
    {
        frame = null;
        len = 0;
        conexion = con;
    }

    public boolean set_hour() {
        Date cdate = new Date();

        len = 11;
        frame = new byte[len];

        frame[0] = 0x3A;
        frame[1] = 0x48;    // H 72

        frame[2] = (byte) (cdate.getSeconds() % 60);
        frame[3] = (byte) (cdate.getMinutes());
        frame[4] = (byte) cdate.getHours();

        if (frame[4] > 12 || frame[4] == 0)
        {
            if (frame[4] == 0)
            {
                frame[4] = 12;
                frame[9] = (byte) 'P';
            }
            else
            {
                frame[4] -= 12;
                frame[9] = (byte)'P';
            }
        }
        else
        {
            frame[9] = (byte) 'A';
        }

        frame[5] = (byte)(cdate.getDay() + 1);
        frame[6] = (byte)(cdate.getDate());
        frame[7] = (byte)(cdate.getMonth() + 1);
        frame[8] = (byte)(cdate.getYear() % 100);

        frame[10] = CR;

        if (DEBUG)
        {
            for (int i = 0; i < len; i++) {
                Log.d("ENVIANDO", String.valueOf(frame[i]));
            }
        }

        if (conexion.isCon())
        {
            conexion.write(frame);
            return true;
        }
        else return false;
    }

    public boolean set_text(String text)
    {
        if (text.length() > 255)
            return false;

        len = text.length() + 3;
        frame = new byte[text.length() + 4];

        frame[0] = 0x3A;
        frame[1] = 0x43;    // C
        frame[2] = (byte) text.length();

        int k = 3;
        for (int i = 0; i < text.length(); i++)
        {
            switch (text.charAt(i))
            {
                case '-' : frame[k++] = (byte)0x10; break;
                case 'ñ' : frame[k++] = (byte)0x11; break;
                case 'Ñ' : frame[k++] = (byte)0x11; break;
                case ' ' : frame[k++] = (byte)0x12; break;
                default:  frame[k++] = (byte)text.charAt(i); break;
            }
        }
        frame[k++] = CR;

        if (DEBUG)
        {
            for (int i = 0; i < len; i++) {
                Log.d("ENVIANDO", String.valueOf(frame[i]));
            }
        }

        if (conexion.isCon())
        {
            conexion.write(frame);
            return true;
        }
        else return false;
    }

}
