#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "almazar.h"

// defined in almazar.c
void extra_newline(void);
void putsn(char* s);
int ran(int range);
int lookup_verb(char *word);
int lookup_noun(char *word);
bool is_transitive(int v);
void listen(int *iverb, int *iobj);
bool yes(char *q);
bool vault_challenge(void);
void describe_object(int obj);
bool has_darkness(int loc);
bool has_river(int loc);
void describe_room(int loc);
void offer_hint(char *hint);
int burden(void);
void describe_inventory_object(int x);
void final_score_and_exit(bool winning);
int random_death_drop_room(void);
int death(void);
bool is_portable(int obj);
int random_gnome_room(void);
int deal_with_gnomes(int loc);
int deal_with_room(int loc);

extern int iplace;

extern struct Place places[R_VAULT+1];

extern int object_position[OPEN_PANEL + 1];  // OP in the original program
extern int lamp_counter;                 // SP1 in the original program
extern bool lamp_is_on;              // SP2 in the original program
extern bool mouse_is_fed;            // SP3 in the original program
// SP4 in the original program was never used
extern bool wizard_is_dead;          // SP5 in the original program
extern int wizard_counter;               // SP6 in the original program
extern int vault_status;  // SP7 in the original program
extern bool river_is_open;           // SP8 in the original program
extern bool chasm_is_bridged;        // SP9 in the original program
// SP10 in the original program was window_is_open, but we don't need it
extern int drawer_status; // SP11 in the original program
extern int chest_status;  // SP12 in the original program
extern int darkness_counter;             // SP13 in the original program
extern int river_counter;                // SP14 in the original program
extern bool chased_by_gnome;         // SC in the original program
extern bool heard_voiceover;         // VO in the original program
extern bool lamp_has_dimmed;         // W1 in the original program
extern bool better_hurry;            // W2 in the original program
extern int hint_penalties;               // PR in the original program
extern int turn_counter;                 // TU in the original program
extern int death_counter;                // DI in the original program

int attempt_get(int iobj, int loc)
{
    if (iobj == EYE && there(EYE_IN_PLACE, loc)) {
        putsn("The god is incensed by your audacity. With a blink of an eye" SOFT_NL
              "you fall dead to the ground.");
        return death();
    } else if (iobj == IDOL && loc == R_IDOL) {
        putsn("It's too heavy for you to take.");
    } else if (iobj == CHEST && loc == R_ATTIC) {
        putsn("It's too heavy for you to take.");
    } else if (iobj == BALL && loc == R_TREE) {
        putsn("It's too heavy for you to take.");
    } else if (iobj == BOOK && loc == R_LIBRARY) {
        putsn("It's too heavy for you to take.");
    } else if (iobj == APPLE && there(SNAKE, loc)) {
        putsn("The snake blocks your way.");
    } else if (iobj == CHAIN && there(WIZARD, loc)) {
        putsn("The wizard won't let you.");
    } else if (iobj == MOUSE && there(MOUSE, loc) && !mouse_is_fed) {
        putsn("The mouse is frightened by you.");
    } else if (iobj == SPICES && there(SPICES_IN_PLACE, loc)) {
        if (toting(WATER_BOTTLE)) {
            putsn("Your container is full.");
        } else if (!toting(BOTTLE)) {
            putsn("You have no container to put it in.");
        } else {
            putsn("OK.");
            apport(SPICES_IN_PLACE, R_LIMBO);
            apport(BOTTLE, R_LIMBO);
            apport(SPICES, -1);
        }
    } else if (iobj == WATER && has_river(loc) && !there(WATER_BOTTLE, loc)) {
        if (toting(WATER_BOTTLE) || toting(SPICES)) {
            putsn("Your container is full.");
        } else if (!toting(BOTTLE)) {
            putsn("You have no container to put it in.");
        } else {
            putsn("OK.");
            apport(BOTTLE, R_LIMBO);
            apport(WATER_BOTTLE, -1);
        }
    } else if (iobj == OIL) {
        if (loc != R_OIL) {
            putsn("There is no sign of any oil here.");
        } else if (!toting(LAMP)) {
            putsn("You don't have a lamp.");
        } else {
            putsn("OK.");
            lamp_counter = 100;
        }
    } else if (iobj == FIGURINE && there(FIGURINE_IN_PLACE, loc)) {
        if (burden() >= 7) {
            putsn("You've got too much to carry.");
        } else {
            putsn("OK.");
            apport(FIGURINE_IN_PLACE, R_LIMBO);
            apport(FIGURINE, -1);
        }
    } else {
        if (iobj == BOTTLE && (there(SPICES, loc) || toting(SPICES))) {
            iobj = SPICES;
        } else if (iobj == BOTTLE && (there(WATER_BOTTLE, loc) || toting(WATER_BOTTLE))) {
           iobj = WATER_BOTTLE;
        } else if (iobj == WATER && (there(WATER_BOTTLE, loc) || toting(WATER_BOTTLE))) {
           iobj = WATER_BOTTLE;
        }

        if (!is_portable(iobj)) {
            putsn("I don't understand.");
        } else if (toting(iobj)) {
            putsn("You're already carrying it.");
        } else if (!there(iobj, loc)) {
            putsn("I don't see it here.");
        } else if (burden() >= 7) {
            putsn("You've got too much to carry.");
        } else {
            putsn("OK.");
            apport(iobj, -1);
        }
    }
    return loc;
}

