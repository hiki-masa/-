#pragma once

#ifndef READ_TXT_HPP
#define READ_TXT_HPP

#include <stdio.h>
#include <string>
#include <vector>

// vector<char>型（文字列）からdouble型（値）の変換
template <typename T> double change_char_to_double(T str) {
	char* value_char = (char*)calloc(sizeof(char), str.size());
	double value;
	// vector<char>をchar配列に格納
	for (int i = 0; i < str.size(); i++) {
		value_char[i] = str[i];
	}

	value = (double)atof(value_char);
	// 使用したchar配列を開放
	free(value_char);

	return value;
}

// ファイルから値を読み取り、全ての値を配列で返す
template <typename T> void get_value(const char* filename, T& value) {
	// 読み取るファイルを開く
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Can't open the file : %s\n", filename);
		exit(0);
	}

	// 変数の初期化
	char tmp_ch;
	std::vector<char> str;

	// 値の読み取り開始
	while (true) {
		tmp_ch = fgetc(fp);

		if (tmp_ch == ' ' || tmp_ch == ',' || tmp_ch == '\n' || tmp_ch == EOF) {
			/* 値かどうかの確認 */
			if (change_char_to_double(str) != NULL) {
				// 読み取った値を格納
				value.push_back(change_char_to_double(str));
			}
			// 配列に関する変数を初期化
			str.erase(str.begin(), str.end());
			/* End Of Fileなら読み込みの終了 */
			if (tmp_ch == EOF) { break; }
		}
		else {
			str.push_back(tmp_ch);
		}
	}

	printf("read the file : %s\n", filename);
}

#endif