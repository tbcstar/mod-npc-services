/*
** Written by MtgCore
** Rewritten by Poszer & Talamortis https://github.com/poszer/ & https://github.com/talamortis/
** AzerothCore 2019 http://www.azerothcore.org/
** Cleaned and made into a module by Micrah https://github.com/milestorme/
*/

#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"

class Npc_Services : public CreatureScript
{
public:
        Npc_Services() : CreatureScript("Npc_Services") { }

        bool OnGossipHello(Player *player, Creature *creature)
        {
		AddGossipItemFor(player, 10, "|TInterface\\icons\\Spell_Nature_Regenerate:40:40:-18|t 恢复HP和MP", GOSSIP_SENDER_MAIN, 1);			// 回复生命值和法力值
		//AddGossipItemFor(player, 10, "|TInterface\\icons\\Achievement_BG_winAB_underXminutes:40:40:-18|t Reset Instances", GOSSIP_SENDER_MAIN, 2);	// 重置副本
		AddGossipItemFor(player, 10, "|TInterface\\icons\\SPELL_HOLY_BORROWEDTIME:40:40:-18|t 重置技能CD", GOSSIP_SENDER_MAIN, 3);				// 重置技能CD
		AddGossipItemFor(player, 10, "|TInterface\\icons\\Achievement_BG_AB_defendflags:40:40:-18|t 离开战斗", GOSSIP_SENDER_MAIN, 4);			// 离开战斗
		AddGossipItemFor(player, 10, "|TInterface\\icons\\Spell_Shadow_DeathScream:40:40:-18|t 消除虚弱", GOSSIP_SENDER_MAIN, 5);				// 消除虚弱
		AddGossipItemFor(player, 10, "|TInterface\\icons\\INV_Hammer_24:40:40:-18|t 修理装备", GOSSIP_SENDER_MAIN, 6);							// 修理装备
		//AddGossipItemFor(player, 10, "|TInterface\\icons\\Achievement_WorldEvent_Lunar:40:40:-18|t Reset Talents", GOSSIP_SENDER_MAIN, 7);			// 重置天赋
		AddGossipItemFor(player, 10, "|TInterface/Icons/INV_Misc_Bag_07:40:40:-18|t 银行", GOSSIP_SENDER_MAIN, 8);                                   // 打开银行
		AddGossipItemFor(player, 10, "|TInterface/Icons/INV_Letter_11:40:40:-18|t 邮箱", GOSSIP_SENDER_MAIN, 9);                                     // 打开邮箱
		//AddGossipItemFor(player, 10, "|TInterface/Icons/achievement_general:40:40:-18|t Learn Dual Talents", GOSSIP_SENDER_MAIN, 10);                // 学习双天赋

		SendGossipMenuFor(player, 1, creature->GetGUID());
        return true;
	}
	bool OnGossipSelect(Player* player, Creature* /*creature*/, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{
		case 1: // Restore HP and MP
				CloseGossipMenuFor(player);
			if (player->IsInCombat())
			{
				CloseGossipMenuFor(player);
				player->GetSession()->SendNotification("你在战斗中!");
				return false;
				}
				else if(player->getPowerType() == POWER_MANA)
						player->SetPower(POWER_MANA, player->GetMaxPower(POWER_MANA));

				player->SetHealth(player->GetMaxHealth());
				player->GetSession()->SendNotification("|cffFFFF00NPC 服务 \n |cffFFFFFFHP & MP 成功恢复!");
				player->CastSpell(player, 31726);
				break;

	    /*case 2: // Reset Instances
				CloseGossipMenuFor(player);
				for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
				{
                    BoundInstancesMap const& m_boundInstances = sInstanceSaveMgr->PlayerGetBoundInstances(player->GetGUIDLow(), Difficulty(i));
                    for (BoundInstancesMap::const_iterator itr = m_boundInstances.begin(); itr != m_boundInstances.end();)
                    {
                        InstanceSave* save = itr->second.save;
                        if (itr->first != player->GetMapId())
                        {
                            uint32 resetTime = itr->second.extended ? save->GetExtendedResetTime() : save->GetResetTime();
                            uint32 ttr = (resetTime >= time(nullptr) ? resetTime - time(nullptr) : 0);
                            sInstanceSaveMgr->PlayerUnbindInstance(player->GetGUIDLow(), itr->first, Difficulty(i), true, player);
                            itr = m_boundInstances.begin();
                        }
                        else
                            ++itr;
                    }
				}

				player->GetSession()->SendNotification("|cffFFFF00NPC 服务 \n |cffFFFFFFInstances 成功重置！");
				player->CastSpell(player, 59908);
                return true;
				break;*/

		case 3: // Reset Cooldowns
				CloseGossipMenuFor(player);
				if (player->IsInCombat())
				{
				CloseGossipMenuFor(player);
				player->GetSession()->SendNotification("你在战斗中！");
				return false;
				}

				player->RemoveAllSpellCooldown();
				player->GetSession()->SendNotification("|cffFFFF00NPC 服务 \n |cffFFFFFF技能CD 成功重置！");
				player->CastSpell(player, 31726);
				break;

		case 4: // Leave Combat
				CloseGossipMenuFor(player);
				player->CombatStop();
				player->GetSession()->SendNotification("|cffFFFF00NPC 服务 \n |cffFFFFFF已离开战斗状态！");
				player->CastSpell(player, 31726);
				break;

		case 5: // Remove Sickness
				CloseGossipMenuFor(player);
				if(player->HasAura(15007))
				player->RemoveAura(15007);
				player->GetSession()->SendNotification("|cffFFFF00NPC 服务 \n |cffFFFFFF虚弱状态已消除！");
				player->CastSpell(player, 31726);
				break;

		case 6: // Repair Items
				CloseGossipMenuFor(player);
				player->DurabilityRepairAll(false, 0, false);
				player->GetSession()->SendNotification("|cffFFFF00NPC 服务 \n |cffFFFFFF装备修理成功!");
				player->CastSpell(player, 31726);
				break;

	   /*case 7: // Reset Talents
				CloseGossipMenuFor(player);
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->GetSession()->SendNotification("|cffFFFF00NPC 服务 \n |cffFFFFFF天赋重置成功！");
				player->CastSpell(player, 31726);
				break;*/

		case 8:	// BANK
				CloseGossipMenuFor(player);
				player->GetSession()->SendShowBank(player->GetGUID());
				break;

		case 9: // MAIL
				CloseGossipMenuFor(player);
				player->GetSession()->SendShowMailBox(player->GetGUID());
				break;

		/*case 10: // Learn Dual Talent Specialization
				CloseGossipMenuFor(player);
				if (player->IsInCombat())
				{
				CloseGossipMenuFor(player);
				player->GetSession()->SendNotification("你在战斗中！");
				return false;
				}

                                player->learnSpell(63644);
                                player->CastSpell(player, 31726);
                                player->CastSpell(player, 63624);
                                player->learnSpell(63645);
                                player->UpdateSpecCount(2);
                                player->GetSession()->SendNotification("|cffFFFF00NPC 服务 \n |cffFFFFFF成功学习双天赋!");
				return true;
				break;*/

            }
                return true;
        }
};

void AddSC_Npc_Services()
{
        new Npc_Services();
}
