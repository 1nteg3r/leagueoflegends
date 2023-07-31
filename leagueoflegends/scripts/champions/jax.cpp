#include "../../stdafx.h"

using namespace scripts;

class JinxModule : public ChampionModule
{
private:
    std::string name = SP_STRING("Jinx");

    Skillshot w = SkillshotManager::RegisterSpell(name, SpellIndex::W, Skillshot(1440.0f, 60.0f, 3300.0f, 0.25f, SkillshotType::SkillshotLine, { CollidableObjects::Objects }));
    Skillshot r = SkillshotManager::RegisterSpell(name, SpellIndex::R, Skillshot(4000.0f, 200.0f, 1700.0f, 0.25f, SkillshotType::SkillshotLine, { CollidableObjects::Objects }));

private:
    float gameTime = 0.0f;

public:
    JinxModule()
    {
        ChampionModuleManager::RegisterModule(name, this);
    }

    void Init() override
    {

    }

    void Update() override
    {
        gameTime = functions::GetGameTime();
    }

    void Attack() override
    {
        auto me = globals::localPlayer;
        prediction::PredictionOutput wPrediction;
        auto wtarget = targetselector::GetEnemyChampionInRange(1440.0f);
        if (wtarget && wtarget->GetDistanceTo(me) > me->GetRealAttackRange() && w.IsCastable() && prediction::GetPrediction(w, wPrediction))
        {
            actions::CastSpell(SpellIndex::W, wPrediction.position);
            return;
        }

        auto target = targetselector::GetEnemyChampionInRange(925.0f);
        

        if (target && me->CanCastSpell(SpellIndex::E))
        {
            if (!target->CanMove())
            {
                actions::CastSpell(SpellIndex::E, target);
                return;
            }
        }

        auto qSpell = me->GetSpellBySlotId(SpellIndex::Q);
        int QSpellLevel = qSpell->GetLevel();
        auto qtarget = targetselector::GetEnemyChampionInRange((QSpellLevel*25.0f) + 75.0f + 635.0f);
        auto qBuff1 = me->GetBuffByName("jinxqicon");
        auto qBuff2 = me->GetBuffByName("JinxQ");
        if (me->CanCastSpell(SpellIndex::Q) && qtarget)
        {
            if (qtarget && qtarget->GetDistanceTo(me) > 600.0f && qBuff1 && qBuff1->GetStacksAlt())
            {
                actions::CastSpell(SpellIndex::Q, me);
            }
            else if (qtarget && qtarget->GetDistanceTo(me) <= 600.0f && qBuff2 && qBuff2->GetStacksAlt())
            {
                actions::CastSpell(SpellIndex::Q, me);
            }
        }
        float rDamages[] = { 300.0f,450.0f,600.0f };
        auto rSpell = me->GetSpellBySlotId(SpellIndex::R);
        int RSpellLevel = rSpell->GetLevel();
        auto Rtarget = targetselector::GetEnemyChampionInRange(4000.0f);
        auto distance = Rtarget->GetDistanceTo(me);
        float baseq = rDamages[RSpellLevel - 1] + 0.15 * me->GetBonusAttackDamage();
        prediction::PredictionOutput rPrediction;
        if (Rtarget && Rtarget->GetHealth() <= baseq && Rtarget->GetDistanceTo(me) > 1000 && r.IsCastable() && prediction::GetPrediction(r, rPrediction))
        {
            actions::CastSpell(SpellIndex::R, rPrediction.position);
            return;
        }




    }

    void Render() override
    {

    }
};

JinxModule module;