void attempt_drop(int iobj, int loc)
{
    if ((iobj == BOTTLE || iobj == WATER) && toting(WATER_BOTTLE)) {
        putsn("OK.");
        apport(WATER_BOTTLE, loc);
    } else if ((iobj == BOTTLE || iobj == SPICES) && toting(SPICES)) {
        putsn("OK.");
        apport(SPICES, loc);
    } else if (!is_portable(iobj)) {
        putsn("I don't understand.");
    } else if (!toting(iobj)) {
        putsn("You're not carrying it.");
    } else if (iobj == FIGURINE) {
        if (there(HAY, loc)) {
            putsn("OK.");
            apport(FIGURINE, loc);
        } else {
            putsn("The delicate figurine breaks upon hitting the ground.");
            apport(FIGURINE, R_LIMBO);
        }
    } else if (iobj == MOUSE) {
        putsn("The mouse runs away.");
        apport(MOUSE, R_LIMBO);
        mouse_is_fed = false;
    } else {
        putsn("OK.");
        apport(iobj, loc);
    }
}

void attempt_quit(void)
{
    if (yes("Are you sure you want to quit?")) {
        final_score_and_exit(false);
    } else {
        putsn("OK.");
    }
}

void attempt_inventory(void)
{
    int x;

    puts("You are carrying:");
    for (x = DIAMOND; x <= WATER_BOTTLE; ++x) {
        if (toting(x)) {
            describe_inventory_object(x);
        }
    }
    if (burden() == 0) {
        puts("    Nothing");
    }
    extra_newline();
}

int attempt_score(bool winning)
{
    int pt = 0;
    int x;

    for (x = DIAMOND; x <= FIGURINE; ++x) {
        if (toting(x)) pt += 7;
        else if (there(x, R_VAULT)) pt += 10;
    }
    pt -= 10 * death_counter;
    pt -= hint_penalties;
    if (winning) pt += 10;
    printf("You have scored %d points.\n", pt);
    extra_newline();
    return pt;
}

void attempt_hint(int loc)
{
    switch (loc) {
        case R_STABLE: offer_hint("Hay is soft."); break;
        case R_RECEPTION: offer_hint("The figurine is extremely delicate."); break;
        case R_ORCHARD: offer_hint("Snakes love rodents."); break;
        case R_BASEMENT: offer_hint("Only a power stronger than the wizard's own can defeat him."); break;
        case R_OUTHOUSE: offer_hint("Complete your task, and return here, and..."); break;
        case R_ATTIC: offer_hint("The hungry are always restless."); break;
        case R_IDOL: offer_hint("Gods are unhappy creatures with many needs."); break;
        case R_RIVER: offer_hint("The river flows toward the Red Sea."); break;
        default: putsn("There is no help to be obtained."); break;
    }
}

