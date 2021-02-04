#pragma once

#ifndef READ_TXT_HPP
#define READ_TXT_HPP

#include <stdio.h>
#include <string>
#include <vector>

// vector<char>�^�i������j����double�^�i�l�j�̕ϊ�
template <typename T> double change_char_to_double(T str) {
	char* value_char = (char*)calloc(sizeof(char), str.size());
	double value;
	// vector<char>��char�z��Ɋi�[
	for (int i = 0; i < str.size(); i++) {
		value_char[i] = str[i];
	}

	value = (double)atof(value_char);
	// �g�p����char�z����J��
	free(value_char);

	return value;
}

// �t�@�C������l��ǂݎ��A�S�Ă̒l��z��ŕԂ�
template <typename T> void get_value(const char* filename, T& value) {
	// �ǂݎ��t�@�C�����J��
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Can't open the file : %s\n", filename);
		exit(0);
	}

	// �ϐ��̏�����
	char tmp_ch;
	std::vector<char> str;

	// �l�̓ǂݎ��J�n
	while (true) {
		tmp_ch = fgetc(fp);

		if (tmp_ch == ' ' || tmp_ch == ',' || tmp_ch == '\n' || tmp_ch == EOF) {
			/* �l���ǂ����̊m�F */
			if (change_char_to_double(str) != NULL) {
				// �ǂݎ�����l���i�[
				value.push_back(change_char_to_double(str));
			}
			// �z��Ɋւ���ϐ���������
			str.erase(str.begin(), str.end());
			/* End Of File�Ȃ�ǂݍ��݂̏I�� */
			if (tmp_ch == EOF) { break; }
		}
		else {
			str.push_back(tmp_ch);
		}
	}

	printf("read the file : %s\n", filename);
}

#endif