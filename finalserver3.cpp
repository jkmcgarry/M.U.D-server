#include"lurkserver.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include"serverClasses.h"
#include<vector>
#include<iostream>
#include<iomanip>
#include<signal.h>
#include<ctime>

vector < characterPKG > monsters;
vector < characterPKG > deadpile;
vector < Player* > players;
vector < connectionPKG > connections;
vector < Player > AllPlayers;
vector < Room* > rooms;
vector < Player* > Me;
vector < Player* >  PlayersInGame;
vector <int> connector;

gamePKG game;
void* network(void*);
uint8_t type = 0;
using std::vector;
using namespace std;

void check_connection(int signal){
	cout << "Pipe Error: " << endl;
}

void sendMSG(messagePKG pkg, int skt){
	uint8_t type = 1;
        write(skt, &type,1);
        uint16_t msgLen = (uint16_t) pkg.message.length();
        write(skt, &msgLen,2);
        char msg[33];
        strncpy(msg, pkg.receiver.c_str(), sizeof(msg));
        msg[sizeof(msg) - 1] = 0;
        write(skt, msg,32);
        strncpy(msg, pkg.sender.c_str(), sizeof(msg));
        msg[sizeof(msg) - 1] = 0;
        write(skt, msg,32);
        char mbuffer[msgLen];
        strncpy(mbuffer, pkg.message.c_str(), sizeof(mbuffer));
        write(skt, mbuffer,msgLen);

}
void sendError(errorPKG pkg, int skt){
	uint8_t type = 7;
	write(skt, &type,1);
	write(skt, &(pkg.errorCode),1);
	uint16_t eLen = (uint16_t) pkg.errorMsg.length();
	char ename[33];
	ename[sizeof(ename) - 1] = 0;
	write(skt, &eLen,2);
	char ebuffer[eLen];
	memcpy(ebuffer, pkg.errorMsg.c_str(), eLen);
	write(skt, ebuffer, eLen);
}
void sendAccept(acceptPKG pkg, int skt){
        uint8_t type = 8;
        write(skt, &type,1);
        write(skt, &(pkg.acception),1);
}
void sendRoom(roomPKG pkg, int skt){
	uint8_t type = 9;
	write(skt, &type,1);
	write(skt, &(pkg.roomNum),2);
	uint16_t rLen = (uint16_t) pkg.roomDes.length();
	char rname[33];
	strncpy(rname, pkg.roomName.c_str(), sizeof(rname));
	rname[sizeof(rname) - 1] = 0;
	write(skt, rname,32);
	write(skt, &rLen,2);
	char rbuffer[rLen];
	memcpy(rbuffer, pkg.roomDes.c_str(), rLen);
	write(skt, rbuffer, rLen);
}
void sendCharacter(characterPKG pkg, int skt){
	uint8_t type = 10;
        write(skt, &type,1);
        uint16_t cLen = (uint16_t) pkg.charDescription.length();
        char cname[33];
        strncpy(cname, pkg.name.c_str(), sizeof(cname));
        cname[sizeof(cname) - 1] = 0;
        write(skt, cname,32);
        write(skt, &(pkg.flags),1);
        write(skt, &(pkg.atk),2);
        write(skt, &(pkg.def),2);
        write(skt, &(pkg.regen),2);
        write(skt, &(pkg.health),2);
        write(skt, &(pkg.gold),2);
        write(skt, &(pkg.room),2);
        write(skt, &cLen,2);
        char cbuffer[cLen];
        memcpy(cbuffer, pkg.charDescription.c_str(), cLen);
        write(skt, cbuffer, cLen);

}
void sendGame(gamePKG pkg, int skt){
        uint8_t type = 11;
        write(skt, &type,1);
        write(skt, &(pkg.points),2);
        write(skt, &(pkg.statCap),2);
        uint16_t dlen = (uint16_t) pkg.description.length();
        char desc[33];
        desc[sizeof(desc) - 1] = 0;
        write(skt, &dlen,2);
        char dbuffer[dlen];
        memcpy(dbuffer, pkg.description.c_str(), dlen);
        write(skt, dbuffer, dlen);
}
void sendRmchng(connectionPKG pkg, int skt){
	uint8_t type = 13;
	write(skt, &type,1);
	write(skt, &(pkg.roomNum),2);
	uint16_t cdLen = (uint16_t) pkg.roomDes.length();
	char rname[33];
	strncpy(rname, pkg.roomName.c_str(), sizeof(rname));
	rname[sizeof(rname) - 1] = 0;
	write(skt, rname,32);
	write(skt, &cdLen,2);
	char cdbuffer[cdLen];
	memcpy(cdbuffer, pkg.roomDes.c_str(), cdLen);
	write(skt, cdbuffer, cdLen);
}