int attempt_move_to_room(int loc, int nextloc)
{
    if (!river_is_open && ((loc == R_RIVER && nextloc == R_WELCOME) ||
                           (loc == R_WELCOME && nextloc == R_RIVER))) {
        putsn("You cannot cross the river.");
        return loc;
    } else if (!chasm_is_bridged && ((loc == R_CHASM && nextloc == R_LEDGE) ||
                                     (loc == R_LEDGE && nextloc == R_CHASM))) {
        putsn("The abyss is too wide to cross.");
        return loc;
    } else if (loc == R_BASEMENT && nextloc != R_BASEMENT && !wizard_is_dead) {
        putsn("The wizard blocks your way.");
        return loc;
    } else if (loc == R_CLOSET && nextloc == R_LIFT && !wizard_is_dead) {
        putsn("There is no way to go in that direction.");
        return loc;
    }

    loc = nextloc;

    describe_room(loc);
    return loc;
}

void attempt_wave(int iobj, int loc)
{
    if (iobj == STAFF && toting(STAFF) && (loc == R_RIVER || loc == R_WELCOME)) {
        if (river_is_open) {
            putsn("A loud noise surrounds you as the river closes.");
            river_is_open = false;
        } else {
            putsn("Holy Moses! The river just split in two! A dry path leads to" SOFT_NL
                  "the other side.");
            river_is_open = true;
        }
    } else if (iobj == ROD && toting(ROD) && (loc == R_CHASM || loc == R_LEDGE)) {
        if (chasm_is_bridged) {
            putsn("The bridge has just as suddenly disappeared.");
            chasm_is_bridged = false;
        } else {
            putsn("A crystal bridge now spans the chasm.");
            chasm_is_bridged = true;
        }
    } else {
        print_default_message(iobj);
    }
}

void attempt_show(int iobj, int loc)
{
    if (iobj == RING && toting(RING) && loc == R_BASEMENT && !wizard_is_dead) {
        putsn("The ring glows brightly. A lightning bolt strikes the wizard!");
        putsn("A platinum chain lies before you.");
        wizard_is_dead = true;
        apport(CHAIN, loc);
        apport(WIZARD, R_LIMBO);
    } else {
        print_default_message(iobj);
    }
}

int attempt_read(int iobj, int loc)
{
    int x;

    if (iobj != MESSAGE && iobj != SIGN && iobj != BOOK) {
        putsn("I don't understand.");
    } else if (loc == R_HALL2 && iobj == MESSAGE) {
        putsn("\"Merlin was here.\"");
    } else if (loc == R_TREE && iobj == MESSAGE) {
        putsn("\"The great Almazar bids you well. Though you will encounter many" SOFT_NL
              "trials, he shall provide for you. He that is both water and flame" SOFT_NL
              "shall send you a gift to aid you in your quest. Thus saith" SOFT_NL
              "Almazar: 'Live, and you shall live.'\"");
    } else if (loc == R_RIVER && iobj == SIGN) {
        putsn("\"Fish from the Amazon.\"");
    } else if (loc == R_WELCOME && (iobj == MESSAGE || iobj == SIGN)) {
        // The original game didn't accept "READ SIGN" here, only "READ MESSAGE".
        putsn("\"Discretion will prevent transgression.\"");
    } else if (loc == R_LIBRARY && iobj == BOOK) {
        if (there(OPEN_WINDOW, loc)) {
            putsn("A small axe flies through the window and strikes you dead.");
            return death();
        } else {
            putsn("\"And so it came to pass that in the third day of the seventh" SOFT_NL
                  "month of the twenty-first year a great event happened. The earth" SOFT_NL
                  "opened and took within it the fruits of the people's labor and" SOFT_NL
                  "gave upon them a greater glory.\"");
        }
    } else if (loc == R_OUTHOUSE && iobj == MESSAGE) {
        bool collected_all_the_treasures = true;
        for (x = DIAMOND; x <= FIGURINE; ++x) {
            if (!there(x, R_VAULT)) collected_all_the_treasures = false;
        }
        if (collected_all_the_treasures && toting(RING)) {
            putsn("In a blaze of glory you find yourself in a land far away!");
            final_score_and_exit(true);
        } else {
            putsn("\"Frodo lives!\"");
        }
    } else {
        putsn("I don't see it here.");
    }
    return loc;
}

void attempt_fill(int iobj, int loc)
{
    if (iobj == LAMP) {
        if (!toting(LAMP)) {
            putsn("You don't have a lamp.");
        } else if (loc != R_OIL) {
            putsn("There is nothing to fill it with.");
        } else {
            attempt_get(OIL, loc);
        }
    } else if (iobj == BOTTLE) {
        if (!toting(SPICES) && !toting(BOTTLE) && !toting(WATER_BOTTLE)) {
            putsn("You're not carrying a bottle.");
        } else if (!has_river(loc)) {
            putsn("There is nothing to fill it with.");
        } else {
            attempt_get(WATER, loc);
        }
    } else {
        putsn("I don't understand.");
    }
}

