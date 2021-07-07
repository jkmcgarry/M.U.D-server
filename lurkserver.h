#ifndef lurkserver
#define lurkserver
#include<string>
#include<iostream>
#include<vector>

using namespace std;



struct messagePKG{
	string sender;
	string receiver;
	string message;
};
struct changeroomPKG{
	uint16_t newRoom;
};
struct fightPKG{
	string enemy;
	uint16_t reward;
};
struct pvpPKG{
	string targetPlayer;
	uint16_t reward;
};
struct lootPKG{
	string targetPlayer;
	uint16_t reward;
};
struct startPKG{

};
struct errorPKG{
	uint8_t errorCode;
	string errorMsg;
};
struct acceptPKG{
	uint8_t acception;

};
struct roomPKG{
	uint16_t roomNum;
	string roomName;
	string roomDes;
};
struct characterPKG{
	string name;
	uint8_t flags;
	uint16_t atk;
	uint16_t def;
	uint16_t regen;
	uint16_t health;
	uint16_t gold;
	uint16_t room;
	string charDescription;
};
struct gamePKG{
	uint16_t points = 120;
	uint16_t statCap = 65265;
	string description = "You've entered the enemy base, your mission to destroy the secret super weapon that threatens the earth. However almost getting caught, you've been forced to hide in a broom closet to allocate your stats before moving forward.";
};
struct leavePKG{

};
struct connectionPKG{
	uint16_t roomNum;
	string roomName;
	string roomDes;
};

void pushMessage(messagePKG &pkg);
void pushChangeRoom(changeroomPKG &pkg);
void pushFight(fightPKG &pkg);
void pushPVP(pvpPKG &pkg);
void pushLoot(lootPKG &pkg);
void pushStart(startPKG &pkg);
void pushError(errorPKG &pkg);
void pushAccept(acceptPKG &pkg);
void pushRoom(roomPKG &pkg);
void pushCharacter(characterPKG &pkg);
void pushGame(gamePKG &pkg);
void pushLeave(leavePKG &pkg);
void pushConnection(connectionPKG &pkg);

void sendMSG(messagePKG pkg, int skt);   //1
//void sendFight(fightPKG pkg, int skt); 
//void sendPVP(pvpPKG pkg, int skt);
void sendError(errorPKG pkg, int skt);   //
void sendAccept(acceptPKG pkg, int skt);
void sendRoom(roomPKG pkg, int skt);
void sendCharacter(characterPKG pkg, int stk);
void sendGame(gamePKG pkg, int skt);
void sendRmchng(connectionPKG pkg, int skt);

//void sendLeave(leavePKG pkg, int skt);
#endif
