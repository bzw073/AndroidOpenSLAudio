package com.rainbow.androidopenslaudio;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
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
        requestMyPermissions();
//        String path = File.separator + "mnt" + File.separator + "sdcard" + Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "mypcm.pcm";
        String path = Environment.getExternalStorageDirectory().getAbsolutePath() + "/mypcm.pcm";
        File f = new File(path);
        String externalStorageState = Environment.getExternalStorageState(f);
        Log.d("STATE", "externalStorageState->" + externalStorageState);
        Log.d("AUDIO", "开始播放音频");
        playAudioWithPCM(path);
        Log.d("AUDIO", "结束播放音频");
    }

    private void requestMyPermissions() {

        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            //没有授权，编写申请权限代码
            ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 100);
        } else {

        }
        if (ContextCompat.checkSelfPermission(this,
                Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            //没有授权，编写申请权限代码
            ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.READ_EXTERNAL_STORAGE}, 100);
        } else {
        }
    }

}