void* handle_client(void* param){
	srand(time(0));
	int cskt = (int)(size_t)param;
	// Interaction with the client starts here
	gamePKG pkg;
	int type;
	bool warning = true;
	Room *room;
	Player me;
	size_t ooops;
	sendGame(pkg,cskt);
	me.cskt=cskt;
	bool canfight = true;
	//bool send = true;
	while (true){
		ooops = read(cskt, &type,1);
		
		if (ooops == 0){
			close(cskt);
			return 0;
		}
		if(type == 1){
			acceptPKG pkg8;
		   	messagePKG pkg1;
			errorPKG pkg7;
			bool send = false;
                   	uint16_t msgLen;
                   	read(cskt, &msgLen, 2);
                   	char mbuffer[msgLen+1];
                   	char buffer[33];
                   	read(cskt, buffer, 32);
                   	buffer[32] = 0;
                   	pkg1.receiver = (buffer);
                   	read(cskt, buffer, 32);
                   	pkg1.sender = (buffer);
                   	read(cskt, mbuffer, msgLen);
                   	mbuffer[msgLen] = 0;
                   	pkg1.message = (mbuffer);
			pkg8.acception = type;
                   	pushMessage(pkg1);
			for(int i = 0; i < PlayersInGame.size(); i++){
				cout << "list of players: " << PlayersInGame[i]->getPName() << endl;
				cout << "sending to: " << pkg1.receiver << endl;
				if(pkg1.receiver == PlayersInGame[i]->getPName()){
		   			sendMSG(pkg1,PlayersInGame[i]->getSkt());
					cout << "Socket: " << PlayersInGame[i]->getSkt() << endl;
					sendAccept(pkg8,cskt);
					send = true;
				}
			}
			if(!send){
					pkg7.errorCode = 1;
					pkg7.errorMsg = "Cannot message that player.";
					sendError(pkg7,cskt);
			}
		   	cout << "message received" << endl;
		}
		if(type == 2){
		        roomPKG pkg9;
			errorPKG pkg7;
			messagePKG pkg1;
			characterPKG pkg10;
			connectionPKG pkg13;
			changeroomPKG pkg2;  //object of room number to change to
			read(cskt,&pkg2.newRoom,2);
			pushChangeRoom(pkg2);
			cout << "request recieved" << endl;
			cout << "name: " << room->getName() << endl;
			if(me.getHP() !=0){
				if(room->isConnected(pkg2.newRoom)){
					room->removePlayer(&me);
					for(int j =0; j < room->players.size();j++){
                                                for(int i=0;i< room->players.size();i++){
                                                        sendCharacter(room->players[i]->getPlayer(),room->players[j]->getSkt());
                                                }
                                        }
					cout << "player list: " << room->players.size() << endl;
					cout << "connection successful" << endl;
					room = rooms[pkg2.newRoom];
					pkg9.roomNum = room->getNum();
					pkg9.roomName = room->getName().c_str();
					pkg9.roomDes  = room->getDes().c_str();
					sendRoom(pkg9, cskt);
					room->addPlayer(&me);
					cout << room->players.size() << endl;
					if (pkg9.roomNum == 22){
						for(int c = 0; c < 100000001; c++){
							if(c == 1000000){
                                                                pkg1.sender = "A voice";
                                                                pkg1.receiver = me.getPName();
                                                                pkg1.message = "You cannot move.";
                                                                sendMSG(pkg1,me.cskt);
                                                        }
                                                        else if(c == 2000000){
                                                                pkg1.sender = "A voice";
                                                                pkg1.receiver = me.getPName();
                                                                pkg1.message = "Time and space have swapped places.";
                                                                sendMSG(pkg1,me.cskt);
                                                        }
                                                        else if(c == 3000000){
                                                                pkg1.sender = "A voice";
                                                                pkg1.receiver = me.getPName();
                                                                pkg1.message = "You musn't look back.";
                                                                sendMSG(pkg1,me.cskt);
                                                        }
                                                        else if(c == 7000000){
                                                                pkg1.sender = "A voice";
                                                                pkg1.receiver = me.getPName();
                                                                pkg1.message = "Look forward, and reach the past.";
                                                                sendMSG(pkg1,me.cskt);
                                                        }

							else if(c == 10000000){
								pkg1.sender = "A voice";
								pkg1.receiver = me.getPName();
								pkg1.message = "You see a blinding light before you.";
								sendMSG(pkg1,me.cskt);
								cout << "you are stuck here" << endl;
								pkg2.newRoom = (rand()%21);
								room->removePlayer(&me);
								room = rooms[pkg2.newRoom];
								pkg9.roomNum = room->getNum();
								pkg9.roomName = room->getName().c_str();
								pkg9.roomDes = room->getDes().c_str();
								sendRoom(pkg9,cskt);
								me.setHP(0);
								me.setFlags(88);
								me.setDesc("Gelified remains of " + me.getPName());
								room->addDead(me.getPlayer());
								room->removePlayer(&me);
								me.remove(&me);
								for(int i = 0; i < room->deadpile.size(); i++){
									sendCharacter(room->getDead(i), cskt);
								}
								room->broadcastSelf(me);
							}
						}
					}
					/*else if(pkg9.roomNum == 7){
						for(int i = 0; i < rooms.size(); i++){
							if(i ==  9){
								for(int m = 0; m < room[9].monsters.size(); m++){
									if(room[9].monsters[m].name =="Sniper"){
										pkg1.sender = "Thoughs";
										pkg1.receiver = me.getPName();
										pkg1.message = "You feel a terrible pain, turns out the sniper in the tower shot you.";
										sendMSG(pkg1,me.cskt);
									}
								} 
							}
						}
					}*/
					if(me.getHP() != 0 && me.getHP() < 150){
						me.setHP(me.getHP() + me.getReg()/2);
						if(me.getHP() > 150){
							me.setHP(150);
						}
						room->broadcastSelf(me);
					}
					for(int j =0; j < room->players.size();j++){
						for(int k = 0; k < room->deadpile.size(); k++){
							sendCharacter(room->getDead(k),room->players[j]->getSkt());
						}
						for(int m = 0; m < room->monsters.size(); m++){
							sendCharacter(room->getMonster(m),room->players[j]->getSkt());
						}
						for(int i=0;i< room->players.size();i++){
							sendCharacter(room->players[i]->getPlayer(),room->players[j]->getSkt());
						}
					}
					cout << "List players: " << endl;
					room->broadcastSelf(me);
					for(int n = 0; n < room->connector.size(); n++){
						pkg13.roomNum = rooms[room->getConnection(n)]->getNum();
						pkg13.roomName = rooms[room->getConnection(n)]->getName().c_str();
						pkg13.roomDes = rooms[room->getConnection(n)]->getDes().c_str();
						sendRmchng(pkg13, cskt);
					}
				}
				else{
					pkg7.errorCode = 2;
					pkg7.errorMsg = "Cannot move there";
					sendError(pkg7,cskt);
					cout << "connection failed" << endl;
				}
			}
			else{
				pkg7.errorCode = 4;
				pkg7.errorMsg = "Cannot move, you are dead";
				sendError(pkg7,cskt);
			}
		}
		if(type == 3){
			characterPKG pkg10;
			errorPKG pkg7;
			messagePKG pkg1;
			unsigned int damage;
			int monsterDamage;
			int combinedDefense;
			int combinedDamage;
			cout << "fighting monsters... " << endl;
			//bool canfight = true;
			//if(me.getHP() <= 0){
				//canfight = false;
			//}
			if(me.getFlags() != 88){
					combinedDefense = 0;
					combinedDamage  = 0;
					for(int n = 0; n < room->monsters.size(); n++){
						//combinedDefense = 0;
						cout << "searching monster vector " << endl;
						cout << "atk value: " << me.getAtk() << endl;
						cout << "monsters defense: " << room->monsters[n].def << endl;
						//cout << "damage dealt" << me.getAtk() - (room->monsters[n].def/2) << endl;
						damage = me.getAtk() - (room->monsters[n].def/2);
						//monsterDamage = (room->monsters[n].atk - me.getDef()/2);
							for(int i = 0; i < room->players.size(); i++){
								if(room->players[i]->getFlags() == 216 || room->players[i]->getPName() == me.getPName()){
									for(int j = 0; j < room->players.size(); j++){
										combinedDamage += room->players[j]->getAtk();
										combinedDefense += room->players[j]->getDef();
									}
									//monsterDamage = ((room->monsters[n].atk * 0.75) - combinedDefense);
									cout << "monster damage: " << monsterDamage << endl;
								}
								monsterDamage = ((room->monsters[n].atk * 0.75) - combinedDefense);
							}
						 	//monsterDamage = ((room->monsters[n].atk/2) - combinedDefense);
									cout << "combined defense: " << combinedDefense << endl;
									cout << "monster Damage: " << monsterDamage << endl;
									cout << "combined Damage computed: " << combinedDamage << endl;
									cout << "damge to deal: " << combinedDamage - (room->monsters[n].def/2) << endl;
							if((combinedDamage - room->monsters[n].def/2) <= 0){
								combinedDamage = 0;
                                				room->monsters[n].health -= combinedDamage;
                            				}
							else{
								room->monsters[n].health -= combinedDamage - (room->monsters[n].def/2);
								combinedDamage = 0;
							}
							if(room->monsters[n].health > 50000 || room->monsters[n].health < 0){
								room->monsters[n].health = 0;
								room->monsters[n].flags = 120;
							}
							if(monsterDamage <= 0){
								monsterDamage = 0;
							}
							else if(monsterDamage > 1000){
								monsterDamage = 0;
							}
							cout << "Combined damage: " << combinedDamage << endl;
							//sendCharacter(room->getMonster(n),room->players[i]->getSkt());
							if(room->monsters[n].health != 0){
								room->monsters[n].health += (room->monsters[n].regen/3);
								for(int i = 0; i < room->players.size(); i++){
									if(room->players[i]->getFlags() == 216 || room->players[i]->getPName() == me.getPName()){
										if(monsterDamage < 0){
											room->players[i]->setHP(room->players[i]->getHP());
										}
										else{
											cout << "players that fought" << endl;
											cout << room->players.size() << endl;
											for(int k = 0; k < room->players.size(); k++){
											room->players[k]->setHP(room->players[k]->getHP() - monsterDamage);
											cout << "Health of : " << room->players[k]->getPName() << " " << room->players[k]->getHP() << endl;
														//if(room->players[k]->getHP() > 100){
															//room->players[k]->setHP(100);
														//}
												if(me.getHP() <= 0 || me.getHP() > 200){
													me.setHP(0);
													//me.setHP(me.getHP());
													me.setFlags(88);
													pkg1.sender = "Commander";
													pkg1.receiver = me.getPName();
													pkg1.message = "What happend out there? " + me.getPName() + " do you copy? "; //+ me.getPName() + "? " + me.getPName() + "!? " + me.getPName() = "!!!!!!!!";
													sendMSG(pkg1,me.cskt);
													room->addDead(me.getPlayer());
													room->removePlayer(&me);
													me.remove(&me);
													for(int i = 0; i < room->deadpile.size(); i++){
														sendCharacter(room->getDead(i),cskt);
													}
												}
											}
											//sendCharacter(room->getMonster(n),room->players[i]->getSkt());
											//room->broadcastSelf(me);
											//sendCharacter(room->players[i]->getPlayer(),room->players[i]->getSkt());
										}
										cout << "monster health left: " << room->monsters[n].health << endl;
									}
								}
							}
						if(room->monsters[n].health <= 0){
							if(room->monsters[n].name == "Pilot"){
								room->monsters[n].flags = 120;
								room->monsters[n].health = 0;
								if(warning){
									pkg1.sender = "Warmech";
									pkg1.receiver = me.getPName();
									pkg1.message = "PILOT NEUTRALIZED. DANGER! COMMENCING AUTOMATED DEFENSE SYSTEMS";
									sendMSG(pkg1,me.cskt);
								}
								for(int m = 0; m < room->monsters.size(); m++)
								{
									if(room->monsters[m].name == "Warmech"){
										room->monsters[m].atk = 250;
										room->monsters[m].def = 45;
										room->monsters[m].regen = 0;
										for(int i = 0; i < room->players.size(); i++){
											sendCharacter(room->getMonster(m),room->players[i]->getSkt());
										}
									}
								}
							}
							if(room->monsters[n].name == "Security bot" || room->monsters[n].name == "Security bot2"){
								room->monsters[n].flags = 120;
								room->monsters[n].health = 0;
								room->addDead(room->monsters[n]);
								for(int i = 0; i < room->players.size(); i++){
									room->players[i]->setHP(room->players[i]->getHP() - 50);
									pkg1.sender = "Robot";
									pkg1.receiver = room->players[i]->getPName();
									pkg1.message = "Systems down.....self-destructing";
									sendMSG(pkg1,room->players[i]->getSkt());
								}
							}
							if(room->monsters[n].name == "Warmech"){
								room->monsters[n].flags = 120;
								room->monsters[n].health = 0;
								room->addDead(room->monsters[n]);
								pkg1.sender = "Commander";
								pkg1.receiver = me.getPName();
								pkg1.message = "The weapon has been destroyed you did it!";
								sendMSG(pkg1,me.cskt);
								for(int i = 0; i < room->players.size(); i++){
									room->players[i]->setHP(1);
									sendCharacter(room->players[i]->getPlayer(),room->players[i]->getSkt());
								}
								for(int k = 0; k < PlayersInGame.size(); k++){
									pkg1.sender = "Commander";
									pkg1.receiver = PlayersInGame[k]->getPName();
									pkg1.message = "The weapon has been destroyed.";
									sendMSG(pkg1,PlayersInGame[k]->getSkt());
								}
							}
							if(room->monsters[n].name == "Helicopter"){
								room->monsters[n].flags = 120;
								room->monsters[n].health = 0;
								room->addDead(room->monsters[n]);
								for(int i = 0; i < room->players.size(); i++){
									pkg1.sender = "Pilot";
									pkg1.receiver = room->players[i]->getPName();
									pkg1.message = "NOOOOOOOOOOOOOO!";
									sendMSG(pkg1, room->players[i]->getSkt());
									pkg1.sender = "Game";
						 			pkg1.receiver = room->players[i]->getPName();
									pkg1.message = "The helicopter crashes into the cliff and explodes.";
									sendMSG(pkg1, room->players[i]->getSkt());
									room->players[i]->setHP(room->players[i]->getHP() - 75);
									sendCharacter(room->players[i]->getPlayer(),room->players[i]->getSkt());
								}	
							}
							else{
								room->monsters[n].health = 0;
								room->monsters[n].flags = 120;
								room->addDead(room->monsters[n]);
								room->monsters.erase(room->monsters.begin()+n);
							}
							if(me.getHP() != 0 && me.getHP() < 150){
								me.setHP(me.getHP()+me.getReg()/2);
								if(me.getHP() >= 150){
									me.setHP(150);
								}
								room->broadcastSelf(me);
							}
							for(int i = 0; i < room->deadpile.size(); i++){
								//sendCharacter(room->getDead(i),cskt);
							}
						}
					if(!canfight){
						pkg7.errorCode = 3;
						pkg7.errorMsg = "No monster(s) to fight. ";
						sendError(pkg7,cskt);
					}
					for(int j =0;j < room->players.size();j++){
						for(int i = 0; i < room->deadpile.size(); i++){
							sendCharacter(room->getDead(i),room->players[j]->getSkt());
						}
						for(int i = 0; i < room->monsters.size(); i++){
							sendCharacter(room->getMonster(i),room->players[j]->getSkt());
						}
						for(int i = 0; i < room->players.size(); i ++){
							sendCharacter(room->players[i]->getPlayer(),room->players[j]->getSkt());
						}
					}
				}
			}
					else{
						pkg7.errorCode = 6;
						pkg7.errorMsg = "cannot fight, you have died.";
						sendError(pkg7,cskt);
					}
		}
		if(type == 4){
			errorPKG pkg7;
			roomPKG pkg9;
			pkg7.errorCode = 61;
			pkg7.errorMsg = "Now's not the time to turn on your friends!";
			sendError(pkg7,cskt);
		}
		if(type == 5){
			characterPKG pkg10;
			lootPKG pkg5;
			errorPKG pkg7;
			char dbuffer[33];
			read(cskt,&(dbuffer),32);
			pkg5.targetPlayer = dbuffer;
			dbuffer[32] = 0;
			cout << "entitiy to loot: " << pkg5.targetPlayer << endl;
			if(me.getHP() != 0){
				for(int i = 0; i < room->deadpile.size(); i++){
					cout << "entity name: " << room->deadpile[i].name << endl;
					if(pkg5.targetPlayer == room->deadpile[i].name){
						if(room->deadpile[i].gold == 0){
							pkg7.errorCode = 7;
							pkg7.errorMsg = "Entitiy has no gold to loot. ";
							sendError(pkg7,cskt);
						}
						else{
							me.setGold(me.getGold()+room->deadpile[i].gold);
							room->deadpile[i].gold = 0;
							room->broadcastSelf(me);
							sendCharacter(room->getDead(i),cskt);
						}
					}
					else{
						pkg7.errorCode = 5;
						pkg7.errorMsg = "Entity is either alive or not in room. ";
						sendError(pkg7,cskt);
					}
				}
				for(int j =0;j < room->players.size();j++){
					for(int i = 0; i < room->deadpile.size(); i++){
						sendCharacter(room->getDead(i),room->players[j]->getSkt());
					}
 
				}
			}
			else{
				pkg7.errorCode = 8;
				pkg7.errorMsg = "Cannot loot, you are dead. ";
				sendError(pkg7,cskt);
			}
		}
		if(type == 6){
			messagePKG pkg1;
			startPKG pkg6;
			roomPKG pkg9;
			connectionPKG pkg13;
			characterPKG pkg10;
			room = rooms[1];
			pkg9.roomNum  = rooms[1]->getNum();
                        pkg9.roomName = rooms[1]->getName().c_str();
                        pkg9.roomDes  = rooms[1]->getDes().c_str();
			room->addPlayer(&me);
			pushStart(pkg6);
			me.setFlags(216);
			room->broadcastSelf(me);
			sendRoom(pkg9, cskt);
			room->broadcastSelf(me);
			for(int i = 0; i < PlayersInGame.size(); i++){
				pkg1.sender = "Commander";
				pkg1.receiver = PlayersInGame[i]->getPName();
				pkg1.message = me.getPName() + " has joined the operation.";
				sendMSG(pkg1,PlayersInGame[i]->getSkt());
			}
			for(int j =0; j < room->players.size();j++){
				for(int i = 0; i < room->players.size(); i ++){
                                    sendCharacter(room->players[i]->getPlayer(),room->players[j]->getSkt());
                                }
                        }
			for(int i = 0; i < room->connector.size();i++){
				pkg13.roomNum = rooms[room->getConnection(i)]->getNum();
				pkg13.roomName = rooms[room->getConnection(i)]->getName().c_str();
				pkg13.roomDes = rooms[room->getConnection(i)]->getDes().c_str();
				sendRmchng(pkg13, cskt);
			}
		}
		if(type == 8){
		   acceptPKG pkg8;
                   read(cskt,&(pkg8.acception),1);
                   pushAccept(pkg8);
		   sendAccept(pkg8, cskt);
		}
		if(type == 9 ){
		   	roomPKG pkg9;
       		   	read(cskt,&(pkg9.roomNum),2);
                   	char rbuffer[33];
                   	read(cskt, rbuffer, 32);
                   	rbuffer[32] = 0;
                   	pkg9.roomName = (rbuffer);
                   	uint16_t rmLen;
                   	read(cskt,&rmLen,2);
                   	char rmbuffer[rmLen+1];
                   	read(cskt, rmbuffer, rmLen);
                   	rmbuffer[rmLen] = 0;
                   	pkg9.roomDes = (rmbuffer);
                   	pushRoom(pkg9);
		   	sendRoom(pkg9,cskt);
			cout << "display current room";

		}
		if(type == 10){
		   	characterPKG pkg10;
			acceptPKG    pkg8;
                   	char namebuffer[33];
                   	read(cskt, namebuffer,32);
			string pName = namebuffer;
                   	namebuffer[32] = 0;
                   	pkg10.name = (namebuffer);
                   	read(cskt,&(pkg10.flags),1);
			pkg10.flags = 192;
                   	read(cskt,&(pkg10.atk),2);
                   	read(cskt,&(pkg10.def),2);
                   	read(cskt,&(pkg10.regen),2);
                   	read(cskt,&(pkg10.health),2);
			pkg10.health = 150;
                   	read(cskt,&(pkg10.gold),2);
                   	read(cskt,&(pkg10.room),2);
                   	uint16_t pdLen;
                   	read(cskt, &pdLen,2);
                   	char cbuffer[pdLen+1];
                   	read(cskt, cbuffer, pdLen);
			string pDescription = cbuffer;
                   	cbuffer[pdLen] = 0;
                   	pkg10.charDescription = (cbuffer);
                   	//pushCharacter(pkg10);
			bool taken = false;
			for(int i = 0; i < PlayersInGame.size(); i++){
				if(pkg10.name == PlayersInGame[i]->getPName()){
					bool take = true;
				}
			}
			if(taken){
				errorPKG pkg7;
				pkg7.errorCode = 9;
				pkg7.errorMsg = "Name taken";
				sendError(pkg7, cskt);
			}
			else if((pkg10.atk)+(pkg10.def)+(pkg10.regen) <= 120){
					pkg8.acception = type;
                                	sendAccept(pkg8, cskt);
					pushCharacter(pkg10);
		   			sendCharacter(pkg10, cskt);
					me.setPName(pName);
					me.setDesc(pDescription);
					me.pushMe(pkg10.name,pkg10.atk,pkg10.def,pkg10.regen,pkg10.health,pkg10.gold,pkg10.charDescription);
					me.globalList(&me);
                        		//me.displayPlayers();
					//pkg8.acception = 10;
					//sendAccept(pkg8, cskt);
					//sendCharacter(pkg10, cskt);
			}
			else{
					//sendAccept(pkg8, cskt);
					errorPKG pkg7;
					pkg7.errorCode = 10;
					pkg7.errorMsg = "Invalid character";
					sendError(pkg7, cskt);
			}
		}
		if(type == 11){
			acceptPKG pkg8;
			sendGame(pkg,cskt);
			pkg8.acception = type;
			sendAccept(pkg8, cskt);
		}
		if(type == 12){
			messagePKG pkg1;
			leavePKG pkg12;
                	pushLeave(pkg12);
			cout << "Goodbye" << endl;
			me.remove(&me);
			for(int i = 0; i < PlayersInGame.size(); i++){
				pkg1.sender = "Commander";
				pkg1.receiver = PlayersInGame[i]->getPName();
                                pkg1.message = me.getPName() + " has abandoned the mission.";
                                sendMSG(pkg1,PlayersInGame[i]->getSkt());

			}
			room->removePlayer(&me);
                        for(int j =0; j < room->players.size();j++){
                                        for(int i = 0; i < room->players.size(); i ++){
                                                sendCharacter(room->players[i]->getPlayer(),room->players[j]->getSkt());
                                        }
                        }
			return 0;
		}
		if(type == 13){//need to adjust this functionalilty later on
		   connectionPKG pkg13;
		   read(cskt,&(pkg13.roomNum),2);
                   char crbuffer[33];
                   read(cskt, crbuffer,32);
                   crbuffer[32] = 0;
                   pkg13.roomName = (crbuffer);
                   uint16_t rdLen;
                   read(cskt, &rdLen, 2);
                   char rmbuffer[rdLen+1];
                   read(cskt, rmbuffer, rdLen);
                   rmbuffer[rdLen] = 0;
                   pkg13.roomDes = (rmbuffer);
                   pushConnection(pkg13);
		   sendRmchng(pkg13, cskt);
		   for(int n = 0; n < room->connector.size(); n++){
                                                pkg13.roomNum = rooms[room->getConnection(n)]->getNum();
                                                pkg13.roomName = rooms[room->getConnection(n)]->getName().c_str();
                                                pkg13.roomDes = rooms[room->getConnection(n)]->getDes().c_str();
                                                sendRmchng(pkg13, cskt);
                                        }
		}
	}
	// End of interaction with client
	close(cskt);
	return 0;
}
int main(int argc, char ** argv){
	struct sigaction sp;
	sp.sa_handler = check_connection;

	sigaction(SIGPIPE, &sp, 0);

	if(argc < 2){
		printf("Usage: %s port\n", argv[0]);
		return 1;
	}
	int skt = socket(AF_INET, SOCK_STREAM, 0);
	if(skt == -1){
		perror("Socket: ");
		return 1;
	}

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[1]));
	saddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(skt, (struct sockaddr*)&saddr, sizeof(struct sockaddr_in))){
		perror("Bind: ");
		return 1;
	}
	if(listen(skt, 5)){
		perror("Listen: ");
		return 1;
	}
	struct sockaddr_in caddr;
	socklen_t caddr_size = sizeof(caddr);
	makeMap();
        printWorld();

	for(;;){
		int cskt = accept(skt, (struct sockaddr*)&caddr, &caddr_size);
		if(cskt == -1){
			perror("Accept: ");
			return 1;
		}
		printf("Accepted connection from: %s\n", inet_ntoa(caddr.sin_addr));

		// Start thread running to handle the new client
		pthread_t new_thread;
		//client_sockets[actual_clients++] = cskt;
		pthread_create(&new_thread, 0, handle_client, (void*)(intptr_t)cskt);
		// At this point, we're not using new_thread for anything
		// But, to gracefully shut down, we might want it!
	}
	makeMap();
	printWorld();
	close(skt);
	return 0;

}

