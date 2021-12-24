#include <ovr41.h>
/* -------------------------
enum
{
 DM_open_kit_w,
 DM_close_kit_w,
 DM_move_push_rug      ,
 DM_open_trap_door     ,
 DM_close_trap_door    ,
 DM_raise_basket       ,
 DM_lower_basket       ,
 DM_push_blue_button   ,
 DM_push_red_button    ,
 DM_push_brown_button  ,
 DM_push_yellow_button ,
 DM_enter_inf_b,
 DM_exit_inflated_boat ,
 DM_move_leaves        ,
 DM_open_grate         ,
 DM_close_grate        ,
 DM_ring_bell          ,
 DM_wind_canary        ,
 DM_wind_broken_canary ,
 DM_wave_sceptre       ,
 DM_raise_sceptre      ,
 DM_touch_mirror       ,
 DM_read_book          ,
 DM_read_advertisement ,
 DM_read_match         ,
 DM_read_map           ,
 DM_read_boat_label    ,
 DM_read_guide         ,
 DM_read_tube          ,
 DM_read_owners_manual ,
 DM_read_prayer        ,
 DM_read_wooden_door   ,
 DM_read_engravings    ,
 DM_open_egg           ,
 DM_climb_k_w,
 DM_climb_t_d     ,
 DM_climbthrough_grate ,
 DM_climbthrough_slide ,
 DM_climb_ch,
 DM_climb_b_d   ,
 DM_climbthrough_gate  ,
 DM_climbthrough_crack ,
 DM_enter_white_house  ,
 DM_slidedown_slide    ,
 DM_climb_m_r     ,
 DM_climbup_white_cliff,
 DM_climbup_tree       ,
 DM_climbdown_tree     ,
 DM_climbup_chimney    ,
 DM_climbdown_chimney  ,
 DM_climbup_ladder     ,
 DM_climbdown_ladder   ,
 DM_climbup_slide      ,
 DM_climbdown_slide    ,
 DM_climb_c_c,
 DM_climbd_c_c  ,
 DM_climbup_stairs     ,
 DM_climbdown_stairs   ,
 DM_examine_sword      ,
 DM_examine_match      ,
 DM_examine_candles    ,
 DM_examine_torch      ,
 DM_examine_thief      ,
 DM_examine_tool_chest ,
 DM_examine_board      ,
 DM_examine_chain      ,
 DM_open_tool_chest    ,
 DM_open_book          ,
 DM_close_book         ,
 DM_open_boarded_window,
 DM_break_boarded_window,
 DM_open_close_dam     ,
 DM_ring_hot_bell      ,
 DM_read_button        ,
 DM_raise_l_g_wall   ,
 DM_raise_lower_chain  ,
 DM_move_chain         ,
 DM_count_candles      ,
 DM_count_leaves       ,
 DM_examine_lamp       ,
 DM_examine_troll      ,
 DM_examine_cyclops    ,
 DM_examine_white_house,
 DM_open_close_b_d     ,
 DM_open_close_s_d     ,
 DM_open_close_b_of_c    ,
 DM_open_close_trunk   ,
 DM_open_close_large_bag,
 DM_open_front_door    ,
 DM_count_matches      ,
 DM_eat_lunch          ,
 DM_eat_garlic         ,
 DM_drink_water        ,
 DM_climbdown_rope     ,
 DM_break_mirror       ,
 DM_lookin_mirror      ,
 DM_lookthrough_k_w ,
 DM_lookunder_rug      ,
 DM_lookunder_leaves   ,
 DM_lookunder_rainbow  ,
 DM_lookin_chimney     ,
 DM_examine_k_w     ,
 DM_lookin_bag_of_coins,
 DM_lookin_trunk       ,
 DM_squeeze_tube       ,
 DM_examine_r_b      ,
 DM_examine_l_b     ,
 DM_lookin_large_bag   ,
 DM_lookthrough_grate  ,
 DM_lookin_water       ,
 DM_whereis_granite_wall,
 DM_whereis_songbird   ,
 DM_whereis_white_house,
 DM_whereis_forest     ,
 DM_read_granite_wall  ,
 DM_examine_zorkmid    ,
 DM_examine_grue       ,
 DM_whereis_zorkmid    ,
 DM_whereis_grue       ,
 DM_listento_troll     ,
 DM_listento_thief     ,
 DM_listento_cyclops   ,
 DM_listento_forest    ,
 DM_listento_songbird  ,
 DM_cross_rainbow      ,
 DM_cross_lake         ,
 DM_cross_stream       ,
 DM_cross_chasm        ,
 DM_exorcise_ghosts    ,
 DM_raise_rug          ,
 DM_raise_trap_door    ,
 DM_smell_gas          ,
 DM_smell_sandwich_bag 
};
-------------------------------- */
/*actions*/

