package com.example.dongjunjie.myapplication;

import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.graphics.Bitmap;
import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.view.MotionEvent;
import android.util.Log;

public class ItemListActivity extends Activity {
    static {
        System.loadLibrary("minigui");
    }

    private static final int SHRINK = 4;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new MyView(this));
    }


    private  class MyView extends SurfaceView implements SurfaceHolder.Callback
    {

        /**
         * @param context
         */
        SurfaceHolder holder;
        private Bitmap mBitmap;
        private Rect  mSrcRt;
        private Rect  mDstRt;
        private boolean mIsMiniGUIRunning = false;

        public MyView(Context context) {
            super(context);
            // TODO Auto-generated constructor stub
            holder=this.getHolder();
            holder.addCallback(this);
        }
        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width,
                int height) {
            // TODO Auto-generated method stub
        }
        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            // TODO Auto-generated method stub
            verifyBitmap();
            new Thread(new MyThread()).start();
        }
        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            // TODO Auto-generated method stub
        }
        class MyThread implements Runnable
        {

            /* (non-Javadoc)
             * @see java.lang.Runnable#run()
             */
            @Override
            public void run() {
                long hwnd = startMiniGUI(mBitmap.getWidth(), mBitmap.getHeight());
                mIsMiniGUIRunning = true;

                new Thread(new UpdateGALThread()).start();

                // TODO Auto-generated method stub
                while (processMessage(hwnd)) { }
                mIsMiniGUIRunning = false;
            }
        }

        class UpdateGALThread implements Runnable {
            @Override
            public void run() {

                while (mIsMiniGUIRunning) {
                    updateGAL(mBitmap);
                    Canvas canvas = holder.lockCanvas(null);//获取画布  
                    canvas.drawBitmap(mBitmap, mSrcRt, mDstRt, null);
                    holder.unlockCanvasAndPost(canvas);//解锁画布，提交画好的图像  
                    try {
                        Thread.sleep(1);
                    } catch (Exception e) {
                    }
                }
            }
        }


        private void verifyBitmap() {
            if (mBitmap != null) {
                int width = mBitmap.getWidth();
                int height = mBitmap.getHeight();
                if (width == (int)(getWidth()/SHRINK) && height == (int)(getHeight()/SHRINK)) {
                    return ;
                }

                mBitmap.recycle();
            }
            if (mSrcRt == null) {
                mSrcRt = new Rect();
            }
            if (mDstRt == null) {
                mDstRt = new Rect();
            }

            mSrcRt.set(0, 0, getWidth()/SHRINK, getHeight()/SHRINK);
            mDstRt.set(0, 0, getWidth(), getHeight());
            mBitmap = Bitmap.createBitmap(getWidth()/SHRINK, getHeight()/SHRINK, Bitmap.Config.RGB_565);
        }

        public boolean onTouchEvent(MotionEvent event) {
            float x = event.getX()/SHRINK;
            float y = event.getY()/SHRINK;

            int button = 0;
            int action = event.getAction();
            if (action == MotionEvent.ACTION_DOWN || action == MotionEvent.ACTION_MOVE) {
                button = 1;
            }

            updateTouchEvent((int)x, (int)y, button);
            return true;
        }
    }

    private static String readFile(String file_name) {
        try {
            File file = new File(file_name);
            FileInputStream fs = new FileInputStream(file);
            byte[] b = new byte[fs.available()];
            fs.read(b);
            return new String(b);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    private String readRaw(int id) {
        try {
            InputStream is = getResources().openRawResource(id);
            byte[] buffer = new byte[is.available()];
            is.read(buffer);
            String result = new String(buffer);
            is.close();
            return result;
        } catch (Exception e) {
        }
        return null;
    }

    private long startMiniGUI(int width, int height) {
        String file = "/data/test.js";
        String source;
        source = readFile(file);
        if (source == null) {
            file = "android://raw/test.js";
            source = readRaw(R.raw.test);
        }

        if (source == null) {
            Log.i("MiniGUI V8", "cannot open script file:"+file);
            return 0;
        }

        return startMiniGUI(width, height, file, source);
    }

    private static native long startMiniGUI(int width, int height, String file, String source);
    private static native void updateGAL(Bitmap bmp);
    private static native boolean processMessage(long hwnd);
    private static native void updateTouchEvent(int x, int y, int button);
}