void attempt_light(int iobj, int loc)
{
    if (iobj == LAMP) {
        if (!toting(LAMP)) {
            putsn("You're not carrying it.");
        } else if (lamp_is_on) {
            putsn("The lamp is already lit.");
        } else if (lamp_counter <= 0) {
            putsn("You cannot light an empty oil lamp.");
        } else if (!toting(MATCHES)) {
            putsn("You don't have any matches.");
        } else {
            putsn("OK.");
            lamp_is_on = true;
            darkness_counter = 0;
        }
    } else if (iobj == INCENSE) {
        if (!toting(INCENSE)) {
            putsn("You're not carrying it.");
        } else if (!toting(MATCHES)) {
            putsn("You don't have any matches.");
        } else {
            apport(INCENSE, R_LIMBO);
            if (loc == R_IDOL) {
                putsn("An emerald eye falls to the ground.");
                apport(EYE_IN_PLACE, R_LIMBO);
                apport(EYE, loc);
            } else {
                putsn("OK.");
            }
        }
    } else {
        putsn("I don't understand.");
    }
}

int attempt_kill(int iobj, int loc)
{
    if (iobj == SNAKE || iobj == WIZARD || iobj == GNOME || iobj == BALL) {
        if (!there(iobj, loc)) {
            putsn("I don't see it here.");
        } else if (!yes("With what, your bare hands?")) {
            putsn("Nothing happens.");
        } else if (iobj == SNAKE) {
            putsn("The snake severs your hand. You have been poisoned.");
            return death();
        } else if (iobj == WIZARD) {
            putsn("The wizard raises his hand and in an instant you find yourself" SOFT_NL
                  "uncomfortably hot. As you look up into the sky you see a large" SOFT_NL
                  "stone idol. You are being offered as a sacrifice.");
            return death();
        } else if (iobj == GNOME) {
            putsn("The gnome is too fast for you.");
        } else {
            putsn("Eech! Your hands just turned into a bloody mess.");
        }
    } else {
        putsn("I don't understand.");
    }
    return loc;
}

int attempt_kick(int iobj, int loc)
{
    if (iobj == BALL) {
        if (there(BALL, loc)) {
            putsn("Ouch! Every bone in your foot just broke.");
        } else {
            putsn("I don't see it here.");
        }
    } else if (iobj == WIZARD) {
        putsn("The wizard raises his hand and in an instant you find yourself" SOFT_NL
              "uncomfortably hot. As you look up into the sky you see a large" SOFT_NL
              "stone idol. You are being offered as a sacrifice.");
        wizard_counter = 0;
        return death();
    } else if (!is_portable(iobj)) {
        putsn("I don't understand.");
    } else if (iobj == FIGURINE && toting(FIGURINE)) {
        // The original game allowed KICK FIGURINE to safely drop the figurine sans hay.
        putsn("The delicate figurine breaks upon hitting the ground.");
        apport(FIGURINE, R_LIMBO);
    } else {
        if (toting(iobj)) {
            apport(iobj, loc);
            putsn("OK.");
        } else if (there(iobj, loc)) {
            putsn("You're not carrying it.");
        } else {
            putsn("I don't see it here.");
        }
    }
    return loc;
}

void attempt_break(int iobj, int loc)
{
    if (iobj == BALL) {
        if (there(BALL, loc)) {
            attempt_kill(BALL, loc);
        } else {
            putsn("I don't see it here.");
        }
    } else if (iobj == FIGURINE && toting(FIGURINE)) {
        putsn("The delicate figurine breaks upon hitting the ground.");
        apport(FIGURINE, R_LIMBO);
    } else if (iobj == BOTTLE && toting(BOTTLE)) {
        putsn("The bottle breaks on impact.");
        apport(BOTTLE, R_LIMBO);
    } else if (iobj == BOTTLE && toting(WATER_BOTTLE)) {
        putsn("The bottle breaks on impact.");
        apport(WATER_BOTTLE, R_LIMBO);
    } else if ((iobj == BOTTLE || iobj == SPICES) && toting(SPICES)) {
        putsn("The bottle breaks on impact.");
        putsn("A sudden wind blows from behind and scatters the spices to the" SOFT_NL
              "four corners of the earth.");
        apport(SPICES, R_LIMBO);
    } else {
        print_default_message(iobj);
    }
}

