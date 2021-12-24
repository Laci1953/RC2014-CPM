#include <ovr43.h>

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

enum
{
  R_NULL,		/* 0  */
  R_WEST_OF_HOUSE,	/* 1  */
  R_STONE_BARROW,	/* 2  */
  R_NORTH_OF_HOUSE,	/* 3  */
  R_SOUTH_OF_HOUSE,	/* 4  */
  R_EAST_OF_HOUSE,	/* 5  */
  R_FOREST_1,		/* 6  */
  R_FOREST_2,		/* 7  */
  R_MOUNTAINS,		/* 8  */
  R_FOREST_3,		/* 9  */
  R_PATH,		/* 10  */
  R_UP_A_TREE,		/* 11  */
  R_GRATING_CLEARING,	/* 12  */
  R_CLEARING,		/* 13  */
  R_KITCHEN,		/* 14  */
  R_ATTIC,		/* 15  */
  R_LIVING_ROOM,	/* 16  */
  R_CELLAR,		/* 17  */
  R_TROLL_ROOM,		/* 18  */
  R_EAST_OF_CHASM,	/* 19  */
  R_GALLERY,		/* 20  */
  R_STUDIO,		/* 21  */
  R_MAZE_1,		/* 22  */
  R_MAZE_2,		/* 23  */
  R_MAZE_3,		/* 24  */
  R_MAZE_4,		/* 25  */
  R_DEAD_END_1,		/* 26  */
  R_MAZE_5,		/* 27  */
  R_DEAD_END_2,		/* 28  */
  R_MAZE_6,		/* 29  */
  R_MAZE_7,		/* 30  */
  R_MAZE_8,		/* 31  */
  R_DEAD_END_3,		/* 32  */
  R_MAZE_9,		/* 33  */
  R_MAZE_10,		/* 34  */
  R_MAZE_11,		/* 35  */
  R_GRATING_ROOM,	/* 36  */
  R_MAZE_12,		/* 37  */
  R_DEAD_END_4,		/* 38  */
  R_MAZE_13,		/* 39  */
  R_MAZE_14,		/* 40  */
  R_MAZE_15,		/* 41  */
  R_CYCLOPS_ROOM,	/* 42 */
  R_STRANGE_PASSAGE,	/* 43 */
  R_TREASURE_ROOM,	/* 44 */
  R_RESERVOIR_SOUTH,	/* 45 */
  R_RESERVOIR,		/* 46 */
  R_RESERVOIR_NORTH,	/* 47 */
  R_STREAM_VIEW,	/* 48 */
  R_IN_STREAM,		/* 49 */
  R_MIRROR_R_1,		/* 50  */
  R_MIRROR_R_2,		/* 51  */
  R_SMALL_CAVE,		/* 52  */
  R_TINY_CAVE,		/* 53  */
  R_COLD_PASSAGE,	/* 54  */
  R_NARROW_PASSAGE,	/* 55  */
  R_WINDING_PASSAGE,	/* 56  */
  R_TWISTING_PASSAGE,	/* 57  */
  R_ATLANTIS_ROOM,	/* 58  */
  R_EW_PASSAGE,		/* 59  */
  R_ROUND_ROOM,		/* 60  */
  R_DEEP_CANYON,	/* 61  */
  R_DAMP_CAVE,		/* 62  */
  R_LOUD_ROOM,		/* 63 */
  R_NS_PASSAGE,		/* 64 */
  R_CHASM_ROOM,		/* 65 */
  R_ENTRANCE_TO_HADES,	/* 66 */
  R_LAND_OF_LIVING_DEAD,/* 67 */
  R_ENGRAVINGS_CAVE,	/* 68 */
  R_EGYPT_ROOM,		/* 69 */
  R_DOME_ROOM,		/* 70  */
  R_TORCH_ROOM,		/* 71  */
  R_NORTH_TEMPLE,	/* 72  */
  R_SOUTH_TEMPLE,	/* 73  */
  R_DAM_ROOM,		/* 74  */
  R_DAM_LOBBY,		/* 75  */
  R_MAINTENANCE_ROOM,	/* 76  */
  R_DAM_BASE,		/* 77  */
  R_RIVER_1,		/* 78 */
  R_RIVER_2,		/* 79  */
  R_RIVER_3,		/* 80  */
  R_WHITE_CLIFFS_NORTH,	/* 81  */
  R_WHITE_CLIFFS_SOUTH,	/* 82 */
  R_RIVER_4,		/* 83 */
  R_RIVER_5,		/* 84 */
  R_SHORE,		/* 85 */
  R_SANDY_BEACH,	/* 86 */
  R_SANDY_CAVE,		/* 87 */
  R_ARAGAIN_FALLS,	/* 88 */
  R_ON_RAINBOW,		/* 89 */
  R_END_OF_RAINBOW,	/* 90  */
  R_CANYON_BOTTOM,	/* 91  */
  R_CLIFF_MIDDLE,	/* 92  */
  R_CANYON_VIEW,	/* 93  */
  R_MINE_ENTRANCE,	/* 94  */
  R_SQUEEKY_ROOM,	/* 95  */
  R_BAT_ROOM,		/* 96  */
  R_SHAFT_ROOM,		/* 97 */
  R_SMELLY_ROOM,	/* 98 */
  R_GAS_ROOM,		/* 99 */
  R_LADDER_TOP,		/* 100  */
  R_LADDER_BOTTOM,	/* 101  */
  R_DEAD_END_5,		/* 102  */
  R_TIMBER_ROOM,	/* 103  */
  R_LOWER_SHAFT,	/* 104  */
  R_MACHINE_ROOM,	/* 105  */
  R_MINE_1,		/* 106  */
  R_MINE_2,		/* 107  */
  R_MINE_3,		/* 108  */
  R_MINE_4,		/* 109  */
  R_SLIDE_ROOM		/* 110  */
};

