#include "lurkserver.h"
#include <string.h>
#include <iostream>
//#include "serverClasses.h"
#include <vector>

using namespace std;
extern gamePKG game;


void pushMessage(messagePKG &package){
	cout << package.sender << " to " << package.receiver << endl;
	cout << package.message << endl;

}
void pushChangeRoom(changeroomPKG &package){
	cout <<" requested room: " << package.newRoom << endl;
	
}
void pushFight(fightPKG &package){

}
void pushPVP(pvpPKG &package){

}
void pushLoot(lootPKG &package){

}
void pushStart(startPKG &package){
	cout << "ready" << endl;
	//cout << "vector " << rooms.size();
}
void pushError(errorPKG &package){

}
void pushAccept(acceptPKG &package){

}
void pushRoom(roomPKG &package){
	cout << package.roomNum << endl;
	cout << package.roomName << endl;
	cout << package.roomDes << endl;
	//cout << vector<Room> rooms() << endl
}
void pushCharacter(characterPKG &package){
	cout << "name: "  << package.name   << endl;
	cout << "atk: "   << package.atk    << endl;
	cout << "def: "   << package.def    << endl;
	cout << "regen: " << package.regen  << endl;
	cout << "life: "  << package.health << endl;
	cout << "gold: "  << package.gold   << endl;
	cout << "room: "  << package.room   << endl;
	cout << package.charDescription << endl;
	//cout << rooms.size();
}
void pushGame(gamePKG &package){
	game = package;
	cout << package.description << endl;
}
void pushLeave(leavePKG &package){

}
void pushConnection(connectionPKG &package){

}

