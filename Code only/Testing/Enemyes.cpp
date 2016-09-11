#include "Enemyes.h"
#include "Game.h"

 Enemy::~Enemy(){
	for(int i=0;;i++){
		if(this==Game::enemyes[i]){
			vector<Enemy*>::iterator ite = Game::enemyes.begin()+i;
			Game::enemyes.erase(Game::enemyes.begin()+i);
			break;
		}
	}
	SAFE_DELETE(Mesh);
 }

void Enemy::Set_Height(Enemy* enemy){
	
	enemy->Position.y = Game::terrain->Get_height(enemy->Position.x,enemy->Position.z);

}