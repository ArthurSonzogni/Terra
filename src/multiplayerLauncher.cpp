#include "multiplayerLauncher.h"
#include <SFML/System.hpp>
#include <iostream>
using namespace std;


int MultiplayerLauncher::getState()
{
	return state;
}

void MultiplayerLauncher::process(PlayerGroup& playerGroup)
{

	if (playerGroup.isServer())
	{
		PlayerGroup& playerServer=playerGroup;
		while(playerServer.getNbPlayer()<1)
		{
			playerServer.waitNewClient();

			Message message=playerServer.checkMessage();
			if (message.type!=Message::Nothing)
			{
				cout<<"message:";
				if (message.type==Message::UdpPort)
					cout<<"UdpPort"<<endl;
				else
					cout<<endl;
			}
		}

		// sending StartPaVrty
		Message message;
		message.type=Message::StartParty;
		for(int i=0;i<playerServer.getNbPlayer();++i)
		{
			message.identity=i;
			playerServer.sendMessage(message);
		}
		

		// sending the grid
		for(int i=0;i<playerServer.getNbPlayer();++i)
		{
			// start LevelLoading
			message.identity=i;
			message.type=Message::LevelLoadingStart;
			message.content.grid=&grid;
			playerServer.sendMessage(message);

			// levelLoading
			int dx,dy,dz;
			grid.get_dimension(dx,dy,dz);
			int nn=dx*dy*dz;
			int message_length=10;
			int a;
			for(a=0;a+message_length<nn;a+=message_length)
			{
				message.type=Message::LevelLoading;
				message.content.gridPacketLength=message_length;
				playerServer.sendMessage(message);
			}
			{
				message.type=Message::LevelLoading;
				message.content.gridPacketLength=nn-a;
				playerServer.sendMessage(message);
			}

			// finish LevelLoading
			message.type=Message::LevelLoadingEnd;
			playerServer.sendMessage(message);
			
				
		}
		
	}
	else
	{
		string ipAddr;
		cout<<"Entrer l'adress Ip:";
		cin>>ipAddr;

		PlayerGroup& playerClient=playerGroup;
		while(not playerClient.connectTo(ipAddr));

	
		Message message;
		for(;;)
		{
			message=playerClient.checkMessage();
			if (message.type==Message::StartParty)
				break;
			else
			;//	sf::sleep(sf::seconds(0.1));
		}

		for(;;)
		{
			message=playerClient.checkMessage();
			if (message.type==Message::LevelLoadingStart)
				break;
			else
			;//		sf::sleep(sf::seconds(0.1));
		}
		for(;;)
		{
			message=playerClient.checkMessage();
			if (message.type==Message::LevelLoadingEnd)
			{
				grid.copy(*(message.content.grid));
				break;
			}
			else
			;//		sf::sleep(sf::seconds(0.1));
		}
		//int dx,dy,dz;
		//grid.get_dimension(dx,dy,dz);
		//cout
			//<<"dx("<<dx<<")"
			//<<"dy("<<dy<<")"
			//<<"dz("<<dz<<")"<<endl;
		//int*** filledPtr;
		//int*** texrurePtr;
		//grid.getPtr(filledPtr,texrurePtr);
		//for(int x=1;x<=dx;++x)
		//{
			//for(int y=1;y<=dy;++y)
			//{
				//for(int z=1;z<=dz;++z)
				//{
					//cout<<filledPtr[x][y][z]<<" ";
				//}
				//cout<<endl;
			//}
			//cout<<endl<<endl;
		//}
		//cout<<endl;
	}
}

Grid* MultiplayerLauncher::getGrid()
{
	return &grid;
}

void MultiplayerLauncher::setGrid(Grid& g)
{
	grid.copy(g);
}
