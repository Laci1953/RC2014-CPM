#define NUM_OBJECTS  80
#define INSIDE  2048

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

enum
{
  O_NOTHING,
  O_YOU,
  O_CYCLOPS,
  O_GHOSTS,
  O_BAT,
  O_THIEF,
  O_TROLL,
  O_LOWERED_BASKET,
  O_RAISED_BASKET,
  O_TROPHY_CASE,
  O_MACHINE,
  O_MAILBOX,
  O_KITCHEN_TABLE,
  O_ATTIC_TABLE,
  O_WATER,
  O_SKULL,
  O_TIMBERS,
  O_LUNCH,
  O_BELL,
  O_HOT_BELL,
  O_BOOK,
  O_AXE,
  O_BROKEN_LAMP,
  O_SCEPTRE,
  O_SANDWICH_BAG,
  O_CHALICE,
  O_GARLIC,
  O_TRIDENT,
  O_BOTTLE,
  O_COFFIN,
  O_PUMP,
  O_DIAMOND,
  O_JADE,
  O_KNIFE,
  O_BURNED_OUT_LANTERN,
  O_BAG_OF_COINS,
  O_LAMP,
  O_EMERALD,
  O_ADVERTISEMENT,
  O_INFLATED_BOAT,
  O_MATCH,
  O_PAINTING,
  O_CANDLES,
  O_GUNK,
  O_LEAVES,
  O_PUNCTURED_BOAT,
  O_INFLATABLE_BOAT,
  O_BAR,
  O_POT_OF_GOLD,
  O_BUOY,
  O_ROPE,
  O_RUSTY_KNIFE,
  O_BRACELET,
  O_TOOL_CHEST,
  O_SCREWDRIVER,
  O_KEYS,
  O_SHOVEL,
  O_COAL,
  O_SCARAB,
  O_LARGE_BAG,
  O_STILETTO,
  O_SWORD,
  O_MAP,
  O_BOAT_LABEL,
  O_TORCH,
  O_GUIDE,
  O_TRUNK,
  O_TUBE,
  O_PUTTY,
  O_OWNERS_MANUAL,
  O_WRENCH,
  O_NEST,
  O_EGG,
  O_BROKEN_EGG,
  O_BAUBLE,
  O_CANARY,
  O_BROKEN_CANARY,
  O_ENGRAVINGS,
  O_ZORKMID,
  O_GRUE
};


struct OBJ_STRUCT
{
  short init_loc;
  short loc;
  short size;
  short capacity;
  short order;
  short prop;
  char init_thiefvalue;
  char thiefvalue;
};


