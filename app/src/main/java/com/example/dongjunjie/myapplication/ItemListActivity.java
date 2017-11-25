package com.example.dongjunjie.myapplication;

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
                long hwnd = startMiniGUI(mBitmap.getWidth(), mBitmap.getHeight(), null);

                // TODO Auto-generated method stub
                while (processMessage(hwnd)) {
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

    private static native long startMiniGUI(int width, int height, String[] args);
    private static native void updateGAL(Bitmap bmp);
    private static native boolean processMessage(long hwnd);
    private static native void updateTouchEvent(int x, int y, int button);
}