enum
{
  A_NOTHING,	/* 0 */
  A_NORTH, 	/* 1 direction actions must be grouped together in this order*/
  A_SOUTH,	/* 2  */
  A_EAST,	/* 3  */
  A_WEST,	/* 4  */
  A_NORTHEAST,	/* 5  */
  A_NORTHWEST,	/* 6  */
  A_SOUTHEAST,	/* 7  */
  A_SOUTHWEST,	/* 8  */
  A_UP,		/* 9  */
  A_DOWN,	/* 10  */
  A_IN,		/* 11  */
  A_OUT,	/* 12  */
  A_ACTIVATE,	/* 13  */
  A_ATTACK,	/* 14  */
  A_BREAK,	/* 15  */
  A_BRIEF,	/* 16  */
  A_BRUSH,	/* 17  */
  A_CLIMB,	/* 18  */
  A_CLIMBDOWN,	/* 19  */
  A_CLIMBTHROUGH,/* 20  */
  A_CLIMBUP,	/* 21  */
  A_CLOSE,	/* 22  */
  A_COUNT,	/* 23 */
  A_CROSS,	/* 24 */
  A_DEACTIVATE,	/* 25  */
  A_DEFLATE,	/* 26  */
  A_DIAGNOSE,	/* 27  */
  A_DIG,	/* 28  */
  A_DISEMBARK,	/* 29  */
  A_DISMOUNT,	/* 30  */
  A_DRINK,	/* 31  */
  A_DROP,	/* 32  */
  A_EAT,	/* 33  */
  A_ECHO,	/* 34  */
  A_EMPTY,	/* 35  */
  A_ENTER,	/* 36  */
  A_EXAMINE,	/* 37  */
  A_EXIT,	/* 38  */
  A_EXORCISE,	/* 39  */
  A_FILL,	/* 40  */
  A_FIX,	/* 41  */
  A_GIVE,	/* 42  */
  A_GO,		/* 43  */
  A_GREET,	/* 44  */
  A_INFLATE,	/* 45  */
  A_INVENTORY,	/* 46  */
  A_JUMP,	/* 47  */
  A_KNOCK,	/* 48  */
  A_LAND,	/* 49  */
  A_LAUNCH,	/* 50  */
  A_LISTENTO,	/* 51  */
  A_LOCK,	/* 52  */
  A_LOOK,	/* 53  */
  A_LOOKBEHIND,	/* 54  */
  A_LOOKIN,	/* 55  */
  A_LOOKON,	/* 56  */
  A_LOOKTHROUGH,/* 57  */
  A_LOOKUNDER,	/* 58  */
  A_LOWER,	/* 59  */
  A_MOUNT,	/* 60  */
  A_MOVE,	/* 61  */
  A_ODYSSEUS,	/* 62  */
  A_OIL,	/* 63  */
  A_OPEN,	/* 64  */
  A_PLAY,	/* 65  */
  A_POUR,	/* 66  */
  A_PRAY,	/* 67  */
  A_PRY,	/* 68  */
  A_PULL,	/* 69  */
  A_PUSH,	/* 70  */
  A_PUT,	/* 71  */
  A_QUIT,	/* 72  */
  A_RAISE,	/* 73  */
  A_READ,	/* 74  */
  A_REMOVE,	/* 75  */
  A_RESTART,	/* 76  */
  A_RESTORE,	/* 77  */
  A_RING,	/* 78  */
  A_SAVE,	/* 79  */
  A_SAY,	/* 80  */
  A_SCORE,	/* 81  */
  A_SLEEP,	/* 82  */
  A_SLEEPON,	/* 83  */
  A_SLIDEDOWN,	/* 84  */
  A_SLIDEUP,	/* 85  */
  A_SMELL,	/* 86  */
  A_SQUEEZE,	/* 87  */
  A_SUPERBRIEF,	/* 88  */
  A_SWIM,	/* 89  */
  A_TAKE,	/* 90  */
  A_TALKTO,	/* 91  */
  A_THROW,	/* 92  */
  A_TIE,	/* 93  */
  A_TOUCH,	/* 94  */
  A_TURN,	/* 95  */
  A_UNLOCK,	/* 96  */
  A_UNTIE,	/* 97  */
  A_VERBOSE,	/* 98  */
  A_VERSION,	/* 99  */
  A_WAIT,	/* 100  */
  A_WAVE,	/* 101  */
  A_WEAR,	/* 102  */
  A_WHEREIS,	/* 103  */
  A_WIND	/* 104  */
};

