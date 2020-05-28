namespace game_framework {
	class gameMap;
	class NPC 
	{
	public:
		NPC(gameMap* currentMap, int nx, int ny);
		virtual ~NPC();
		virtual void LoadBitmap() = 0;				// 載入圖形
		virtual void OnMove() = 0;					// 移動NPC
		virtual void OnShow() = 0;					// 將NPC圖形貼到畫面
		virtual void SetIsTalkingToHero(bool flag) = 0;
		bool isTalkingToHero = false;
		//virtual void TouchedByHero(int x1, int x2, int y1, int y2);
		void SetHeroState(int x1, int x2, int y1, int y2, int HP, int Gold, int AttackDamage, int Level);
	protected:
		int x, y;						// NPC左上角座標
		map<string, int> hero;
		gameMap* currentMap;
	};


	class NPC_oldMan : public NPC
	{
	public:
		NPC_oldMan(gameMap* currentMap, int nx, int ny);
		~NPC_oldMan();
		int GetX1();
		int GetY1();
		int GetX2();
		int GetY2();
		bool isAroundHero();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動NPC
		void OnShow();					// 將NPC圖形貼到畫面
	private:
		CAnimation animation;			 // NPC的動畫
		CAnimation InteractionBar;      //互動圖
		
		void SetIsTalkingToHero(bool flag);
		//void TouchedByHero(int x1, int x2, int y1, int y2);
		CMovingBitmap LevelUpBar;
		CMovingBitmap LevelUpInterface;
		CMovingBitmap PauseBar;
	};
}

