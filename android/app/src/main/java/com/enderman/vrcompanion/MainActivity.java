package com.enderman.vrcompanion;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

// Companion app entry point. Streaming from the render host is not built yet.
public class MainActivity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        TextView text = new TextView(this);
        text.setText("VR Companion 0.1.0\nWaiting for a render host. Streaming is not built yet.");
        text.setPadding(48, 48, 48, 48);
        text.setTextSize(20);
        setContentView(text);
    }
}
