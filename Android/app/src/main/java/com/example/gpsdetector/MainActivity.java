package com.example.gpsdetector;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.telephony.SmsManager;
import android.telephony.TelephonyManager;
import android.util.Log;
import android.view.View;
import android.webkit.WebView;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.util.UUID;

public class MainActivity extends AppCompatActivity implements IMap {
    private String TAG = "MainActivity";
    private Bluetooth bluetooth;
    private Button btn_smsgps;
    private WebView web_map;
    private TextView txt_bluetoohmsg;
    private Permission permission;
    private Loading loading;
    private String myPhonenumber;

    int PERMISSION_ALL = 1;


    String[] PERMISSIONS = {
            Manifest.permission.SEND_SMS,
            android.Manifest.permission.READ_PHONE_STATE,
            android.Manifest.permission.READ_SMS
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        bluetooth = new Bluetooth(this, this);
        loading = new Loading(this);
        init();
        initSMSReceive();
    }

    private void initSMSReceive() {
        SmsReceiver.bindListener(this);
    }

    private void init() {
        permission = new Permission(this,this);
        if (!permission.hasPermissions(this, PERMISSIONS)) {
            ActivityCompat.requestPermissions(this, PERMISSIONS, PERMISSION_ALL);
        }
        if (!permission.checkSendSms()){
            permission.reqReadSMS();
        }
        TelephonyManager tMgr = (TelephonyManager) getSystemService(Context.TELEPHONY_SERVICE);
        myPhonenumber = tMgr.getLine1Number();
        btn_smsgps = (Button) findViewById(R.id.btn_smsgps);
        btn_smsgps.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (permission.checkSendSms()){
                    onSendSms();
                }else{
                    permission.reqReadSMS();
                }
            }
        });
        web_map = (WebView) findViewById(R.id.web_map);
        txt_bluetoohmsg = (TextView) findViewById(R.id.txt_bluetoothmsg);
        web_map.getSettings().setLoadsImagesAutomatically(true);
        web_map.getSettings().setJavaScriptEnabled(true);
        web_map.loadUrl("https://www.google.com/maps/place/Philippines/@11.6736057,118.1255611,6z/data=!3m1!4b1!4m8!1m2!3m1!2zMCJOIDAiRQ!3m4!1s0x324053215f87de63:0x784790ef7a29da57!8m2!3d12.5116654!4d122.9974365");
    }

    private void onSendSms() {
        Log.d(TAG, "Send SMS");
        SmsManager smsManager = SmsManager.getDefault();
        smsManager.sendTextMessage("+639513112562", null,
                "getGps"+String.valueOf(UUID.randomUUID().toString().substring(1,3)),
                null, null);
        Toast.makeText(getApplicationContext(), "SMS sent.",
                Toast.LENGTH_LONG).show();
    }


    private void mapPin(String lat , String lang){
        // format https://www.google.com/maps/place/14%C2%B034'45.8%22N+121%C2%B002'09.2%22E
        web_map.loadUrl("https://www.google.com/maps/place/"+lat+"+"+lang);
    }

    @Override
    public void locate(String lat, String lang) {
        mapPin(lat,lang);
    }

    @Override
    public void message(String msg) {
        txt_bluetoohmsg.setText(msg);
    }

    @Override
    public void checkConnected(Float connect) {

    }
}
