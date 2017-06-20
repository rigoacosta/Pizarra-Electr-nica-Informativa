package com.example.administrator.cartel;

import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.bluetooth.*;
import android.util.Log;
import android.view.View;
import android.widget.*;
import java.util.*;

public class MainActivity extends AppCompatActivity {

    Button con_btn, send_btn, date_btn, close_btn;
    EditText trama;
    TextView tv2;
    private BluetoothAdapter BA;
    private Set<BluetoothDevice> pairedDevices;

    private String moduleID = "HC-06";
    private final String TAG = "BlueLED";

    private BlueCon conn = null;
    private Cartel cartel = null;

    private BluetoothDevice hc = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        con_btn = (Button) findViewById(R.id.button1);
        send_btn = (Button) findViewById(R.id.button2);
        date_btn = (Button) findViewById(R.id.button3);
        close_btn = (Button) findViewById(R.id.button4);
        trama = (EditText) findViewById(R.id.editText1);

        tv2 = (TextView) findViewById(R.id.textView2);

        BA = BluetoothAdapter.getDefaultAdapter();

        con_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                pairedDevices = BA.getBondedDevices();

                for(BluetoothDevice bt : pairedDevices)
                {
                    if (bt.getName().equals(moduleID))
                    {
                        hc = bt;
                        Log.d("OK-conectando","HC");
                    }
                }
                if (hc != null)
                {
                    if (conn == null)
                    {
                        conn = new BlueCon(hc);
                        BA.cancelDiscovery();
                        conn.conect();
                        cartel = new Cartel(conn);
                        Toast.makeText(getApplicationContext(), "Conectando a HC-06." ,Toast.LENGTH_LONG).show();
                    }
                    else
                    {
                        if (!conn.isCon())
                        {
                            conn.conect();
                            cartel = new Cartel(conn);
                            Toast.makeText(getApplicationContext(), "Conectando a HC-06." ,Toast.LENGTH_LONG).show();
                        }
                        else
                        {
                            Toast.makeText(getApplicationContext(), "Ya esta conectando a HC-06." ,Toast.LENGTH_LONG).show();
                        }
                    }
                }
                else
                {
                    Toast.makeText(getApplicationContext(), "HC-06 no encontrado." ,Toast.LENGTH_LONG).show();
                }

            }
        });

        send_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String f = trama.getText().toString();

                if (cartel == null)
                {
                    Toast.makeText(getApplicationContext(), "No esta conectado al cartel" ,Toast.LENGTH_LONG).show();
                    return;
                }

                boolean ok = cartel.set_text(f);
                if (ok)
                {
                    Toast.makeText(getApplicationContext(), "Mensaje enviado correctamente" ,Toast.LENGTH_LONG).show();
                }
                else
                {
                    Toast.makeText(getApplicationContext(), "El mensaje falló" ,Toast.LENGTH_LONG).show();
                }


                if (conn.isCon())
                {
                    tv2.setText("Conectado");
                    tv2.setTextColor(Color.GREEN);
                }
                else
                {
                    tv2.setText("No conectado");
                    tv2.setTextColor(Color.RED);
                }
            }
        });

        date_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                if (cartel == null)
                {
                    Toast.makeText(getApplicationContext(), "No esta conectado al cartel" ,Toast.LENGTH_LONG).show();
                    return;
                }

                boolean ok = cartel.set_hour();
                if (ok)
                {
                    Toast.makeText(getApplicationContext(), "Tiempo enviado correctamente" ,Toast.LENGTH_LONG).show();
                }
                else
                {
                    Toast.makeText(getApplicationContext(), "El tiempo falló" ,Toast.LENGTH_LONG).show();
                }

                if (conn.isCon())
                {
                    tv2.setText("Conectado");
                    tv2.setTextColor(Color.GREEN);
                }
                else
                {
                    tv2.setText("No conectado");
                    tv2.setTextColor(Color.RED);
                }
            }
        });

        close_btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (conn != null)
                {
                    if (conn.isCon())
                    {
                        conn.cancel();
                    }
                }

                finish();
            }
        });
    }
}
