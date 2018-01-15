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


#define MAX 2

using namespace std;

string readFromADescriptor(int fd, int consoleMessage = 1);
void threadClientHandle(int cfd);
string stringDecorator(string s);
vector<vector<int>> csvToVectorMatrix(string str);
string takeOneRowOneColForField(vector<vector<int>> m1, vector<vector<int>> m2, int row, int col);
string matrixToCsv(vector<vector<int>> m);

class Machine
{
	int machineNumber;
	int port;
	string ip;
	int processedRow;
	int processedCol;
	int result;
	bool flag;
	int fd;

	socklen_t x;
	struct sockaddr_in maddr;
	int on;
	int mfd;

public:
	static int maxFd;
	Machine(int machineNumber, int port, string ip)
	{
		this->machineNumber = machineNumber;
		this->port = port;
		this->ip = ip;

		maddr.sin_family = PF_INET;
		maddr.sin_port = htons(port);
		maddr.sin_addr.s_addr = inet_addr(ip.c_str());
		mfd = socket(PF_INET, SOCK_STREAM, 0);
		on = 1;
		setsockopt(mfd,SOL_SOCKET, SO_REUSEADDR, (char*) &on, sizeof(on));
		connect(mfd, (struct sockaddr*)&maddr, sizeof(maddr));
		fd = mfd;
		if(fd > maxFd) maxFd = fd;
	}
	
	void setMachineNumber(int machineNumber)
	{
		this->machineNumber = machineNumber;
	}
	int getMachineNumber()
	{
		return machineNumber;
	}

	void setPort(int port)
	{
		this->port = port;
	}
	int getPort()
	{
		return port;
	}

	void setProcessedRow(int processedRow)
	{
		this->processedRow = processedRow;
	}
	int getProcessedRow()
	{
		return processedRow;
	}

	void setProcessedCol(int processedCol)
	{
		this->processedCol = processedCol;
	}
	int getProcessedCol()
	{
		return processedCol;
	}

	void setResult(int result)
	{
		this->result = result;
	}
	int getResult()
	{
		return result;
	}

	void setFd(int fd)
	{
		this->fd = fd;
	}
	int getFd()
	{
		return fd;
	}

	void setIp(string ip)
	{
		this->ip = ip;
	}
	string getIp()
	{
		return ip;
	}

	void setFlag(bool flag)
	{
		this->flag = flag;
	}
	bool isFlag()
	{
		return flag;
	}
};

vector<Machine>* readData();

int Machine::maxFd = 0;

