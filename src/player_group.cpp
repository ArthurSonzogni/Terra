#include "player_group.h"
#include <SFML/Network.hpp>

Grid messageGridBuffer;
int messageGridIndex;

PlayerGroup::PlayerGroup(bool Server)
{
	server=Server;
	serverListener.setBlocking(false);
	serverPlayer.socket.setBlocking(true);
	newClientPlayer=NULL;
	if (Server)
	{
		serverListener.listen(SERVER_PORT);
		joueurId=0;
	}
	else
	{
		joueurId=1;
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
		bool breakTheLoop=false;
		for(int i=0;i<int(clientsPlayer.size()) and !breakTheLoop;++i)
		{
			switch (clientsPlayer[i]->socket.receive(packet))
			{
				case sf::Socket::Done:
				{
					packet>>message;
					if (message.type==Message::UdpPort)
					{
						clientsPlayer[i]->udpPort=message.content.udpPort;
						cout<<"my port:"<<udpPort<<" is port:"<<clientsPlayer[i]->udpPort<<endl;
						return checkMessage();
					}
					else if (message.type==Message::StartParty)
					{
						joueurId=message.content.joueurId;
						breakTheLoop=true;
					}
					else
					{
						message.identity=i;
						breakTheLoop=true;
					}
				} break;
				case sf::Socket::NotReady:
				{
					cout<<"I am Not ready"<<endl;
				} break;
				case sf::Socket::Error:
				{
					cout<<"Error"<<endl;
				} break;
				case sf::Socket::Disconnected:
				{
					cout<<"Disconnected"<<endl;
				} break;
			}
		}
	}
	else
	{
		switch(serverPlayer.socket.receive(packet))
		{
			case sf::Socket::Done:
			{
				cout<<"done"<<endl;
				packet>>message;
				cout<<"message.type="<<int(message.type)<<endl;
				if (message.type==Message::UdpPort)
				{
					serverPlayer.udpPort=message.content.udpPort;
					message.type=Message::Nothing;
					cout<<"my port:"<<udpPort<<" is port:"<<serverPlayer.udpPort<<endl;
					return checkMessage();
				}
				else
				{
					//message.identity=-1;
				}
			} break;
			case sf::Socket::NotReady:
			{
				cout<<"I am Not ready"<<endl;
			} break;
			case sf::Socket::Error:
			{
				cout<<"Error"<<endl;
			} break;
			case sf::Socket::Disconnected:
			{
				cout<<"Disconnected"<<endl;
			} break;
			
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
		switch(clientsPlayer[message.identity]->socket.send(paquet))
		{
			case sf::Socket::Done:
			{
				//sf::sleep(sf::seconds(0.1));
				return;
			} break;

			case sf::Socket::NotReady:
			{
				cout<<"Not Ready"<<endl;
				sf::sleep(sf::seconds(1.0));
				sendMessage(message);
			};
			
			case sf::Socket::Disconnected:
			{
				cout<<"disconnected"<<endl;

			} break;
			
			case sf::Socket::Error:
			{
				cout<<"error"<<endl;
				sf::sleep(sf::seconds(1.0));
				sendMessage(message);
			} break;
		}
	}
	else
	{
		switch(serverPlayer.socket.send(paquet))
		{
			case sf::Socket::Done:
			{
				return;
			} break;

			case sf::Socket::NotReady:
			{
				cout<<"Not Ready"<<endl;
				sendMessage(message);
			};

			case sf::Socket::Disconnected:
			{
				cout<<"disconnected"<<endl;
			} break;
			
			case sf::Socket::Error:
			{
				cout<<"error"<<endl;
			} break;
		}
	}
}
sf::Packet& operator<<(sf::Packet& packet, const Message& message)
{
	//cout<<"message<<"<<int(message.type)<<endl;
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
		case Message::StartParty:
		{
			return packet<<(message.type)<<(message.identity);
		} break;
		case Message::Okay:
		{
			return packet<<(message.type);
		} break;
		case Message::LevelLoadingStart:
		{
			messageGridBuffer.copy(*(message.content.grid));
			messageGridIndex=0;
			int dx,dy,dz;
			messageGridBuffer.get_dimension(dx,dy,dz);
			
			return packet
				<<(message.type)
				<<sf::Uint32(dx)
				<<sf::Uint32(dy)
				<<sf::Uint32(dz);

		} break;
		case Message::LevelLoading:
		{
			packet<<(message.type);
			int length=message.content.gridPacketLength;
			packet<<length;
			
			cout<<"sending :"<<messageGridIndex<<endl;

			sf::Uint8 filled, texture;
			int dx,dy,dz;
			messageGridBuffer.get_dimension(dx,dy,dz);

			int*** filledPtr;
			int*** texturePtr;
			messageGridBuffer.getPtr(filledPtr,texturePtr);

			for(int i=0;i<length;++i)
			{
				int x=messageGridIndex%dx+1;
				int y=(messageGridIndex/dx)%dy+1;
				int z=(messageGridIndex/dx)/dy+1;
				filled=filledPtr[x][y][z];
				texture=texturePtr[x][y][z];
				packet<<filled<<texture;
				messageGridIndex++;
			}
			return packet;
		}break;
		case Message::LevelLoadingEnd:
		{
			return packet<<(message.type);
		}break;
		case Message::BowlMatrix:
		{
			return packet<<(message.type)
				<<(message.content.bowlMatrix.player)
				<<(message.content.bowlMatrix.mat[0])
				<<(message.content.bowlMatrix.mat[1])
				<<(message.content.bowlMatrix.mat[2])
				<<(message.content.bowlMatrix.mat[3])
				<<(message.content.bowlMatrix.mat[4])
				<<(message.content.bowlMatrix.mat[5])
				<<(message.content.bowlMatrix.mat[6])
				<<(message.content.bowlMatrix.mat[7])
				<<(message.content.bowlMatrix.mat[8])
				<<(message.content.bowlMatrix.mat[9])
				<<(message.content.bowlMatrix.mat[10])
				<<(message.content.bowlMatrix.mat[11])
				<<(message.content.bowlMatrix.mat[12])
				<<(message.content.bowlMatrix.mat[13])
				<<(message.content.bowlMatrix.mat[14])
				<<(message.content.bowlMatrix.mat[15]);
		} break;
		case Message::Move:
		{
			return packet<<(message.type)
				<<(message.content.moveAngle);
		} break;
		default: return packet;
	}
}
sf::Packet& operator>>(sf::Packet& packet, Message& message)
{
	sf::Packet& p=(packet>>message.type);
	//cout<<"message>> "<<int(message.type)<<endl;
	if (p)
		cout<<"p est vrai"<<endl;
	else
		cout<<"p est faux"<<endl;
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
		case Message::StartParty:
		{
			return p>>(message.identity);
		} break;
		case Message::Okay:
		{
			return p;
		} break;
		case Message::LevelLoadingStart:
		{
			sf::Uint32 dx,dy,dz;
			sf::Packet& p=packet>>dx>>dy>>dz;
			messageGridBuffer.set_dimension(dx,dy,dz);
			messageGridIndex=0;
			return p;
		} break;
		case Message::LevelLoading:
		{
			int length;
			packet>>length;
			cout<<"receiving :"<<messageGridIndex<<endl;
			sf::Uint8 filled, texture;
			int dx,dy,dz;
			messageGridBuffer.get_dimension(dx,dy,dz);
			for(int i=0;i<length;++i)
			{
				int x=messageGridIndex%dx;
				int y=(messageGridIndex/dx)%dy;
				int z=(messageGridIndex/dx)/dy;
				packet>>filled>>texture;
				messageGridBuffer.assignBlock(1+x,1+y,1+z,filled,texture);
				messageGridIndex++;
			}
			return packet;
		}break;
		case Message::LevelLoadingEnd:
		{
			message.content.grid=messageGridBuffer.allocCopy();
			return p;
		} break;
		case Message::BowlMatrix:
		{
			return p
				>>(message.content.bowlMatrix.player)
				>>(message.content.bowlMatrix.mat[0])
				>>(message.content.bowlMatrix.mat[1])
				>>(message.content.bowlMatrix.mat[2])
				>>(message.content.bowlMatrix.mat[3])
				>>(message.content.bowlMatrix.mat[4])
				>>(message.content.bowlMatrix.mat[5])
				>>(message.content.bowlMatrix.mat[6])
				>>(message.content.bowlMatrix.mat[7])
				>>(message.content.bowlMatrix.mat[8])
				>>(message.content.bowlMatrix.mat[9])
				>>(message.content.bowlMatrix.mat[10])
				>>(message.content.bowlMatrix.mat[11])
				>>(message.content.bowlMatrix.mat[12])
				>>(message.content.bowlMatrix.mat[13])
				>>(message.content.bowlMatrix.mat[14])
				>>(message.content.bowlMatrix.mat[15]);
		} break;
		case Message::Move:
		{
			return packet>>(message.content.moveAngle);
		} break;
		default: return packet;
	}
}

int PlayerGroup::getNbPlayer()
{
	return clientsPlayer.size();
}
