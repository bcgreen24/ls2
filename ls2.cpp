#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <unistd.h>

using namespace std;

float round_num(float var);
string num2bytestring(long long size);

int main(int argc, char **argv)
{
	// initialization
	struct stat sb;
    struct dirent *de;
    struct file_info{
    	string owner;
    	long long size;
    	string date_modified;
    	string filename;
    };
    vector <file_info> file_entries;
    vector <string> filenames;
    DIR *dr;
    char temp[255];
    char path[255] = "";
	char* cwd = getcwd(temp, sizeof(temp));

    if(argc == 1){
    	dr = opendir(".");
    }else if(argc == 2) {
    	strcat(path, temp);
    	strcat(path, "/");
    	strcat(path, argv[1]);
    	dr = opendir(path);
    	if (chdir(path) != 0){
    		cout << "Could not change directories." << endl;
    		return 1;
    	}
    }

    if (dr == NULL)
    {
        printf("Could not open directory" );
        return 0;
    }

    // print column headers
    cout << left << setw(14) << "Owner\t";
    cout << left << setw(4) << "Size\t\t";
    cout << left << setw(31) << "Date Modified\t";
    cout << setw(23) << "Filename" << endl;
    cout << "==========================================================================================" << endl;

    // file file_entries vector with file_info structs
    while ((de = readdir(dr)) != NULL)
        if ( stat(de->d_name, &sb ) == 0 ) {
        	char *t = ctime(&sb.st_mtime);
        	t[strcspn(t, "\r\n")] = '\0';

        	file_info f_info;
        	f_info.owner = to_string(sb.st_uid);
        	f_info.filename = de->d_name;
        	f_info.size = sb.st_size;
        	f_info.date_modified = t;

        	file_entries.push_back(f_info);
        }
        else {
            cout << "Couldn't stat file: " << de->d_name << endl;
        }

    // file filenames vector with filenames - used for sorting
    for(int i = 0; i < file_entries.size(); i++) {
    	filenames.push_back(file_entries[i].filename);
    }

    sort(filenames.begin(), filenames.end());

    for(int i = 0; i < filenames.size(); i++){
    	for(int j = 0; j < file_entries.size(); j++){
    		if(filenames[i] == file_entries[j].filename){
				cout << setw(16) << file_entries[j].owner;
				cout << setw(16) << num2bytestring(file_entries[j].size);
				cout << setw(24) << file_entries[j].date_modified << "\t";
				cout << file_entries[j].filename << endl;
				break;
			}
    	}
    }
    closedir(dr);
    cout << file_entries.size() << " files listed." << endl;
    return 0;
}

string num2bytestring(long long size) {
	string bytestring = "";
	if(size < 1024) {
		bytestring = to_string(size) + "B";
	}else if(size >= 1000 && size < (1000*1000)) {
		bytestring = to_string(size/1000) + "KB";
	}else{
		bytestring = to_string(size/1000/1000) + "MB";
	}
	return bytestring;
}

float round_num(float var)
{
	float value = (int)(var * 100 + .5);
	return (float)value / 100;
}



