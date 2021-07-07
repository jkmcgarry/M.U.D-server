#ifndef Classes
#define Classes
#include<string>
#include"lurkserver.h"
#include<vector>


class Player{
	public:
	characterPKG pkg;
	Player(){}
	characterPKG getPlayer();
	//vector < Player* > PlayersInGame;
	int cskt;
	void globalList(Player*);
	void remove(Player*);
	void setPName(string);
	void setFlags(char);
        void setAtk(int);
        void setDef(int);
        void setReg(int);
        void setHP(int);
        void setGold(int);
        void setRoom(int);
        void setDesc(string);
	string getPName();
        string getDesc();
	int getSkt();
        int getAtk();
        int getDef();
        int getReg();
        int getHP();
        int getGold();
	int getRm();
        char getFlags();
	void pushMe(string,int,int,int,int,int,string);
	void displayPlayers();
};

class Room{
	public:
	int    roomNum;
	string roomName;
	string roomDes;
	string name;
	characterPKG pkg;
	//int cskt;
	int atk;
	int def;
	int regen; 
	int health;
	int gold;
	char flags;
	string Des;
	Room(){deadpile=vector<characterPKG>();}
	vector < characterPKG  > monsters;
	vector < characterPKG  > deadpile;
	vector < Player* > players;
	vector < connectionPKG > connections;
	vector < int > connector;
	vector < Player* > PlayersInGame;
	void broadcastSelf(Player);
	int getNum();
	string getName();
	string getDes();
	int getConnection(int);
	void setNum(int);
	void setName(string);
	void setDes(string);
	void addConnection(int);
	void removeConnections();
	bool isConnected(int);
	characterPKG getDead(int);
	void addPlayer(Player*);
	void removePlayer(Player*);
	void removeMonster(characterPKG);
	void addDead(characterPKG);
	void addDeadPlayer(Player*);
	void addMonster(string,int,int,int,int,int,string);
	characterPKG getMonster(int);
        void setCName(string);
        void setAtk(int);
        void setDef(int);
        void setReg(int);
        void setHP(int);
        void setGold(int);
        void setRm(int);
        void setDesc(string);
        string getCName();
        string getDesc();
        int getAtk();
        int getDef();
        int getReg();
        int getHP();
        int getGold();
	char getFlags();
	void setDamage(int, int);

};


void makeMap();
void printWorld();


#endif