/* -------------------------------------
enum
{
 GoFrom_StoneBarrow_West      ,
 GoFrom_WestOfHouse_Southwest ,
 GoFrom_EastOfHouse_West      ,
 GoFrom_Kitchen_East          ,
 GoFrom_LivingRoom_West       ,
 GoFrom_Cellar_Up             ,
 GoFrom_TrollRoom_East        ,
 GoFrom_TrollRoom_West        ,
 GoFrom_GratingRoom_Up        ,
 GoFrom_CyclopsRoom_East      ,
 GoFrom_CyclopsRoom_Up        ,
 GoFrom_ReservoirSouth_North  ,
 GoFrom_ReservoirNorth_South  ,
 GoFrom_EntranceToHades_South ,
 GoFrom_DomeRoom_Down         ,
 GoFrom_OntoRainbowRoutine    ,
 GoFrom_Maze2_Down            ,
 GoFrom_Maze7_Down            ,
 GoFrom_Maze9_Down            ,
 GoFrom_Maze12_Down           ,
 GoFrom_GratingClearing_Down  ,
 GoFrom_LivingRoom_Down       ,
 GoFrom_SouthTemple_Down      ,
 GoFrom_WhiteCliffsNorth_South,
 GoFrom_WhiteCliffsNorth_West ,
 GoFrom_WhiteCliffsSouth_North,
 GoFrom_TimberRoom_West       ,
 GoFrom_LowerShaft_East       ,
 GoFrom_Kitchen_Down          ,
 GoFrom_Studio_Up             ,
 GoFrom_LandOfLivingDead_North,
 GoFrom_StrangePassage_West   ,
 GoFrom_NorthTemple_North     ,
 GoFrom_MineEntrance_West     ,
 GoFrom_DamLobby_North_Or_East
};
------------------------------------- */
struct GOFROM_STRUCT
{
  int room;
  int action; /*go-direction action (but technically could be any action)*/
  int (*f)(void); /*function returns 1 if action completed; otherwise fall through*/
};

