package com.yxy.helloworld;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("helloworld");
        System.loadLibrary("mycpp-lib");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        hookThread();
        String str=stringFromJNI();
//        Log.i("tag5","C++ str "+str);
    }
    public native String stringFromJNI();

    public native void hookThread();
}