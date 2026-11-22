package com.enderman.vrcompanion;

import android.app.Activity;
import android.os.Bundle;
import android.widget.ScrollView;
import android.widget.TextView;

// Companion app entry point. Shows this device's GPU and system data.
public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView text = new TextView(this);
        text.setText("VR Companion 0.1.0\n\n" + DeviceInfo.nativeGetJson() + "\n\nStreaming is not built yet.");
        text.setTextIsSelectable(true);
        text.setTypeface(android.graphics.Typeface.MONOSPACE);
        text.setPadding(32, 32, 32, 32);
        text.setTextSize(14);
        ScrollView scroll = new ScrollView(this);
        scroll.addView(text);
        setContentView(scroll);
    }
}
