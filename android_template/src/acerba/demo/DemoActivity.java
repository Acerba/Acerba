package org.acerba.demo;

import android.app.*;
import android.content.*;
import android.text.InputType;
import android.view.*;

import org.libsdl.app.SDLActivity;

public class DemoActivity extends SDLActivity {
	
	@Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
		
		  if (hasFocus) {
			  getWindow().getDecorView().setSystemUiVisibility(
				  View.SYSTEM_UI_FLAG_LAYOUT_STABLE
				  | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
				  | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
				  | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
				  | View.SYSTEM_UI_FLAG_FULLSCREEN
				  | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
		  }
    }
	
}
