#ifndef PLAYER_GROUP_CH3KS0LA
#define PLAYER_GROUP_CH3KS0LA

#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include "grid.h"
using namespace std;


#define SERVER_PORT 2355


class Message
{
	public:
	enum
	{
		MoveKeyLeft,
		MoveKeyRight,
		MoveKeyUp,
		MoveKeyDown
	};

		enum MessageType
		{
			Nothing,
			UdpPort,
			StartParty,
			LevelLoadingStart,
			LevelLoading,
			LevelLoadingEnd,
			Okay,
			BowlMatrix,
			Move
		};

		sf::Int8 type;
		sf::Int8 identity;

		union
		{
			sf::Uint8 joueurId;
			sf::Uint16 udpPort;
			struct
			{
				sf::Uint8 player;
				float mat[16];
			} bowlMatrix;
			float moveAngle;
			Grid* grid;
			sf::Int8 gridPacketLength;
		} content;
};

sf::Packet& operator<<(sf::Packet& packet, const Message& message);
sf::Packet& operator>>(sf::Packet& packet, Message& message);

class Player
{
	public:
		sf::TcpSocket socket;
		int udpPort;
		float bowlMatrix[16];
};

class PlayerGroup
{
	public:
		PlayerGroup(bool server);
		~PlayerGroup();
		bool isServer();
		/*---Server-----------*/
		bool waitNewClient();
		int getNbPlayer();

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

		vector<float[16]> bowlPosition;
		sf::Uint8 joueurId;


};

#endif /* end of include guard: PLAYER_GROUP_CH3KS0LA */