/*fixed (unmoving) objects*/

enum
{
  F_SCENERY_VIS = 2048, /*some anonymous scenery object, visible*/
  F_SCENERY_NOTVIS,     /*                               not visible*/

  F_NOTVIS,             /*fixed object not visible*/

  F_AMB,                /*amibiguous (ask for clarification)*/

  /*game-specific data follows*/

  F_SLIDE,
  F_BOARD,
  F_SONGBIRD,
  F_WHITE_HOUSE,
  F_FOREST,
  F_TREE,
  F_KITCHEN_WINDOW,
  F_CHIMNEY,
  F_BOARDED_WINDOW,
  F_CRACK,
  F_GRATE,
  F_CLIMBABLE_CLIFF,
  F_WHITE_CLIFF,
  F_BODIES,
  F_RAINBOW,
  F_RIVER,
  F_LADDER,
  F_TRAP_DOOR,
  F_STAIRS,
  F_MOUNTAIN_RANGE,
  F_BOLT,
  F_BUBBLE,
  F_ALTAR,
  F_YELLOW_BUTTON,
  F_BROWN_BUTTON,
  F_RED_BUTTON,
  F_BLUE_BUTTON,
  F_RUG,
  F_DAM,
  F_FRONT_DOOR,
  F_BARROW_DOOR,
  F_BARROW,
  F_BONES,
  F_LEAK,
  F_MIRROR2,
  F_MIRROR1,
  F_PRAYER,
  F_RAILING,
  F_SAND,
  F_MACHINE_SWITCH,
  F_WOODEN_DOOR,
  F_PEDESTAL,
  F_CONTROL_PANEL,
  F_NAILS,
  F_GRANITE_WALL,
  F_CHAIN,
  F_GATE,
  F_STUDIO_DOOR,
  F_CHASM,
  F_LAKE,
  F_STREAM,
  F_GAS
};

#define O_THIEF 5
#define O_LOWERED_BASKET 7
#define O_RAISED_BASKET 8
#define O_BELL 18
#define O_SCEPTRE 23
#define O_ADVERTISEMENT 38
#define O_INFLATED_BOAT 39
#define O_MATCH 40
#define O_CANDLES 42
#define O_LEAVES 44
#define O_TOOL_CHEST 53
#define O_SWORD 61
#define O_MAP 62
#define O_BOAT_LABEL 63
#define O_TORCH 64
#define O_GUIDE 65
#define O_TUBE 67
#define O_OWNERS_MANUAL 69
#define O_EGG 72
#define O_CANARY 75
#define O_BROKEN_CANARY 76
#define O_ENGRAVINGS 77
#define O_BOOK 20

struct DM_STRUCT
{
  int action;
  int obj; /*can be obj or fobj*/
  void (*f)(void);
};

