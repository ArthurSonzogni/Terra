#ifndef PLAYER_GROUP_CH3KS0LA
#define PLAYER_GROUP_CH3KS0LA

#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
using namespace std;

#define SERVER_PORT 2202

class Message
{
	public:

		enum MessageType
		{
			Nothing,
			UdpPort
		};

		sf::Int8 type;
		sf::Int8 identity;

		union
		{
			sf::Uint16 udpPort;
		} content;
};

sf::Packet& operator<<(sf::Packet& packet, const Message& message);
sf::Packet& operator>>(sf::Packet& packet, Message& message);

class Player
{
	public:
		sf::TcpSocket socket;
		int udpPort;
};

class PlayerGroup
{
	public:
		PlayerGroup(bool server);
		~PlayerGroup();
		bool isServer();
		/*---Server-----------*/
		bool waitNewClient();

		/*---Client----*/
		bool connectTo(sf::IpAddress);

		Message checkMessage();
		void sendMessage(const Message& message);
	private:
		bool server;

		/*---Server-----------*/
		Player* newClientPlayer;
		vector<Player*> clientsPlayer;
		sf::TcpListener serverListener;

		/*---Client----*/
		Player serverPlayer;

		/*---Both------*/
		sf::UdpSocket udpSocket;
		int udpPort;


};

#endif /* end of include guard: PLAYER_GROUP_CH3KS0LA */
