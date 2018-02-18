package com.example.dongjunjie.myapplication;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.graphics.Bitmap;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.res.AssetManager;
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
    private static final String scripts[] = {
        "javascript",
        "python",
        "lua"
    };
    private static final String tests[] = {
        "test.js",
        "win.py",
        "luatest.lua"
    };
    private static final int test_ids[] = {
        R.raw.test,
        R.raw.win,
        R.raw.luatest
    };
    private static final int SCRIPT_JS = 0;
    private static final int SCRIPT_PYTHON = 1;

    private static final int SHRINK = 4;

    private int mScriptType = -1;

    private MyView mMyView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mMyView = new MyView(this);
        setContentView(mMyView);

        selectScriptType();
        try {
            copyAssetDirToFiles(this, "python2.7");
        } catch(IOException e) {
            e.printStackTrace();
        }
    }

    private void selectScriptType() {
        AlertDialog.Builder singleChoiceDialog =
            new AlertDialog.Builder(ItemListActivity.this);
        singleChoiceDialog.setTitle("Select the script");
        singleChoiceDialog.setSingleChoiceItems(scripts, 0,
            new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                mScriptType = which;
                Log.i("MIUIGUI", "Select the script:" + scripts[mScriptType]);
            }
        });
        singleChoiceDialog.setPositiveButton("OK",
            new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                if (mScriptType == -1) {
                    mScriptType = 0;
                }
                Log.i("MIUIGUI", "Select the script:" + scripts[mScriptType]);
                mMyView.post(new Runnable() {
                    @Override
                    public void run() {
                        mMyView.startThread();
                    }
                });
            }
        });
        singleChoiceDialog.show();
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
        private boolean mIsMINIGUIRunning = false;
        private boolean mIsSurfaceCreated = false;

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
            mIsSurfaceCreated = true;
            if (mScriptType >= 0) {
                startThread();
            }
        }

        public void startThread() {
            if (mScriptType >= 0 && mIsSurfaceCreated) {
                try {
                    new Thread(new MyThread()).start();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
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
                mIsMINIGUIRunning = true;

                new Thread(new UpdateGALThread()).start();

                // TODO Auto-generated method stub
                while (processMessage(hwnd)) { }
                mIsMINIGUIRunning = false;
            }
        }

        class UpdateGALThread implements Runnable {
            @Override
            public void run() {

                while (mIsMINIGUIRunning) {
                    updateGAL(mBitmap);
                    Canvas canvas = holder.lockCanvas(null);//获取画布
                    if (canvas != null) {
                        canvas.drawBitmap(mBitmap, mSrcRt, mDstRt, null);
                        holder.unlockCanvasAndPost(canvas);//解锁画布，提交画好的图像
                        try {
                            Thread.sleep(1);
                        } catch (Exception e) {
                        }
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
        String script = tests[mScriptType];
        int script_id = test_ids[mScriptType];

        Log.i("MINIGUI", "startMiniGUI type="+scripts[mScriptType] + ",script="+script+", id="+script_id);

        if (mScriptType == SCRIPT_PYTHON) {
            setPythonPath(getFilesDir().getPath() + "/python2.7");
        }

        String file = "/data/" + script;
        String source;
        source = readFile(file);
        if (source == null) {
            file = "android://raw/" + script;
            source = readRaw(script_id);
        }

        if (source == null) {
            Log.i("MINIGUI", "cannot open script file:"+file);
            return 0;
        }

        return startMiniGUI(width, height, file, source);
    }

	public static void copyAssetDirToFiles(Context context, String dirname)
			throws IOException {
		File dir = new File(context.getFilesDir() + "/" + dirname);
        if (dir.exists()) {
            return ;
        }
		dir.mkdir();

		AssetManager assetManager = context.getAssets();
		String[] children = assetManager.list(dirname);
		for (String child : children) {
			child = dirname + '/' + child;
			String[] grandChildren = assetManager.list(child);
			if (0 == grandChildren.length)
				copyAssetFileToFiles(context, child);
			else
				copyAssetDirToFiles(context, child);
		}
	}

	public static void copyAssetFileToFiles(Context context, String filename)
			throws IOException {
		InputStream is = context.getAssets().open(filename);
		byte[] buffer = new byte[is.available()];
		is.read(buffer);
		is.close();

		File of = new File(context.getFilesDir() + "/" + filename);
		of.createNewFile();
		FileOutputStream os = new FileOutputStream(of);
		os.write(buffer);
		os.close();
        android.util.Log.i("MINIGUI", "Copy to " + context.getFilesDir() + "/" + filename);
	}


    private static native long startMiniGUI(int width, int height, String file, String source);
    private static native void updateGAL(Bitmap bmp);
    private static native boolean processMessage(long hwnd);
    private static native void updateTouchEvent(int x, int y, int button);
    private static native void setPythonPath(String path);
}
