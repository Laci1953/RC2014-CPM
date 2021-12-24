#include <ovr38.h>
#include <ovr39.h>
#include <ovr50.h>

#define O_CYCLOPS 2  
#define O_GHOSTS 3  
#define O_BAT 4  
#define O_THIEF 5  
#define O_TROLL 6  
#define O_INFLATED_BOAT 39  
#define O_LAMP 36  
#define O_CANDLES 42  

#define F_CHASM 2100 
#define F_CLIMBABLE_CLIFF 2063 
#define F_RIVER 2067 
#define F_MIRROR2 2086 
#define F_MIRROR1 2087 

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

/*-----------------
enum
{
 DoMiscGiveTo_cyclops,
 DoMiscGiveTo_thief  ,
 DoMiscGiveTo_troll  ,
 DoMiscThrowTo_chasm          ,
 DoMiscThrowTo_river          ,
 DoMiscThrowTo_mirror         ,
 DoMiscThrowTo_troll          ,
 DoMiscThrowTo_cyclops        ,
 DoMiscThrowTo_thief          ,
 PrintDesc_LivingRoom     ,
 PrintDesc_EastOfHouse    ,
 PrintDesc_Kitchen        ,
 PrintDesc_Attic          ,
 PrintDesc_GratingClearing,
 PrintDesc_GratingRoom    ,
 PrintDesc_DamRoom        ,
 PrintDesc_ReservoirSouth ,
 PrintDesc_Reservoir      ,
 PrintDesc_ReservoirNorth ,
 PrintDesc_LoudRoom       ,
 PrintDesc_DeepCanyon     ,
 PrintDesc_MachineRoom    ,
 PrintDesc_AragainFalls   ,
 PrintDesc_WestOfHouse    ,
 PrintDesc_MirrorRoom1    ,
 PrintDesc_MirrorRoom2    ,
 PrintDesc_TorchRoom      ,
 PrintDesc_DomeRoom       ,
 PrintDesc_CyclopsRoom    ,
 PrintDesc_UpATree        ,
 PrintDesc_Ghosts      ,
 PrintDesc_Bat         ,
 PrintDesc_Troll       ,
 PrintDesc_Thief       ,
 PrintDesc_Cyclops     ,
 PrintDesc_InflatedBoat,
 PrintDesc_Lamp        ,
 PrintDesc_Candles     
};
----------------------------- */

struct DOMISCTO_STRUCT
{
  int action;
  int to; /*can be obj or fobj*/
  void (*f)(int);
};

struct DOMISCTO_STRUCT DoMiscGiveTo[] =
{
  { 0 , O_CYCLOPS , (void*)DoMiscGiveTo_cyclops },
  { 0 , O_THIEF   , (void*)DoMiscGiveTo_thief   },
  { 0 , O_TROLL   , (void*)DoMiscGiveTo_troll   },

  { 0, 0, 0 }
};

struct DOMISCTO_STRUCT DoMiscThrowTo[] =
{
  { 0 , F_CHASM           , (void*)DoMiscThrowTo_chasm           },
  { 0 , F_CLIMBABLE_CLIFF , (void*)DoMiscThrowTo_river           },
  { 0 , F_RIVER           , (void*)DoMiscThrowTo_river           },
  { 0 , F_MIRROR1         , (void*)DoMiscThrowTo_mirror          },
  { 0 , F_MIRROR2         , (void*)DoMiscThrowTo_mirror          },
  { 0 , O_TROLL            , (void*)DoMiscThrowTo_troll           },
  { 0 , O_CYCLOPS          , (void*)DoMiscThrowTo_cyclops         },
  { 0 , O_THIEF            , (void*)DoMiscThrowTo_thief           },

  { 0, 0, 0 }
};

struct OVERRIDEROOMDESC_STRUCT
{
  int room;
  void (*f)(void);
};

struct OVERRIDEROOMDESC_STRUCT OverrideRoomDesc[] =
{
  { R_LIVING_ROOM       , (void*)PrintDesc_LivingRoom      },
  { R_EAST_OF_HOUSE     , (void*)PrintDesc_EastOfHouse     },
  { R_KITCHEN           , (void*)PrintDesc_Kitchen         },
  { R_ATTIC             , (void*)PrintDesc_Attic           },
  { R_GRATING_CLEARING  , (void*)PrintDesc_GratingClearing },
  { R_GRATING_ROOM      , (void*)PrintDesc_GratingRoom     },
  { R_DAM_ROOM          , (void*)PrintDesc_DamRoom         },
  { R_RESERVOIR_SOUTH   , (void*)PrintDesc_ReservoirSouth  },
  { R_RESERVOIR         , (void*)PrintDesc_Reservoir       },
  { R_RESERVOIR_NORTH   , (void*)PrintDesc_ReservoirNorth  },
  { R_LOUD_ROOM         , (void*)PrintDesc_LoudRoom        },
  { R_DEEP_CANYON       , (void*)PrintDesc_DeepCanyon      },
  { R_MACHINE_ROOM      , (void*)PrintDesc_MachineRoom     },
  { R_ARAGAIN_FALLS     , (void*)PrintDesc_AragainFalls    },
  { R_WEST_OF_HOUSE     , (void*)PrintDesc_WestOfHouse     },
  { R_MIRROR_R_1     , (void*)PrintDesc_MirrorRoom1     },
  { R_MIRROR_R_2     , (void*)PrintDesc_MirrorRoom2     },
  { R_TORCH_ROOM        , (void*)PrintDesc_TorchRoom       },
  { R_DOME_ROOM         , (void*)PrintDesc_DomeRoom        },
  { R_CYCLOPS_ROOM      , (void*)PrintDesc_CyclopsRoom     },
  { R_UP_A_TREE         , (void*)PrintDesc_UpATree         },

  { 0, 0 }
};

struct OVERRIDEOBJECTDESC_STRUCT
{
  int obj;
  void (*f)(int);
};

struct OVERRIDEOBJECTDESC_STRUCT OverrideObjectDesc[] =
{
  { O_GHOSTS        , (void*)PrintDesc_Ghosts       },
  { O_BAT           , (void*)PrintDesc_Bat          },
  { O_TROLL         , (void*)PrintDesc_Troll        },
  { O_THIEF         , (void*)PrintDesc_Thief        },
  { O_CYCLOPS       , (void*)PrintDesc_Cyclops      },
  { O_INFLATED_BOAT , (void*)PrintDesc_InflatedBoat },
  { O_LAMP          , (void*)PrintDesc_Lamp         },
  { O_CANDLES       , (void*)PrintDesc_Candles      },

  { 0, 0 }
};
