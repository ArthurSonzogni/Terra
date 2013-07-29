#include "player_group.h"


PlayerGroup::PlayerGroup(bool Server)
{
	server=Server;
	serverListener.setBlocking(false);
	serverPlayer.socket.setBlocking(false);
	newClientPlayer=NULL;
	if (Server)
	{
		serverListener.listen(SERVER_PORT);
	}

	// udpSocket
	udpSocket.setBlocking(false);
	udpSocket.bind(sf::Socket::AnyPort);
	udpPort=udpSocket.getLocalPort();
}
PlayerGroup::~PlayerGroup()
{
	if (newClientPlayer) delete newClientPlayer;
	vector<Player*>::iterator it;
	for(it=clientsPlayer.begin();it!=clientsPlayer.end();++it)
		if (*it)
			delete *it;
}
bool PlayerGroup::isServer()
{
	return server;
}

/*---Server-----------*/
bool PlayerGroup::waitNewClient()
{
	if (newClientPlayer==NULL)
	{
		newClientPlayer=new Player;
		newClientPlayer->socket.setBlocking(false);
	}
	
	if (serverListener.accept(newClientPlayer->socket)==sf::Socket::Done)
	{
		clientsPlayer.push_back(newClientPlayer);
		newClientPlayer=NULL;
		cout<<"new client connected!"<<endl;
		// sending information about udp port
		{
			Message message;
			sf::Packet packet;
			message.type=Message::UdpPort;
			message.identity=clientsPlayer.size()-1;
			message.content.udpPort=udpPort;
			sendMessage(message);
		}
		return true;
	}
	else return false;
}

/*---Client----*/
bool PlayerGroup::connectTo(sf::IpAddress ipAddr)
{
	if (serverPlayer.socket.connect(ipAddr,SERVER_PORT)==sf::Socket::Done)
	{
		cout<<"connected to the server!"<<endl;
		// sending information about udp port
		{
			Message message;
			sf::Packet packet;
			message.type=Message::UdpPort;
			message.identity=-1;
			message.content.udpPort=udpPort;
			sendMessage(message);
		}

		return true;
	}
	else return false;
}

Message PlayerGroup::checkMessage()
{
	Message message;
	message.type=Message::Nothing;
	sf::Packet packet;
	if (server)
	{
		for(int i=0;i<clientsPlayer.size();++i)
		{
			if(clientsPlayer[i]->socket.receive(packet)==sf::Socket::Done)
			{
				packet>>message;
				if (message.type==Message::UdpPort)
				{
					clientsPlayer[i]->udpPort=message.content.udpPort;
					message.type=Message::Nothing;
					cout<<"my port:"<<udpPort<<" is port:"<<clientsPlayer[i]->udpPort<<endl;
				}
				else
				{
					message.identity=i;
					break;
				}
			}
		}
	}
	else
	{
		if (serverPlayer.socket.receive(packet)==sf::Socket::Done)
		{
			packet>>message;
			if (message.type==Message::UdpPort)
			{
				serverPlayer.udpPort=message.content.udpPort;
				message.type=Message::Nothing;
				cout<<"my port:"<<udpPort<<" is port:"<<serverPlayer.udpPort<<endl;
			}
			else
			{
				message.identity=-1;
			}
		}
	}
	return message;
}
void PlayerGroup::sendMessage(const Message& message)
{
	sf::Packet paquet;
	paquet<<message;
	if (server)
	{
		clientsPlayer[message.identity]->socket.send(paquet);	
	}
	else
	{
		serverPlayer.socket.send(paquet);
	}
	
}
sf::Packet& operator<<(sf::Packet& packet, const Message& message)
{
	switch (message.type)
	{
		case Message::Nothing:
		{
			return packet;
		} break;
		case Message::UdpPort:
		{
			return packet<<(message.type)<<(message.content.udpPort);
		} break;
		
	}
}
sf::Packet& operator>>(sf::Packet& packet, Message& message)
{
	sf::Packet& p=(packet>>message.type);
	switch (message.type)
	{
		case Message::Nothing:
		{
			return p;
		} break;
		case Message::UdpPort:
		{
			return p>>(message.content.udpPort);
		} break;
		
	}
	
}
