namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以用鍵盤或滑鼠控制的敵人
	// 看懂就可以改寫成自己的程式了
	/////////////////////////////////////////////////////////////////////////////

	enum ACTION
	{
		STAND_LEFT, 
		MOVE_LEFT, 
		ATTACK_LEFT, 
		STAND_RIGHT, 
		MOVE_RIGHT, 
		ATTACK_RIGHT,
		GET_HIT,
		DEAD
	};

	enum Item_Type
	{
		BRONZE_COIN,
		SILVER_COIN,
		GOLDEN_COIN
	};

	class gameMap;
	class CEnemy
	{
	public:
		CEnemy(gameMap* pointer, int x, int y);
		virtual ~CEnemy();
		virtual void LoadBitmap() = 0;				// 載入圖形
		virtual void OnMove() = 0;					// 移動敵人
		virtual void OnShow() = 0;					// 將敵人圖形貼到畫面
		virtual int GetX1() = 0;
		virtual int GetY1() = 0;
		virtual int GetX2() = 0;
		virtual int GetY2() = 0;
		void SetHeroXY(int x1, int x2, int y1, int y2);				// 設定英雄位置
		void SetHeroAttackRange(int x1, int x2, int y1, int y2);	// 設定英雄攻擊範圍
		virtual void GetAttack(const int damage) = 0;		// 被主角攻擊
		virtual void AttackByEnemy(int* heroHP) = 0;		// 攻擊主角
		virtual bool isDead() = 0;
		virtual string GetEnemyType() = 0;
		void SetEnemyXY(int SetX, int SetY);
	protected:
		int x, y;						// 敵人左上角座標
		gameMap* currentMap;		// 所在地圖
		map<string, int> hero;
		map<string, int> heroAttackRange;
	};

	class bullet
	{
	public:
		bullet(gameMap* point, int nx, int ny, int step);
		~bullet();
		int GetX1();					// 子彈左上角 x 座標
		int GetY1();					// 子彈左上角 y 座標
		int GetX2();					// 子彈右下角 x 座標
		int GetY2();					// 子彈右下角 y 座標
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		bool isDelet();

	protected:
		CAnimation animation;
		int x;
		int y;
		gameMap *currentMap;
		int distance;
		int STEP_SIZE;
	};
	class bullet_sunFlower : public bullet
	{
	public:
		bullet_sunFlower(gameMap* point, int nx, int ny, int step);
		~bullet_sunFlower();
		void LoadBitmap();				// 載入圖形
	};

	class CEnemy_sunFlower : public CEnemy
	{
	public:
		CEnemy_sunFlower(gameMap* pointer, int x, int y);
		~CEnemy_sunFlower();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP);
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation animationLeft;		// 敵人的動畫(左)
		CAnimation moveRightAnimation;  // 向右移動動畫
		CAnimation moveLeftAnimation;   // 向左移動動畫
		CAnimation AttackRightAnimation;// 攻擊右邊
		CAnimation AttackLeftAnimation; // 攻擊左邊
		CAnimation jumpAnimation;		// 跳躍動畫
		CAnimation DeadAnimation;		// 死亡動畫
		CAnimation GetHitAnimation;		// 被打到的動畫
		CAnimation HitAnimation;		// 打擊效果
		string EnemyType = "Sun_Flower";
		bool isMovingDown;			// 是否正在往下移動
		bool isMovingLeft;			// 是否正在往左移動
		bool isMovingRight;			// 是否正在往右移動
		bool isMovingUp;			// 是否正在往上移動
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyHP;					// 敵人生命值
		int enemyAttackDamage;		//敵人攻擊力
		ACTION DetectHero(ACTION state);		// 偵測英雄位置:  1:距離左邊200 2:距離左邊150 3:距離右邊150 4:距離右邊200
		int attackDelay;
		int attackDelayCount;
		ACTION state;
		vector<bullet_sunFlower*> allBullet;
		int GetHitDelayCount = 0;
		bool GetHit = false;
	};
	
	class CEnemy_Cactus : public CEnemy
	{
	public:
		CEnemy_Cactus(gameMap* pointer, int x, int y);
		~CEnemy_Cactus();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP);
		bool isDead();
		string GetEnemyType();
	private:
		CAnimation animation;			// 敵人的動畫
		CAnimation AttackAnimation;// 攻擊
		CAnimation DeadAnimation;
		CAnimation GetHitAnimation;		// 被打到的動畫
		CAnimation HitAnimation;		// 打擊效果
		string EnemyType = "Cactus";
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyHP;					// 敵人生命值
		int enemyAttackDamage;		//敵人攻擊力
		int AttackDelayCount;		// 攻擊頻率
		int SetAttackDelay;
		bool AttackFlag;			// 等於true(發出尖刺動畫出現)時才攻擊到主角
		bool ReadyToAttack;			// 開始攻擊動作
		bool GetHit = false;
		int GetHitDelayCount = 0;
	};


	class CEnemy_Statue : public CEnemy
	{
	public:
		CEnemy_Statue(gameMap* pointer, int x, int y);
		~CEnemy_Statue();
		int  GetX1();					// 敵人左上角 x 座標
		int  GetY1();					// 敵人左上角 y 座標
		int  GetX2();					// 敵人右下角 x 座標
		int  GetY2();					// 敵人右下角 y 座標
		int  GetWidth();				// 
		int  GetHeight();
		void LoadBitmap();				// 載入圖形
		void OnMove();					// 移動敵人
		void OnShow();					// 將敵人圖形貼到畫面
		void GetAttack(const int damage);		// 被攻擊
		void AttackByEnemy(int* heroHP);
		bool isDead();
		string GetEnemyType();

	private:
		CMovingBitmap Statue;
		CMovingBitmap Statue_Broken;
		CAnimation HitAnimation;		// 打擊效果
		string EnemyType = "Statue";
		int floor;				// 地板的Y座標
		bool rising;				// true表上升、false表下降
		int initial_velocity;		// 初始速度
		int velocity;				// 目前的速度(點/次)
		int enemyHP = 500;					// 敵人生命值
		int enemyAttackDamage = 0;		//敵人攻擊力
		int GetHitDelayCount = 0;
		bool GetHit = false;
	};

	
}