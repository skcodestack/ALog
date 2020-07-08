package com.sk.test;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.os.Bundle;
import android.view.View;
import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Environment;

import com.sk.alog.ALogger;
import com.sk.alog.LogConfig;
import com.sk.test.R;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    private static final int PERMISSION_REQUEST_CODE = 0x0014;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, PERMISSION_REQUEST_CODE);
            return;
        }


        init();
    }

    public void btn_write(View view) {
        ALogger.write(LogUtil.getStack());
    }


    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == PERMISSION_REQUEST_CODE) {
            init();
        }
    }

    private void init() {

        File file = new File(Environment.getExternalStorageDirectory(), "log_file.txt");
        File filecache = new File(Environment.getExternalStorageDirectory(), "log_file_cache.txt");
        ALogger.init(new LogConfig.Builder()
                .bufferPath(filecache.getAbsolutePath())
                .isCompress(true)
                .logPath(file.getAbsolutePath())
                .bufferSize(4096)
                .build());
    }

    @Override
    protected void onDestroy() {
        ALogger.release();
        super.onDestroy();
    }

    public void btn_flush(View view) {
        ALogger.flush();
    }
}