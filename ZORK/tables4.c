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



/*phrase must contain a maximum of two words*/

/*for matching to work, verb with adverb must come before same verb with no adverb*/

/*words must be separated by exactly one space*/

/*verbs don't need to (but should) be alphabetized*/


struct VERBTOACTION_STRUCT
{
  char *phrase;
  short action;
};

struct VERBTOACTION_STRUCT VerbToAction[] =
{
  /*two words (verb-adverb)*/

  { "blow out"      , A_DEACTIVATE   },
  { "blow up"       , A_INFLATE      },
  { "break open"    , A_BREAK        },
  { "burn down"     , A_ACTIVATE     },
  { "cast out"      , A_EXORCISE     },
  { "climb down"    , A_CLIMBDOWN    },
  { "climb in"      , A_ENTER        },
  { "climb into"    , A_ENTER        },
  { "climb on"      , A_MOUNT        },
  { "climb onto"    , A_MOUNT        },
  { "climb out"     , A_EXIT         },
  { "climb through" , A_CLIMBTHROUGH },
  { "climb up"      , A_CLIMBUP      },
  { "dig in"        , A_DIG          },
  { "drive away"    , A_EXORCISE     },
  { "drive out"     , A_EXORCISE     },
  { "gaze at"       , A_EXAMINE      },
  { "gaze behind"   , A_LOOKBEHIND   },
  { "gaze in"       , A_LOOKIN       },
  { "gaze inside"   , A_LOOKIN       },
  { "gaze into"     , A_LOOKIN       },
  { "gaze on"       , A_LOOKON       },
  { "gaze through"  , A_LOOKTHROUGH  },
  { "gaze under"    , A_LOOKUNDER    },
  { "get in"        , A_ENTER        },
  { "get inside"    , A_ENTER        },
  { "get into"      , A_ENTER        },
  { "get off"       , A_DISMOUNT     },
  { "get on"        , A_MOUNT        },
  { "get out"       , A_EXIT         },
  { "go inside"     , A_ENTER        },
  { "go into"       , A_ENTER        },
  { "go through"    , A_CLIMBTHROUGH },
  { "knock on"      , A_KNOCK        },
  { "knock with"    , A_KNOCK        },
  { "l at"          , A_EXAMINE      },
  { "l behind"      , A_LOOKBEHIND   },
  { "l in"          , A_LOOKIN       },
  { "l inside"      , A_LOOKIN       },
  { "l into"        , A_LOOKIN       },
  { "l on"          , A_LOOKON       },
  { "l through"     , A_LOOKTHROUGH  },
  { "l under"       , A_LOOKUNDER    },
  { "lay on"        , A_SLEEPON      },
  { "lift up"       , A_RAISE        },
  { "listen to"     , A_LISTENTO     },
  { "listen for"    , A_LISTENTO     },
  { "look at"       , A_EXAMINE      },
  { "look behind"   , A_LOOKBEHIND   },
  { "look in"       , A_LOOKIN       },
  { "look inside"   , A_LOOKIN       },
  { "look into"     , A_LOOKIN       },
  { "look on"       , A_LOOKON       },
  { "look through"  , A_LOOKTHROUGH  },
  { "look under"    , A_LOOKUNDER    },
  { "open up"       , A_OPEN         },
  { "pick up"       , A_TAKE         },
  { "pump up"       , A_INFLATE      },
  { "put down"      , A_DROP         },
  { "put on"        , A_WEAR         },
  { "put out"       , A_DEACTIVATE   },
  { "raise up"      , A_RAISE        },
  { "read from"     , A_READ         },
  { "sit in"        , A_ENTER        },
  { "sleep on"      , A_SLEEPON      },
  { "slide down"    , A_SLIDEDOWN    },
  { "slide up"      , A_SLIDEUP      },
  { "stand on"      , A_MOUNT        },
  { "stand up"      , A_DISEMBARK    },
  { "stare at"      , A_EXAMINE      },
  { "stare behind"  , A_LOOKBEHIND   },
  { "stare in"      , A_LOOKIN       },
  { "stare inside"  , A_LOOKIN       },
  { "stare into"    , A_LOOKIN       },
  { "stare on"      , A_LOOKON       },
  { "stare through" , A_LOOKTHROUGH  },
  { "stare under"   , A_LOOKUNDER    },
  { "swim in"       , A_SWIM         },
  { "swim across"   , A_SWIM         },
  { "take off"      , A_REMOVE       },
  { "take out"      , A_TAKE         },
  { "talk to"       , A_TALKTO       },
  { "tie up"        , A_TIE          },
  { "turn off"      , A_DEACTIVATE   },
  { "turn on"       , A_ACTIVATE     },
  { "walk through"  , A_CLIMBTHROUGH },
  { "what is"       , A_EXAMINE      },
  { "what are"      , A_EXAMINE      },
  { "where is"      , A_WHEREIS      },
  { "where am"      , A_WHEREIS      },
  { "where are"     , A_WHEREIS      },
  { "who am"        , A_EXAMINE      },
  { "who is"        , A_EXAMINE      },
  { "wind up"       , A_WIND         },

