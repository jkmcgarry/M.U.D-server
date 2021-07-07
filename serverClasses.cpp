#include"serverClasses.h"
#include"lurkserver.h"
#include<string>
#include<iostream>
#include<vector>
#include<algorithm>
#include<string.h>

extern vector<Room*> rooms;
extern vector<Player*> AllPlayers;
extern vector<Player*> PlayersInGame;
using namespace std;
//Room::Room(){monsters = vector <characterPKG>(); deadpile = vector <characterPKG>(); players = vector <Player*>(); connections = vector <connectionPKG>(); connector = vector <int>();}

void Player::pushMe(string name,int a,int d, int r,int h,int g, string des){
	pkg.name = name;
	pkg.flags = 192;
	pkg.atk = a;
	pkg.def = d;
	pkg.regen = r;
	pkg.health = h;
	pkg.gold = g;
	pkg.room = 1;
	pkg.charDescription = des;
	//PlayersInGame.push_back(pkg);
}
characterPKG Player::getPlayer(){
	return pkg;
}
void Room::broadcastSelf(Player player){
	for(int p = 0; p < players.size(); p++){
              cout << players[p]->getPName() << endl;
		for(int j = 0; j < players.size(); j++){
              		sendCharacter(player.getPlayer(),players[j]->cskt);
		}
        }
}

string Player::getPName(){
        return pkg.name;
}
string Player::getDesc(){
        return pkg.charDescription;
}
int Player::getSkt(){
	return cskt;
}
int Player::getAtk(){
        return pkg.atk;
}
int Player::getDef(){
        return pkg.def;
}
int Player::getReg(){
        return pkg.regen;
}
int Player::getHP(){
        return pkg.health;
}
int Player::getGold(){
        return pkg.gold;
}
char Player::getFlags(){
        return pkg.flags;
}
int Player::getRm(){
	return pkg.room;
}
void Player::setPName(string n){
        pkg.name = n;
}
void Player::setDesc(string s){
        pkg.charDescription = s;
}
void Player::setFlags(char i){
	pkg.flags = i;
}
void Player::setAtk(int i){
        pkg.atk = i;
}
void Player::setDef(int i){
        pkg.def = i;
}
void Player::setReg(int i){
        pkg.regen = i;
}
void Player::setHP(int i){
        pkg.health = i;
}
void Player::setGold(int i){
        pkg.gold = i;
}
void Player::setRoom(int i){
	pkg.room = i;
}

void Room::addPlayer(Player* i){
	players.push_back(i);
	//cout << i << endl;
	cout << "Curent player vector size: " << players.size() << endl;
}
void Player::globalList(Player* i){
	PlayersInGame.push_back(i);
}
void Player::remove(Player* p){
	for(int i = 0; i < PlayersInGame.size(); i++){
		if(p->getPName() == PlayersInGame[i]->getPName()){
			PlayersInGame.erase(PlayersInGame.begin()+i);
		}
	}
}
void Room::removePlayer(Player* i){
	for(int n = 0; n < players.size(); n++){
		cout << "name i: " << i->getPName() << endl;
                cout << "comparison: " << players[n]->getPName() << endl;
		if(i->getPName() == players[n]->getPName()){
			//cout << "name i: " << i->getPName() << endl;
			//cout << "comparison: " << players[n]->getPName() << endl;
			players.erase(players.begin()+n);
			cout << "new size: " << players.size() << endl;
		}
	}
}
void Room::removeMonster(characterPKG pkg){
	for(int i = 0; i < monsters.size(); i++){
		if(monsters[i].health == 0){
			cout << "removing monster " << endl;
			monsters.erase(monsters.begin()+i);
			//monsters
		}
	}
}
characterPKG Room::getDead(int dead){
	return deadpile[dead];
}
void Room::addDeadPlayer(Player* dead){
	//deadpile.push_back(dead);
}
void Room::addDead(characterPKG pkg){
	deadpile.push_back(pkg);
	cout << "added to deadpile" << endl;
}
int Room::getNum(){
	return roomNum;
}
string Room::getName(){
	return roomName;
}
string Room::getDes(){
	return roomDes;
}
int Room::getConnection(int a){
	return connector[a];
}
void Room::setNum(int n){
	roomNum = n;
}
void Room::setName(string s){
	roomName = s;
}
void Room::setDes(string d){
	roomDes = d;
}

characterPKG Room::getMonster(int mon){
	return monsters[mon];
}
string Room::getCName(){
        return name;
}
string Room::getDesc(){
        return Des;
}
int Room::getAtk(){
        return atk;
}
int Room::getDef(){
        return def;
}
int Room::getReg(){
        return regen;
}
int Room::getHP(){
        return health;
}
int Room::getGold(){
        return gold;
}
char Room::getFlags(){
	return flags;
}
void Room::setCName(string n){
	name = n;
}
void Room::setDesc(string s){
	Des = s;
}
void Room::setAtk(int i){
	atk = i;
}
void Room::setDef(int i){
	def = i;
}
void Room::setReg(int i){
	regen = i;
}
void Room::setHP(int i){
	health = i;
}
void Room::setGold(int i){
	gold = i;
}

