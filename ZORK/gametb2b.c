#include <ovr41.h>
/*------------------------------
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
---------------------------- */
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

#define O_CYCLOPS 2
#define O_GHOSTS 3
#define O_THIEF 5
#define O_TROLL 6
#define O_LOWERED_BASKET 7
#define O_RAISED_BASKET 8
#define O_WATER 14
#define O_LUNCH 17
#define O_SANDWICH_BAG 24
#define O_GARLIC 26
#define O_BAG_OF_COINS 35
#define O_LAMP 36
#define O_MATCH 40
#define O_CANDLES 42
#define O_LEAVES 44
#define O_ROPE 50
#define O_TOOL_CHEST 53
#define O_LARGE_BAG 59
#define O_TRUNK 66
#define O_TUBE 67
#define O_ZORKMID 78
#define O_GRUE 79
#define O_BOOK 20
#define O_HOT_BELL 19

struct DM_STRUCT_TMP
{
  int action;
  int obj; /*can be obj or fobj*/
  void (*f)(void);
};

struct DM_STRUCT_TMP tmp[] =
{
  { A_OPEN         , O_TOOL_CHEST       , (void*)DM_open_tool_chest             },
  { A_OPEN         , O_BOOK             , (void*)DM_open_book                   },
  { A_CLOSE        , O_BOOK             , (void*)DM_close_book                  },
  { A_OPEN         , F_BOARDED_WINDOW  , (void*)DM_open_boarded_window         },
  { A_BREAK        , F_BOARDED_WINDOW  , (void*)DM_break_boarded_window        },
  { A_OPEN         , F_DAM             , (void*)DM_open_close_dam              },
  { A_CLOSE        , F_DAM             , (void*)DM_open_close_dam              },
  { A_RING         , O_HOT_BELL         , (void*)DM_ring_hot_bell               },
  { A_READ         , F_YELLOW_BUTTON   , (void*)DM_read_button                 },
  { A_READ         , F_BROWN_BUTTON    , (void*)DM_read_button                 },
  { A_READ         , F_RED_BUTTON      , (void*)DM_read_button                 },
  { A_READ         , F_BLUE_BUTTON     , (void*)DM_read_button                 },
  { A_RAISE        , F_GRANITE_WALL    , (void*)DM_raise_l_g_wall    },
  { A_LOWER        , F_GRANITE_WALL    , (void*)DM_raise_l_g_wall    },
  { A_RAISE        , F_CHAIN           , (void*)DM_raise_lower_chain           },
  { A_LOWER        , F_CHAIN           , (void*)DM_raise_lower_chain           },
  { A_MOVE         , F_CHAIN           , (void*)DM_move_chain                  },
  { A_COUNT        , O_CANDLES          , (void*)DM_count_candles               },
  { A_COUNT        , O_LEAVES           , (void*)DM_count_leaves                },
  { A_EXAMINE      , O_LAMP             , (void*)DM_examine_lamp                },
  { A_EXAMINE      , O_TROLL            , (void*)DM_examine_troll               },
  { A_EXAMINE      , O_CYCLOPS          , (void*)DM_examine_cyclops             },
  { A_EXAMINE      , F_WHITE_HOUSE     , (void*)DM_examine_white_house         },
  { A_OPEN         , F_BARROW_DOOR     , (void*)DM_open_close_b_d      },
  { A_CLOSE        , F_BARROW_DOOR     , (void*)DM_open_close_b_d      },
  { A_OPEN         , F_STUDIO_DOOR     , (void*)DM_open_close_s_d      },
  { A_CLOSE        , F_STUDIO_DOOR     , (void*)DM_open_close_s_d      },
  { A_OPEN         , O_BAG_OF_COINS     , (void*)DM_open_close_b_of_c     },
  { A_CLOSE        , O_BAG_OF_COINS     , (void*)DM_open_close_b_of_c     },
  { A_OPEN         , O_TRUNK            , (void*)DM_open_close_trunk            },
  { A_CLOSE        , O_TRUNK            , (void*)DM_open_close_trunk            },
  { A_OPEN         , O_LARGE_BAG        , (void*)DM_open_close_large_bag        },
  { A_CLOSE        , O_LARGE_BAG        , (void*)DM_open_close_large_bag        },
  { A_OPEN         , F_FRONT_DOOR      , (void*)DM_open_front_door             },
  { A_COUNT        , O_MATCH            , (void*)DM_count_matches               },
  { A_OPEN         , O_MATCH            , (void*)DM_count_matches               },
  { A_EAT          , O_LUNCH            , (void*)DM_eat_lunch                   },
  { A_EAT          , O_GARLIC           , (void*)DM_eat_garlic                  },
  { A_DRINK        , O_WATER            , (void*)DM_drink_water                 },
  { A_CLIMBDOWN    , O_ROPE             , (void*)DM_climbdown_rope              },
  { A_BREAK        , F_MIRROR1         , (void*)DM_break_mirror                },
  { A_BREAK        , F_MIRROR2         , (void*)DM_break_mirror                },
  { A_LOOKIN       , F_MIRROR1         , (void*)DM_lookin_mirror               },
  { A_LOOKIN       , F_MIRROR2         , (void*)DM_lookin_mirror               },
  { A_EXAMINE      , F_MIRROR1         , (void*)DM_lookin_mirror               },
  { A_EXAMINE      , F_MIRROR2         , (void*)DM_lookin_mirror               },
  { A_LOOKTHROUGH  , F_KITCHEN_WINDOW  , (void*)DM_lookthrough_k_w  },
  { A_LOOKIN       , F_KITCHEN_WINDOW  , (void*)DM_lookthrough_k_w  },
  { A_LOOKUNDER    , F_RUG             , (void*)DM_lookunder_rug               },
  { A_LOOKUNDER    , O_LEAVES           , (void*)DM_lookunder_leaves            },
  { A_LOOKUNDER    , F_RAINBOW         , (void*)DM_lookunder_rainbow           },
  { A_LOOKIN       , F_CHIMNEY         , (void*)DM_lookin_chimney              },
  { A_EXAMINE      , F_CHIMNEY         , (void*)DM_lookin_chimney              },
  { A_EXAMINE      , F_KITCHEN_WINDOW  , (void*)DM_examine_k_w      },
  { A_LOOKIN       , O_BAG_OF_COINS     , (void*)DM_lookin_bag_of_coins         },
  { A_EXAMINE      , O_BAG_OF_COINS     , (void*)DM_lookin_bag_of_coins         },
  { A_LOOKIN       , O_TRUNK            , (void*)DM_lookin_trunk                },
  { A_EXAMINE      , O_TRUNK            , (void*)DM_lookin_trunk                },
  { A_SQUEEZE      , O_TUBE             , (void*)DM_squeeze_tube                },
  { A_EXAMINE      , O_RAISED_BASKET    , (void*)DM_examine_r_b       },
  { A_EXAMINE      , O_LOWERED_BASKET   , (void*)DM_examine_l_b      },
  { A_LOOKIN       , O_LARGE_BAG        , (void*)DM_lookin_large_bag            },
  { A_EXAMINE      , O_LARGE_BAG        , (void*)DM_lookin_large_bag            },
  { A_LOOKTHROUGH  , F_GRATE           , (void*)DM_lookthrough_grate           },
  { A_LOOKIN       , F_GRATE           , (void*)DM_lookthrough_grate           },
  { A_EXAMINE      , O_WATER            , (void*)DM_lookin_water                },
  { A_LOOKIN       , O_WATER            , (void*)DM_lookin_water                },
  { A_WHEREIS      , F_GRANITE_WALL    , (void*)DM_whereis_granite_wall        },
  { A_WHEREIS      , F_SONGBIRD        , (void*)DM_whereis_songbird            },
  { A_WHEREIS      , F_WHITE_HOUSE     , (void*)DM_whereis_white_house         },
  { A_WHEREIS      , F_FOREST          , (void*)DM_whereis_forest              },
  { A_READ         , F_GRANITE_WALL    , (void*)DM_read_granite_wall           },
  { A_EXAMINE      , O_ZORKMID          , (void*)DM_examine_zorkmid             },
  { A_EXAMINE      , O_GRUE             , (void*)DM_examine_grue                },
  { A_WHEREIS      , O_ZORKMID          , (void*)DM_whereis_zorkmid             },
  { A_WHEREIS      , O_GRUE             , (void*)DM_whereis_grue                },
  { A_LISTENTO     , O_TROLL            , (void*)DM_listento_troll              },
  { A_LISTENTO     , O_THIEF            , (void*)DM_listento_thief              },
  { A_LISTENTO     , O_CYCLOPS          , (void*)DM_listento_cyclops            },
  { A_LISTENTO     , F_FOREST          , (void*)DM_listento_forest             },
  { A_LISTENTO     , F_SONGBIRD        , (void*)DM_listento_songbird           },
  { A_CROSS        , F_RAINBOW         , (void*)DM_cross_rainbow               },
  { A_CROSS        , F_LAKE            , (void*)DM_cross_lake                  },
  { A_CROSS        , F_STREAM          , (void*)DM_cross_stream                },
  { A_CROSS        , F_CHASM           , (void*)DM_cross_chasm                 },
  { A_EXORCISE     , O_GHOSTS           , (void*)DM_exorcise_ghosts             },
  { A_RAISE        , F_RUG             , (void*)DM_raise_rug                   },
  { A_RAISE        , F_TRAP_DOOR       , (void*)DM_raise_trap_door             },
  { A_SMELL        , F_GAS             , (void*)DM_smell_gas                   },
  { A_SMELL        , O_SANDWICH_BAG     , (void*)DM_smell_sandwich_bag          },

  { 0, 0, 0 }
};