/*A_IN and A_OUT can also be handled here*/
struct GOFROM_STRUCT GoFrom[] =
{
  { R_STONE_BARROW         , A_WEST      , (void*)GoFrom_StoneBarrow_West       },
  { R_STONE_BARROW         , A_IN        , (void*)GoFrom_StoneBarrow_West       },
  { R_WEST_OF_HOUSE        , A_SOUTHWEST , (void*)GoFrom_WestOfHouse_Southwest  },
  { R_WEST_OF_HOUSE        , A_IN        , (void*)GoFrom_WestOfHouse_Southwest  },
  { R_EAST_OF_HOUSE        , A_WEST      , (void*)GoFrom_EastOfHouse_West       },
  { R_EAST_OF_HOUSE        , A_IN        , (void*)GoFrom_EastOfHouse_West       },
  { R_KITCHEN              , A_EAST      , (void*)GoFrom_Kitchen_East           },
  { R_KITCHEN              , A_OUT       , (void*)GoFrom_Kitchen_East           },
  { R_LIVING_ROOM          , A_WEST      , (void*)GoFrom_LivingRoom_West        },
  { R_CELLAR               , A_UP        , (void*)GoFrom_Cellar_Up              },
  { R_TROLL_ROOM           , A_EAST      , (void*)GoFrom_TrollRoom_East         },
  { R_TROLL_ROOM           , A_WEST      , (void*)GoFrom_TrollRoom_West         },
  { R_GRATING_ROOM         , A_UP        , (void*)GoFrom_GratingRoom_Up         },
  { R_CYCLOPS_ROOM         , A_EAST      , (void*)GoFrom_CyclopsRoom_East       },
  { R_CYCLOPS_ROOM         , A_UP        , (void*)GoFrom_CyclopsRoom_Up         },
  { R_RESERVOIR_SOUTH      , A_NORTH     , (void*)GoFrom_ReservoirSouth_North   },
  { R_RESERVOIR_NORTH      , A_SOUTH     , (void*)GoFrom_ReservoirNorth_South   },
  { R_ENTRANCE_TO_HADES    , A_SOUTH     , (void*)GoFrom_EntranceToHades_South  },
  { R_ENTRANCE_TO_HADES    , A_IN        , (void*)GoFrom_EntranceToHades_South  },
  { R_DOME_ROOM            , A_DOWN      , (void*)GoFrom_DomeRoom_Down          },
  { R_ARAGAIN_FALLS        , A_WEST      , (void*)GoFrom_OntoRainbowRoutine     },
  { R_ARAGAIN_FALLS        , A_UP        , (void*)GoFrom_OntoRainbowRoutine     },
  { R_END_OF_RAINBOW       , A_UP        , (void*)GoFrom_OntoRainbowRoutine     },
  { R_END_OF_RAINBOW       , A_NORTHEAST , (void*)GoFrom_OntoRainbowRoutine     },
  { R_END_OF_RAINBOW       , A_EAST      , (void*)GoFrom_OntoRainbowRoutine     },
  { R_MAZE_2               , A_DOWN      , (void*)GoFrom_Maze2_Down             },
  { R_MAZE_7               , A_DOWN      , (void*)GoFrom_Maze7_Down             },
  { R_MAZE_9               , A_DOWN      , (void*)GoFrom_Maze9_Down             },
  { R_MAZE_12              , A_DOWN      , (void*)GoFrom_Maze12_Down            },
  { R_GRATING_CLEARING     , A_DOWN      , (void*)GoFrom_GratingClearing_Down   },
  { R_LIVING_ROOM          , A_DOWN      , (void*)GoFrom_LivingRoom_Down        },
  { R_SOUTH_TEMPLE         , A_DOWN      , (void*)GoFrom_SouthTemple_Down       },
  { R_WHITE_CLIFFS_NORTH   , A_SOUTH     , (void*)GoFrom_WhiteCliffsNorth_South },
  { R_WHITE_CLIFFS_NORTH   , A_WEST      , (void*)GoFrom_WhiteCliffsNorth_West  },
  { R_WHITE_CLIFFS_SOUTH   , A_NORTH     , (void*)GoFrom_WhiteCliffsSouth_North },
  { R_TIMBER_ROOM          , A_WEST      , (void*)GoFrom_TimberRoom_West        },
  { R_LOWER_SHAFT          , A_EAST      , (void*)GoFrom_LowerShaft_East        },
  { R_LOWER_SHAFT          , A_OUT       , (void*)GoFrom_LowerShaft_East        },
  { R_KITCHEN              , A_DOWN      , (void*)GoFrom_Kitchen_Down           },
  { R_STUDIO               , A_UP        , (void*)GoFrom_Studio_Up              },
  { R_LAND_OF_LIVING_DEAD  , A_OUT       , (void*)GoFrom_LandOfLivingDead_North },
  { R_STRANGE_PASSAGE      , A_IN        , (void*)GoFrom_StrangePassage_West    },
  { R_NORTH_TEMPLE         , A_OUT       , (void*)GoFrom_NorthTemple_North      },
  { R_MINE_ENTRANCE        , A_IN        , (void*)GoFrom_MineEntrance_West      },
  { R_DAM_LOBBY            , A_NORTH     , (void*)GoFrom_DamLobby_North_Or_East },
  { R_DAM_LOBBY            , A_EAST      , (void*)GoFrom_DamLobby_North_Or_East },

  { 0, 0, 0 }
};
