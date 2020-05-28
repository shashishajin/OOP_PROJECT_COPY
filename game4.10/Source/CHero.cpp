#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "gameMap.h"
#include "CHero.h"


namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CEraser: Eraser class
	/////////////////////////////////////////////////////////////////////////////

	CHero::CHero()
	{
	
		currentVillage = NULL;
		currentWild = NULL;
		currentMap = NULL;

		for (int i = 0; i < 100; i++) LifeBarRed.push_back(new CMovingBitmap);    //100個血條圖片

		FullHP = 200;				// 主角預設血量為100
		heroAttackDamage = 20;		// 主角預設攻擊力為20
		Gold = 1000;
		Initialize();
	}

	CHero::~CHero()
	{
		delete currentVillage;
		delete currentWild;
		//for (vector<gameMap*>::iterator i = maps.begin(); i != maps.end(); i++) delete (*i);
		for (vector<CMovingBitmap*>::iterator i = LifeBarRed.begin(); i != LifeBarRed.end(); i++) delete (*i);
	}

	int CHero::GetX1()
	{
		return x;
	}

	int CHero::GetY1()
	{
		return y;
	}

	int CHero::GetX2()
	{
		return x + animation.Width();
	}

	int CHero::GetY2()
	{
		return y + animation.Height();
	}

	int CHero::GetWidth()
	{
		return animation.Width();
	}

	int CHero::GetHeight()
	{
		return animation.Height();
	}

	int CHero::GetCenterX()
	{
		return x + animation.Width() / 2;
	}

	int CHero::GetCenterY()
	{
		return y + animation.Height() / 2;
	}



	void CHero::Initialize()
	{
		const int X_POS = 0;
		const int Y_POS = 0;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isAttacking = isRolling = isInvincible = isTalkingToNPC = false;
		PreviousMovement = 0;			//紀錄上一個動作
		const int INITIAL_VELOCITY = 15;	// 初始上升速度
		const int FLOOR = 100;				// 地板座標
		floor = FLOOR;
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
		animation.SetDelayCount(5);
		animation1.SetDelayCount(5);
		sword.SetDelayCount(3);
		HeroDashLeft.SetDelayCount(3);
		HeroDashRight.SetDelayCount(3);
		swordAttack.SetDelayCount(3);
		swordAttack1.SetDelayCount(3);
		HeroAttackMovement.SetDelayCount(3);
		HeroAttackMovement1.SetDelayCount(3);
		sword1.SetDelayCount(3);
		moveRightAnimation.SetDelayCount(3);
		jumpAnimation.SetDelayCount(4);
		jumpAnimation1.SetDelayCount(4);
		moveLeftAnimation.SetDelayCount(3);
		HeroRollLeft.SetDelayCount(5);
		HeroRollRight.SetDelayCount(5);
		SwordRollRight.SetDelayCount(5);
		SwordRollLeft.SetDelayCount(5);
		SwordDashLeft.SetDelayCount(3);
		SwordDashRight.SetDelayCount(3);
		FireSwordRightAnimation.SetDelayCount(3);
		FireSwordLeftAnimation.SetDelayCount(3);
		FireCircle.SetDelayCount(3);
		Fire1.SetDelayCount(3);
		Fire2.SetDelayCount(3);
		Fire3.SetDelayCount(3);
		SetAttackDelayCount = AttackDelayCount = DashColdDown = 15;
		ShowGoldDelayCount = 0;
		RollDelayCount = 15;
		InvincibleDelayCount = 30;
		MoveDelayCount = 10;					// 主角預設血量為100
		CurrentHP = FullHP;
		AttackRange = 100;					// 主角攻擊範圍
		isSelectingMap = false;
		AttackRange = 150;
	}

	void CHero::LoadBitmap()
	{
		animation.AddBitmap(IDB_HeroNoMove_1, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_2, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_3, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_4, RGB(255, 255, 255));
		animation.AddBitmap(IDB_HeroNoMove_5, RGB(255, 255, 255));

		animation1.AddBitmap(IDB_HERONOMOVELEFT_1, RGB(255, 255, 255));
		animation1.AddBitmap(IDB_HERONOMOVELEFT_2, RGB(255, 255, 255));
		animation1.AddBitmap(IDB_HERONOMOVELEFT_3, RGB(255, 255, 255));
		animation1.AddBitmap(IDB_HERONOMOVELEFT_4, RGB(255, 255, 255));
		animation1.AddBitmap(IDB_HERONOMOVELEFT_5, RGB(255, 255, 255));

		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_1, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_2, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_3, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_4, RGB(255, 255, 255));
		moveRightAnimation.AddBitmap(IDB_HEROMOVERIGHT_5, RGB(255, 255, 255));

		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_1, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_2, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_3, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_4, RGB(255, 255, 255));
		moveLeftAnimation.AddBitmap(IDB_HEROMOVELEFT_5, RGB(255, 255, 255));

		//jumpAnimation.AddBitmap(IDB_HEROJUMP_3, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_4, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_5, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_6, RGB(255, 255, 255));
		jumpAnimation.AddBitmap(IDB_HEROJUMP_7, RGB(255, 255, 255));

		jumpAnimation1.AddBitmap(IDB_HEROJUMP_0_1, RGB(255, 255, 255));
		jumpAnimation1.AddBitmap(IDB_HEROJUMP_1_1, RGB(255, 255, 255));
		jumpAnimation1.AddBitmap(IDB_HEROJUMP_2_1, RGB(255, 255, 255));
		jumpAnimation1.AddBitmap(IDB_HEROJUMP_3_1, RGB(255, 255, 255));
		jumpAnimation1.AddBitmap(IDB_HEROJUMP_4_1, RGB(255, 255, 255));

		HeroDashLeft.AddBitmap(IDB_DASH_LEFT_1, RGB(255, 255, 255));
		HeroDashLeft.AddBitmap(IDB_DASH_LEFT_2, RGB(255, 255, 255));
		HeroDashLeft.AddBitmap(IDB_DASH_LEFT_3, RGB(255, 255, 255));
		HeroDashLeft.AddBitmap(IDB_DASH_LEFT_4, RGB(255, 255, 255));

		SwordDashLeft.AddBitmap(IDB_SWORDDASHLEFT_0, RGB(255, 255, 255));
		SwordDashLeft.AddBitmap(IDB_SWORDDASHLEFT_1, RGB(255, 255, 255));
		SwordDashLeft.AddBitmap(IDB_SWORDDASHLEFT_2, RGB(255, 255, 255));
		SwordDashLeft.AddBitmap(IDB_SWORDDASHLEFT_3, RGB(255, 255, 255));

		HeroDashRight.AddBitmap(IDB_DASH_RIGHT_1, RGB(255, 255, 255));
		HeroDashRight.AddBitmap(IDB_DASH_RIGHT_2, RGB(255, 255, 255));
		HeroDashRight.AddBitmap(IDB_DASH_RIGHT_3, RGB(255, 255, 255));
		HeroDashRight.AddBitmap(IDB_DASH_RIGHT_4, RGB(255, 255, 255));

		SwordDashRight.AddBitmap(IDB_SWORDDASHRIGHT_0, RGB(255, 255, 255));
		SwordDashRight.AddBitmap(IDB_SWORDDASHRIGHT_1, RGB(255, 255, 255));
		SwordDashRight.AddBitmap(IDB_SWORDDASHRIGHT_2, RGB(255, 255, 255));
		SwordDashRight.AddBitmap(IDB_SWORDDASHRIGHT_3, RGB(255, 255, 255));

		sword.AddBitmap(IDB_sword_1, RGB(255, 255, 255));
		sword.AddBitmap(IDB_sword_2, RGB(255, 255, 255));
		sword.AddBitmap(IDB_sword_3, RGB(255, 255, 255));
		sword.AddBitmap(IDB_sword_4, RGB(255, 255, 255));
		sword.AddBitmap(IDB_sword_5, RGB(255, 255, 255));

		sword.AddBitmap(IDB_sword_7, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_1_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_2_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_3_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_4_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_5_1, RGB(255, 255, 255));
		sword1.AddBitmap(IDB_SWORD_7_1, RGB(255, 255, 255));
		
		HeroAttackMovement.AddBitmap(IDB_HEROATTACK_1, RGB(255, 255, 255));
		HeroAttackMovement.AddBitmap(IDB_HEROATTACK_2, RGB(255, 255, 255));
		HeroAttackMovement.AddBitmap(IDB_HEROATTACK_3, RGB(255, 255, 255));
		HeroAttackMovement.AddBitmap(IDB_HEROATTACK_4, RGB(255, 255, 255));
		
		HeroAttackMovement1.AddBitmap(IDB_HEROATTACK_1_1, RGB(255, 255, 255));
		HeroAttackMovement1.AddBitmap(IDB_HEROATTACK_2_1, RGB(255, 255, 255));
		HeroAttackMovement1.AddBitmap(IDB_HEROATTACK_3_1, RGB(255, 255, 255));
		HeroAttackMovement1.AddBitmap(IDB_HEROATTACK_4_1, RGB(255, 255, 255));

		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_1, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_2, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_3, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_4, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_5, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_6, RGB(255, 255, 255));
		HeroRollLeft.AddBitmap(IDB_ROLL_LEFT_6, RGB(255, 255, 255));

		SwordRollRight.AddBitmap(IDB_SWORDROLLRIGHT_0, RGB(255, 255, 255));
		SwordRollRight.AddBitmap(IDB_SWORDROLLRIGHT_1, RGB(255, 255, 255));
		SwordRollRight.AddBitmap(IDB_SWORDROLLRIGHT_2, RGB(255, 255, 255));
		SwordRollRight.AddBitmap(IDB_SWORDROLLRIGHT_3, RGB(255, 255, 255));

		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_1, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_2, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_3, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_4, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_5, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_6, RGB(255, 255, 255));
		HeroRollRight.AddBitmap(IDB_ROLL_RIGHT_6, RGB(255, 255, 255));

		SwordRollLeft.AddBitmap(IDB_SWORDROLLLEFT_0, RGB(255, 255, 255));
		SwordRollLeft.AddBitmap(IDB_SWORDROLLLEFT_1, RGB(255, 255, 255));
		SwordRollLeft.AddBitmap(IDB_SWORDROLLLEFT_2, RGB(255, 255, 255));
		SwordRollLeft.AddBitmap(IDB_SWORDROLLLEFT_3, RGB(255, 255, 255));

		swordAttack.AddBitmap(IDB_SWORDATTACK_2,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_3,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_4,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_5,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_6,RGB(255, 255, 255));
		swordAttack.AddBitmap(IDB_SWORDATTACK_7,RGB(255, 255, 255));
		
		SwordDashLeft.AddBitmap(IDB_SWORDDASHLEFT_0, RGB(255, 255, 255));
		SwordDashLeft.AddBitmap(IDB_SWORDDASHLEFT_0, RGB(255, 255, 255));
		SwordDashLeft.AddBitmap(IDB_SWORDDASHLEFT_0, RGB(255, 255, 255));
		SwordDashLeft.AddBitmap(IDB_SWORDDASHLEFT_0, RGB(255, 255, 255));

		SwordDashRight.AddBitmap(IDB_SWORDDASHRIGHT_0, RGB(255, 255, 255));
		SwordDashRight.AddBitmap(IDB_SWORDDASHRIGHT_1, RGB(255, 255, 255));
		SwordDashRight.AddBitmap(IDB_SWORDDASHRIGHT_2, RGB(255, 255, 255));
		SwordDashRight.AddBitmap(IDB_SWORDDASHRIGHT_3, RGB(255, 255, 255));


		swordAttack1.AddBitmap(IDB_SWORDATTACK_3_1, RGB(255, 255, 255));
		swordAttack1.AddBitmap(IDB_SWORDATTACK_4_1, RGB(255, 255, 255));
		swordAttack1.AddBitmap(IDB_SWORDATTACK_5_1, RGB(255, 255, 255));
		swordAttack1.AddBitmap(IDB_SWORDATTACK_6_1, RGB(255, 255, 255));
		swordAttack1.AddBitmap(IDB_SWORDATTACK_7_1, RGB(255, 255, 255));
	
		FireSwordRightAnimation.AddBitmap(IDB_FIRESWORDRIGHT_0, RGB(255, 255, 255));
		FireSwordRightAnimation.AddBitmap(IDB_FIRESWORDRIGHT_1, RGB(255, 255, 255));
		FireSwordRightAnimation.AddBitmap(IDB_FIRESWORDRIGHT_2, RGB(255, 255, 255));
		FireSwordRightAnimation.AddBitmap(IDB_FIRESWORDRIGHT_3, RGB(255, 255, 255));
		FireSwordRightAnimation.AddBitmap(IDB_FIRESWORDRIGHT_4, RGB(255, 255, 255));
		FireSwordRightAnimation.AddBitmap(IDB_FIRESWORDRIGHT_5, RGB(255, 255, 255));
		FireSwordRightAnimation.AddBitmap(IDB_FIRESWORDRIGHT_6, RGB(255, 255, 255));

		FireSwordLeftAnimation.AddBitmap(IDB_FIRESWORDLEFT_0, RGB(255, 255, 255));
		FireSwordLeftAnimation.AddBitmap(IDB_FIRESWORDLEFT_1, RGB(255, 255, 255));
		FireSwordLeftAnimation.AddBitmap(IDB_FIRESWORDLEFT_2, RGB(255, 255, 255));
		FireSwordLeftAnimation.AddBitmap(IDB_FIRESWORDLEFT_3, RGB(255, 255, 255));
		FireSwordLeftAnimation.AddBitmap(IDB_FIRESWORDLEFT_4, RGB(255, 255, 255));
		FireSwordLeftAnimation.AddBitmap(IDB_FIRESWORDLEFT_5, RGB(255, 255, 255));
		FireSwordLeftAnimation.AddBitmap(IDB_FIRESWORDLEFT_6, RGB(255, 255, 255));

		FireCircle.AddBitmap(IDB_FIRECIRCLE_0, RGB(63, 72, 204));
		FireCircle.AddBitmap(IDB_FIRECIRCLE_1, RGB(63, 72, 204));
		FireCircle.AddBitmap(IDB_FIRECIRCLE_2, RGB(63, 72, 204));
		FireCircle.AddBitmap(IDB_FIRECIRCLE_3, RGB(63, 72, 204));
		FireCircle.AddBitmap(IDB_FIRECIRCLE_4, RGB(63, 72, 204));
		FireCircle.AddBitmap(IDB_FIRECIRCLE_5, RGB(63, 72, 204));

		Fire1.AddBitmap(IDB_FIRE_0, RGB(63, 72, 204));
		Fire1.AddBitmap(IDB_FIRE_1, RGB(63, 72, 204));
		Fire1.AddBitmap(IDB_FIRE_2, RGB(63, 72, 204));
		Fire1.AddBitmap(IDB_FIRE_3, RGB(63, 72, 204));
		Fire1.AddBitmap(IDB_FIRE_4, RGB(63, 72, 204));
		Fire1.AddBitmap(IDB_FIRE_5, RGB(63, 72, 204));
		
		Fire2.AddBitmap(IDB_FIRE_0, RGB(63, 72, 204));
		Fire2.AddBitmap(IDB_FIRE_1, RGB(63, 72, 204));
		Fire2.AddBitmap(IDB_FIRE_2, RGB(63, 72, 204));
		Fire2.AddBitmap(IDB_FIRE_3, RGB(63, 72, 204));
		Fire2.AddBitmap(IDB_FIRE_4, RGB(63, 72, 204));
		Fire2.AddBitmap(IDB_FIRE_5, RGB(63, 72, 204));

		Fire3.AddBitmap(IDB_FIRE_0, RGB(63, 72, 204));
		Fire3.AddBitmap(IDB_FIRE_1, RGB(63, 72, 204));
		Fire3.AddBitmap(IDB_FIRE_2, RGB(63, 72, 204));
		Fire3.AddBitmap(IDB_FIRE_3, RGB(63, 72, 204));
		Fire3.AddBitmap(IDB_FIRE_4, RGB(63, 72, 204));
		Fire3.AddBitmap(IDB_FIRE_5, RGB(63, 72, 204));


		LifeBarHead.LoadBitmap(IDB_LIFEBARHEAD, RGB(255, 255, 255));
		StartGameBar.LoadBitmap(IDB_UI_GAME_START);
		WorldMap_UI_1.LoadBitmap(IDB_WORLDMAP_UI);
		QuitButton.LoadBitmap(IDB_UI_QUIT, RGB(0, 0, 0));
		Num.LoadBitmap();
		Num_Red.LoadBitmap();
		Num_Gold.LoadBitmap();
		BlackMask.LoadBitmap(IDB_BLACKMASK, RGB(27, 36, 46));
		Word_G.LoadBitmap(IDB_WORD_G, RGB(255, 255, 255));
		Word_Gold.LoadBitmap(IDB_WORD_GOLD, RGB(255, 255, 255));
		//DamageTaken.LoadBitmap();
		for (vector<CMovingBitmap*>::iterator i = LifeBarRed.begin(); i != LifeBarRed.end(); i++) (*i)->LoadBitmap(IDB_LIFEBAR, RGB(255, 255, 255));

		SelectMap(0);
	}

	int CHero::GetHeroFullHP()
	{
		return FullHP;
	}

	int CHero::GetHeroCurrentHP()
	{
		return CurrentHP;
	}

	int CHero::GetPreviousMove()
	{
		return PreviousMovement;
	}

	void CHero::OnMove()
	{
		if (isInHome)//在home時
		{
			CurrentHP = FullHP;
		}
		else
		{
			ClearedStage = currentWild->GetisStageClear();
		}

		const int STEP_SIZE = 10;
		animation.OnMove();
		animation1.OnMove();
		sword.OnMove();
		sword1.OnMove();
		HeroAttackMovement.OnMove();
		HeroAttackMovement1.OnMove();
		swordAttack.OnMove();
		swordAttack1.OnMove();
		moveRightAnimation.OnMove();
		HeroDashLeft.OnMove();
		HeroDashRight.OnMove();
		moveLeftAnimation.OnMove();
		jumpAnimation.OnMove();
		jumpAnimation1.OnMove();
		HeroRollLeft.OnMove();
		HeroRollRight.OnMove();
		SwordRollRight.OnMove();
		SwordRollLeft.OnMove();
		SwordDashRight.OnMove();
		SwordDashLeft.OnMove();
		FireSwordRightAnimation.OnMove();
		FireSwordLeftAnimation.OnMove();
		FireCircle.OnMove();
		Fire1.OnMove();
		Fire2.OnMove();
		Fire3.OnMove();
		if (SpecialEffectCount == 0) SpecialEffect = 0;			//被攻擊3次後，特殊效果消失
		if (ShowGoldDelayCount > 0) ShowGoldDelayCount--;
		if(AttackDelayCount !=0) AttackDelayCount--;    //攻速
		if (RollDelayCount != 0) RollDelayCount--;		//翻滾
		if (InvincibleDelayCount != 0) InvincibleDelayCount--;  //無敵時間
		if (DashColdDown != 0) DashColdDown--;      //衝刺
		if (MoveDelayCount != 0) MoveDelayCount--;   //紀錄上個動作的保持時間
		if (MoveDelayCount == 0) SetPreviousMove(0);  //抹除上個動作紀錄
		if (InvincibleDelayCount == 0) isInvincible = false;

		if (isMovingLeft)
		{
			setHeroDirection("left");   //角色向左看
			if (currentMap->isSpace(GetX1(), GetY1()) && currentMap->isSpace(GetX1(), GetY2() - 10)) // 當x座標還沒碰到牆
			{
				if (PreviousMovement == 1 && DashColdDown==0)
				{
					x -= 80;    //衝刺距離
					DashColdDown = 15;    // 衝刺冷卻時間
					HeroDashLeft.Reset();  //重置動畫
					SwordDashLeft.Reset();
				}
				else
				{
					x -= STEP_SIZE;    //正常走
				}
			}
		}
		if (isMovingRight)
		{
			setHeroDirection("right");   //角色向右看
			if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2()-10)) // 當y座標還沒碰到牆
			{
				if (PreviousMovement == 2 && DashColdDown == 0)
				{
					x += 80;     //衝刺距離
					DashColdDown = 15;    // 衝刺冷卻時間
					HeroDashRight.Reset();   //重置動畫
					SwordDashRight.Reset();
				}
				else
				{
					x += STEP_SIZE;    //正常走
				}
			}
		}
		if (isRolling)
		{
			if (faceDirection == "right")
			{
				HeroRollRight.OnMove();
				SwordRollRight.OnMove();
				for (int i = 0; i < 15; i++)
				{
					if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10)) // 當y座標還沒碰到牆
					{
						x ++;
					}
				}
				if (HeroRollRight.IsFinalBitmap()) isRolling = false;
			}
			else
			{
				HeroRollLeft.OnMove();
				SwordRollLeft.OnMove();

				for (int i = 0; i < 15; i++)
				{
					if (currentMap->isSpace(GetX2(), GetY1()) && currentMap->isSpace(GetX2(), GetY2() - 10)) // 當y座標還沒碰到牆
					{
						x --;
					}
				}
				if (HeroRollLeft.IsFinalBitmap()) isRolling = false;
			}
		}
		if (isMovingUp && y == (floor))
		{
			rising = true;						// 改為上升狀態
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

		if (isAttacking && !isInHome)
		{
			if (faceDirection == "right")
			{
				currentWild->SetHeroAttackRange(GetCenterX() - AttackRange, GetCenterX() + AttackRange + 50, GetCenterY() - 50, GetCenterY() + 50);
			}
			else
			{
				currentWild->SetHeroAttackRange(GetCenterX() - AttackRange - 50, GetCenterX() + AttackRange, GetCenterY() - 50, GetCenterY() + 50);
			}
			currentWild->AttackByHero(heroAttackDamage);
		}

		if (isTalkingToNPC && isInHome)
		{
			currentVillage->HeroTalkToNPC(true);
		}
		else
		{
			currentVillage->HeroTalkToNPC(false);
		}

		if (!isRolling && !isInvincible && !isInHome)
		{
			bleed = AttackByEnemy();
			currentWild->SetHeroXY(GetX1(), GetX2(), GetY1(), GetY2());
			if (ShowGoldDelayCount > 0)
			{
				HeroGetItem();
			}
			else
			{
				GetGold = HeroGetItem();   //重設顯示數字
			}
			
		}
		currentMap->SetSXSY(GetCenterX() - SIZE_X / 2, GetCenterY() - SIZE_Y / 2);
		if (isInHome)
		{
			currentVillage->setHeroState(GetX1(), GetX2(), GetY1(), GetY2(), FullHP, Gold, heroAttackDamage, HeroLevel);
			currentVillage->OnMove();
		}
		else
		{
			currentWild->setHeroState(GetX1(), GetX2(), GetY1(), GetY2(), FullHP, Gold, heroAttackDamage, HeroLevel);
			currentWild->OnMove();
		}
	}

	void CHero::SetMovingDown(bool flag)
	{
		isMovingDown = flag;
	}

	void CHero::SetMovingLeft(bool flag)
	{
		isMovingLeft = flag;
	}

	void CHero::SetMovingRight(bool flag)
	{
		isMovingRight = flag;
	}

	void CHero::SetMovingUp(bool flag)
	{
		if (isMovingUp == false && flag == true)
		{
			jumpAnimation.Reset();
			jumpAnimation1.Reset();
			isMovingUp = true;
		}
	}

	void CHero::SetTalkingToNPC(bool flag)
	{
		isTalkingToNPC = flag;
	}

	void CHero::SetEndTalking()
	{
		isTalkingToNPC = false;
	}
	void CHero::SetHeroAttack(bool flag)
	{
		if (AttackDelayCount == 0 && flag == true) {
			swordAttack1.Reset();
			swordAttack.Reset();
			HeroAttackMovement.Reset();
			HeroAttackMovement1.Reset();
			FireSwordRightAnimation.Reset();
			FireSwordLeftAnimation.Reset();
			isAttacking = true;
			AttackDelayCount = SetAttackDelayCount;
			isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isRolling = false;   //角色不能邊走邊攻擊
		}
	}
	void CHero::SetHeroRoll (bool flag)
	{
		if (RollDelayCount == 0 && flag == true && !rising) {
			HeroRollLeft.Reset();
			HeroRollRight.Reset();
			isRolling = true;
			RollDelayCount = 15;
			
			isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isAttacking = false;   //角色不能邊走邊攻擊
		}
	}
	void CHero::SetXY(int nx, int ny)
	{
		x = nx; y = ny;
	}
	
	void CHero::SetPreviousMove(int move)
	{
		PreviousMovement = move;
	}

	void CHero::SetMoveDelayCount(int delay)
	{
		MoveDelayCount = delay;
	}

	void CHero::setHeroDirection(string direction)
	{
		faceDirection = direction;
	}


	void CHero::changeLifeBarLength()
	{
		int xMove = currentMap->ScreenX(x - 250);
		int yMove = currentMap->ScreenY(y - 201);
		int counter = xMove;
		float lengthOfLifeBar = ((float)CurrentHP / (float)FullHP) * 300;  //重新計算血條長度

		for (vector<CMovingBitmap*>::iterator i = LifeBarRed.begin(); i != LifeBarRed.end(); i++)
		{
			//TRACE("%d\n", xMove);
			(*i)->SetTopLeft(xMove, yMove);
			if((*i)->Left() < 45+lengthOfLifeBar) (*i)->ShowBitmap();
			xMove += 3;
		}
	}
	void CHero::OnShow()
	{
		//處理UI的顯示
		if (isInHome) currentVillage->OnShow();
		else currentWild->OnShow();
		
		LifeBarHead.SetTopLeft(currentMap->ScreenX(x-290), currentMap->ScreenY(y-205));
		LifeBarHead.ShowBitmap();  //顯示血條
		changeLifeBarLength();
		ShowNumber(3, Gold, currentMap->ScreenX(x + 250), currentMap->ScreenY(y - 195));
		Word_Gold.SetTopLeft(currentMap->ScreenX(x + 200), currentMap->ScreenY(y - 195));
		Word_Gold.ShowBitmap();
		if (isInHome && currentVillage->GetHeroIsTalkingToNPC())
		{
			ShowNumber(1, Gold, currentMap->ScreenX(x + 190), currentMap->ScreenY(y - 175));
			ShowNumber(1, HeroLevel, currentMap->ScreenX(x + 90), currentMap->ScreenY(y - 175));
			ShowNumber(1, FullHP, currentMap->ScreenX(x + 110), currentMap->ScreenY(y - 135));
			ShowNumber(1, heroAttackDamage, currentMap->ScreenX(x + 140), currentMap->ScreenY(y - 110));
		}
		ShowNumber(2, CurrentHP, currentMap->ScreenX(x - 280), currentMap->ScreenY(y - 170));
		if(bleed != 0 && InvincibleDelayCount!= 0) ShowNumber(2,bleed, currentMap->ScreenX(x - 50), currentMap->ScreenY(y - 50));
		//TRACE("%d\n", GetGold);
		if (ShowGoldDelayCount > 0)
		{
			ShowNumber(3, GetGold, currentMap->ScreenX(x - 50), currentMap->ScreenY(y - 30));
			Word_G.SetTopLeft(currentMap->ScreenX(x + 2), currentMap->ScreenY(y - 30));
			Word_G.ShowBitmap();
		}
		/*if (AttackByEnemy() != 0)
		{
			DamageTaken.SetInteger(AttackByEnemy());
			DamageTaken.ShowBitmap();
		}*/

		//處理劍的顯示
		if (SpecialEffect == 1)
		{
			switch (SpecialEffectCount)
			{
			case 3:
				Fire1.SetTopLeft(currentMap->ScreenX(x - 30), currentMap->ScreenY(y));
				Fire2.SetTopLeft(currentMap->ScreenX(x + 45), currentMap->ScreenY(y));
				Fire3.SetTopLeft(currentMap->ScreenX(x + 10), currentMap->ScreenY(y - 45));
				Fire1.OnShow();
				Fire2.OnShow();
				Fire3.OnShow();
				break;
			case 2:
				Fire1.SetTopLeft(currentMap->ScreenX(x - 30), currentMap->ScreenY(y));
				Fire2.SetTopLeft(currentMap->ScreenX(x + 45), currentMap->ScreenY(y));
				Fire1.OnShow();
				Fire2.OnShow();
				break;
			case 1:
				Fire1.SetTopLeft(currentMap->ScreenX(x - 30), currentMap->ScreenY(y));
				Fire1.OnShow();
				break;
			default:
				break;
			}
			FireCircle.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y + 5));
			FireCircle.OnShow();
		}
		if (isAttacking)
		{
			if (faceDirection == "right")
			{
				swordAttack.SetTopLeft(currentMap->ScreenX(x - 40), currentMap->ScreenY(y + 10));
				swordAttack.OnShow();
				if (SpecialEffect == 1)
				{
					FireSwordRightAnimation.SetTopLeft(currentMap->ScreenX(x - 50), currentMap->ScreenY(y - 10));
					FireSwordRightAnimation.OnShow();
				}
			}
			else
			{
				swordAttack1.SetTopLeft(currentMap->ScreenX(x - 95), currentMap->ScreenY(y + 10));
				swordAttack1.OnShow();
				if (SpecialEffect == 1)
				{
					FireSwordLeftAnimation.SetTopLeft(currentMap->ScreenX(x - 90), currentMap->ScreenY(y - 10));
					FireSwordLeftAnimation.OnShow();
				}
			}

			if (swordAttack.IsFinalBitmap() || swordAttack1.IsFinalBitmap())
			{
				isAttacking = false;
			}
		}
		else if (isMovingRight)		// 向右走
		{
			if (!(PreviousMovement != 2 && DashColdDown == 0))
			{
				SwordDashRight.SetTopLeft(currentMap->ScreenX(x - 100), currentMap->ScreenY(y + 35));
				SwordDashRight.OnShow();
			}
			else
			{
				sword.SetTopLeft(currentMap->ScreenX(x - 80), currentMap->ScreenY(y + 30));
				sword.OnShow();
			}
		}
		else if (isMovingLeft)		// 向右走
		{
			if (!(PreviousMovement != 2 && DashColdDown == 0))
			{
				SwordDashLeft.SetTopLeft(currentMap->ScreenX(x + 43), currentMap->ScreenY(y + 35));
				SwordDashLeft.OnShow();
			}
			else
			{
				sword1.SetTopLeft(currentMap->ScreenX(x + 17), currentMap->ScreenY(y + 30));
				sword1.OnShow();
			}
		}
		else if (isRolling)
		{
			if (faceDirection == "right")
			{
				SwordRollRight.SetTopLeft(currentMap->ScreenX(x - 25), currentMap->ScreenY(y-5));
				SwordRollRight.OnShow();
			}
			else
			{
				SwordRollLeft.SetTopLeft(currentMap->ScreenX(x - 20), currentMap->ScreenY(y - 5));
				SwordRollLeft.OnShow();
			}
		}
		else
		{
			if (faceDirection == "right")   // 靜止向右看
			{
				sword.SetTopLeft(currentMap->ScreenX(x - 85), currentMap->ScreenY(y + 30));
				sword.OnShow();
			}
			else							// 靜止向左看
			{
				sword1.SetTopLeft(currentMap->ScreenX(x + 17), currentMap->ScreenY(y + 30));
				sword1.OnShow();
			}
		}

		//處理主角的顯示
		if (isMovingUp)	// 往上跳
		{
			if (faceDirection == "right")
			{
				jumpAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				jumpAnimation.OnShow();
				if (jumpAnimation.IsFinalBitmap())
				{
					isMovingUp = false;
					jumpAnimation.Reset();
				}
			}
			else
			{
				jumpAnimation1.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				jumpAnimation1.OnShow();
				if (jumpAnimation1.IsFinalBitmap())
				{
					isMovingUp = false;
					jumpAnimation1.Reset();
				}
			}
			
		}
		else if (isMovingRight)		// 向右走
		{
			if (PreviousMovement != 2 && DashColdDown == 0)
			{
				moveRightAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				moveRightAnimation.OnShow();
			}
			else   //右衝刺
			{
				HeroDashRight.SetTopLeft(currentMap->ScreenX(x - 50), currentMap->ScreenY(y));
				HeroDashRight.OnShow();
				if(HeroDashRight.IsFinalBitmap())
				{
					DashColdDown = 0;    //動畫結束
				}
			}
		}
		else if (isMovingLeft)	// 向左走
		{
			if (PreviousMovement != 1 && DashColdDown==0)
			{
				moveLeftAnimation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				moveLeftAnimation.OnShow();
			}
			else     //左衝刺
			{
				HeroDashLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroDashLeft.OnShow();
				if (HeroDashLeft.IsFinalBitmap())
				{
					DashColdDown = 0;
				}
			}
		}
		else if (isRolling)
		{
			if (faceDirection == "right")
			{
				HeroRollRight.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y+15));
				HeroRollRight.OnShow();
			}
			else
			{
				HeroRollLeft.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y+15));
				HeroRollLeft.OnShow();
			}
		}
		else if (isAttacking)
		{
			if (faceDirection == "right")
			{
				HeroAttackMovement.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroAttackMovement.OnShow();
				/*swordAttack.SetTopLeft(currentMap->ScreenX(x - 40), currentMap->ScreenY(y));
				swordAttack.OnShow();*/
			}
			else
			{
				HeroAttackMovement1.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				HeroAttackMovement1.OnShow();
				/*swordAttack1.SetTopLeft(currentMap->ScreenX(x - 75), currentMap->ScreenY(y));
				swordAttack1.OnShow();*/
			}
		}
		else
		{
			if (faceDirection == "right")   // 靜止向右看
			{
				animation.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				animation.OnShow();
			}
			else							// 靜止向左看
			{
				animation1.SetTopLeft(currentMap->ScreenX(x), currentMap->ScreenY(y));
				animation1.OnShow();
			}
		}
		
		if (isInHome)
		{
			StartGameBar.SetTopLeft(currentMap->ScreenX(x - 100), currentMap->ScreenY(y + 150));
			StartGameBar.ShowBitmap();
		}

		if (isSelectingMap)
		{
			WorldMap_UI_1.SetTopLeft(currentMap->ScreenX(x - 270), currentMap->ScreenY(y - 170));
			WorldMap_UI_1.ShowBitmap();
		}
		if (ClearedStage)
		{
			BlackMask.SetTopLeft(0, 0);
			BlackMask.ShowBitmap();
			QuitButton.SetTopLeft(currentMap->ScreenX(x + 100), currentMap->ScreenY(y + 170));
			QuitButton.ShowBitmap();
		}

	}

	void CHero::SetHeroHP(int inputHP)
	{
		FullHP = inputHP;
	}

	/*
	void CHero::SetMap(int index)
	{
		GAME_ASSERT((int)maps.size() > index, "CHero: SetMap input index over range");
		currentMap = maps[index];

		const int X_POS = 0;
		const int Y_POS = 0;
		x = X_POS;
		y = Y_POS;
		isMovingLeft = isMovingRight = isMovingUp = isMovingDown = isAttacking = false;

		const int INITIAL_VELOCITY = 15;	// 初始上升速度
		const int FLOOR = 100;				// 地板座標
		floor = FLOOR;
		rising = false;
		initial_velocity = INITIAL_VELOCITY;
		velocity = initial_velocity;
	}
	*/

	int CHero::AttackByEnemy()
	{
		int hp = CurrentHP;
		currentWild->AttackByEnemy(&CurrentHP);

		if (CurrentHP != hp)
		{
			if (SpecialEffectCount > 0)
			{
				SpecialEffectCount -= 1;
				if (SpecialEffectCount == 0 && SpecialEffect == 1)   //火焰石效果消失
				{
					heroAttackDamage /= 2;   //回歸原本的攻擊力
				}
			}
			isInvincible = true;
			InvincibleDelayCount = 30;
		}
		return hp - CurrentHP;
	}

	int CHero::HeroGetItem()
	{
		int Coin = Gold;
		int SpecialEffectDectect = SpecialEffect;
		currentWild->HeroGetItem(&Gold, &SpecialEffect, &SpecialEffectCount, &CurrentHP, FullHP);
		if (Coin < Gold && (Gold - Coin)>=10)
		{
			//TRACE("%d\n",Gold-Coin);
			ShowGoldDelayCount = 30;
		}
		if (SpecialEffectDectect != SpecialEffect && SpecialEffect == 1) heroAttackDamage *= 2;  //火焰石攻擊力加倍
		return Gold - Coin;
	}

	void CHero::ShowNumber(int color, int num, int x, int y)
	{
		if (color == 1)
		{
			Num.SetInteger(num);
			Num.SetTopLeft(x, y);
			Num.ShowBitmap();
		}
		else if(color == 2)
		{
			Num_Red.SetInteger(num);
			Num_Red.SetTopLeft(x, y);
			Num_Red.ShowBitmap();
		}
		else
		{
			Num_Gold.SetInteger(num);
			Num_Gold.SetTopLeft(x, y);
			Num_Gold.ShowBitmap();
		}
	}

	void CHero::HeroLevelUp()
	{
		FullHP += 5;
		heroAttackDamage += 5;
		HeroLevel += 1;
		Gold -= 10;
	}

	void CHero::SelectMap(int MapNumber)
	{
		if (MapNumber == 0)
			isInHome = true;
		else
			isInHome = false;

		switch (MapNumber)
		{
		case 0:
			if(currentVillage!=NULL)
				delete currentVillage;
			currentVillage = new gameMap_village();
			currentVillage->LoadBitmap();
			currentMap = currentVillage;
			
			break;
		case 1:
			
			if(currentWild!=NULL)
				delete currentWild;
			currentWild = new gameMap_Lv1();
			currentWild->LoadBitmap();
			CAudio::Instance()->Play(6, false);
			currentMap = currentWild;
			break;
			
		case 2:
			
			if(currentWild!=NULL)
				delete currentWild;
			currentWild = new gameMap_Lv2();
			currentWild->LoadBitmap();
			CAudio::Instance()->Play(6, false);
			currentMap = currentWild;
			break;
		}
		x = 0;
		y = 0;

	}

	void CHero::ResetHeroState()
	{
		//currentMap->isStageClear = false;
		ClearedStage = false;
		CurrentHP = FullHP;
		isInHome = true;
		SelectMap(0);
		if (SpecialEffect == 1 && SpecialEffectCount != 0) heroAttackDamage /= 2;
		SpecialEffect = 0;
		SpecialEffectCount = 0;
		x = 0;    
		y = 0;
	}

	void CHero::OnLButtonDown(int Mx, int My)
	{
		if (isTalkingToNPC)
		{
			if ((Mx <= 630) && (My <= 50) && (Mx >= 609) && (My >= 28)) SetEndTalking();   //右上角xx
			if ((Mx <= 625) && (My <= 216) && (Mx >= 561) && (My >= 199)) SetEndTalking();  //cancel
			if ((Mx <= 619) && (My <= 184) && (Mx >= 524) && (My >= 144) && Gold >= 10) HeroLevelUp();
		}

		if (isInHome)   //在村莊
		{
			if (isSelectingMap)   //按GAME_START後選擇地圖畫面
			{
				if ((Mx <= 612) && (My <= 87) && (Mx >= 570) && (My >= 48)) isSelectingMap = false;   //右上角xx
				if ((Mx <= 222) && (My <= 260) && (Mx >= 187) && (My >= 222))   //第一章地圖
				{
					SelectMap(1);
					isSelectingMap = false;
				}
				if ((Mx <= 300) && (My <= 261) && (Mx >= 259) && (My >= 219))   //第一章地圖
				{
					SelectMap(2);
					isSelectingMap = false;
				}
			}
			else
			{
				if ((Mx <= 437) && (My <= 415) && (Mx >= 188) && (My >= 355)) isSelectingMap = true;
			}
		}
		if (ClearedStage)
		{
			if ((Mx <= 487) && (My <= 422) && (Mx >= 396) && (My >= 380))
			{
				ResetHeroState();
			}
		}
	}

	bool CHero::GetHeroIsRolling()
	{
		return isRolling;
	}
}