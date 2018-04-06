package com.smartpullup.smartpullup;

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.facebook.login.LoginManager;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

/**
 * Created by bjorn on 1/03/2018.
 */

public class ProfileFragment extends Fragment {
    private static final String TAG = "FragmentProfile";

    private Button btnLogout;
    private TextView userNameTxtView;
    private TextView userEmailTxtView;

    private String userName;
    private String userEmail;

    private String userId;

    private FirebaseAuth mAuth;
    private FirebaseDatabase database;
    FirebaseUser currentUser;
    private DatabaseReference userDatabase;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_profile, container, false);

        btnLogout = (Button)view.findViewById(R.id.btn_Logout);
        userNameTxtView = (TextView) view.findViewById(R.id.userName_textView);
        userEmailTxtView = (TextView) view.findViewById(R.id.userEmail_textView);

        database = FirebaseDatabase.getInstance();
        userDatabase = database.getReference("Users");

        mAuth = FirebaseAuth.getInstance();
        currentUser = mAuth.getCurrentUser();

        userId = currentUser.getUid();

        userDatabase.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {

               Log.d("User", userId);
               Log.d("User",dataSnapshot.getValue().toString());

               userName = dataSnapshot.child(userId).child("firstName").getValue().toString() + " " + dataSnapshot.child(userId).child("lastName").getValue().toString();
               userEmail =  dataSnapshot.child(userId).child("email").getValue().toString();

               userNameTxtView.setText(userName);
               userEmailTxtView.setText(userEmail);

            }

            @Override
            public void onCancelled(DatabaseError databaseError) {

                Toast.makeText(getContext(),databaseError.getMessage(),
                        Toast.LENGTH_SHORT).show();
            }
        });


        btnLogout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Logout(view);
            }
        });

        return view;
    }

    @Override
    public void onStart() {
        super.onStart();


    }

    public void Logout(View v){
        mAuth.signOut();
        LoginManager.getInstance().logOut();
        Intent i = new Intent(getActivity(), LoginActivity.class);
        startActivity(i);
        getActivity().finish();
    }
}