void attempt_toss(int iobj, int loc)
{
    int p;

    if (iobj == BOTTLE || iobj == SPICES) {
        attempt_break(iobj, loc);
    } else if (iobj == DAGGER && toting(DAGGER) && there(GNOME, loc)) {
        p = ran(100) - burden();
        if (p > 40) {
            putsn("You killed a nasty knife-throwing gnome!");
            apport(GNOME, R_LIMBO);
        } else {
            putsn("You missed! You ought to get your eyes examined.");
        }
        apport(DAGGER, loc);
    } else {
        attempt_drop(iobj, loc);
    }
}

int attempt_eat(int iobj, int loc)
{
    if (iobj == MOUSE && toting(MOUSE)) {
        putsn("Yuck! The mouse was used for carcinogenic testing. You're dead.");
        apport(MOUSE, R_LIMBO);
        return death();
    }

    if (iobj == SPICES && (there(SPICES, loc) || toting(SPICES))) {
        putsn("Whew! That's strong stuff!");
        apport(BOTTLE, object_position[SPICES]);
        apport(SPICES, R_LIMBO);
    } else if (iobj == SPICES && there(SPICES_IN_PLACE, loc)) {
        putsn("Whew! That's strong stuff!");
        apport(SPICES_IN_PLACE, R_LIMBO);
    } else if (iobj == CHEESE && (there(CHEESE, loc) || toting(CHEESE))) {
        putsn("The cheese was stale, but you managed to consume the entire box.");
        apport(CHEESE, R_LIMBO);
    } else if (iobj == MOUSE && there(MOUSE, loc)) {
        putsn("The mouse runs away.");
        apport(MOUSE, R_LIMBO);
    } else {
        putsn("The only thing edible here is you!");
    }
    return loc;
}

void attempt_drink(int iobj, int loc)
{
    if (iobj == WATER) {
        if (toting(WATER_BOTTLE)) {
            putsn("Eech! What lousy-tasting water.");
            apport(WATER_BOTTLE, R_LIMBO);
            apport(BOTTLE, -1);
        } else if (has_river(loc)) {
            putsn("Eech! What lousy-tasting water.");
        } else {
            putsn("There's no water within reach.");
        }
    } else {
        putsn("I don't understand.");
    }
}

void attempt_feed(int iobj, int loc)
{
    if (iobj == MOUSE) {
        if (!there(MOUSE, loc)) {
            putsn("I don't see it here.");
        } else if (!toting(CHEESE)) {
            putsn("The only thing edible here is you!");
        } else {
            putsn("OK.");
            mouse_is_fed = true;
        }
    } else if (iobj == SNAKE) {
        if (!there(SNAKE, loc)) {
            puts("I don't see it here.");
        } else if (!toting(MOUSE)) {
            puts("The only thing edible here is you!");
        } else {
            putsn("The mouse was used for carcinogenic testing. The snake is dead.");
            putsn("A golden apple hangs by an apple tree.");
            apport(MOUSE, R_LIMBO);
            apport(SNAKE, R_LIMBO);
            apport(APPLE, loc);
        }
    } else {
        putsn("I don't understand.");
    }
}

void attempt_turn(int iobj, int loc)
{
    if (iobj == HEAD && loc == R_LIVING) {
        if (there(OPEN_PANEL, loc)) {
            putsn("The panel closes and seals the passage.");
            apport(OPEN_PANEL, R_LIMBO);
        } else {
            putsn("A panel slides open, revealing a passage to the north.");
            apport(OPEN_PANEL, loc);
        }
    } else {
        print_default_message(iobj);
    }
}

