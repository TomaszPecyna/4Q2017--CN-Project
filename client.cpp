#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <iostream>

#define MAX 2

using namespace std;

string readFromADescriptor(int fd, int consoleMessage = 1);
string stringDecorator(string s);

int main()
{
	string myArrayString1 = "5,2,6,1\n0,6,2,0\n3,8,1,4\n1,8,5,6";
	string myArrayString2 = "7,5,8,0\n1,8,2,6\n9,4,3,8\n5,3,7,9";
	//string index = "6\n127284";
	string index = stringDecorator(myArrayString1 + myArrayString2);
	struct sockaddr_in addr;
	addr.sin_family = PF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = inet_addr("192.168.1.100");
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	int on = 1;
	setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on));
	connect(fd, (struct sockaddr*)&addr, sizeof(addr));



	if(write(fd, index.c_str(), index.size()*sizeof(char)) == -1)
	{
		perror("write error");
		exit(1);
	}
	string myMessage = readFromADescriptor(fd, 1);
	close(fd);
	//cout << myMessage;
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
