/*room properties bit flags*/

#define R_DESCRIBED     1  /* set when room description already printed */
#define R_BODYOFWATER   2
#define R_LIT           4  /* set when there is natural light or a light fixture */
#define R_WATERHERE     8
#define R_SACRED       16  /* set when thief not allowed in room */
#define R_MAZE         32
#define R_ALWAYSDESC   64

#define NUM_ROOMS  111 /* including null room 0 */

struct ROOM_STRUCT
{
  char *scenery;
  short prop;
  short init_prop;
};

/*scenery: single words separated by commas with no whitespace*/

struct ROOM_STRUCT Room[NUM_ROOMS] =
{
  /*                          scenery cur/init prop*/

  /* null room                */  { 0, 0, 0                                              },

  /* R_WEST_OF_HOUSE       */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_STONE_BARROW        */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_NORTH_OF_HOUSE      */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_SOUTH_OF_HOUSE      */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_EAST_OF_HOUSE       */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_FOREST_1            */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_FOREST_2            */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_MOUNTAINS           */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_FOREST_3            */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_PATH                */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_UP_A_TREE           */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_GRATING_CLEARING    */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_CLEARING            */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_KITCHEN             */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_ATTIC               */  { 0, 0, R_SACRED                                       },
  /* R_LIVING_ROOM         */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_CELLAR              */  { 0, 0, 0                                              },
  /* R_TROLL_ROOM          */  { 0, 0, 0                                              },
  /* R_EAST_OF_CHASM       */  { 0, 0, 0                                              },
  /* R_GALLERY             */  { 0, 0, R_LIT                                          },
  /* R_STUDIO              */  { 0, 0, 0                                              },
  /* R_MAZE_1              */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_2              */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_3              */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_4              */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_DEAD_END_1          */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_5              */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_DEAD_END_2          */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_6              */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_7              */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_8              */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_DEAD_END_3          */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_9              */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_10             */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_11             */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_GRATING_ROOM        */  { 0, 0, 0                                              },
  /* R_MAZE_12             */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_DEAD_END_4          */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_13             */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_14             */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_MAZE_15             */  { 0, 0, R_ALWAYSDESC | R_MAZE                          },
  /* R_CYCLOPS_ROOM        */  { 0, 0, 0                                              },
  /* R_STRANGE_PASSAGE     */  { 0, 0, 0                                              },
  /* R_TREASURE_ROOM       */  { 0, 0, 0                                              },
  /* R_RESERVOIR_SOUTH     */  { 0, 0, R_WATERHERE                                    },
  /* R_RESERVOIR           */  { 0, 0, R_WATERHERE | R_BODYOFWATER                    },
  /* R_RESERVOIR_NORTH     */  { 0, 0, R_WATERHERE                                    },
  /* R_STREAM_VIEW         */  { 0, 0, R_WATERHERE                                    },
  /* R_IN_STREAM           */  { 0, 0, R_WATERHERE | R_BODYOFWATER                    },
  /* R_MIRROR_R_1       */  { 0, 0, 0                                              },
  /* R_MIRROR_R_2       */  { 0, 0, R_LIT                                          },
  /* R_SMALL_CAVE          */  { 0, 0, 0                                              },
  /* R_TINY_CAVE           */  { 0, 0, 0                                              },
  /* R_COLD_PASSAGE        */  { 0, 0, 0                                              },
  /* R_NARROW_PASSAGE      */  { 0, 0, 0                                              },
  /* R_WINDING_PASSAGE     */  { 0, 0, 0                                              },
  /* R_TWISTING_PASSAGE    */  { 0, 0, 0                                              },
  /* R_ATLANTIS_ROOM       */  { 0, 0, 0                                              },
  /* R_EW_PASSAGE          */  { 0, 0, 0                                              },
  /* R_ROUND_ROOM          */  { 0, 0, 0                                              },
  /* R_DEEP_CANYON         */  { 0, 0, 0                                              },
  /* R_DAMP_CAVE           */  { 0, 0, 0                                              },
  /* R_LOUD_ROOM           */  { 0, 0, 0                                              },
  /* R_NS_PASSAGE          */  { 0, 0, 0                                              },
  /* R_CHASM_ROOM          */  { 0, 0, 0                                              },
  /* R_ENTRANCE_TO_HADES   */  { 0, 0, R_LIT                                          },
  /* R_LAND_OF_LIVING_DEAD */  { 0, 0, R_LIT                                          },
  /* R_ENGRAVINGS_CAVE     */  { 0, 0, 0                                              },
  /* R_EGYPT_ROOM          */  { 0, 0, 0                                              },
  /* R_DOME_ROOM           */  { 0, 0, 0                                              },
  /* R_TORCH_ROOM          */  { 0, 0, 0                                              },
  /* R_NORTH_TEMPLE        */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_SOUTH_TEMPLE        */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_DAM_ROOM            */  { 0, 0, R_WATERHERE | R_LIT                            },
  /* R_DAM_LOBBY           */  { 0, 0, R_LIT                                          },
  /* R_MAINTENANCE_ROOM    */  { 0, 0, 0                                              },
  /* R_DAM_BASE            */  { 0, 0, R_SACRED | R_WATERHERE | R_LIT                 },
  /* R_RIVER_1             */  { 0, 0, R_SACRED | R_WATERHERE | R_LIT | R_BODYOFWATER },
  /* R_RIVER_2             */  { 0, 0, R_SACRED | R_WATERHERE | R_BODYOFWATER         },
  /* R_RIVER_3             */  { 0, 0, R_SACRED | R_WATERHERE | R_BODYOFWATER         },
  /* R_WHITE_CLIFFS_NORTH  */  { 0, 0, R_SACRED | R_WATERHERE                         },
  /* R_WHITE_CLIFFS_SOUTH  */  { 0, 0, R_SACRED | R_WATERHERE                         },
  /* R_RIVER_4             */  { 0, 0, R_SACRED | R_WATERHERE | R_BODYOFWATER         },
  /* R_RIVER_5             */  { 0, 0, R_SACRED | R_WATERHERE | R_LIT | R_BODYOFWATER },
  /* R_SHORE               */  { 0, 0, R_SACRED | R_WATERHERE | R_LIT                 },
  /* R_SANDY_BEACH         */  { 0, 0, R_SACRED | R_WATERHERE                         },
  /* R_SANDY_CAVE          */  { 0, 0, 0                                              },
  /* R_ARAGAIN_FALLS       */  { 0, 0, R_SACRED | R_WATERHERE | R_LIT                 },
  /* R_ON_RAINBOW          */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_END_OF_RAINBOW      */  { 0, 0, R_WATERHERE | R_LIT                            },
  /* R_CANYON_BOTTOM       */  { 0, 0, R_SACRED | R_WATERHERE | R_LIT                 },
  /* R_CLIFF_MIDDLE        */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_CANYON_VIEW         */  { 0, 0, R_SACRED | R_LIT                               },
  /* R_MINE_ENTRANCE       */  { 0, 0, 0                                              },
  /* R_SQUEEKY_ROOM        */  { 0, 0, 0                                              },
  /* R_BAT_ROOM            */  { 0, 0, R_SACRED                                       },
  /* R_SHAFT_ROOM          */  { 0, 0, 0                                              },
  /* R_SMELLY_ROOM         */  { 0, 0, 0                                              },
  /* R_GAS_ROOM            */  { 0, 0, R_SACRED                                       },
  /* R_LADDER_TOP          */  { 0, 0, 0                                              },
  /* R_LADDER_BOTTOM       */  { 0, 0, 0                                              },
  /* R_DEAD_END_5          */  { 0, 0, 0                                              },
  /* R_TIMBER_ROOM         */  { 0, 0, R_SACRED                                       },
  /* R_LOWER_SHAFT         */  { 0, 0, R_SACRED                                       },
  /* R_MACHINE_ROOM        */  { 0, 0, 0                                              },
  /* R_MINE_1              */  { 0, 0, 0                                              },
  /* R_MINE_2              */  { 0, 0, 0                                              },
  /* R_MINE_3              */  { 0, 0, 0                                              },
  /* R_MINE_4              */  { 0, 0, 0                                              },
  /* R_SLIDE_ROOM          */  { 0, 0, 0                                              }
};



/*collection of scenery nouns from all rooms*/
/*single words separated by commas with no whitespace*/
/*alphabetical order is not necessary*/

char *SceneryNouns = "Test1,Test2"; /* any capital letters prevent words from being matched*/

