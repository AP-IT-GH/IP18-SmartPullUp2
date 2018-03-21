package com.smartpullup.smartpullup;


import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.RelativeLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.google.firebase.auth.FirebaseAuth;

import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivityJSON";

    @SuppressLint("HandlerLeak")
    private FirebaseAuth mAuth;

    private SectionsPagerAdapter mSectionsStatePagerAdapter;
    private ViewPager mViewPager;

    public final ExerciseFragment ExerciseFragment = new ExerciseFragment();
    public final LeaderboardFragment LeaderboardFragment = new LeaderboardFragment();
    public final ProfileFragment ProfileFragment = new ProfileFragment();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //firebase Authentication
        mAuth = FirebaseAuth.getInstance();

        //navigation
        mSectionsStatePagerAdapter = new SectionsPagerAdapter(getSupportFragmentManager());

        mViewPager =(ViewPager) findViewById(R.id.container);
        setupViewPager(mViewPager);

        TabLayout tabLayout = (TabLayout) findViewById(R.id.bottomNav);
        tabLayout.setupWithViewPager(mViewPager);

        tabLayout.getTabAt(0).setIcon(R.drawable.ic_home);
        tabLayout.getTabAt(1).setIcon(R.drawable.ic_podium);
        tabLayout.getTabAt(2).setIcon(R.drawable.ic_person);

    }

    private void setupViewPager(ViewPager viewPager){
        SectionsPagerAdapter adapter = new SectionsPagerAdapter(getSupportFragmentManager());
        adapter.addFragment(ExerciseFragment);
        adapter.addFragment(LeaderboardFragment);
        adapter.addFragment(ProfileFragment);
        viewPager.setAdapter(adapter);

    }

    public void ConnectToBar(View view) {
        Intent intentConnect = new Intent(MainActivity.this, ConnectBarActivity.class);
        startActivity(intentConnect);
    }

    @Override
    public void onPause()
    {
        super.onPause();

        Intent MyIntentService = new Intent(this, BTReceiverService.class);
        stopService(MyIntentService );
    }

}
