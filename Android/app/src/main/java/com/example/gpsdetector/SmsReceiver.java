package com.example.gpsdetector;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.telephony.SmsMessage;
import android.util.Log;

public class SmsReceiver extends BroadcastReceiver {
    private String TAG = "SmsReceiver";
    //interface
    public  static IMap iMap;

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG , "onReceive");
        Bundle data  = intent.getExtras();

        Object[] pdus = (Object[]) data.get("pdus");

        for(int i=0;i<pdus.length;i++){
            SmsMessage smsMessage = SmsMessage.createFromPdu((byte[]) pdus[i]);

            String sender = smsMessage.getDisplayOriginatingAddress();
            //Check the sender to filter messages which we require to read
            Log.d(TAG,"sender "+sender);
            if (sender.equals("+639513112562"))
            {

                String messageBody = smsMessage.getMessageBody();
                Log.d(TAG,"messageBody "+messageBody);

                String[] lalongs = messageBody.split(",");
                if (lalongs.length == 1){
                    iMap.message("Sms : \n"+lalongs[0]);
                }else{
                    String printLocation ="Sms : \n Location : \n"+"\t Latitude : "+lalongs[0] +"\n\t Longtitude : "+lalongs[1];
                    iMap.message(printLocation);
                    iMap.locate(lalongs[0],lalongs[1]);
                }
            }
        }

    }

    public static void bindListener(IMap listner) {
        iMap = listner;
    }
}