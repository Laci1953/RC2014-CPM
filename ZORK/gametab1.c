#include <ovr44.h>
/* ---------------------------------
enum
{
 DMWT_tie_rope                     ,
 DMWT_tie_railing                   ,
 DMWT_untie_rope                   ,
 DMWT_turn_bolt                     ,
 DMWT_fix_leak                      ,
 DMWT_i_f_ible_boat ,
 DMWT_i_f_iated_boat   ,
 DMWT_i_f_p_boat  ,
 DMWT_d_ied_boat        ,
 DMWT_d_iable_boat      ,
 DMWT_d_pun_boat       ,
 DMWT_f_pun_boat           ,
 DMWT_lock_grate                   ,
 DMWT_unlock_grate                 ,
 DMWT_act_lamp               ,
 DMWT_dea_lamp             ,
 DMWT_act_match              ,
 DMWT_dea_match            ,
 DMWT_act_candles             ,
 DMWT_dea_candles           ,
 DMWT_act_machine             ,
 DMWT_dig_sand                     ,
 DMWT_fill_bottle                  ,
 DMWT_attack_bat                   ,
 DMWT_att_ghosts                ,
 DMWT_att_cyclops               ,
 DMWT_attack_thief                 ,
 DMWT_attack_troll                 ,
 DMWT_attack_you              ,
 DMWT_act_leaves              ,
 DMWT_activate_book                ,
 DMWT_act_s_bag        ,
 DMWT_act_adv       ,
 DMWT_act_ifed_boat       ,
 DMWT_act_paint            ,
 DMWT_act_pun_boat      ,
 DMWT_act_ifble_boat     ,
 DMWT_activate_coal                ,
 DMWT_act_b_label          ,
 DMWT_activate_guide               ,
 DMWT_activate_nest                ,
 DMWT_act_white_h         ,
 DMWT_act_front_d          ,
 DMWT_act_torch               ,
 DMWT_dea_torch             ,
 DMWT_turn_book                    ,
 DMWT_pour_water                   ,
 DMWT_pour_putty                   ,
 DMWT_oil_bolt                     ,
 DMWT_brush_teeth                  ,
 DMWT_tie_cyclops                  ,
 DMWT_tie_thief                    ,
 DMWT_tie_troll                    
};
-------------------------------------- */
/*actions*/
#define  A_TIE 93  
#define  A_UNTIE 97
#define  A_TURN 95 
#define  A_FIX 41  
#define  A_INFLATE 45
#define  A_FILL 40 
#define  A_DEFLATE 26
#define  A_LOCK 52 
#define  A_UNLOCK 96 
#define  A_ACTIVATE 13
#define  A_ATTACK 14 
#define  A_DEACTIVATE 25 
#define  A_DIG 28 
#define  A_POUR 66
#define  A_OIL 63 
#define  A_BRUSH 17

/* objects */

#define O_YOU 1
#define O_CYCLOPS 2
#define O_GHOSTS 3
#define O_BAT 4
#define O_THIEF 5
#define O_TROLL 6
#define O_MACHINE 10
#define O_WATER 14
#define O_BOOK 20 
#define O_SANDWICH_BAG 24
#define O_BOTTLE 28
#define O_LAMP 36
#define O_ADVERTISEMENT 38
#define O_INFLATED_BOAT 39
#define O_MATCH 40
#define O_PAINTING 41
#define O_CANDLES 42
#define O_LEAVES 44
#define O_PUNCTURED_BOAT 45
#define O_INFLATABLE_BOAT 46
#define O_ROPE 50
#define O_COAL 57
#define O_BOAT_LABEL 63
#define O_TORCH 64
#define O_GUIDE 65
#define O_PUTTY 68
#define O_NEST 71

/*fixed (unmoving) objects*/
#define  F_RAILING 2089
#define  F_BOLT 2072 
#define  F_LEAK 2085
#define  F_GRATE 2062
#define  F_SAND 2090
#define  F_MACHINE_SWITCH 2091
#define  F_WHITE_HOUSE 2055
#define  F_FRONT_DOOR 2081

struct DOMISCWITH_STRUCT
{
  int action;
  int obj; /*can be obj or fobj*/
  void (*f)(int);
};

