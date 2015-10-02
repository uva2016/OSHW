#include <vector>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

using namespace std;

bool isValid(string line) {
for(int i = 0; i < line.size(); i++) {
        if(line[i] != (char) 60) {
        if(line[i] != (char) 62) {
        if(line[i] != (char) 124) {
        if(line[i] != (char) 45) {
        if(line[i] != (char) 46) {
        if(line[i] != (char) 47) {
        if(line[i] != (char) 95) {
        if(line[i] != (char) 32) {
        if(line[i] < (char) 65 || line[i] > (char) 90 ) {
        if(line[i] < (char) 97 || line[i] > (char) 122 ) {
	if(line[i] < (char) 48 || line[i] > (char) 57) {
		return false;
	}
        }
        }
        }
        }
	}
	}
	}
	}
	}
	}
}
return true;
}

string reduce(string& input) {
	size_t initial = input.find_first_not_of(' ');
	size_t final = input.find_last_not_of(' ');
	if(initial == - 1 || final == -1){
		return "";
	}
	return input.substr(initial, (final - initial + 1));
}

int toArray(string line, string res[]) {
	int pointer1 = 0;
	int pointer2 = 0;
	int current = 0;


	for(int i = 0; i < line.size(); ++i) {

		if(line[i] == 32){
			pointer2 = i;
			int difference = pointer2 - pointer1;
			string temp = line.substr(pointer1, difference);
			string temp2 = reduce(temp);
			if (temp2.length() > 0) {
				res[current] = temp;
				current++;
			}

			pointer1 = pointer2 + 1;
			//cout << "Added to the array" << endl;
		}
		if(i == line.size() - 1){
			pointer2 = i;
			int difference = pointer2 - pointer1 + 1;
			string temp = line.substr(pointer1, difference);
			string temp2 = reduce(temp);
			if (temp2.length() > 0) {
				res[current] = temp;
				current++;
			}
		}

	}
	for(int j = 0; j < 100; ++j){
		if(res[j].length() > 0) {
			//cout << res[j] << endl;
		}
	}

}

int inputRedirect(string line) {
	int count = 0;
	for(int i = 0; i < line.size(); i++) {
	        if(line[i] == (char) 60) {
			count++;
		}
	}
	return count;
}

int outputRedirect(string line) {
	int count = 0;
        for(int i = 0; i < line.size(); i++) {
                if(line[i] == (char) 62) {
                        count++;
                }
        }
        return count;
}


int pipe(string line) {
	int count = 0;
	for(int i = 0; i < line.size(); i++) {
                if(line[i] == (char) 124) {
                        count++;
                }
        }
        return count;
}

vector<int> getCmds(string res[]) {
        vector<int> toks;
	toks.push_back(0);
	int i = 0;
	while(res[i] != "") {
                if(res[i] == "|") {
                        toks.push_back(i+1);
                }
		i++;
        }
        return toks;
}

bool lineCheck(string line) {
	if(!isValid(line)) {
		cout << "Invalid Command - Invalid characters are present." << endl;		
		return false;
        }

	if(line.size() > 100) {
		cout << "Invalid Command - Command is greater than 100 characters." << endl;		
		return false;
        }

	if((inputRedirect(line)>=1 || outputRedirect(line)>=1) && pipe(line)>=1) {
		cout << "Invalid Command - There are both pipes and redirects in the command." << endl;                
		return false;
        }

	if(line[line.size() - 1] == '|' || line[line.size() - 1] == '<' || line[line.size() - 1] == '>') {
		cout << "Invalid Command - An operator is at the end of the command." << endl;		
		return false;
	}

	if(inputRedirect(line) >= 2 || outputRedirect(line) >= 2) {
		cout << "Invalid Command - There are 2+ redirect operators in one command." << endl;		
		return false;
	}

	if(line[0] == '>' || line[0] == '<' || line[0] == '|') {
		cout << "Invalid Command - An operator is at the beginning of the command." << endl;	
		return false;
	}

	if(line.find('>') < line.find('<') && line.find('<') < 200) {
		cout << "Invalid Command - Redirect operators are out of order." << endl;
	}
	return true;
}

main() {
while (1) {
	string line, found;
	getline (cin, line);

	if(!lineCheck(line)) {
		continue;
	}

	string res [100];
	toArray(line, res);

	int i = 0;
	bool flag = false;
	int resSize;
	while(res[i] != "") {
		if(res[i].size() > 1) {
			if(res[i].find('|') < 100 || res[i].find('<') < 100 || res[i].find('>') < 100) {
				cout << "Invalid Command - Operators aren't separated by whitespace." << endl;
				flag = true;
			}
		}
		if(res[i] == "<") {
			if(res[i+2] != ">" && res[i+2] != "") {
				cout << "Invalid Command - Input redirect should be followed by an input file, not a command." << endl;
				flag = true;
			}
		}
		if(res[i] == ">") {
			if(res[i+2] != "") {
				cout << "Invalid Command - Output redirect should be followed by an output file, not a command." << endl;
				flag = true;
			}
		}
		i++;
	}
	resSize = i;
	if(flag) {
		continue;
	}

	vector<int> cmds = getCmds(res);
	for(int i = 0; i < cmds.size(); i++) {
		cout << "Working on cmd" << endl;
		int first = cmds[i];
		int last, in, out;
		string inputStr, outputStr;
		bool changeInput = false;
		bool changeOutput = false;
		vector<string> arguments(0);

		if(i == cmds.size()-1) {
			last = resSize;
		} else {
			last = cmds[i+1]-1;
		}
		string command  = res[first];
		for(int k = first; k < last; k++) {
			if(res[k] == "<") {
				inputStr = res[k+1];
                                changeInput = true;
				continue;
			}
			else if(res[k] == ">") {
				outputStr = res[k+1];
                                changeOutput = true;
				continue;
			}
			else {
				arguments.push_back(res[k]);
			}
		}

		int pid = fork();
		if(pid == 0) { //child
			if(changeInput) {
				const char * is = inputStr.c_str();
				in = open(is, O_RDONLY);
				dup2(in, 0);
				close(in);
			}
			if(changeOutput) {
				const char * os = outputStr.c_str();
				out = open(os, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
				dup2(out, 1);
				close(out);
			}

			vector<char *> argv(arguments.size() + 1);

			for (std::size_t j = 0; j != arguments.size(); ++j) {
			    argv[j] = &arguments[j][0];
			}

			execvp(argv[0], argv.data());


		} else { //parent
			//pid_t waitpid(pid, 0);
		}
	}

} //end while
return 0;
} //end main