int main()
{
	/* setting connection options */
	socklen_t x;
	struct sockaddr_in addr,addr2;
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	addr.sin_family = PF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = INADDR_ANY;

	int on = 1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	
	bind (fd, (struct sockaddr*)&addr, sizeof(addr));	

	listen(fd,5);

	/* wait for a new client and create a new thread if found */
	while(1)
	{
		int fd2 = accept(fd, (struct sockaddr*)&addr2, &x);
		setsockopt(fd2, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	
		thread myThread(threadClientHandle, fd2);
		myThread.detach();
	}
	close(fd);

}


void threadClientHandle(int cfd)
{
	/* needed variables declaration */
	vector<Machine>* machines = readData();
	int numberOfMachines = machines->size();
	int initializedMachines = 0;
	static struct timeval tTimeout;
	fd_set fsMask, fsRmask, fsWmask;
	int nMaxfd, nFound;
	string theEnd = stringDecorator("end");

	string tmpMessage = readFromADescriptor(cfd, 1);
	string myArrayString1 = tmpMessage.substr(0, tmpMessage.size()/2);
	string myArrayString2 = tmpMessage.substr(tmpMessage.size()/2);

	vector<vector<int>> myArrayVector1 = csvToVectorMatrix(myArrayString1);
	vector<vector<int>> myArrayVector2 = csvToVectorMatrix(myArrayString2);

	/* initialization of empty (for now) result matrix */
	vector<vector<int>>* result = new vector<vector<int>>;
	for (int i = 0; i < myArrayVector1.size(); i++)
	{
		vector<int>* row = new vector<int>;
		for (int i = 0; i < myArrayVector1.size(); i++)
		{
			row->push_back(-1);
		}
		result->push_back(*row);
	}
	int mfd;

	mfd = machines->at(0).getFd();

	FD_ZERO(&fsMask);
   	FD_ZERO(&fsRmask);
   	nMaxfd = machines->at(0).maxFd;
	fsWmask = fsMask;
   	tTimeout.tv_sec = 5;
       	tTimeout.tv_usec = 0;

	string fieldString;

	/* connecting to machines and sending them initial data to compute */
	
	int i = -1;
	int j = 0;
	for(; initializedMachines < numberOfMachines && i < int(myArrayVector1.size());)
	{
		i++;
		j = 0;
		for(; initializedMachines < numberOfMachines && j < myArrayVector1.size(); j++)
		{
			fieldString = stringDecorator(takeOneRowOneColForField(myArrayVector1, myArrayVector2, i, j));
			machines->at(initializedMachines).setProcessedRow(i);
			machines->at(initializedMachines).setProcessedCol(j);
			machines->at(initializedMachines).setFlag(true);
			int fd = machines->at(initializedMachines).getFd();
			FD_SET(fd, &fsRmask);
			if(write(fd, fieldString.c_str(), fieldString.size()*sizeof(char)) == -1)
			{
				perror("write error");
				exit(1);
			}
			initializedMachines++;
		}
	}
	string tmpMyMessage = "";
	Machine* currentMachine;

	for (; i < myArrayVector1.size();)
	{
		for (; j < myArrayVector1.size();)
		{
			tTimeout.tv_sec = 5;
       			tTimeout.tv_usec = 0;
			
			/* simple select function waiting for ready to read descriptor, which means that machine is ready to be used again */
			nFound = select(nMaxfd + 1, &fsRmask, &fsWmask, (fd_set*) 0, &tTimeout);

	
			if (nFound < 0)
			{
				perror("select error");
				exit(1);
			}
			if (nFound == 0)
			{
				write(1, "timeout error", 14);
			}
			for(int nFd = 0; nFd <= nMaxfd; nFd++)
			{
				if (FD_ISSET(nFd, &fsRmask))
				{
					/* checking which machine is ready to be used */
					for (size_t m = 0; m < numberOfMachines; m++)
					{
						if (machines->at(m).getFd() == nFd) 
						{						
							currentMachine = &machines->at(m);
							
						}
					}
					tmpMyMessage = readFromADescriptor(nFd, 1);
					currentMachine->setFlag(false);
					result->at(currentMachine->getProcessedRow()).at(currentMachine->getProcessedCol()) = stoi(tmpMyMessage);
					
					FD_SET(nFd, &fsRmask);
					fieldString = stringDecorator(takeOneRowOneColForField(myArrayVector1, myArrayVector2, i, j));
					if(write(nFd, fieldString.c_str(), fieldString.size()*sizeof(char)) == -1)
					{
						perror("write error");
						exit(1);
					}
					currentMachine->setProcessedRow(i);
					currentMachine->setProcessedCol(j);
					currentMachine->setFlag(true);
					j++;
					if(i < myArrayVector1.size() && j >= myArrayVector1.size())
					{
						j = 0;
						i++;
					}
					else break;
					if (i >= myArrayVector1.size())
					{
						j = myArrayVector1.size();
						break;
					}
				}
				
			}
			for (size_t m = 0; m < numberOfMachines; m++)
			{					
				FD_SET(machines->at(m).getFd(), &fsRmask);
			}
		}
	}

	/* telling machines that their work with this matrix is done */
	for (size_t m = 0; m < numberOfMachines; m++)
	{
		if (machines->at(m).isFlag()) 
		{
			currentMachine = &machines->at(m);						
			tmpMyMessage = readFromADescriptor(machines->at(m).getFd(), 1);
			FD_CLR(machines->at(m).getFd(), &fsMask);
			result->at(currentMachine->getProcessedRow()).at(currentMachine->getProcessedCol()) = stoi(tmpMyMessage);
			currentMachine->setFlag(false);
			
		}
	}

	for (size_t m = 0; m < numberOfMachines; m++)
	{
		currentMachine = &machines->at(m);
		if(write(currentMachine->getFd(), theEnd.c_str(), theEnd.size()*sizeof(char)) == -1)
		{	
			perror("write error");
			exit(1);
		}
	}

	string myMessage = matrixToCsv(*result);
	if(write(cfd, myMessage.c_str(), myMessage.size()*sizeof(char)) == -1)
	{
		perror("write error");
		exit(1);
	}
	close(cfd);
	close(mfd);
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

string takeOneRowOneColForField(vector<vector<int>> m1, vector<vector<int>> m2, int row, int col)
{
	string str = "";
	for (auto i : m1.at(row))
	{
		str.append(to_string(i));
		str.append(",");
	}
	str.pop_back();
	str.append("\n");
	for (auto j : m2)
	{
		str.append(to_string(j.at(col)));
		str.append(",");
	}
	str.pop_back();
	return str;
}

string matrixToCsv(vector<vector<int>> m)
{
	string str = "";
	for (auto row : m)
	{
		for (auto num : row)
		{
			str.append(to_string(num));
			str.append(",");
		}
		str.pop_back();
		str.append("\n");
	}
	str.pop_back();
	return str;
}

vector<Machine>* readData()
{
	vector<Machine>* machines = new vector<Machine>;
	ifstream file;
	int port;
	string ip;
	int machine;
	file.open("data", ios::out);
	while(file >> machine >> port >> ip)
	{
		machines->push_back(Machine(machine, port,  ip));
	}
	for (auto m : *machines)
	{
		cout << m.getMachineNumber() << " " << m.getPort() << " " << m.getIp() << endl;
	}
	return machines;
}