  /*one word*/

  { "acquire"       , A_TAKE         },
  { "activate"      , A_ACTIVATE     },
  { "advance"       , A_GO           },
  { "ascend"        , A_CLIMBUP      },
  { "ask"           , A_TALKTO       },
  { "attach"        , A_TIE          },
  { "attack"        , A_ATTACK       },
  { "banish"        , A_EXORCISE     },
  { "bathe"         , A_SWIM         },
  { "begone"        , A_EXORCISE     },
  { "bite"          , A_EAT          },
  { "board"         , A_ENTER        },
  { "brandish"      , A_WAVE         },
  { "break"         , A_BREAK        },
  { "brief"         , A_BRIEF        },
  { "brush"         , A_BRUSH        },
  { "burn"          , A_ACTIVATE     },
  { "carry"         , A_TAKE         },
  { "catch"         , A_TAKE         },
  { "chuck"         , A_THROW        },
  { "climb"         , A_CLIMB        },
  { "close"         , A_CLOSE        },
  { "consume"       , A_EAT          },
  { "count"         , A_COUNT        },
  { "cross"         , A_CROSS        },
  { "d"             , A_DOWN         },
  { "damage"        , A_BREAK        },
  { "deactivate"    , A_DEACTIVATE   },
  { "deflate"       , A_DEFLATE      },
  { "descend"       , A_CLIMBDOWN    },
  { "describe"      , A_EXAMINE      },
  { "destroy"       , A_BREAK        },
  { "diagnose"      , A_DIAGNOSE     },
  { "dig"           , A_DIG          },
  { "discard"       , A_DROP         },
  { "disembark"     , A_DISEMBARK    },
  { "dislodge"      , A_MOVE         },
  { "dispatch"      , A_ATTACK       },
  { "dive"          , A_JUMP         },
  { "donate"        , A_GIVE         },
  { "down"          , A_DOWN         },
  { "douse"         , A_DEACTIVATE   },
  { "drink"         , A_DRINK        },
  { "drop"          , A_DROP         },
  { "e"             , A_EAST         },
  { "east"          , A_EAST         },
  { "eat"           , A_EAT          },
  { "echo"          , A_ECHO         },
  { "empty"         , A_EMPTY        },
  { "enter"         , A_ENTER        },
  { "examine"       , A_EXAMINE      },
  { "excavate"      , A_DIG          },
  { "exit"          , A_EXIT         },
  { "exorcise"      , A_EXORCISE     },
  { "extinguish"    , A_DEACTIVATE   },
  { "fasten"        , A_TIE          },
  { "feed"          , A_GIVE         },
  { "feel"          , A_TOUCH        },
  { "fight"         , A_ATTACK       },
  { "fill"          , A_FILL         },
  { "find"          , A_WHEREIS      },
  { "fix"           , A_FIX          },
  { "flick"         , A_ACTIVATE     },
  { "flip"          , A_ACTIVATE     },
  { "ford"          , A_CROSS        },
  { "free"          , A_UNTIE        },
  { "gaze"          , A_LOOK         },
  { "get"           , A_TAKE         },
  { "give"          , A_GIVE         },
  { "glue"          , A_FIX          },
  { "go"            , A_GO           },
  { "grab"          , A_TAKE         },
  { "grease"        , A_OIL          },
  { "greet"         , A_GREET        },
  { "head"          , A_GO           },
  { "hello"         , A_GREET        },
  { "hide"          , A_PUT          },
  { "hit"           , A_ATTACK       },
  { "hold"          , A_TAKE         },
  { "hurl"          , A_THROW        },
  { "hurt"          , A_ATTACK       },
  { "i"             , A_INVENTORY    },
  { "ignite"        , A_ACTIVATE     },
  { "imbibe"        , A_DRINK        },
  { "in"            , A_IN           },
  { "incinerate"    , A_ACTIVATE     },
  { "inflate"       , A_INFLATE      },
  { "injure"        , A_ATTACK       },
  { "insert"        , A_PUT          },
  { "inspect"       , A_EXAMINE      },
  { "inventory"     , A_INVENTORY    },
  { "journey"       , A_GO           },
  { "jump"          , A_JUMP         },
  { "kill"          , A_ATTACK       },
  { "knock"         , A_KNOCK        },
  { "l"             , A_LOOK         },
  { "land"          , A_LAND         },
  { "launch"        , A_LAUNCH       },
  { "leap"          , A_JUMP         },
  { "leave"         , A_EXIT         },
  { "lift"          , A_RAISE        },
  { "light"         , A_ACTIVATE     },
  { "locate"        , A_WHEREIS      },
  { "lock"          , A_LOCK         },
  { "look"          , A_LOOK         },
  { "loosen"        , A_MOVE         },
  { "lower"         , A_LOWER        },
  { "lubricate"     , A_OIL          },
  { "mount"         , A_MOUNT        },
  { "move"          , A_MOVE         },
  { "murder"        , A_ATTACK       },
  { "n"             , A_NORTH        },
  { "ne"            , A_NORTHEAST    },
  { "north"         , A_NORTH        },
  { "northeast"     , A_NORTHEAST    },
  { "northwest"     , A_NORTHWEST    },
  { "nw"            , A_NORTHWEST    },
  { "obtain"        , A_TAKE         },
  { "odysseus"      , A_ODYSSEUS     },
  { "offer"         , A_GIVE         },
  { "oil"           , A_OIL          },
  { "open"          , A_OPEN         },
  { "out"           , A_OUT          },
  { "pat"           , A_TOUCH        },
  { "patch"         , A_FIX          },
  { "peal"          , A_RING         },
  { "pet"           , A_TOUCH        },
  { "place"         , A_PUT          },
  { "play"          , A_PLAY         },
  { "plug"          , A_FIX          },
  { "pour"          , A_POUR         },
  { "pray"          , A_PRAY         },
  { "press"         , A_PUSH         },
  { "proceed"       , A_GO           },
  { "pry"           , A_PRY          },
  { "pull"          , A_PULL         },
  { "pump"          , A_INFLATE      },
  { "push"          , A_PUSH         },
  { "put"           , A_PUT          },
  { "q"             , A_QUIT         },
  { "quit"          , A_QUIT         },
  { "raise"         , A_RAISE        },
  { "read"          , A_READ         },
  { "release"       , A_UNTIE        },
  { "remove"        , A_REMOVE       },
  { "repair"        , A_FIX          },
  { "restart"       , A_RESTART      },
  { "restore"       , A_RESTORE      },
  { "ring"          , A_RING         },
  { "rotate"        , A_TURN         },
  { "rub"           , A_TOUCH        },
  { "run"           , A_GO           },
  { "s"             , A_SOUTH        },
  { "save"          , A_SAVE         },
  { "say"           , A_SAY          },
  { "score"         , A_SCORE        },
  { "se"            , A_SOUTHEAST    },
  { "secure"        , A_TIE          },
  { "seek"          , A_WHEREIS      },
  { "set"           , A_PUT          },
  { "shut"          , A_CLOSE        },
  { "skim"          , A_READ         },
  { "slay"          , A_ATTACK       },
  { "sleep"         , A_SLEEP        },
  { "smash"         , A_BREAK        },
  { "smell"         , A_SMELL        },
  { "sniff"         , A_SMELL        },
  { "south"         , A_SOUTH        },
  { "southeast"     , A_SOUTHEAST    },
  { "southwest"     , A_SOUTHWEST    },
  { "spill"         , A_POUR         },
  { "spin"          , A_TURN         },
  { "squeeze"       , A_SQUEEZE      },
  { "stab"          , A_ATTACK       },
  { "stand"         , A_DISEMBARK    },
  { "stare"         , A_LOOK         },
  { "step"          , A_GO           },
  { "stop"          , A_FIX          },
  { "strike"        , A_ATTACK       },
  { "stuff"         , A_PUT          },
  { "superbrief"    , A_SUPERBRIEF   },
  { "sw"            , A_SOUTHWEST    },
  { "swallow"       , A_DRINK        },
  { "swim"          , A_SWIM         },
  { "swing"         , A_WAVE         },
  { "take"          , A_TAKE         },
  { "taste"         , A_EAT          },
  { "tell"          , A_TALKTO       },
  { "throw"         , A_THROW        },
  { "thrust"        , A_WAVE         },
  { "tie"           , A_TIE          },
  { "toss"          , A_THROW        },
  { "touch"         , A_TOUCH        },
  { "travel"        , A_GO           },
  { "turn"          , A_TURN         },
  { "u"             , A_UP           },
  { "ulysses"       , A_ODYSSEUS     },
  { "unattach"      , A_UNTIE        },
  { "unfasten"      , A_UNTIE        },
  { "unhook"        , A_UNTIE        },
  { "unlock"        , A_UNLOCK       },
  { "untie"         , A_UNTIE        },
  { "up"            , A_UP           },
  { "verbose"       , A_VERBOSE      },
  { "version"       , A_VERSION      },
  { "w"             , A_WEST         },
  { "wade"          , A_SWIM         },
  { "wait"          , A_WAIT         },
  { "walk"          , A_GO           },
  { "wave"          , A_WAVE         },
  { "wear"          , A_WEAR         },
  { "west"          , A_WEST         },
  { "what"          , A_EXAMINE      },
  { "what's"        , A_EXAMINE      },
  { "whats"         , A_EXAMINE      },
  { "where's"       , A_WHEREIS      },
  { "wheres"        , A_WHEREIS      },
  { "who"           , A_EXAMINE      },
  { "who's"         , A_EXAMINE      },
  { "whos"          , A_EXAMINE      },
  { "wind"          , A_WIND         },
  { "x"             , A_EXAMINE      },
  { "z"             , A_WAIT         },

  {0, 0}
};