struct DM_STRUCT DoMisc[] =
{
  { A_OPEN         , F_KITCHEN_WINDOW  , (void*)DM_open_kit_w         },
  { A_CLOSE        , F_KITCHEN_WINDOW  , (void*)DM_close_kit_w        },
  { A_MOVE         , F_RUG             , (void*)DM_move_push_rug               },
  { A_PUSH         , F_RUG             , (void*)DM_move_push_rug               },
  { A_OPEN         , F_TRAP_DOOR       , (void*)DM_open_trap_door              },
  { A_CLOSE        , F_TRAP_DOOR       , (void*)DM_close_trap_door             },
  { A_RAISE        , O_RAISED_BASKET    , (void*)DM_raise_basket                },
  { A_RAISE        , O_LOWERED_BASKET   , (void*)DM_raise_basket                },
  { A_LOWER        , O_RAISED_BASKET    , (void*)DM_lower_basket                },
  { A_LOWER        , O_LOWERED_BASKET   , (void*)DM_lower_basket                },
  { A_PUSH         , F_BLUE_BUTTON     , (void*)DM_push_blue_button            },
  { A_PUSH         , F_RED_BUTTON      , (void*)DM_push_red_button             },
  { A_PUSH         , F_BROWN_BUTTON    , (void*)DM_push_brown_button           },
  { A_PUSH         , F_YELLOW_BUTTON   , (void*)DM_push_yellow_button          },
  { A_ENTER        , O_INFLATED_BOAT    , (void*)DM_enter_inf_b         },
  { A_EXIT         , O_INFLATED_BOAT    , (void*)DM_exit_inflated_boat          },
  { A_MOVE         , O_LEAVES           , (void*)DM_move_leaves                 },
  { A_OPEN         , F_GRATE           , (void*)DM_open_grate                  },
  { A_CLOSE        , F_GRATE           , (void*)DM_close_grate                 },
  { A_RING         , O_BELL             , (void*)DM_ring_bell                   },
  { A_WIND         , O_CANARY           , (void*)DM_wind_canary                 },
  { A_WIND         , O_BROKEN_CANARY    , (void*)DM_wind_broken_canary          },
  { A_WAVE         , O_SCEPTRE          , (void*)DM_wave_sceptre                },
  { A_RAISE        , O_SCEPTRE          , (void*)DM_raise_sceptre               },
  { A_TOUCH        , F_MIRROR1         , (void*)DM_touch_mirror                },
  { A_TOUCH        , F_MIRROR2         , (void*)DM_touch_mirror                },
  { A_READ         , O_BOOK             , (void*)DM_read_book                   },
  { A_READ         , O_ADVERTISEMENT    , (void*)DM_read_advertisement          },
  { A_READ         , O_MATCH            , (void*)DM_read_match                  },
  { A_READ         , O_MAP              , (void*)DM_read_map                    },
  { A_READ         , O_BOAT_LABEL       , (void*)DM_read_boat_label             },
  { A_READ         , O_GUIDE            , (void*)DM_read_guide                  },
  { A_READ         , O_TUBE             , (void*)DM_read_tube                   },
  { A_READ         , O_OWNERS_MANUAL    , (void*)DM_read_owners_manual          },
  { A_READ         , F_PRAYER          , (void*)DM_read_prayer                 },
  { A_READ         , F_WOODEN_DOOR     , (void*)DM_read_wooden_door            },
  { A_READ         , O_ENGRAVINGS       , (void*)DM_read_engravings             },
  { A_OPEN         , O_EGG              , (void*)DM_open_egg                    },
  { A_BREAK        , O_EGG              , (void*)DM_open_egg                    },
  { A_PRY          , O_EGG              , (void*)DM_open_egg                    },
  { A_CLIMBTHROUGH , F_KITCHEN_WINDOW  , (void*)DM_climb_k_w },
  { A_ENTER        , F_KITCHEN_WINDOW  , (void*)DM_climb_k_w },
  { A_EXIT         , F_KITCHEN_WINDOW  , (void*)DM_climb_k_w },
  { A_CLIMBTHROUGH , F_TRAP_DOOR       , (void*)DM_climb_t_d      },
  { A_ENTER        , F_TRAP_DOOR       , (void*)DM_climb_t_d      },
  { A_CLIMBTHROUGH , F_GRATE           , (void*)DM_climbthrough_grate          },
  { A_ENTER        , F_GRATE           , (void*)DM_climbthrough_grate          },
  { A_CLIMBTHROUGH , F_SLIDE           , (void*)DM_climbthrough_slide          },
  { A_ENTER        , F_SLIDE           , (void*)DM_climbthrough_slide          },
  { A_CLIMBTHROUGH , F_CHIMNEY         , (void*)DM_climb_ch        },
  { A_ENTER        , F_CHIMNEY         , (void*)DM_climb_ch        },
  { A_CLIMBTHROUGH , F_BARROW_DOOR     , (void*)DM_climb_b_d    },
  { A_ENTER        , F_BARROW_DOOR     , (void*)DM_climb_b_d    },
  { A_ENTER        , F_BARROW          , (void*)DM_climb_b_d    },
  { A_CLIMBTHROUGH , F_GATE            , (void*)DM_climbthrough_gate           },
  { A_ENTER        , F_GATE            , (void*)DM_climbthrough_gate           },
  { A_CLIMBTHROUGH , F_CRACK           , (void*)DM_climbthrough_crack          },
  { A_ENTER        , F_CRACK           , (void*)DM_climbthrough_crack          },
  { A_ENTER        , F_WHITE_HOUSE     , (void*)DM_enter_white_house           },
  { A_SLIDEDOWN    , F_SLIDE           , (void*)DM_slidedown_slide             },
  { A_CLIMBUP      , F_MOUNTAIN_RANGE  , (void*)DM_climb_m_r      },
  { A_CLIMB        , F_MOUNTAIN_RANGE  , (void*)DM_climb_m_r      },
  { A_CLIMBUP      , F_WHITE_CLIFF     , (void*)DM_climbup_white_cliff         },
  { A_CLIMB        , F_WHITE_CLIFF     , (void*)DM_climbup_white_cliff         },
  { A_CLIMBUP      , F_TREE            , (void*)DM_climbup_tree                },
  { A_CLIMB        , F_TREE            , (void*)DM_climbup_tree                },
  { A_CLIMBDOWN    , F_TREE            , (void*)DM_climbdown_tree              },
  { A_CLIMBUP      , F_CHIMNEY         , (void*)DM_climbup_chimney             },
  { A_CLIMB        , F_CHIMNEY         , (void*)DM_climbup_chimney             },
  { A_CLIMBDOWN    , F_CHIMNEY         , (void*)DM_climbdown_chimney           },
  { A_CLIMBUP      , F_LADDER          , (void*)DM_climbup_ladder              },
  { A_CLIMB        , F_LADDER          , (void*)DM_climbup_ladder              },
  { A_CLIMBDOWN    , F_LADDER          , (void*)DM_climbdown_ladder            },
  { A_CLIMBUP      , F_SLIDE           , (void*)DM_climbup_slide               },
  { A_CLIMB        , F_SLIDE           , (void*)DM_climbup_slide               },
  { A_CLIMBDOWN    , F_SLIDE           , (void*)DM_climbdown_slide             },
  { A_CLIMBUP      , F_CLIMBABLE_CLIFF , (void*)DM_climb_c_c     },
  { A_CLIMB        , F_CLIMBABLE_CLIFF , (void*)DM_climb_c_c     },
  { A_CLIMBDOWN    , F_CLIMBABLE_CLIFF , (void*)DM_climbd_c_c   },
  { A_CLIMBUP      , F_STAIRS          , (void*)DM_climbup_stairs              },
  { A_CLIMB        , F_STAIRS          , (void*)DM_climbup_stairs              },
  { A_CLIMBDOWN    , F_STAIRS          , (void*)DM_climbdown_stairs            },
  { A_EXAMINE      , O_SWORD            , (void*)DM_examine_sword               },
  { A_EXAMINE      , O_MATCH            , (void*)DM_examine_match               },
  { A_EXAMINE      , O_CANDLES          , (void*)DM_examine_candles             },
  { A_EXAMINE      , O_TORCH            , (void*)DM_examine_torch               },
  { A_EXAMINE      , O_THIEF            , (void*)DM_examine_thief               },
  { A_EXAMINE      , O_TOOL_CHEST       , (void*)DM_examine_tool_chest          },
  { A_EXAMINE      , F_BOARD           , (void*)DM_examine_board               },
  { A_EXAMINE      , F_CHAIN           , (void*)DM_examine_chain               },
};