void attempt_unlock(int iobj, int loc)
{
    // Notice that unlocking an open container makes it no longer open.
    if (iobj == VAULT) {
        if (loc != R_OUTSIDE) {
            putsn("I don't see it here.");
        } else if (!vault_challenge()) {
            putsn("That combination is incorrect.");
        } else {
            putsn("You have managed to unlock the vault.");
            vault_status = STATUS_UNLOCKED;
        }
    } else if (iobj == CHEST) {
        if (loc != R_ATTIC) {
            putsn("I don't see it here.");
        } else if (!toting(KEY)) {
            putsn("You have no keys.");
        } else {
            putsn("OK.");
            chest_status = STATUS_UNLOCKED;
        }
    } else if (iobj == DRAWER) {
        if (loc != R_BEDROOM) {
            putsn("I don't see it here.");
        } else if (!toting(KEY)) {
            putsn("You have no keys.");
        } else {
            putsn("OK.");
            drawer_status = STATUS_UNLOCKED;
        }
    } else {
        puts("I don't understand.");
    }
}

int attempt_jump(int loc)
{
    if (loc == R_CHASM || loc == R_LEDGE) {
        putsn("You fell into the chasm. You are dead.");
        return death();
    } else if (loc == R_HILL) {
        putsn("That jump would have won a gold medal. Unfortunately, you landed" SOFT_NL
              "head-first on a pile of rocks.");
        return death();
    } else if (has_river(loc)) {
        // It doesn't matter whether the river is open.
        putsn("You did not jump far enough. You fell into the river.");
        putsn("A school of piranhas devours you.");
        return death();
    } else {
        putsn("Nothing happens.");
        return loc;
    }
}

int attempt_cross(int iobj, int loc)
{
    if (iobj == RIVER && (loc == R_RIVER || loc == R_WELCOME)) {
        if (river_is_open) {
            return attempt_move_to_room(loc, (loc == R_RIVER) ? R_WELCOME : R_RIVER);
        } else {
            putsn("You can't cross the river.");
        }
    } else if (iobj == CHASM && (loc == R_CHASM || loc == R_LEDGE)) {
        if (chasm_is_bridged) {
            return attempt_move_to_room(loc, (loc == R_CHASM) ? R_LEDGE : R_CHASM);
        } else {
            putsn("The abyss is too wide to cross.");
        }
    } else if (iobj == RIVER || iobj == CHASM) {
        putsn("I don't see it here.");
    } else {
        putsn("I don't understand.");
    }
    return loc;
}

int attempt_swim(int iobj, int loc)
{
    if (iobj == RIVER || iobj == WATER) {
        if (has_river(loc)) {
            putsn("A school of piranhas devours you.");
            return death();
        } else {
            putsn("There's no water within reach.");
        }
    } else {
        putsn("I don't understand.");
    }
    return loc;
}

void attempt_open(int iobj, int loc)
{
    if (iobj == VAULT) {
        if (loc != R_OUTSIDE) {
            putsn("I don't see it here.");
        } else if (vault_status == STATUS_OPEN) {
            putsn("It's already open.");
        } else if (vault_status == STATUS_LOCKED) {
            putsn("The vault is locked.");
        } else {
            putsn("OK.");
            vault_status = STATUS_OPEN;
        }
    } else if (iobj == CHEST) {
        if (loc != R_ATTIC) {
            putsn("I don't see it here.");
        } else if (chest_status == STATUS_OPEN) {
            putsn("It's already open.");
        } else if (chest_status == STATUS_LOCKED) {
            putsn("It's locked.");
        } else {
            putsn("The chest is empty.");
            chest_status = STATUS_OPEN;
        }
    } else if (iobj == DRAWER) {
        if (loc != R_BEDROOM) {
            putsn("I don't see it here.");
        } else if (drawer_status == STATUS_OPEN) {
            putsn("It's already open.");
        } else if (drawer_status == STATUS_LOCKED) {
            putsn("It's locked.");
        } else {
            drawer_status = STATUS_OPEN;
            if (there(PEARLS, R_LIMBO)) {
                putsn("A string of pearls is lying in the drawer!");
                apport(PEARLS, loc);
            } else {
                putsn("OK.");
            }
        }
    } else if (iobj == WINDOW) {
        if (loc != R_LIBRARY) {
            putsn("I don't see it here.");
        } else if (there(OPEN_WINDOW, loc)) {
            putsn("It's already open.");
        } else {
            putsn("OK.");
            apport(OPEN_WINDOW, loc);
            apport(CLOSED_WINDOW, R_LIMBO);
        }
    } else {
        putsn("I don't understand.");
    }
}

