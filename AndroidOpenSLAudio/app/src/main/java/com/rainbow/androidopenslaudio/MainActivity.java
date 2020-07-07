package com.rainbow.androidopenslaudio;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public native void playAudioWithPCM(String url);

    public void playPCM(View view) {
        String path = Environment.getExternalStorageDirectory() + "/mypcm.pcm";
        File f = new File(path);
        String externalStorageState = Environment.getExternalStorageState(f);
        Log.d("STATE", "externalStorageState->" + externalStorageState);
        Log.d("AUDIO", "开始播放音频");
        playAudioWithPCM(path);
        Log.d("AUDIO", "结束播放音频");
    }

}
