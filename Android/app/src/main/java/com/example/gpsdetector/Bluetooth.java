package com.example.gpsdetector;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.os.CountDownTimer;
import android.os.Handler;
import android.text.TextUtils;
import android.util.Log;
import android.widget.Toast;


import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;
public class Bluetooth  extends Thread{

    private String TAG = "TAGBluetooth";

    String address = null , name=null;

    BluetoothAdapter myBluetooth = null;
    BluetoothSocket btSocket = null;
    Set<BluetoothDevice> pairedDevices;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private Context context;
    private IBluetooth iBluetooth;
    private CountDownTimer countDownTimer;
    public Bluetooth(Context context,IBluetooth iBluetooth) {
        this.context = context;
        this.iBluetooth = iBluetooth;
        try {
            bluetooth_connect_device();
        } catch (IOException e) {
            e.printStackTrace();
        }
        countDownTimer = new CountDownTimer(30000, 1000) {

            public void onTick(long millisUntilFinished) {
            }

            public void onFinish() {
            }
        };
    }

    public void bluetooth_connect_device() throws IOException {
//        try
//        {
//            Log.d(TAG,"start" );
//            myBluetooth = BluetoothAdapter.getDefaultAdapter();
//            address = myBluetooth.getAddress();
//            pairedDevices = myBluetooth.getBondedDevices();
//            if (pairedDevices.size()>0)
//            {
//                for(BluetoothDevice bt : pairedDevices)
//                {
//                    address=bt.getAddress().toString();
//                    name = bt.getName().toString();
//                    iBluetooth.message("Connected");
//                    Log.d(TAG,"loop name"+name+"loop address Address: "+address );
//                }
//            }else{
//                iBluetooth.message("Bluetooth not paired, please pair the blutooth HC-05 and refresh the application");
//            }
//            Log.d(TAG, String.valueOf(pairedDevices.size()));
//
//        }
//        catch(Exception ex){
//            Log.d(TAG,"excepe "+ex.getLocalizedMessage());
//        }
        myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
        BluetoothDevice dispositivo = myBluetooth.getRemoteDevice("20:16:03:30:99:92");//connects to the device's address and checks if it's available
        btSocket = dispositivo.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
        mmInputStream = btSocket.getInputStream();
        mmOutputStream = btSocket.getOutputStream();
        btSocket.connect();
        sendtoGps("pasok");
        beginListenForData();
//        try {
//            Log.d(TAG,"BT Name: "+name+"\nBT Address: "+address );
//            sendMessage("putaaa");
//        }
//        catch(Exception e){
//            e.printStackTrace();
//        }
    }

    private int count  = 0;
    private boolean isStart = true;
    private void sendSensorData(String data) {
        int startOfLine = data.indexOf("(");
        int endOfLineIndex = data.indexOf(")");
        Log.d(TAG,"DATA "+ data);
        Log.d(TAG, String.valueOf(endOfLineIndex));
        if (!TextUtils.isEmpty(data)){
            countDownTimer.start();
        }
        sendtoGps("pasok");
        if (endOfLineIndex > 0 && startOfLine == 0) {
            if (isStart){
                isStart = false;
                count=123123;
            }

            data = data.replace("(","");
            data = data.replace(")","");
            String[] latlangs = data.split(",");
            String printLocation ="Location : \n"+"\t Latitude : "+latlangs[0] +"\n\t Longtitude : "+latlangs[1];
            iBluetooth.message(printLocation);
            if (count > 20){
                count=0;
                iBluetooth.locate(latlangs[0],latlangs[1]);
            }
            count++;
        }

        int start = data.indexOf("[");
        int end = data.indexOf("]");
        Log.d(TAG, String.valueOf(start) + "  "+  String.valueOf(end));
        if(end > 0 && start == 0) {
            iBluetooth.message("Unstable the gps");
        }
    }

    volatile boolean stopWorker;
    int readBufferPosition;
    byte[] readBuffer;
    Thread workerThread;
    InputStream mmInputStream;
    OutputStream mmOutputStream;

    private void sendtoGps(String msg){
//        Log.d(TAG, "write: Writing to outputstream: " + msg.getBytes());
//        try {
//            mmOutputStream.write(msg.getBytes());
//        } catch (IOException e) {
//            Log.e(TAG, "write: Error writing to output stream. " + e.getMessage() );
//        }
    }
    private void beginListenForData() {
        final Handler
                handler = new Handler();
        final byte delimiter = 10; //This is the ASCII code for a newline character

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];
        workerThread = new Thread(new Runnable()
        {
            public void run()
            {
                while(!Thread.currentThread().isInterrupted() && !stopWorker)
                {
                    try
                    {
                        int bytesAvailable = mmInputStream.available();
                        if(bytesAvailable > 0)
                        {
                            byte[] packetBytes = new byte[bytesAvailable];
                            mmInputStream.read(packetBytes);
                            for(int i=0;i<bytesAvailable;i++)
                            {
                                byte b = packetBytes[i];
                                if(b == delimiter)
                                {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    final String data = new String(encodedBytes, "US-ASCII");
                                    readBufferPosition = 0;
                                    handler.post(new Runnable()
                                    {
                                        public void run()
                                        {
                                            sendSensorData(data);
                                        }
                                    });
                                }
                                else
                                {
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    }
                    catch (IOException ex)
                    {
                        stopWorker = true;
                    }
                }
            }
        });

        workerThread.start();
    }

    public interface IBluetooth{
        void locate(String lat,String lang);
        void message(String msg);
        void checkConnected(Float connect);
    }

}
