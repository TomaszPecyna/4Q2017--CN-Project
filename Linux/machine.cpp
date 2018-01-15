#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#define MAX 200

using namespace std;

string readFromADescriptor(int fd, int consoleMessage = 1);
void multiplicationHandle(int fd2);
string stringDecorator(string s);
int multiplyFieldInMatrix(vector<vector<int>> m);
vector<vector<int>> csvToVectorMatrix(string str);
void readData(char** argv);

int PORT;
string IP;
int MACHINE;


int main(int co, char** argv)
{
	/* initial socket setup */	
	readData(argv);
	socklen_t x;
	struct sockaddr_in addr,addr2;
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	addr.sin_family = PF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = INADDR_ANY;

	int on = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	
	bind (fd, (struct sockaddr*)&addr, sizeof(addr));	

	listen(fd,5);

	while(1)
	{
		/* wait for a connection */
		int fd2 = accept(fd, (struct sockaddr*)&addr2, &x);
		setsockopt(fd2, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	
		multiplicationHandle(fd2);
	}
	close(fd);

}

void multiplicationHandle(int fd2)
{
	char buf [MAX];
	string message;
	int nbytes;

	/* start handling request */
	while(1)
	{
		try
		{
			message = readFromADescriptor(fd2, 1);
		}
		catch(const std::exception& e)
		{
			break;
		}
		if(message.compare("end") == 0) break;
		vector<vector<int>> toMultiply = csvToVectorMatrix(message);
		int field = multiplyFieldInMatrix(toMultiply);
		string toSend = stringDecorator(to_string(field));

		/* random sleep for complex computations simulation */
		srand ( time(NULL) );
		int randomTime = rand() %5;
		sleep(randomTime);
	
		/* sending return value */
		if(write(fd2, toSend.c_str(), toSend.size()*sizeof(char)) == -1)
		{
			perror("write error");
			exit(1);
		}
	}	
	
	close(fd2);
}

string readFromADescriptor(int fd, int consoleMessage)
{
	char buf[MAX];
	int nbytes;
	int bytesRead = 0;
	string returnMessage = "";
	string lengthMessage = "";
	string messageRead;
	int pos = 0;
	while((nbytes = read(fd, buf, sizeof(buf))) > 0)
	{
		string messageReadTMP = buf;
		messageRead = messageReadTMP.substr(0, MAX);
		if((pos = messageRead.find("\n")) >= 0)
		{
			lengthMessage.append(messageRead.substr(0, pos));
			if(consoleMessage == 1)
			{
				cout << "--------------";
				cout << "\nsize of file: " << lengthMessage;
			}
			returnMessage.append(messageRead.substr(pos+1));
			bytesRead += messageRead.size() - pos - 1;
			break;
		}
		lengthMessage.append(messageRead);
	}

	do
	{
		if(nbytes == MAX)
		{
			nbytes = read(fd, buf, sizeof(buf));
		}
		string messageReadTMP = buf;
		returnMessage.append(messageReadTMP.substr(0, nbytes));
		bytesRead += nbytes;
		if(bytesRead >= stoi(lengthMessage))
		{
			returnMessage = returnMessage.substr(0, stoi(lengthMessage));
			if(consoleMessage == 1)
			{
				cout << "\nmessage read: " << returnMessage;
				cout << "\n--------------\n";
			}
		}
	}while(bytesRead < stoi(lengthMessage));

	if(nbytes == -1)
	{
		perror("read error");
		exit(1);
	}
	return returnMessage;
}

string stringDecorator(string s)
{
	size_t size = s.size();
	string decorated = to_string(size);
	decorated.append("\n");
	return decorated.append(s);
}

vector<vector<int>> csvToVectorMatrix(string str)
{
	vector<int>* row = new vector<int>;	
	vector<vector<int>>* matrix = new vector<vector<int>>;
	std::stringstream ss(str);
	int i;
	while (ss >> i)
	{
		row->push_back(i);
		if (ss.peek() == '\n' || ss.peek() == EOF)
		{
			matrix->push_back(*row);
			row = new vector<int>;
		}
		if (ss.peek() == ',')
			ss.ignore();
	}
	return *matrix;
}

int multiplyFieldInMatrix(vector<vector<int>> m)
{
	int sum = 0;
	for (size_t i = 0; i < m.at(0).size(); i++)
	{
		sum += m.at(0).at(i) * m.at(1).at(i);
	}
	return sum;
}

void readData(char** argv)
{
	int port;
	string ip;
	ifstream file;
	int machine = stoi(string(1, *(argv[1])));
	MACHINE = machine;
	file.open("data", ios::out);
	while(file >> machine >> port >> ip)
	{
		if(MACHINE == machine)
		PORT = port;
		IP = ip;
	}
	cout << MACHINE << " " << PORT << " " << IP << endl;
}