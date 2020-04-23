package an.qt.Settings;
import java.lang.Runnable;
import android.view.Window;
import android.view.WindowManager;
import android.app.Activity;
import android.util.Log;

public class BrightnessRunnable implements Runnable {
    private Window window = null;
    private Activity activity = null;
    private float brightness = -1.0f;
    public BrightnessRunnable(Window w, int b){
        window = w;
        brightness = ((float)b)/100;
    }
    public BrightnessRunnable(Activity act, int b){
        activity = act;
        brightness = ((float)b)/100;
    }
    public void run(){
        if(activity != null){
            window = activity.getWindow();
        }
        WindowManager.LayoutParams lp = window.getAttributes();
        lp.screenBrightness = brightness;
        window.setAttributes(lp);
        Log.e("QtSettings", "BrightnessRunnable run");
    }
}