#pragma once
#include <iostream>
#include <stdio.h>
#include <windows.h>

class Crypto {
public:
	static string Encrypt(string to_enc) {
		if (to_enc == "")
			return "";
		
		string pass = "Zargarov";
		string command = "echo " + to_enc + " | openssl\\bin\\openssl.exe aes-128-ecb -pbkdf2 -a -A -pass pass:" + pass + " > crptmp";

		system(command.c_str());

		fstream crptmpf("crptmp", fstream::in);
		getline(crptmpf, to_enc);

		crptmpf.close();
		remove("crptmp");

		return to_enc;
	}

	static string Decrypt(string to_dec) {
		if (to_dec == "")
			return "";

		string pass = "Zargarov";

		string command = "echo " + to_dec + " | openssl\\bin\\openssl.exe aes-128-ecb -pbkdf2 -a -d -pass pass:" + pass + " > crptmp";
		system(command.c_str());
		
		

		fstream crptmpf("crptmp", fstream::in);
		getline(crptmpf, to_dec);
		crptmpf.close();
		remove("crptmp");
		return to_dec;
	}
};