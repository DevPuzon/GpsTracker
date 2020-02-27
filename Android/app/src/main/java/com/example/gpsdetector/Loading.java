package com.example.gpsdetector;
import android.app.ProgressDialog;
import android.content.Context;

public class Loading {
    public ProgressDialog loadDialog;
    public String messageTitle ="Loading";
    public String message ="Please wait";
    public Loading(Context context) {
        loadDialog =  new ProgressDialog(context);
        loadDialog.setTitle(messageTitle);
        loadDialog.setMessage(message);
        loadDialog.setCancelable(false);
    }
}
