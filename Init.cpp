#include "iostream"
#include "stdio.h"
#include "unistd.h"
#include "string"
#include "vector"
#include "fstream"
#include "boost/asio.hpp"
#include "boost/array.hpp"
#include "boost/regex.hpp"
#include "boost/algorithm/string/regex.hpp"

using namespace std;

int main(int argc,char *argv[]){

	block:
	try{
		string port = "";

		int i = 0;
		while(i < 2){
			FILE *file = popen("bash /.../2.sh","r");
			if(file){
				i = 0;
				char buffer[1024];
				while(!feof(file)){
					if(fgets(buffer,sizeof(buffer),file) != NULL){
						port = "/dev/" + boost::regex_replace(string(buffer),boost::regex("\n"),"");

						i = 2;

						break;
					}

					i++;
				}
			}
			fclose(file);
		}

		boost::asio::io_service io_service;
		boost::asio::serial_port serial_port(io_service,port);

		boost::system::error_code error;
		serial_port.set_option(boost::asio::serial_port_base::baud_rate(115200),error);

		if(error){
			cerr << port << endl;
			throw boost::system::system_error(error);
		}
		
		sleep(2);

		bool on = false;

		while(true){
			boost::array<char, 1> buffer;
			try{
				boost::system::error_code error2;
				char ch;
				boost::asio::read(serial_port,boost::asio::buffer(&ch,1),error2);
//				string read(buffer.begin(),buffer.begin()+1);
				
				if(ch == 'm'){
					if(on == false){
						system("xset -display :0.0 dpms force on");
						on = true;
					}
				}
				if(ch == 'n'){
					on = false;
				}

				if(error2){
					throw boost::system::system_error(error2);
				}
			}catch(exception &exception2){
				serial_port.close();
				cout << "Read exception" << endl;
				cout << exception2.what() << endl;
				goto block;
			}

			FILE *command = popen("/.../power_info","r");
			bool charging = false;
			if(command){
				char buffer[1024];
				while(!feof(command)){
					if(fgets(buffer,sizeof(buffer),command) != NULL){
						if(string(buffer).find(" charging") != string::npos){
							charging = true;
						}

						if(string(buffer).find("percentage") != string::npos){
							vector<string> split;
							boost::algorithm::split_regex(split,boost::regex_replace(string(buffer),boost::regex("\\s+"),""),boost::regex(":"));
							int percentage = atoi(boost::regex_replace(split.at(1),boost::regex("%"),"").c_str());

							if(percentage <= 50 && !charging){
								try{
									boost::system::error_code error3;
									boost::asio::write(serial_port,boost::asio::buffer("1"),error3);
								}catch(exception &ex3){
									cout << ex3.what() << endl;
									goto block;
								}
							}

							if(percentage == 100){
								try{
									boost::system::error_code error3;
									boost::asio::write(serial_port,boost::asio::buffer("0"),error3);
								}catch(exception &ex3){
									cout << ex3.what() << endl;
									goto block;
								}
							}
						}
					}
				}
			}

			fclose(command);
			
			ifstream file("/.../Arduino_command");
			string r;
			file >> r;
			file.close();
			
			if(strlen(r.c_str()) > 0){
				boost::asio::write(serial_port,boost::asio::buffer(r.c_str(),strlen(r.c_str())));
	
				ofstream file2("/.../Arduino_command");
				file2 << "";
				file2.close();
			}

			usleep(100000);
		}

	}catch(exception &ex){
		cerr << "1\t" << ex.what() << endl;
		goto block;
	}

	return 0;
}