struct DOMISCWITH_STRUCT DoMiscWithTo[] =
{
  { A_TIE        , O_ROPE            , (void*)DMWT_tie_rope                     },
  { A_TIE        , F_RAILING        , (void*)DMWT_tie_railing                   },
  { A_UNTIE      , O_ROPE            , (void*)DMWT_untie_rope                   },
  { A_TURN       , F_BOLT           , (void*)DMWT_turn_bolt                     },
  { A_FIX        , F_LEAK           , (void*)DMWT_fix_leak                      },
  { A_INFLATE    , O_INFLATABLE_BOAT , (void*)DMWT_i_f_ible_boat },
  { A_INFLATE    , O_INFLATED_BOAT   , (void*)DMWT_i_f_iated_boat   },
  { A_INFLATE    , O_PUNCTURED_BOAT  , (void*)DMWT_i_f_p_boat  },
  { A_FILL       , O_INFLATABLE_BOAT , (void*)DMWT_i_f_ible_boat },
  { A_FILL       , O_INFLATED_BOAT   , (void*)DMWT_i_f_iated_boat   },
  { A_FILL       , O_PUNCTURED_BOAT  , (void*)DMWT_i_f_p_boat  },
  { A_DEFLATE    , O_INFLATED_BOAT   , (void*)DMWT_d_ied_boat        },
  { A_DEFLATE    , O_INFLATABLE_BOAT , (void*)DMWT_d_iable_boat      },
  { A_DEFLATE    , O_PUNCTURED_BOAT  , (void*)DMWT_d_pun_boat       },
  { A_FIX        , O_PUNCTURED_BOAT  , (void*)DMWT_f_pun_boat           },
  { A_LOCK       , F_GRATE          , (void*)DMWT_lock_grate                   },
  { A_UNLOCK     , F_GRATE          , (void*)DMWT_unlock_grate                 },
  { A_ACTIVATE   , O_LAMP            , (void*)DMWT_act_lamp               },
  { A_DEACTIVATE , O_LAMP            , (void*)DMWT_dea_lamp             },
  { A_ACTIVATE   , O_MATCH           , (void*)DMWT_act_match              },
  { A_DEACTIVATE , O_MATCH           , (void*)DMWT_dea_match            },
  { A_ACTIVATE   , O_CANDLES         , (void*)DMWT_act_candles             },
  { A_DEACTIVATE , O_CANDLES         , (void*)DMWT_dea_candles           },
  { A_ACTIVATE   , O_MACHINE         , (void*)DMWT_act_machine             },
  { A_ACTIVATE   , F_MACHINE_SWITCH , (void*)DMWT_act_machine             },
  { A_TURN       , F_MACHINE_SWITCH , (void*)DMWT_act_machine             },
  { A_DIG        , F_SAND           , (void*)DMWT_dig_sand                     },
  { A_FILL       , O_BOTTLE          , (void*)DMWT_fill_bottle                  },
  { A_ATTACK     , O_BAT             , (void*)DMWT_attack_bat                   },
  { A_ATTACK     , O_GHOSTS          , (void*)DMWT_att_ghosts                },
  { A_ATTACK     , O_CYCLOPS         , (void*)DMWT_att_cyclops               },
  { A_ATTACK     , O_THIEF           , (void*)DMWT_attack_thief                 },
  { A_ATTACK     , O_TROLL           , (void*)DMWT_attack_troll                 },
  { A_ATTACK     , O_YOU             , (void*)DMWT_attack_you              },
  { A_ACTIVATE   , O_LEAVES          , (void*)DMWT_act_leaves              },
  { A_ACTIVATE   , O_BOOK            , (void*)DMWT_activate_book                },
  { A_ACTIVATE   , O_SANDWICH_BAG    , (void*)DMWT_act_s_bag        },
  { A_ACTIVATE   , O_ADVERTISEMENT   , (void*)DMWT_act_adv       },
  { A_ACTIVATE   , O_INFLATED_BOAT   , (void*)DMWT_act_ifed_boat       },
  { A_ACTIVATE   , O_PAINTING        , (void*)DMWT_act_paint            },
  { A_ACTIVATE   , O_PUNCTURED_BOAT  , (void*)DMWT_act_pun_boat      },
  { A_ACTIVATE   , O_INFLATABLE_BOAT , (void*)DMWT_act_ifble_boat     },
  { A_ACTIVATE   , O_COAL            , (void*)DMWT_activate_coal                },
  { A_ACTIVATE   , O_BOAT_LABEL      , (void*)DMWT_act_b_label          },
  { A_ACTIVATE   , O_GUIDE           , (void*)DMWT_activate_guide               },
  { A_ACTIVATE   , O_NEST            , (void*)DMWT_activate_nest                },
  { A_ACTIVATE   , F_WHITE_HOUSE    , (void*)DMWT_act_white_h         },
  { A_ACTIVATE   , F_FRONT_DOOR     , (void*)DMWT_act_front_d          },
  { A_ACTIVATE   , O_TORCH           , (void*)DMWT_act_torch               },
  { A_DEACTIVATE , O_TORCH           , (void*)DMWT_dea_torch             },
  { A_TURN       , O_BOOK            , (void*)DMWT_turn_book                    },
  { A_POUR       , O_WATER           , (void*)DMWT_pour_water                   },
  { A_POUR       , O_PUTTY           , (void*)DMWT_pour_putty                   },
  { A_OIL        , F_BOLT           , (void*)DMWT_oil_bolt                     },
  { A_BRUSH      , O_YOU             , (void*)DMWT_brush_teeth                  },
  { A_TIE        , O_CYCLOPS         , (void*)DMWT_tie_cyclops                  },
  { A_TIE        , O_THIEF           , (void*)DMWT_tie_thief                    },
  { A_TIE        , O_TROLL           , (void*)DMWT_tie_troll                    },

  { 0, 0, (void*)0 }
};