struct OBJ_STRUCT Obj[NUM_OBJECTS] =
{
/*     label                      init_loc                     loc  size capacity order prop    init / cur*/
/*                                                                                              thiefvalue*/
  { /* O_NOTHING            */  0                            , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_YOU                */  R_WEST_OF_HOUSE           , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_CYCLOPS            */  R_CYCLOPS_ROOM            , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_GHOSTS             */  R_ENTRANCE_TO_HADES       , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_BAT                */  R_BAT_ROOM                , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_THIEF              */  R_ROUND_ROOM              , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_TROLL              */  R_TROLL_ROOM              , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_LOWERED_BASKET     */  R_LOWER_SHAFT             , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_RAISED_BASKET      */  R_SHAFT_ROOM              , 0 ,   0 ,     50 ,   0 ,  0 ,  0 , 0 },
  { /* O_TROPHY_CASE        */  R_LIVING_ROOM             , 0 ,   0 ,  10000 ,   0 ,  0 ,  0 , 0 },
  { /* O_MACHINE            */  R_MACHINE_ROOM            , 0 ,   0 ,     50 ,   0 ,  0 ,  0 , 0 },
  { /* O_MAILBOX            */  R_WEST_OF_HOUSE           , 0 ,   0 ,     10 ,   0 ,  0 ,  0 , 0 },
  { /* O_KITCHEN_TABLE      */  R_KITCHEN                 , 0 ,   0 ,     50 ,   0 ,  0 ,  0 , 0 },
  { /* O_ATTIC_TABLE        */  R_ATTIC                   , 0 ,   0 ,     40 ,   0 ,  0 ,  0 , 0 },
  { /* O_WATER              */    INSIDE + O_BOTTLE        , 0 ,   4 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_SKULL              */  R_LAND_OF_LIVING_DEAD     , 0 ,   0 ,      0 ,   0 ,  0 , 10 , 0 },
  { /* O_TIMBERS            */  R_TIMBER_ROOM             , 0 ,  50 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_LUNCH              */    INSIDE + O_SANDWICH_BAG  , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_BELL               */  R_NORTH_TEMPLE            , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_HOT_BELL           */  0                            , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_BOOK               */  R_SOUTH_TEMPLE            , 0 ,  10 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_AXE                */    INSIDE + O_TROLL         , 0 ,  25 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_BROKEN_LAMP        */  0                            , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_SCEPTRE            */    INSIDE + O_COFFIN        , 0 ,   3 ,      0 ,   0 ,  0 ,  6 , 0 },
  { /* O_SANDWICH_BAG       */    INSIDE + O_KITCHEN_TABLE , 0 ,   9 ,      9 ,   0 ,  0 ,  0 , 0 },
  { /* O_CHALICE            */  R_TREASURE_ROOM           , 0 ,  10 ,      5 ,   0 ,  0 ,  5 , 0 },
  { /* O_GARLIC             */    INSIDE + O_SANDWICH_BAG  , 0 ,   4 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_TRIDENT            */  R_ATLANTIS_ROOM           , 0 ,  20 ,      0 ,   0 ,  0 , 11 , 0 },
  { /* O_BOTTLE             */    INSIDE + O_KITCHEN_TABLE , 0 ,   0 ,      4 ,   0 ,  0 ,  0 , 0 },
  { /* O_COFFIN             */  R_EGYPT_ROOM              , 0 ,  55 ,     35 ,   0 ,  0 , 15 , 0 },
  { /* O_PUMP               */  R_RESERVOIR_NORTH         , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_DIAMOND            */  0                            , 0 ,   0 ,      0 ,   0 ,  0 , 10 , 0 },
  { /* O_JADE               */  R_BAT_ROOM                , 0 ,  10 ,      0 ,   0 ,  0 ,  5 , 0 },
  { /* O_KNIFE              */    INSIDE + O_ATTIC_TABLE   , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_BURNED_OUT_LANTERN */  R_MAZE_5                  , 0 ,  20 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_BAG_OF_COINS       */  R_MAZE_5                  , 0 ,  15 ,      0 ,   0 ,  0 ,  5 , 0 },
  { /* O_LAMP               */  R_LIVING_ROOM             , 0 ,  15 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_EMERALD            */    INSIDE + O_BUOY          , 0 ,   0 ,      0 ,   0 ,  0 , 10 , 0 },
  { /* O_ADVERTISEMENT      */    INSIDE + O_MAILBOX       , 0 ,   2 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_INFLATED_BOAT      */  0                            , 0 ,  20 ,    100 ,   0 ,  0 ,  0 , 0 },
  { /* O_MATCH              */  R_DAM_LOBBY               , 0 ,   2 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_PAINTING           */  R_GALLERY                 , 0 ,  15 ,      0 ,   0 ,  0 ,  6 , 0 },
  { /* O_CANDLES            */  R_SOUTH_TEMPLE            , 0 ,  10 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_GUNK               */  0                            , 0 ,  10 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_LEAVES             */  R_GRATING_CLEARING        , 0 ,  25 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_PUNCTURED_BOAT     */  0                            , 0 ,  20 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_INFLATABLE_BOAT    */  R_DAM_BASE                , 0 ,  20 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_BAR                */  R_LOUD_ROOM               , 0 ,  20 ,      0 ,   0 ,  0 ,  5 , 0 },
  { /* O_POT_OF_GOLD        */  R_END_OF_RAINBOW          , 0 ,  15 ,      0 ,   0 ,  0 , 10 , 0 },
  { /* O_BUOY               */  R_RIVER_4                 , 0 ,  10 ,     20 ,   0 ,  0 ,  0 , 0 },
  { /* O_ROPE               */  R_ATTIC                   , 0 ,  10 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_RUSTY_KNIFE        */  R_MAZE_5                  , 0 ,  20 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_BRACELET           */  R_GAS_ROOM                , 0 ,  10 ,      0 ,   0 ,  0 ,  5 , 0 },
  { /* O_TOOL_CHEST         */  R_MAINTENANCE_ROOM        , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_SCREWDRIVER        */  R_MAINTENANCE_ROOM        , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_KEYS               */  R_MAZE_5                  , 0 ,  10 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_SHOVEL             */  R_SANDY_BEACH             , 0 ,  15 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_COAL               */  R_DEAD_END_5              , 0 ,  20 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_SCARAB             */  R_SANDY_CAVE              , 0 ,   8 ,      0 ,   0 ,  0 ,  5 , 0 },
  { /* O_LARGE_BAG          */    INSIDE + O_THIEF         , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_STILETTO           */    INSIDE + O_THIEF         , 0 ,  10 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_SWORD              */  R_LIVING_ROOM             , 0 ,  30 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_MAP                */    INSIDE + O_TROPHY_CASE   , 0 ,   2 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_BOAT_LABEL         */    INSIDE + O_INFLATED_BOAT , 0 ,   2 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_TORCH              */  R_TORCH_ROOM              , 0 ,  20 ,      0 ,   0 ,  0 ,  6 , 0 },
  { /* O_GUIDE              */  R_DAM_LOBBY               , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_TRUNK              */  R_RESERVOIR               , 0 ,  35 ,      0 ,   0 ,  0 ,  5 , 0 },
  { /* O_TUBE               */  R_MAINTENANCE_ROOM        , 0 ,   5 ,      7 ,   0 ,  0 ,  0 , 0 },
  { /* O_PUTTY              */    INSIDE + O_TUBE          , 0 ,   6 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_OWNERS_MANUAL      */  R_STUDIO                  , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_WRENCH             */  R_MAINTENANCE_ROOM        , 0 ,  10 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_NEST               */  R_UP_A_TREE               , 0 ,   0 ,     20 ,   0 ,  0 ,  0 , 0 },
  { /* O_EGG                */    INSIDE + O_NEST          , 0 ,   0 ,      6 ,   0 ,  0 ,  5 , 0 },
  { /* O_BROKEN_EGG         */  0                            , 0 ,   0 ,      6 ,   0 ,  0 ,  2 , 0 },
  { /* O_BAUBLE             */  0                            , 0 ,   0 ,      0 ,   0 ,  0 ,  1 , 0 },
  { /* O_CANARY             */    INSIDE + O_EGG           , 0 ,   0 ,      0 ,   0 ,  0 ,  4 , 0 },
  { /* O_BROKEN_CANARY      */    INSIDE + O_BROKEN_EGG    , 0 ,   0 ,      0 ,   0 ,  0 ,  1 , 0 },
  { /* O_ENGRAVINGS         */  R_ENGRAVINGS_CAVE         , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_ZORKMID            */  0                            , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 },
  { /* O_GRUE               */  0                            , 0 ,   0 ,      0 ,   0 ,  0 ,  0 , 0 }
};