void Room::addMonster(string s, int a, int d, int r, int hp, int g, string c){
	characterPKG temp;
	temp.name = s;
	temp.flags = 248;
	temp.atk = a;
	temp.def = d;
	temp.regen = r;
	temp.health = hp;
	temp.gold   = g;
	temp.charDescription = c;
	monsters.push_back(temp);
}

void Room::addConnection(int i){
	connector.push_back(i);
	for(int i = 0; i < 1/*connector.size()*/; i++)
        {
		//cout << "Connections made...." << endl;
                //cout << connector[i] << endl;
		//cout << "size: " << connector.size() << endl;
        }
}
bool Room::isConnected(int n){
	cout << "looking for room " << n << endl;
	if(rooms.size() > 0){
		for(int i = 0; i < rooms.size(); i++){
			cout << "room has connections to: " << i << endl;
			if(n == connector[i]){
				return true;
			}
		}
	}
	return false;
}

void makeMap(){
	Room* room = new Room();
	room->setName("NULL ROOM");
	room->setDes("You really shouldn't be in this room.");
	room->setNum(0);
	room->addMonster("super ultra weapon",10000,7500,200,5000,200000,"The super secret weapon you weren't supposed to know about.");
	room->addMonster("alpacaman", 50000, 10000, 1000, 2500, 40000, "It's alpacaman so, you're probably doomed.");
	rooms.push_back(room);

	room = new Room();
	room->setName("Utility closet");
	room->setDes("It was the only place to hide in okay.");
	room->setNum(1);
	room->addConnection(2);
	rooms.push_back(room);

	room = new Room();
	room->setName("Main floor");
	room->setDes("The main area of the compound, an access elevator is in the distance.");
	room->setNum(2);
	room->addConnection(1);
	room->addConnection(3);
	room->addConnection(6);
	room->addConnection(7);
	room->addMonster("Security bot",80,40,0,200,50,"Guards the lobby.");
	rooms.push_back(room);

	room = new Room();
        room->setName("Corridor");
        room->setDes("Leads to storage and a training area.");
        room->setNum(3);
        room->addConnection(2);
	room->addConnection(4);
	room->addConnection(5);
        rooms.push_back(room);

	room = new Room();
        room->setName("Supply Room");
        room->setDes("Stores various equipment and food.");
        room->setNum(4);
        room->addConnection(3);
        room->addMonster("Sleepy guard",30,20,1,100,20,"Guard on the verge of passing out.");
        rooms.push_back(room);

	room = new Room();
        room->setName("Training room");
        room->setDes("Area for training and testing.");
        room->setNum(5);
        room->addConnection(3);
        room->addMonster("Test dummy",0,5000,0,150,0,"A surprisingly durable practice dummy.");
        room->addMonster("Guard training",50,25,40,80,20,"In the middle of training, tired and unarmed.");
        rooms.push_back(room);

	room = new Room();
        room->setName("Entrance");
        room->setDes("Outside entrance to the base.");
        room->setNum(6);
        room->addConnection(2);
        room->addMonster("Guard",60,50,5,120,30,"Keeping watch ever vigilant of enemies.");
        room->addMonster("Guard2",60,50,5,120,30,"Keeping watch ever vigilant of enemies.");
        rooms.push_back(room);

	room = new Room();
        room->setName("Outer compound");
        room->setDes("Outdoor area between both buildings.");
        room->setNum(7);
        room->addConnection(2);
	room->addConnection(8);
	room->addConnection(9);
	room->addConnection(10);
        room->addMonster("Guard Dog(Leader)",45,25,10,75,0,"Alpha of the group.");
	room->addMonster("Guard Dog2",35,25,10,65,0,"Wandering outside to detect intruders.");
	room->addMonster("Guard Dog3",35,25,10,65,0,"Wandering outside to detect intruders.");
        rooms.push_back(room);

	room = new Room();
        room->setName("Woods");
        room->setDes("Very dark and thick forest.");
        room->setNum(8);
        room->addConnection(7);
        room->addMonster("Moose",85,35,20,170,60,"A very agitated herd of moose.");
        rooms.push_back(room);

	room = new Room();
        room->setName("Watch tower");
        room->setDes("Lookout over the compound.");
        room->setNum(9);
        room->addConnection(7);
        room->addMonster("Lookout",35,20,20,100,10,"Watchman operating searchlight.");
        room->addMonster("Sniper",70,40,5,130,50,"Takes out targets the watchman finds.");
        rooms.push_back(room);

	room = new Room();
        room->setName("Research building");
        room->setDes("Area for testing and developing weapons.");
        room->setNum(10);
        room->addConnection(7);
	room->addConnection(11);
	room->addConnection(12);
	room->addConnection(13);
        room->addMonster("Security bot",80,50,0,150,20,"Simple robot guarding the building.");
        room->addMonster("Security bot2",80,50,0,150,20,"Simple robot guarding the building.");
        rooms.push_back(room);

	room = new Room();
        room->setName("Stairs");
        room->setDes("Very long flight of stairs leading downwards.");
        room->setNum(11);
        room->addConnection(10);
	room->addConnection(14);
	room->addConnection(15);
	room->addConnection(16);
        rooms.push_back(room);

	room = new Room();
        room->setName("Elevator");
        room->setDes("Main elevator for the lower floors.");
        room->setNum(12);
        room->addConnection(10);
	room->addConnection(18);
	room->addConnection(19);
	room->addConnection(20);
	room->addConnection(21);
        rooms.push_back(room);

	room = new Room();
        room->setName("Offices");
        room->setDes("Full of computers file cabinets and scattered papers.");
        room->setNum(13);
        room->addConnection(10);
        room->addMonster("Scientist",10,20,50,80,500,"Not meant for fighting, only thing on short notice was a stapler.");
        room->addMonster("Scientist",20,20,50,80,500,"Not meant for fighting, but is trying to use a coat rack.");
	room->addMonster("Guard",60,50,5,120,30,"Protecting the scientists, but can't help but laugh at their display.");
        rooms.push_back(room);

	room = new Room();
        room->setName("Hangar");
        room->setDes("Numerous vehicles are stored here.");
        room->setNum(14);
        room->addConnection(11);
        room->addMonster("Pilot",0,50,60,170,20,"You can see him through the cockpit of the mech.");
        room->addMonster("Warmech",125,90,10,1500,2000,"Armed with a missile defense system, laser and dual miniguns. This must be it.");
        rooms.push_back(room);


	room = new Room();
	room->setName("Armory");
	room->setDes("Various weapons and ammunition fill the room.");
	room->setNum(15);
	room->addConnection(11);
	rooms.push_back(room);

	room = new Room();
	room->setName("Cliffside");
	room->setDes("Cliffside connection where planes and helicopters fly in and out from .");
	room->setNum(16);
	rooms.push_back(room);
	room->addMonster("Helicopter",90,70,0,250,750,"A Hind?");
	room->addConnection(11);
	room->addConnection(17);

	room = new Room();
	room->setName("Silo");
	room->setDes("UNAUTHORIZED ACCESS.");
	room->setNum(17);
	room->addMonster("Guard",10,40,60,100,10,"Completely passed out, only his kicks pose a threat.");
	room->addMonster("Warheads",0,450,0,600,0,"Nuclear warheads for detonation in case of compromise."); 
	rooms.push_back(room);
	room->addConnection(16);

	room = new Room();
        room->setName("Cell block");
        room->setDes("Meant for storing prisoners for experiments.");
        room->setNum(18);
	room->addConnection(12);
	room->addMonster("Guard dog",75,30,0,75,50,"Vicious dog for tracking runaway prisoners.");
	room->addMonster("Warden",30,30,15,90,300,"Oversees the block, but isn't very strong.");
        rooms.push_back(room);

	room = new Room();
        room->setName("Control room");
        room->setDes("Full of sophisticated devices and a massive monitor of the world.");
        room->setNum(19);
	room->addMonster("Turret1",80,20,0,75,0,"A machine gun turret, mounted on the wall.");
	room->addMonster("Turret2",80,20,0,75,0,"A machine gun turret, mounted on the wall.");
	room->addMonster("Commander",85,50,20,250,300,"The man behind it all, enhanced by robotics to be hard to kill");
        rooms.push_back(room);
	room->addConnection(12);

	room = new Room();
        room->setName("Maintenance tunnels");
        room->setDes("Very dark, dirty and hot, the elevator is above you. Looks like the tunnels go all over");
        room->setNum(20);
        rooms.push_back(room);
	room->addConnection(4);
	room->addConnection(15);
	room->addConnection(10);
	room->addConnection(2);

	room = new Room();
        room->setName("Observation room");
        room->setDes("Full of machines, a massive one is in the back.");
        room->setNum(21);
	room->addMonster("Engineer",35,40,20,200,75,"Maintains and works on new machines.");
        rooms.push_back(room);
	room->addConnection(12);
	room->addConnection(22);

	room = new Room();
        room->setName("Strange device");
        room->setDes("A large metal cylinder, labeled TIME MACHINE.");
        room->setNum(22);
        rooms.push_back(room);
}

void printWorld(){
	for(int i = 0; i < 1; i++)
        {
                //cout << "room num: " << rooms[i]->getNum() << endl;
		//cout << "names: " << rooms[i]->getName().c_str() << endl;
		//cout << "des: " << rooms[i]->getDes().c_str() << endl;
		cout << "rooms made....." << endl;
		cout << "map populated....." << endl;
		cout << "done!" << endl;
		cout << endl;
        }

}
