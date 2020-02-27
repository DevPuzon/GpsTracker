package com.example.gpsdetector;

import android.Manifest;
import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.pm.PackageManager;

import androidx.appcompat.app.AlertDialog;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;


public class Permission {
    public Context context;
    public Activity activity;

    public Permission(Context context, Activity activity) {
        this.context = context;
        this.activity = activity;
    }
    public boolean hasPermissions(Context context, String... permissions) {
        if (context != null && permissions != null) {
            for (String permission : permissions) {
                if (ActivityCompat.shouldShowRequestPermissionRationale(activity,
                        permission)) {
                    new AlertDialog.Builder(context)
                            .setCancelable(true)
                            .setTitle("Permission")
                            .setMessage("Please allow all permissions in App Settings.")
                            .setPositiveButton("Back", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int which) {
                                    activity.finish();
                                }
                            })
                            .setIcon(android.R.drawable.ic_dialog_alert)
                            .show();
                }
                if (ActivityCompat.checkSelfPermission(context, permission) != PackageManager.PERMISSION_GRANTED) {

                    return false;
                }
            }
        }
        return true;
    }
    public boolean checkSendSms() {
        int result = ContextCompat.checkSelfPermission(context,
                Manifest.permission.SEND_SMS);
        if (result == PackageManager.PERMISSION_GRANTED) {
            return true;
        } else {
            return false;
        }
    }
    public void reqSendSms() {
        if (ActivityCompat.shouldShowRequestPermissionRationale(activity,
                Manifest.permission.SEND_SMS)) {
            new AlertDialog.Builder(context)
                    .setCancelable(true)
                    .setTitle("Send Sms Permission")
                    .setMessage("Please allow sms permission in App Settings.")
                    .setPositiveButton("Back", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            activity.finish();
                        }
                    })
                    .setIcon(android.R.drawable.ic_dialog_alert)
                    .show();
        } else {
            ActivityCompat.requestPermissions(activity,
                    new String[]{Manifest.permission.SEND_SMS}, 11);
        }
    }
    public boolean checkReadSMS() {
        int result = ContextCompat.checkSelfPermission(context,
                Manifest.permission.RECEIVE_SMS);
        if (result == PackageManager.PERMISSION_GRANTED) {
            return true;
        } else {
            return false;
        }
    }
    public void reqReadSMS() {
        if (ActivityCompat.shouldShowRequestPermissionRationale(activity,
                Manifest.permission.RECEIVE_SMS)) {
            new AlertDialog.Builder(context)
                    .setCancelable(true)
                    .setTitle("SMS Permission")
                    .setMessage("Please allow sms permission in App Settings.")
                    .setPositiveButton("Back", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            activity.finish();
                        }
                    })
                    .setIcon(android.R.drawable.ic_dialog_alert)
                    .show();
        } else {
            ActivityCompat.requestPermissions(activity,
                    new String[]{Manifest.permission.RECEIVE_SMS}, 111);
        }
    }

}
