#ifndef _BOT_FUNC_HPP_
#define _BOT_FUNC_HPP_

//prototypes of bot functions...

void BotSpawnInit(bot_t *pBot);
void BotCreate(int fixedPersNr = -1);
void BotStartGame(bot_t *pBot);
int BotInFieldOfView(bot_t *pBot, Vector dest);
bool BotEntityIsVisible(bot_t *pBot, Vector dest);
void BotFindItem(bot_t *pBot);
void BotThink(bot_t *pBot);

void BotFixIdealPitch(edict_t *pEdict);
float BotChangePitch(bot_t *pBot, float speed);
void BotFixIdealYaw(edict_t *pEdict);
float BotChangeYaw(bot_t *pBot, float speed);

edict_t *BotFindEnemy(bot_t *pBot);
Vector BotBodyTarget(edict_t *pBotEnemy, bot_t *pBot);
bool BotFireWeapon(Vector v_enemy, bot_t *pBot);
void BotShootAtEnemy(bot_t *pBot);

#endif // _BOT_FUNC_HPP_