void attempt_close(int iobj, int loc)
{
    if (iobj == VAULT) {
        if (loc != R_OUTSIDE) {
            putsn("I don't see it here.");
        } else if (vault_status == STATUS_LOCKED) {
            putsn("It's already closed.");
        } else {
            putsn("OK.");
            vault_status = STATUS_LOCKED;
        }
    } else if (iobj == CHEST) {
        if (loc != R_ATTIC) {
            putsn("I don't see it here.");
        } else if (chest_status == STATUS_LOCKED) {
            putsn("It's already closed.");
        } else {
            putsn("OK.");
            chest_status = STATUS_LOCKED;
        }
    } else if (iobj == DRAWER) {
        if (loc != R_BEDROOM) {
            putsn("I don't see it here.");
        } else if (drawer_status == STATUS_LOCKED) {
            putsn("It's already closed.");
        } else {
            putsn("OK.");
            drawer_status = STATUS_LOCKED;
        }
    } else if (iobj == WINDOW) {
        if (loc != R_LIBRARY) {
            putsn("I don't see it here.");
        } else if (there(CLOSED_WINDOW, loc)) {
            putsn("It's already closed.");
        } else {
            putsn("OK.");
            apport(CLOSED_WINDOW, loc);
            apport(OPEN_WINDOW, R_LIMBO);
        }
    } else {
        puts("I don't understand.");
    }
}

void attempt_off(int iobj)
{
    if (iobj != LAMP) {
        putsn("I don't understand.");
    } else if (!toting(LAMP)) {
        putsn("You're not carrying it.");
    } else if (!lamp_is_on) {
        putsn("Your lamp is already off.");
    } else {
        putsn("OK.");
        lamp_is_on = false;
    }
}

int attempt_pull(int iobj, int loc)
{
    if (iobj == ROPE && loc == R_HIDDEN) {
        putsn("A trap door opens beneath you. The fall knocks you unconscious. When" SOFT_NL
              "you awaken you find that...\n");
        return attempt_move_to_room(R_LIMBO, R_BASEMENT);
    } else {
        print_default_message(iobj);
        return loc;
    }
}

void attempt_empty(int iobj)
{
    if (iobj == BOTTLE) {
        if (toting(SPICES)) {
            putsn("A sudden wind blows from behind and scatters the spices to the" SOFT_NL
                  "four corners of the earth.");
            apport(SPICES, R_LIMBO);
            apport(BOTTLE, -1);
        } else if (toting(WATER_BOTTLE)) {
            putsn("OK.");
            apport(WATER_BOTTLE, R_LIMBO);
            apport(BOTTLE, -1);
        } else if (toting(BOTTLE)) {
            putsn("Your bottle does not contain anything.");
        } else {
            putsn("You're not carrying it.");
        }
    } else {
        print_default_message(iobj);
    }
}

int attempt_enter(int loc)
{
    int nextloc = loc;
    switch (loc) {
        case R_START: nextloc = R_SHACK; break;
        case R_CAVE: nextloc = R_FILTER; break;
        case R_MINE: nextloc = R_MINE33; break;
        case R_HOUSE: nextloc = R_FOYER; break;
        case R_OUTOUT: nextloc = R_OUTHOUSE; break;
        case R_OUTSIDE: {
            if (vault_status == STATUS_OPEN) {
                nextloc = R_VAULT;
            } else {
                putsn("You have to open the vault to get inside it.");
            }
            break;
        }
        default: {
            putsn("You can't enter anything from here.");
            break;
        }
    }
    if (nextloc != loc) {
        return attempt_move_to_room(loc, nextloc);
    } else {
        return loc;
    }
}

int attempt_leave(int loc) 
{
    int nextloc = loc;
    switch (loc) {
        case R_SHACK: nextloc = R_START; break;
        case R_FILTER: nextloc = R_CAVE; break;
        case R_MINE33: nextloc = R_MINE; break;
        case R_FOYER: nextloc = R_HOUSE; break;
        case R_OUTHOUSE: nextloc = R_OUTOUT; break;
        case R_VAULT: nextloc = R_OUTSIDE; break;
        default: {
            putsn("You cannnot leave from here.");
            break;
        }
    }
    if (nextloc != loc) {
        return attempt_move_to_room(loc, nextloc);
    } else {
        return loc;
    }
}

