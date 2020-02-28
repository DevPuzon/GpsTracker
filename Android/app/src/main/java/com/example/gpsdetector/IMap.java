package com.example.gpsdetector;

public interface IMap {
    void locate(String lat,String lang);
    void message(String msg);
    void checkConnected(Float connect);
}
