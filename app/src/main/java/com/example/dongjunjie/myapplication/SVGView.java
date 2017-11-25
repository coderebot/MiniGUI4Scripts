package com.example.dongjunjie.myapplication;

import android.graphics.Canvas;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.content.Context;
import android.util.AttributeSet;
import android.view.View;

public class SVGView extends View {
    static {
        System.loadLibrary("render");
    }

    private Bitmap mBitmap;
    public SVGView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public SVGView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public SVGView(Context context) {
        this(context, null);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        verifyBitmap();
        nativeUpdateBitmap(mBitmap);

        canvas.drawBitmap(mBitmap, 0, 0, null);
    }


    private void verifyBitmap() {
        if (mBitmap != null) {
            int width = mBitmap.getWidth();
            int height = mBitmap.getHeight();
            if (width == getWidth() && height == getHeight()) {
                return ;
            }

            mBitmap.recycle();
        }
        mBitmap = Bitmap.createBitmap(getWidth(), getHeight(), Bitmap.Config.ARGB_8888);
    }

    private static native void nativeUpdateBitmap(Bitmap bitmap);
}
