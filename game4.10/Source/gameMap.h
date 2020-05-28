namespace game_framework {
	class NPC;
	class CEnemy;
	class Item;
	class MapObject;
	class gameMap {
	public:
		gameMap(string fileName);
		~gameMap();

		int ScreenX(int x);
		int ScreenY(int y);
		int GetBlockY(int y);
		bool isSpace(int x, int y);  //判斷是否為空氣，是就回傳True
		void SetSXSY(int x, int y);   //設定(sx, sy)為螢幕(的左上角)在地圖上的點座標
		void LoadBitmap();    //載入地圖
		void OnShow();		  //顯示地圖


	protected:
		CMovingBitmap ground; //建立草地圖案
		CMovingBitmap ground1; //建立土地圖案
		CMovingBitmap ground2; //建立斜坡1圖案
		CMovingBitmap ground3; //建立斜坡2圖案
		int map[32][96];    //建立一個地圖矩陣的index;
		const int X, Y;		  //大地圖左上角x,y座標
		const int MW, MH;	  //每張小地圖的寬高度
		int sx, sy;			  //(sx, sy)為螢幕(的左上角)在地圖上的點座標
	};


	class gameMap_village : public gameMap {
	public:
		gameMap_village();
		~gameMap_village();

		void LoadBitmap();    //載入地圖
		void OnShow();		  //顯示地圖
		void OnMove();
		void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level);
		void HeroTalkToNPC(bool flag);
		bool GetHeroIsTalkingToNPC();

	private:
		vector<NPC*> allNPC;
		bool HeroIsTalkingToNPC = false;
	};

	class gameMap_wild : public gameMap {
	public:
		gameMap_wild(string fileName);
		virtual ~gameMap_wild() = default;

		virtual void LoadBitmap() = 0;    //載入地圖
		virtual void OnShow() = 0;		  //顯示地圖
		virtual void OnMove() = 0;
		virtual void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level) = 0;
		virtual void AttackByHero(const int damage) = 0;		// 攻擊
		virtual void AttackByEnemy(int *heroHP) = 0;
		virtual void SetHeroAttackRange(int x1, int x2, int y1, int y2) = 0;
		virtual bool GetisStageClear() = 0;
		virtual void HeroGetItem(int *HeroGold ,int *SpecialEffect, int *SpecialEffectCount, int *HeroHP, int FullHP) = 0;
		virtual void SetHeroXY(int x1, int x2, int y1, int y2) = 0;				// 設定英雄位置
	};

	class gameMap_Lv1 : public gameMap_wild {
	public:
		gameMap_Lv1();
		~gameMap_Lv1();

		void LoadBitmap();    //載入地圖
		void OnShow();		  //顯示地圖
		void OnMove();
		void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level);
		void AttackByHero(const int damage);		// 攻擊
		void AttackByEnemy(int *heroHP);
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);
		bool GetisStageClear();
		void HeroGetItem(int *HeroGold, int *SpecialEffect, int *SpecialEffectCount, int *HeroHP, int FullHP);
		void SetHeroXY(int x1, int x2, int y1, int y2);
	protected:
		int HeroX1;
		int HeroY1;
		int HeroX2;
		int HeroY2;
		int HeroAttackX1;
		int HeroAttackY1;
		int HeroAttackX2;
		int HeroAttackY2;
		int ItemExistTime = 300;
	private:
		vector<CEnemy*> allEnemy;
		vector<Item*> allItem;
		vector<MapObject*> allObject;
		bool isStageClear = false;
		void DropItem(int x, int y);
		void MapObjectInteration();
	};

	class gameMap_Lv2 : public gameMap_wild {
	public:
		gameMap_Lv2();
		~gameMap_Lv2();
		void LoadBitmap();    //載入地圖
		void OnShow();		  //顯示地圖
		void OnMove();
		void setHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level);
		void AttackByHero(const int damage);		// 攻擊
		void AttackByEnemy(int *heroHP);
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);
		bool GetisStageClear();
		void HeroGetItem(int *HeroGold, int *SpecialEffect, int *SpecialEffectCount, int *HeroHP, int FullHP);
		void SetHeroXY(int x1, int x2, int y1, int y2);
	protected:
		int HeroX1;
		int HeroY1;
		int HeroX2;
		int HeroY2;
		int HeroAttackX1;
		int HeroAttackY1;
		int HeroAttackX2;
		int HeroAttackY2;
		int ItemExistTime = 300;
	private:
		vector<CEnemy*> allEnemy;
		bool isStageClear = false;
		vector<MapObject*> allObject;
		vector<Item*> allItem;
		void DropItem(int x, int y);
		void MapObjectInteration();
	};

	class Item
	{
	public:
		Item(gameMap* point, int nx, int ny, int ExistTime);
		virtual ~Item() = default;
		//~Item();
		int GetX1();					// 物品左上角 x 座標
		int GetY1();					// 物品左上角 y 座標
		int GetX2();					// 物品右下角 x 座標
		int GetY2();					// 物品右下角 y 座標
		void OnMove();					// 移動物品
		void OnShow();					// 將物品圖形貼到畫面
		bool isDelete();
		virtual int GetItemValue() = 0;
		virtual void LoadBitmap() = 0;
	protected:
		CAnimation animation;
		int x;
		int y;
		gameMap *currentMap;
		int ExistTime = 999999;
	};

	class Item_Bronze_Coin : public Item
	{
	public:
		Item_Bronze_Coin(gameMap* point, int nx, int ny, int ExistTime);
		~Item_Bronze_Coin();
		void LoadBitmap();				// 載入圖形
		int GetItemValue();
	};

	class Item_Silver_Coin : public Item
	{
	public:
		Item_Silver_Coin(gameMap* point, int nx, int ny, int ExistTime);
		~Item_Silver_Coin();
		void LoadBitmap();				// 載入圖形
		int GetItemValue();
	};

	class Item_Golden_Coin : public Item
	{
	public:
		Item_Golden_Coin(gameMap* point, int nx, int ny, int ExistTime);
		~Item_Golden_Coin();
		void LoadBitmap();				// 載入圖形
		int GetItemValue();
	};

	class Item_Fire_Stone : public Item
	{
	public:
		Item_Fire_Stone(gameMap* point, int nx, int ny, int ExistTime);
		~Item_Fire_Stone();
		void LoadBitmap();				// 載入圖形
		int GetItemValue();
	};

	class Item_RedPot_Small : public Item
	{
	public:
		Item_RedPot_Small(gameMap* point, int nx, int ny, int ExistTime);
		~Item_RedPot_Small();
		void LoadBitmap();				// 載入圖形
		int GetItemValue();
	};

	class Item_RedPot_Medium : public Item
	{
	public:
		Item_RedPot_Medium(gameMap* point, int nx, int ny, int ExistTime);
		~Item_RedPot_Medium();
		void LoadBitmap();				// 載入圖形
		int GetItemValue();
	};

	class Item_RedPot_Full : public Item
	{
	public:
		Item_RedPot_Full(gameMap* point, int nx, int ny, int ExistTime);
		~Item_RedPot_Full();
		void LoadBitmap();				// 載入圖形
		int GetItemValue();
	};

	class MapObject
	{
	public:
		MapObject(gameMap* point, int nx, int ny, bool InitialState, int SetInterationCode);
		virtual ~MapObject() = default;
		virtual int GetX1() = 0;					// 物品左上角 x 座標
		virtual int GetY1() = 0;					// 物品左上角 y 座標
		virtual int GetX2() = 0;					// 物品右下角 x 座標
		virtual int GetY2() = 0;					// 物品右下角 y 座標
		virtual void OnMove() = 0;					// 移動物品
		virtual void OnShow() = 0;					// 將物品圖形貼到畫面
		virtual void LoadBitmap() = 0;
		virtual void GetAttack(int HeroX1, int HeroY1, int HeroX2, int HeroY2) = 0;
		virtual int GetInterationCode() = 0;
		virtual void SetState(bool State) = 0;
		virtual bool GetState() = 0;
		virtual void AttackByObject(int HeroX1, int HeroY1, int HeroX2, int HeroY2, int *heroHP) = 0;
	protected:
		int x;
		int y;
		bool ObjectState;
		gameMap *currentMap;
		int InterationCode = 0;			// 預設為0,用於跟其他地圖物件互動ex:-1 to 1 ;-5 to 5
	};

	class Switch : public MapObject
	{
	public:
		Switch(gameMap* point, int nx, int ny, bool InitialState , int SetInterationCode);
		~Switch();
		int GetX1();
		int GetY1();
		int GetX2();
		int GetY2();
		void OnMove();
		void OnShow();
		void LoadBitmap();
		void GetAttack(int HeroX1, int HeroY1, int HeroX2, int HeroY2);
		int GetInterationCode();
		void SetState(bool State);
		bool GetState();
		void AttackByObject(int HeroX1, int HeroY1, int HeroX2, int HeroY2, int *heroHP);
	private:
		CMovingBitmap SwitchOn;
		CMovingBitmap SwitchOff;
		int GetHitDelayCount = 0;
		
	};

	class Spike : public MapObject
	{
	public:
		Spike(gameMap* point, int nx, int ny, bool InitialState , int SetInterationCode);
		~Spike();
		int GetX1();
		int GetY1();
		int GetX2();
		int GetY2();
		void OnMove();
		void OnShow();
		void LoadBitmap();
		void GetAttack(int HeroX1, int HeroY1, int HeroX2, int HeroY2);
		int GetInterationCode();
		void SetState(bool State);
		bool GetState();
		void AttackByObject(int HeroX1, int HeroY1, int HeroX2, int HeroY2, int *heroHP);
	private:
		CMovingBitmap SpikeUp;
		CMovingBitmap SpikeDown;
		int GetHitDelayCount = 0;
		int SpikeDamage = 20;
	};
}