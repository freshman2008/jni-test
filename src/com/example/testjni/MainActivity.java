package com.example.testjni;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {
	private TextView tv;
	private Button btnHello;
	private Button btnWhat;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv = (TextView) this.findViewById(R.id.textView1);
        btnHello = (Button) this.findViewById(R.id.button1);
        btnHello.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				tv.setText(helloFromJNI());
			}
		});
        
        btnWhat = (Button) this.findViewById(R.id.button2);
        btnWhat.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				tv.setText(whatFromJNI("Good!"));
				setUpSSLJNI();
			}
		});
    }
    
    /**
     * 调用 Native c++ 的方法
     */
    public native String helloFromJNI();
    public native String whatFromJNI(String info);
    public native void setUpSSLJNI();

    /**
     * 库加载
     */
    static {
        System.loadLibrary("test_jni");
    } 
}
