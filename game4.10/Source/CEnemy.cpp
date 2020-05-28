#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEnemy.h"
#include "gameMap.h"


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CEnemy: Enemy base class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy::CEnemy(gameMap* pointer, int x, int y) : currentMap(pointer), x(x), y(y) {};
	CEnemy::~CEnemy() {};

	void CEnemy::SetHeroXY(int x1, int x2, int y1, int y2)
	{
		hero["x1"] = x1;
		hero["x2"] = x2;
		hero["y1"] = y1;
		hero["y2"] = y2;
	}

	void CEnemy::SetEnemyXY(int SetX, int SetY)
	{
		x = SetX;
		y = SetY;
	}

	void CEnemy::SetHeroAttackRange(int x1, int x2, int y1, int y2)
	{
		heroAttackRange["x1"] = x1;
		heroAttackRange["x2"] = x2;
		heroAttackRange["y1"] = y1;
		heroAttackRange["y2"] = y2;
	}

	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_sunFlower: Enemy sunFlower class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_sunFlower::CEnemy_sunFlower(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		isMovingRight = true;
		rising = false;
		animation.SetDelayCount(5);
		animationLeft.SetDelayCount(5);
		moveRightAnimation.SetDelayCount(3);
		moveLeftAnimation.SetDelayCount(3);
		DeadAnimation.SetDelayCount(3);
		AttackLeftAnimation.SetDelayCount(2);
		AttackRightAnimation.SetDelayCount(2);
		HitAnimation.SetDelayCount(2);
		GetHitAnimation.SetDelayCount(3);
		enemyHP = 150;							//敵人預設生命值
		enemyAttackDamage = 10;					//敵人預設攻擊力
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		attackDelayCount = attackDelay = 70;
		velocity = initial_velocity;
		state = STAND_LEFT;
	}
	
	CEnemy_sunFlower::~CEnemy_sunFlower()
	{
		for (vector<bullet_sunFlower*>::iterator i = allBullet.begin(); i != allBullet.end(); i++)
			delete *i;
	}

	int CEnemy_sunFlower::GetX1()
	{
		return x;
	}

	int CEnemy_sunFlower::GetY1()
	{
		return y;
	}

	int CEnemy_sunFlower::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy_sunFlower::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy_sunFlower::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy_sunFlower::GetHeight()
	{
		return animation.Height();
	}

	void CEnemy_sunFlower::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount==0)
		{
			GetHitAnimation.Reset();
			CAudio::Instance()->Play(11, false);
			GetHitDelayCount = 15;
			enemyHP -= damage;
			state = GET_HIT;
		}
	}

	void CEnemy_sunFlower::AttackByEnemy(int *heroHP)
	{
		vector<bullet_sunFlower*>::iterator iter = allBullet.begin();
		while (iter != allBullet.end())
		{
			if (((*iter)->GetX2() >= hero["x1"]) && (hero["x2"] >= (*iter)->GetX1()) && ((*iter)->GetY2() >= hero["y1"]) && (hero["y2"] >= (*iter)->GetY1()))
			{
				*heroHP -= enemyAttackDamage;
				delete *iter;
				iter = allBullet.erase(iter);
			}
			else
				iter++;
		}

		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()))
		{
			//*heroHP -= enemyAttackDamage;
		}
	}

	ACTION CEnemy_sunFlower::DetectHero(ACTION state)
	{
		if(attackDelayCount <= 0)
		{
			if ((GetX2() - GetWidth()/2 >= hero["x1"]) && (hero["x2"] >= GetX1() - 200) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_LEFT)
					AttackLeftAnimation.Reset();
				return ATTACK_LEFT;
			}
			if ((GetX2() + 200 >= hero["x1"]) && (hero["x2"] >= GetX1() + GetWidth() / 2) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
			{
				if (state != ATTACK_RIGHT)
					AttackRightAnimation.Reset();
				return ATTACK_RIGHT;
			}
		}

		if ((GetX1() - 190 >= hero["x1"]) && (hero["x2"] >= GetX1() - 270) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_LEFT;
		}
		if ((GetX2() + 270 >= hero["x1"]) && (hero["x2"] >= GetX2() + 190) && (GetY2() + 100 >= hero["y1"]) && (hero["y2"] >= GetY1() - 100))
		{
			return MOVE_RIGHT;
		}

		if (state == MOVE_LEFT || state == ATTACK_LEFT || state == STAND_LEFT)
		{
			return STAND_LEFT;
		}
		if (state == MOVE_RIGHT || state == ATTACK_RIGHT || state == STAND_RIGHT)
		{
			return STAND_RIGHT;
		}
		
		if (state == GET_HIT)
		{
			return GET_HIT;
		}

		return STAND_LEFT;
	}

	string CEnemy_sunFlower::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_sunFlower::LoadBitmap()
	{


		animation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_7, RGB(255, 255, 255));
		//animation.AddBitmap(IDB_SUNFLOWERNOMOVE_3, RGB(255, 255, 255));

		animationLeft.AddBitmap(IDB_SUNFLOWERLEFTWALK_2, RGB(255, 255, 255));
		animationLeft.AddBitmap(IDB_SUNFLOWERLEFTWALK_7, RGB(255, 255, 255));
		//animationLeft.AddBitmap(IDB_SUNFLOWERNOMOVELEFT_2, RGB(255, 255, 255));


		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_1, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_2, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_3, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_4, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_5, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_6, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_7, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_8, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_SUNFLOWERRIGHTWALK_9, RGB(255, 255, 255));

		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_0, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_1, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_2, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_3, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_4, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_5, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_6, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_7, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_SUNFLOWERLEFTWALK_8, RGB(255, 255, 255));

		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_0, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_1, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_2, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_3, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_4, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_5, RGB(255, 255, 255));
		AttackRightAnimation.AddBitmap(IDB_SUNFLOWERATTACKRIGHT_6, RGB(255, 255, 255));

		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_0, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_1, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_2, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_3, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_4, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_5, RGB(255, 255, 255));
		AttackLeftAnimation.AddBitmap(IDB_SUNFLOWERATTACKLEFT_6, RGB(255, 255, 255));

		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_0, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_1, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_2, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_3, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_4, RGB(255, 255, 255));
		DeadAnimation.AddBitmap(IDB_SUNFLOWER_DEAD_4, RGB(255, 255, 255));

		GetHitAnimation.AddBitmap(IDB_SUNFLOWERGETHITRIGHT_0, RGB(255, 255, 255));
		GetHitAnimation.AddBitmap(IDB_SUNFLOWERGETHITRIGHT_1, RGB(255, 255, 255));
		GetHitAnimation.AddBitmap(IDB_SUNFLOWERGETHITRIGHT_2, RGB(255, 255, 255));

		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));
		
	}

	void CEnemy_sunFlower::OnMove()
	{
		const int STEP_SIZE = 2;

		animation.OnMove();
		animationLeft.OnMove();
		moveRightAnimation.OnMove();
		moveLeftAnimation.OnMove();
		AttackLeftAnimation.OnMove();
		AttackRightAnimation.OnMove();
		
		if (GetHitDelayCount > 0) GetHitDelayCount--;
		else if (GetHitDelayCount == 0) HitAnimation.Reset();
		vector<bullet_sunFlower*>::iterator iter = allBullet.begin();
		while (iter != allBullet.end())
		{
			if ((*iter)->isDelet())
			{
				delete *iter;
				iter = allBullet.erase(iter);
			}
			else
				iter++;
		}

		for (vector<bullet_sunFlower*>::iterator i = allBullet.begin(); i != allBullet.end(); i++)
			(*i)->OnMove();

		if (attackDelayCount > 0) attackDelayCount--;

		state = DetectHero(state);

		if (state == MOVE_LEFT)
		{
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10)) // 當座標還沒碰到牆
				x -= STEP_SIZE;
		}

		if (state == MOVE_RIGHT)
		{
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10)) // 當座標還沒碰到牆
				x += STEP_SIZE;
		}

		if (state == ATTACK_LEFT && AttackLeftAnimation.IsFinalBitmap())
		{
			allBullet.push_back(new bullet_sunFlower(currentMap, GetX1(), GetY1() + 40, -5));
			allBullet.back()->LoadBitmap();
		}

		if (state == ATTACK_RIGHT && AttackRightAnimation.IsFinalBitmap())
		{
			allBullet.push_back(new bullet_sunFlower(currentMap, GetX2(), GetY1() + 40, 5));
			allBullet.back()->LoadBitmap();
		}

		if (state == GET_HIT && !GetHitAnimation.IsFinalBitmap() && !HitAnimation.IsFinalBitmap())
		{
			state = GET_HIT;
			GetHitAnimation.OnMove();
			HitAnimation.OnMove();
		}

		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap())
		{
			state = DEAD;
			DeadAnimation.OnMove();
		}

		if (rising) {							// 上升狀態
			if (velocity > 0) {
				y -= velocity;					// 當速度 > 0時，y軸上升(移動velocity個點，velocity的單位為 點/次)
				velocity--;						// 受重力影響，下次的上升速度降低
				if (!currentMap->isSpace(GetX1(), GetY1()) || !currentMap->isSpace(GetX2(), GetY1()))  // 當x座標碰到天花板
				{
					rising = false;
					velocity = 1;
				}
			}
			else {
				rising = false;					// 當速度 <= 0，上升終止，下次改為下降
				velocity = 1;					// 下降的初速(velocity)為1
			}
		}
		else {									// 下降狀態
			if (currentMap->isSpace(GetX1(), GetY2()) && currentMap->isSpace(GetX2(), GetY2())) {  // 當y座標還沒碰到地板
				y += velocity;					// y軸下降(移動velocity個點，velocity的單位為 點/次)
				velocity++;						// 受重力影響，下次的下降速度增加
			}
			else {
				floor = currentMap->GetBlockY(GetY2()) - GetHeight();
				y = floor;					// 當y座標低於地板，更正為地板上
				velocity = initial_velocity;	// 重設上升初始速度
			}
		}
	}

	void CEnemy_sunFlower::OnShow()
	{
		for (vector<bullet_sunFlower*>::iterator i = allBullet.begin(); i != allBullet.end(); i++)
			(*i)->OnShow();

		switch (state)
		{
		case STAND_LEFT:
			animationLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animationLeft.OnShow();
			break;
		case STAND_RIGHT:
			animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animation.OnShow();
			break;
		case MOVE_LEFT:
			moveLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveLeftAnimation.OnShow();
			break;
		case ATTACK_LEFT:
			if(AttackLeftAnimation.IsFinalBitmap()) attackDelayCount = attackDelay;

			AttackLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackLeftAnimation.OnShow();
			break;
		case MOVE_RIGHT:
			moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			moveRightAnimation.OnShow();
			break;
		case ATTACK_RIGHT:
			if (AttackRightAnimation.IsFinalBitmap())attackDelayCount = attackDelay;

			AttackRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackRightAnimation.OnShow();
			break;
		case GET_HIT:
			GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			GetHitAnimation.OnShow();
			if (!HitAnimation.IsFinalBitmap())
			{
				HitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HitAnimation.OnShow();
			}
			
			//if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
			break;
		case DEAD:
			if (enemyHP <= 0)
			{
				if (!DeadAnimation.IsFinalBitmap())
				{
					DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
					DeadAnimation.OnShow();
				}
			}
			break;
		}
		/*else if (GetHit)
		{
		GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		GetHitAnimation.OnShow();
		if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
		}*/
	}

	bool CEnemy_sunFlower::isDead()
	{
		if (enemyHP <= 0 && DeadAnimation.IsFinalBitmap()) return true;
		else return false;
	}


	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_Cactus: Enemy Cactus class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_Cactus::CEnemy_Cactus(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
		AttackDelayCount = 0;					// 設定攻擊頻率
		AttackFlag = false;		
		ReadyToAttack = false;
		enemyHP = 150;
		enemyAttackDamage = 40;
		AttackAnimation.SetDelayCount(4);
		DeadAnimation.SetDelayCount(3);
		animation.SetDelayCount(5);
		GetHitAnimation.SetDelayCount(3);
		HitAnimation.SetDelayCount(3);
	}

	CEnemy_Cactus::~CEnemy_Cactus() {}

	int CEnemy_Cactus::GetX1()
	{
		return x;
	}

	int CEnemy_Cactus::GetY1()
	{
		return y;
	}

	int CEnemy_Cactus::GetX2()
	{
		return x + animation.Width();
	}

	int CEnemy_Cactus::GetY2()
	{
		return y + animation.Height();
	}

	int CEnemy_Cactus::GetWidth()
	{
		return animation.Width();
	}

	int CEnemy_Cactus::GetHeight()
	{
		return animation.Height();
	}

	string CEnemy_Cactus::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_Cactus::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount==0)
		{
			GetHitAnimation.Reset();
			HitAnimation.Reset();
			CAudio::Instance()->Play(9, false);
			enemyHP -= damage;
			GetHit = true;
			GetHitDelayCount = 15;
		}
	}


	void CEnemy_Cactus::LoadBitmap()
	{
		animation.AddBitmap(IDB_CACTUSNOMOVE_0, RGB(255, 127, 39));
		animation.AddBitmap(IDB_CACTUSNOMOVE_1, RGB(255, 127, 39));
		animation.AddBitmap(IDB_CACTUSNOMOVE_2, RGB(255, 127, 39));

		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_0, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_1, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_2, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_3, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_4, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_5, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_6, RGB(255, 127, 39));
		AttackAnimation.AddBitmap(IDB_CACTUSATTACK_7, RGB(255, 127, 39));

		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_0, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_1, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_2, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_3, RGB(255, 127, 39));
		DeadAnimation.AddBitmap(IDB_CACTUSDEAD_4, RGB(255, 127, 39));

		GetHitAnimation.AddBitmap(IDB_CACTUSGETHIT_0, RGB(255, 127, 39));
		GetHitAnimation.AddBitmap(IDB_CACTUSGETHIT_1, RGB(255, 127, 39));
		GetHitAnimation.AddBitmap(IDB_CACTUSGETHIT_2, RGB(255, 127, 39));
	
		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_3, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_4, RGB(63, 72, 204));
	}

	void CEnemy_Cactus::OnMove()
	{
		const int STEP_SIZE = 0;
		if (GetHitDelayCount > 0)GetHitDelayCount--;
		if (AttackDelayCount > 0) AttackDelayCount--;
		
		AttackAnimation.OnMove();
		GetHitAnimation.OnMove();
		HitAnimation.OnMove();
		animation.OnMove();
		if (AttackAnimation.IsFinalBitmap()) ReadyToAttack = false;
		if (ReadyToAttack && AttackDelayCount==0)
		{
			AttackAnimation.Reset();
			AttackDelayCount = 150;
		}
		//if(GetHit) GetHitAnimation.Reset();
		if (enemyHP <= 0 && !DeadAnimation.IsFinalBitmap()) DeadAnimation.OnMove();

	}

	void CEnemy_Cactus::OnShow()
	{
		if (enemyHP <= 0)
		{
			if (!DeadAnimation.IsFinalBitmap())
			{
				DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				DeadAnimation.OnShow();
			}
		}
		else if (ReadyToAttack)
		{
			AttackAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			AttackAnimation.OnShow();
			if (AttackAnimation.GetCurrentBitmapNumber() == 4)
			{
				CAudio::Instance()->Play(10, false);
				AttackFlag = true;   //發出尖刺時攻擊
			}
			else AttackFlag = false;
		}
		else if (GetHit)
		{

			GetHitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			GetHitAnimation.OnShow();
			HitAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			HitAnimation.OnShow();
			if (GetHitAnimation.IsFinalBitmap()) GetHit = false;
			if (HitAnimation.IsFinalBitmap()) GetHit = false;
		}
		else
		{
			animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			animation.OnShow();
		}
	}

	void CEnemy_Cactus::AttackByEnemy(int *heroHP)
	{
		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) &&(AttackDelayCount==0))
		{
			ReadyToAttack = true;
		}
		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()) && AttackFlag)
		{
			*heroHP -= enemyAttackDamage;
		}
	}

	bool CEnemy_Cactus::isDead()
	{
		if (enemyHP <= 0) return true;
		else return false;
	}


	/////////////////////////////////////////////////////////////////////////////
	// CEnemy_Statue: Enemy Statue class
	/////////////////////////////////////////////////////////////////////////////
	CEnemy_Statue::CEnemy_Statue(gameMap* pointer, int x, int y) : CEnemy(pointer, x, y)
	{
		const int INITIAL_VELOCITY = 15;		// 初始上升速度
		const int FLOOR = 100;					// 地板座標
		floor = FLOOR;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
		GetHitDelayCount = 0;
		HitAnimation.SetDelayCount(3);
	}

	CEnemy_Statue::~CEnemy_Statue() {}

	int CEnemy_Statue::GetX1()
	{
		return x;
	}

	int CEnemy_Statue::GetY1()
	{
		return y;
	}

	int CEnemy_Statue::GetX2()
	{
		return x + Statue.Width();
	}

	int CEnemy_Statue::GetY2()
	{
		return y + Statue.Height();
	}

	int CEnemy_Statue::GetWidth()
	{
		return Statue.Width();
	}

	int CEnemy_Statue::GetHeight()
	{
		return Statue.Height();
	}

	string CEnemy_Statue::GetEnemyType()
	{
		return EnemyType;
	}

	void CEnemy_Statue::GetAttack(const int damage)
	{
		if ((GetX2() >= heroAttackRange["x1"]) && (heroAttackRange["x2"] >= GetX1()) && (GetY2() >= heroAttackRange["y1"]) && (heroAttackRange["y2"] >= GetY1()) && GetHitDelayCount==0)
		{
			HitAnimation.Reset();
			CAudio::Instance()->Play(11, false);
			GetHitDelayCount = 15;
			GetHit = true;
			enemyHP -= damage;
		}
	}


	void CEnemy_Statue::LoadBitmap()
	{
		Statue.LoadBitmap(IDB_STATUE, RGB(255, 0, 0));
		Statue_Broken.LoadBitmap(IDB_STATUE_BROKEN, RGB(255, 0, 0));

		HitAnimation.AddBitmap(IDB_HIT_0, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_1, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_2, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_3, RGB(63, 72, 204));
		HitAnimation.AddBitmap(IDB_HIT_4, RGB(63, 72, 204));
	}

	void CEnemy_Statue::OnMove()
	{
		if (GetHitDelayCount > 0) GetHitDelayCount--;
		HitAnimation.OnMove();
	}

	void CEnemy_Statue::OnShow()
	{
		
		if (enemyHP <= 0)
		{
			Statue_Broken.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			Statue_Broken.ShowBitmap();
			/*if (!DeadAnimation.IsFinalBitmap())
			{
				DeadAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				DeadAnimation.OnShow();
			}*/
		}
		else
		{
			Statue.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
			Statue.ShowBitmap();
		}
		if (GetHit)
		{
			HitAnimation.SetTopLeft(currentMap->ScreenX(x + 5), currentMap->ScreenY(y + 50));
			HitAnimation.OnShow();
			if (HitAnimation.IsFinalBitmap()) GetHit = false;
		}
	}

	void CEnemy_Statue::AttackByEnemy(int *heroHP)
	{
		if ((GetX2() >= hero["x1"]) && (hero["x2"] >= GetX1()) && (GetY2() >= hero["y1"]) && (hero["y2"] >= GetY1()))
		{
			*heroHP -= enemyAttackDamage;
		}
	}

	bool CEnemy_Statue::isDead()
	{
		if (enemyHP <= 0) return true;
		else return false;
	}

	/////////////////////////////////////////////////////////////////////////////
	// bullet: bullet base class
	/////////////////////////////////////////////////////////////////////////////
	bullet::bullet(gameMap* point,int nx, int ny, int step)
	{
		x = nx;
		y = ny;
		currentMap = point;
		distance = 0;
		STEP_SIZE = step;
		animation.SetDelayCount(2);
	}

	bullet::~bullet() {}

	int bullet::GetX1()
	{
		return x;
	}

	int bullet::GetY1()
	{
		return y;
	}

	int bullet::GetX2()
	{
		return x + animation.Width();
	}

	int bullet::GetY2()
	{
		return y + animation.Height();
	}

	bool bullet::isDelet()
	{
		if (distance > 500)
		{
			return true;
		}

		if (!currentMap->isSpace(GetX1(), GetY1()))  // 當x座標碰到牆
		{
			return true;
		}

		return false;
	}

	void bullet::OnMove()
	{
		animation.OnMove();

		x += STEP_SIZE;

		if (STEP_SIZE > 0)
		{
			distance += STEP_SIZE;
		}
		else
		{
			distance -= STEP_SIZE;
		}
	}
	void bullet::OnShow()
	{
		animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
		animation.OnShow();
	}

	/////////////////////////////////////////////////////////////////////////////
	// bullet_sunFlower : bullet_sunFlower class
	/////////////////////////////////////////////////////////////////////////////
	bullet_sunFlower::bullet_sunFlower(gameMap* point, int nx, int ny, int step) : bullet(point, nx, ny, step) {}

	bullet_sunFlower::~bullet_sunFlower() {}

	void bullet_sunFlower::LoadBitmap()
	{
		animation.AddBitmap(IDB_SUNFLOWERBULLET_0, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_1, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_2, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_3, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_4, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_5, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_6, RGB(63, 72, 204));
		animation.AddBitmap(IDB_SUNFLOWERBULLET_7, RGB(63, 72, 204));
	}

}