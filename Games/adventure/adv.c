//
//This is Daimler's 350-point "Adventure" (circa June 1990, according
//to Russel Dalenberg). Its version information lists
//    
//        -Conversion to BDS  C by J. R. Jaeger
//        -Unix standardization by Jerry D. Pohl.
//        -OS/2 Conversion by Martin Heller
//        -Conversion to TurboC 2.0 by Daimler
//    
//It contains Jerry Pohl's original ADVENT.DOC (dated 12 JUNE 1984),
//plus comments from Martin Heller (dated 30-Aug-1988). Strangely for
//an expansion, Daimler's version actually introduces a number of typos
//into the data files, and disables a handful of inessential verbs
//(READ, EAT, FILL) with the comment that there is "no room" for them
//(presumably in the PC's limited memory).
// -------------------------------------------------------------------
//
// Adapted for HiTech C Z80 under CP/M by Ladislau Szilagyi, Oct. 2023
// Uncommented Daimler's disabled verbs - game is complete again !
// Added a new pseudo-random number generator (Xorshift)
//
// Single C source version (2025) by Ladislau Szilagyi, June 2025
//
// Compile-it on CP/M using 
//   https://github.com/Laci1953/HiTech-C-compiler-enhanced
//

/*	program ADVENT.C					*\
\*	WARNING: "advent.c" allocates GLOBAL storage space by	*\
\*		including "advdef.h".				*\
\*		All other modules use "advdec.h"		*/


#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>

void xrndseed(void);
unsigned int xrnd(void);
int yes(int,int,int);
int stimer();


#define 	rindex(a,b)	strrchr(a,b)

//#define 	setmem(a,b,c)	memset(a,c,b)
#define 	setmem(a,b,c)	memset(a,b,c)	//HiTech LIBC bug !


#define	MAXOBJ	100		/* max # of objects in cave	*/
#define	MAXWC	301		/* max # of adventure words	*/
#define	MAXLOC	140		/* max # of cave locations	*/
#define	WORDSIZE	20	/* max # of chars in commands	*/
#define	MAXMSG	201		/* max # of long location descr	*/

#define	MAXTRAV	(16+1)	/* max # of travel directions from loc	*/
			/* +1 for terminator travel[x].tdest=-1	*/
#define	DWARFMAX	7	/* max # of nasty dwarves	*/
#define	MAXDIE	3		/* max # of deaths before close	*/
#define	MAXTRS	79		/* max # of			*/

/*
	Object definitions
*/
#define	KEYS	1
#define	LAMP	2
#define	GRATE	3
#define	CAGE	4
#define	ROD	5
#define	ROD2	6
#define	STEPS	7
#define	BIRD	8
#define	DOOR	9
#define	PILLOW	10
#define	SNAKE	11
#define	FISSURE	12
#define	TABLET	13
#define	CLAM	14
#define	OYSTER	15
#define	MAGAZINE	16
#define	DWARF	17
#define	KNIFE	18
#define	FOOD	19
#define	BOTTLE	20
#define	WATER	21
#define	OIL	22
#define	MIRROR	23
#define	PLANT	24
#define	PLANT2	25
#define	AXE	28
#define	DRAGON	31
#define	CHASM	32
#define	TROLL	33
#define	TROLL2	34
#define	BEAR	35
#define	MESSAGE	36
#define	VEND	38
#define	BATTERIES	39
#define	NUGGET	50
#define	COINS	54
#define	CHEST	55
#define	EGGS	56
#define	TRIDENT	57
#define	VASE	58
#define	EMERALD	59
#define	PYRAMID	60
#define	PEARL	61
#define	RUG	62
#define	SPICES	63
#define	CHAIN	64

/*
	Verb definitions
*/
#define	NULLX	21
#define	BACK	8
#define	LOOK	57
#define	CAVE	67
#define	ENTRANCE	64
#define	DEPRESSION	63

/*
	Action verb definitions
*/
#define	TAKE	1
#define	DROP	2
#define	SAY	3
#define	OPEN	4
#define	NOTHING	5
#define	LOCK	6
#define	ON	7
#define	OFF	8
#define	WAVE	9
#define	CALM	10
#define	WALK	11
#define	KILL	12
#define	POUR	13
#define	EAT	14
#define	DRINK	15
#define	RUB	16
#define	THROW	17
#define	QUIT	18
#define	FIND	19
#define	INVENTORY	20
#define	FEED	21
#define	FILL	22
#define	BLAST	23
#define	SCORE	24
#define	FOO	25
#define	BRIEF	26
#define	READ	27
#define	BREAK	28
#define	WAKE	29
#define	SUSPEND	30
#define	HOURS	31
#define	LOG	32

/*
	BIT mapping of "cond" array which indicates location status
*/
#define	LIGHT	1
#define	WATOIL	2
#define	LIQUID	4
#define	NOPIRAT	8
#define	HINTC	16
#define	HINTB	32
#define	HINTS	64
#define	HINTM	128
#define	HINT	240

/*
	Structure definitions
*/
struct	wac {
	char	*aword;
	int	acode;
	};

struct	trav {
	int	tdest;
	int	tverb;
	int	tcond;
	};

/*	WARNING: the travel array for the cave is stored as MAXLOC
	         strings.  the strings are an array of 1..MAXTRAV
	         LONG INTEGERS.  this requires 32 bit LONG INTEGERS.
	         these values are used in database.c "gettrav".
	         tdset*1000000 + tverb*1000 + tcond = value stored
*/

char	*cave[] =
	{
	"2002000,2044000,2029000,3003000,3012000,3019000,3043000,4005000,4013000,4014000,4046000,4030000,5006000,5045000,5043000,8063000,",
	"1002000,1012000,1007000,1043000,1045000,1030000,5006000,5045000,5046000,",
	"1003000,1011000,1032000,1044000,11062000,33065000,79005000,79014000,",
	"1004000,1012000,1045000,5006000,5043000,5044000,5029000,7005000,7046000,7030000,8063000,",
	"4009000,4043000,4030000,5006050,5007050,5045050,6006000,5044000,5046000,",
	"1002000,1045000,4009000,4043000,4044000,4030000,5006000,5046000,",
	"1012000,4004000,4045000,5006000,5043000,5044000,8005000,8015000,8016000,8046000,595060000,595014000,595030000,",
	"5006000,5043000,5046000,5044000,1012000,7004000,7013000,7045000,9003303,9019303,9030303,593003000,",
	"8011303,8029303,593011000,10017000,10018000,10019000,10044000,14031000,11051000,",
	"9011000,9020000,9021000,9043000,11019000,11022000,11044000,11051000,14031000,",
	"8063303,9064000,10017000,10018000,10023000,10024000,10043000,12025000,12019000,12029000,12044000,3062000,14031000,",
	"8063303,9064000,11030000,11043000,11051000,13019000,13029000,13044000,14031000,",
	"8063303,9064000,11051000,12025000,12043000,14023000,14031000,14044000,",
	"8063303,9064000,11051000,13023000,13043000,20030150,20031150,20034150,15030000,16033000,16044000,",
	"18036000,18046000,17007000,17038000,17044000,19010000,19030000,19045000,22029150,22031150,22034150,22035150,22023150,22043150,14029000,34055000,",
	"14001000,",
	"15038000,15043000,596039312,21007412,597041412,597042412,597044412,597069412,27041000,",
	"15038000,15011000,15045000,",
	"15010000,15029000,15043000,28045311,28036311,29046311,29037311,30044311,30007311,32045000,74049035,32049211,74066000,",
	"001000,",
	"001000,",
	"15001000,",
	"67043000,67042000,68044000,68061000,25030000,25031000,648052000,",
	"67029000,67011000,",
	"23029000,23011000,31056724,26056000,",
	"88001000,",
	"596039312,21007412,597041412,597042412,597043412,597069412,17041000,40045000,41044000,",
	"19038000,19011000,19046000,33045000,33055000,36030000,36052000,",
	"19038000,19011000,19045000,",
	"19038000,19011000,19043000,62044000,62029000,",
	"89001524,90001000,",
	"19001000,",
	"3065000,28046000,34043000,34053000,34054000,35044000,302071159,100071000,",
	"33030000,33055000,15029000,",
	"33043000,33055000,20039000,",
	"37043000,37017000,28029000,28052000,39044000,65070000,",
	"36044000,36017000,38030000,38031000,38056000,",
	"37056000,37029000,37011000,595060000,595014000,595030000,595004000,595005000,",
	"36043000,36023000,64030000,64052000,64058000,65070000,",
	"41001000,",
	"42046000,42029000,42023000,42056000,27043000,59045000,60044000,60017000,",
	"41029000,42045000,43043000,45046000,80044000,",
	"42044000,44046000,45043000,",
	"43043000,48030000,50046000,82045000,",
	"42044000,43045000,46043000,47046000,87029000,87030000,",
	"45044000,45011000,",
	"45043000,45011000,",
	"44029000,44011000,",
	"50043000,51044000,",
	"44043000,49044000,51030000,52046000,",
	"49044000,50029000,52043000,53046000,",
	"50044000,51043000,52046000,53029000,55045000,86030000,",
	"51044000,52045000,54046000,",
	"53044000,53011000,",
	"52044000,55045000,56030000,57043000,",
	"55029000,55011000,",
	"13030000,13056000,55044000,58046000,83045000,84043000,",
	"57043000,57011000,",
	"27001000,",
	"41043000,41029000,41017000,61044000,62045000,62030000,62052000,",
	"60043000,62045000,107046100,",
	"60044000,63045000,30043000,61046000,",
	"62046000,62011000,",
	"39029000,39056000,39059000,65044000,65070000,103045000,103074000,106043000,",
	"64043000,66044000,556046080,68061000,556029080,70029050,39029000,556045060,72045075,71045000,556030080,106030000,",
	"65047000,67044000,556046080,77025000,96043000,556050050,97072000,",
	"66043000,23044000,23042000,24030000,24031000,",
	"23046000,69029000,69056000,65045000,",
	"68030000,68061000,120046331,119046000,109045000,113075000,",
	"71045000,65030000,65023000,111046000,",
	"65048000,70046000,110045000,",
	"65070000,118049000,73045000,97048000,97072000,",
	"72046000,72017000,72011000,",
	"19043000,120044331,121044000,75030000,",
	"76046000,77045000,",
	"75045000,",
	"75043000,78044000,66045000,66017000,",
	"77046000,",
	"3001000,",
	"42045000,80044000,80046000,81043000,",
	"80044000,80011000,",
	"44046000,44011000,",
	"57046000,84043000,85044000,",
	"57045000,83044000,114050000,",
	"83043000,83011000,",
	"52029000,52011000,",
	"45029000,45030000,",
	"25030000,25056000,25043000,20039000,92044000,92027000,",
	"25001000,",
	"23001000,",
	"95045000,95073000,95023000,72030000,72056000,",
	"88046000,93043000,94045000,",
	"92046000,92027000,92011000,",
	"92046000,92027000,92023000,95045309,95003309,95073309,611045000,",
	"94046000,94011000,92027000,91044000,",
	"66044000,66011000,",
	"66048000,72044000,72017000,98029000,98045000,98073000,",
	"97046000,97072000,99044000,",
	"98050000,98073000,301043000,301023000,100043000,",
	"301044000,301023000,301011000,99044000,302071159,33071000,101047000,101022000,",
	"100046000,100071000,100011000,",
	"103030000,103074000,103011000,",
	"102029000,102038000,104030000,618046114,619046115,64046000,",
	"103029000,103074000,105030000,",
	"104029000,104011000,103074000,",
	"64029000,65044000,108043000,",
	"131046000,132049000,133047000,134048000,135029000,136050000,137043000,138044000,139045000,61030000,",
	"556043095,556045095,556046095,556047095,556048095,556049095,556050095,556029095,556030095,106043000,626044000,",
	"69046000,113045000,113075000,",
	"71044000,20039000,",
	"70045000,50030040,50039040,50056040,53030050,45030000,",
	"131049000,132045000,133043000,134050000,135048000,136047000,137044000,138030000,139029000,140046000,",
	"109046000,109011000,109109000,",
	"84048000,",
	"116049000,",
	"115047000,593030000,",
	"118049000,660041233,660042233,660069233,660047233,661041332,303041000,21039332,596039000,",
	"72030000,117029000,",
	"69045000,69011000,653043000,65307000,",
	"69045000,74043000,",
	"74043000,74011000,653045000,653007000,",
	"123047000,660041233,660042233,660069233,660049233,303041000,596039000,124077000,126028000,129040000,",
	"122044000,124043000,124077000,126028000,129040000,",
	"123044000,125047000,125036000,128048000,128037000,128030000,126028000,129040000,",
	"124046000,124077000,126045000,126028000,127043000,127017000,",
	"125046000,125023000,125011000,124077000,610030000,610039000,",
	"125044000,125011000,125017000,124077000,126028000,",
	"124045000,124029000,124077000,129046000,129030000,129040000,126028000,",
	"128044000,128029000,124077000,130043000,130019000,130040000,130003000,126028000,",
	"129044000,124077000,126028000,",
	"107044000,132048000,133050000,134049000,135047000,136029000,137030000,138045000,139046000,112043000,",
	"107050000,131029000,133045000,134046000,135044000,136049000,137047000,138043000,139030000,112048000,",
	"107029000,131030000,132044000,134047000,135049000,136043000,137045000,138050000,139048000,112046000,",
	"107047000,131045000,132050000,133048000,135043000,136030000,137046000,138029000,139044000,112049000,",
	"107045000,131048000,132030000,133046000,134043000,136044000,137049000,138047000,139050000,112029000,",
	"107043000,131044000,132029000,133049000,134030000,135046000,137050000,138048000,139047000,112045000,",
	"107048000,131047000,132046000,133030000,134029000,135050000,136045000,138049000,139043000,112044000,",
	"107030000,131043000,132047000,133029000,134044000,135045000,136046000,137048000,139049000,112050000,",
	"107049000,131050000,132043000,133044000,134045000,135030000,136048000,137029000,138046000,112047000,",
	"112045000,112011000,"
	};

long    idx1[MAXLOC] = {
        3l,160l,304l,367l,448l,
        507l,564l,689l,855l,980l,
        1086l,1333l,1385l,1567l,1694l,
        2033l,2083l,2224l,2332l,2415l,
        2472l,2496l,2525l,2647l,2770l,
        2894l,2963l,3029l,3125l,3164l,
        3274l,3282l,3314l,3490l,3547l,
        4023l,4151l,4229l,4335l,4477l,
        4574l,4733l,4793l,4853l,4913l,
        4973l,4986l,4999l,5012l,5072l,
        5132l,5192l,5252l,5312l,5325l,
        5385l,5398l,5581l,5594l,5691l,
        5863l,5977l,6045l,6058l,6270l,
        6398l,6557l,6892l,7187l,7242l,
        7302l,7447l,7512l,7532l,7688l,
        7744l,7803l,7896l,7953l,8065l,
        8125l,8139l,8153l,8213l,8273l,
        8287l,8301l,8361l,8516l,8589l,
        8643l,8818l,9043l,9096l,9154l,
        9364l,9499l,9698l,9944l,10149l,
        10283l,10357l,10504l,10769l,10834l,
        10888l,11197l,11262l,11328l,11802l,
        12278l,12486l,12553l,12884l,12899l,
        13652l,14160l,14346l,14427l,14494l,
        14561l,14628l,14722l,14818l,15026l,
        15215l,16503l,16733l,16843l,16980l,
        17180l,17247l,17312l,17379l,17446l,
        17511l,17576l,17641l,17708l,17773l
        };

long    idx2[MAXLOC] = {
        3l,35l,62l,89l,110l,
        131l,152l,184l,209l,237l,
        265l,292l,344l,372l,404l,
        433l,483l,519l,554l,586l,
        644l,668l,697l,736l,760l,
        784l,853l,919l,1015l,1054l,
        1164l,1172l,1204l,1224l,1281l,
        1310l,1339l,1417l,1523l,1554l,
        1651l,1692l,1752l,1812l,1872l,
        1932l,1946l,1960l,1974l,2034l,
        2094l,2154l,2214l,2274l,2288l,
        2348l,2362l,2390l,2404l,2501l,
        2538l,2575l,2643l,2657l,2689l,
        2817l,2850l,2889l,2914l,2969l,
        3029l,3077l,3142l,3162l,3214l,
        3270l,3329l,3422l,3479l,3591l,
        3651l,3665l,3679l,3739l,3799l,
        3813l,3827l,3887l,3918l,3991l,
        4045l,4091l,4117l,4170l,4228l,
        4265l,4290l,4319l,4347l,4370l,
        4398l,4424l,4452l,4479l,4544l,
        4598l,4623l,4688l,4715l,4745l,
        4775l,4809l,4876l,4902l,4917l,
        4954l,4991l,5024l,5057l,5124l,
        5191l,5258l,5291l,5316l,5345l,
        5386l,5421l,5457l,5491l,5528l,
        5556l,5623l,5688l,5755l,5822l,
        5887l,5952l,6017l,6084l,6149l
        };

long    idx3[MAXOBJ] = {
        3l,63l,153l,208l,274l,
        355l,436l,524l,636l,770l,
        833l,889l,981l,1110l,1200l,
        1377l,1469l,1473l,1477l,1522l,
        1640l,1668l,1693l,1709l,2151l,
        2315l,2335l,2424l,2518l,2541l,
        2557l,2780l,3020l,3196l,3250l,
        3451l,3643l,3674l,3821l,3924l,
        3952l,3956l,3960l,3964l,3968l,
        3972l,3976l,3980l,3984l,3988l,
        4062l,4112l,4166l,4223l,4269l,
        4329l,4444l,4509l,4733l,4812l,
        4891l,4957l,5072l,5120l
        };

long    idx4[MAXMSG] = {
        3l,485l,537l,655l,716l,
        760l,785l,810l,842l,884l,
        959l,1073l,1119l,1148l,1194l,
        1301l,1376l,1427l,1465l,1580l,
        1631l,1796l,1832l,1891l,1924l,
        1950l,2060l,2113l,2152l,2180l,
        2276l,2298l,2318l,2371l,2398l,
        2427l,2458l,2487l,2520l,2545l,
        2571l,2666l,2687l,2698l,2735l,
        2790l,2855l,2886l,2947l,2979l,
        3033l,4327l,4342l,4359l,4366l,
        4397l,4485l,4609l,4659l,4781l,
        4809l,4819l,4860l,5032l,5394l,
        5717l,5810l,5842l,5874l,6040l,
        6067l,6104l,6138l,6268l,6306l,
        6401l,6444l,6492l,6517l,6531l,
        6546l,6717l,6921l,7054l,7171l,
        7312l,7372l,7385l,7398l,7411l,
        7424l,7493l,7566l,7613l,7665l,
        7708l,7780l,7820l,7854l,7900l,
        7990l,8033l,8097l,8170l,8214l,
        8248l,8306l,8345l,8382l,8408l,
        8434l,8488l,8565l,8630l,8733l,
        8804l,8874l,8991l,9059l,9129l,
        9197l,9267l,9328l,9391l,9592l,
        9688l,9825l,9892l,10117l,10254l,
        10373l,10503l,10712l,10986l,11202l,
        11294l,11474l,11518l,11577l,11649l,
        11685l,11741l,13063l,13100l,13156l,
        13229l,13270l,13293l,13333l,13418l,
        13474l,13542l,13605l,13672l,13793l,
        13807l,13937l,14078l,14222l,14291l,
        14332l,14382l,14619l,14759l,14830l,
        14889l,14950l,15008l,15134l,15178l,
        15210l,15242l,15272l,15333l,15368l,
        15395l,15442l,15509l,15564l,15737l,
        15780l,15800l,15870l,16064l,16101l,
        16236l,16564l,16636l,16719l,16820l,
        16873l,16945l,17067l,17195l,17238l,
        17274l,17335l,17433l,17502l,17612l,
        17637l
        };

/*
	Adventure vocabulary & encryption
*/
struct	wac	wc[] =
	{
	"spelunker today",	1016,
	"?",		3051,
	"above",	29,
	"abra",		3050,
	"abracadabra",	3050,
	"across",	42,
	"ascend",	29,
	"attack",	2012,
	"awkward",	26,
	"axe",		1028,
	"back",		8,
	"barren",	40,
	"bars",		1052,
	"batteries",	1039,
	"battery",	1039,
	"beans",	1024,
	"bear",		1035,
	"bed",		16,
	"bedquilt",	70,
	"bird",		1008,
	"blast",	2023,
	"blowup",	2023,
	"bottle",	1020,
	"box",		1055,
	"break",	2028,
	"brief",	2026,
	"broken",	54,
	"building",	12,
	"cage",		1004,
	"calm",		2010,
	"canyon",	25,
	"capture",	2001,
	"carpet",	1040,
	"carry",	2001,
	"catch",	2001,
	"cave",		67,
	"cavern",	73,
	"chain",	1064,
	"chant",	2003,
	"chasm",	1032,
	"chest",	1055,
	"clam",		1014,
	"climb",	56,
	"close",	2006,
	"cobblestone",	18,
	"coins",	1054,
	"continue",	2011,
	"crack",	33,
	"crap",		3079,
	"crawl",	17,
	"cross",	69,
	"d",		30,
	"damn",		3079,
	"damnit",	3079,
	"dark",		22,
	"debris",	51,
	"depression",	63,
	"descend",	30,
	"describe",	57,
	"detonate",	2023,
	"devour",	2014,
	"diamonds",	1051,
	"dig",		3066,
	"discard",	2002,
	"disturb",	2029,
	"dome",		35,
	"door",		1009,
	"down",		30,
	"downstream",	4,
	"downward",	30,
	"dragon",	1031,
	"drawing",	1029,
	"drink",	2015,
	"drop",		2002,
	"dump",		2002,
	"dwarf",	1017,
	"dwarves",	1017,
	"e",		43,
	"east",		43,
	"eat",		2014,
	"egg",		1056,
	"eggs",		1056,
	"emerald",	1059,
	"enter",	3,
	"entrance",	64,
	"examine",	57,
	"excavate",	3066,
	"exit",		11,
	"explore",	2011,
	"extinguish",	2008,
	"fee",		2025,
	"fee",		3001,
	"feed",		2021,
	"fie",		2025,
	"fie",		3002,
	"fight",	2012,
	"figure",	1027,
	"fill",		2022,
	"find",		2019,
	"fissure",	1012,
	"floor",	58,
	"foe",		2025,
	"foe",		3003,
	"follow",	2011,
	"foo",		2025,
	"foo",		3004,
	"food",		1019,
	"forest",	6,
	"fork",		77,
	"forward",	7,
	"free",		2002,
	"fuck",		3079,
	"fum",		2025,
	"fum",		3005,
	"get",		2001,
	"geyser",	1037,
	"giant",	27,
	"go",		2011,
	"gold",		1050,
	"goto",		2011,
	"grate",	1003,
	"gully",	13,
	"h2o",		1021,
	"hall",		38,
	"headlamp",	1002,
	"help",		3051,
	"hill",		2,
	"hit",		2012,
	"hocus",	3050,
	"hole",		52,
	"hours",	2031,
	"house",	12,
	"ignite",	2023,
	"in",		19,
	"info",		3142,
	"information",	3142,
	"inside",	19,
	"inventory",	2020,
	"inward",	19,
	"issue",	1016,
	"jar",		1020,
	"jewel",	1053,
	"jewelry",	1053,
	"jewels",	1053,
	"jump",		39,
	"keep",		2001,
	"key",		1001,
	"keys",		1001,
	"kill",		2012,
	"knife",	1018,
	"knives",	1018,
	"lamp",		1002,
	"lantern",	1002,
	"leave",	11,
	"left",		36,
	"light",	2007,
	"lock",		2006,
	"log",		2032,
	"look",		57,
	"lost",		3068,
	"low",		24,
	"machine",	1038,
	"magazine",	1016,
	"main",		76,
	"message",	1036,
	"ming",		1058,
	"mirror",	1023,
	"mist",		3069,
	"moss",		1040,
	"mumble",	2003,
	"n",		45,
	"ne",		47,
	"nest",		1056,
	"north",	45,
	"nothing",	2005,
	"nowhere",	21,
	"nugget",	1050,
	"null",		21,
	"nw",		50,
	"off",		2008,
	"office",	76,
	"oil",		1022,
	"on",		2007,
	"onward",	7,
	"open",		2004,
	"opensesame",	3050,
	"oriental",	72,
	"out",		11,
	"outdoors",	32,
	"outside",	11,
	"over",		41,
	"oyster",	1015,
	"passage",	23,
	"pause",	2030,
	"pearl",	1061,
	"persian",	1062,
	"peruse",	2027,
	"pillow",	1010,
	"pirate",	1030,
	"pit",		31,
	"placate",	2010,
	"plant",	1024,
	"plant",	1025,
	"platinum",	1060,
	"plover",	71,
	"plugh",	65,
	"pocus",	3050,
	"pottery",	1058,
	"pour",		2013,
	"proceed",	2011,
	"pyramid",	1060,
	"quit",		2018,
	"rations",	1019,
	"read",		2027,
	"release",	2002,
	"reservoir",	75,
	"retreat",	8,
	"return",	8,
	"right",	37,
	"road",		2,
	"rock",		15,
	"rod",		1005,
	"rod",		1006,
	"room",		59,
	"rub",		2016,
	"rug",		1062,
	"run",		2011,
	"s",		46,
	"save",		2030,
	"say",		2003,
	"score",	2024,
	"se",		48,
	"secret",	66,
	"sesame",	3050,
	"shadow",	1027,
	"shake",	2009,
	"shard",	1058,
	"shatter",	2028,
	"shazam",	3050,
	"shell",	74,
	"shit",		3079,
	"silver",	1052,
	"sing",		2003,
	"slab",		61,
	"slit",		60,
	"smash",	2028,
	"snake",	1011,
	"south",	46,
	"spelunker",	1016,
	"spice",	1063,
	"spices",	1063,
	"stairs",	10,
	"stalactite",	1026,
	"steal",	2001,
	"steps",	1007,
	"steps",	34,
	"stop",		3139,
	"stream",	14,
	"strike",	2012,
	"surface",	20,
	"suspend",	2030,
	"sw",		49,
	"swim",		3147,
	"swing",	2009,
	"tablet",	1013,
	"take",		2001,
	"tame",		2010,
	"throw",	2017,
	"toss",		2017,
	"tote",		2001,
	"touch",	57,
	"travel",	2011,
	"treasure",	1055,
	"tree",		3064,
	"trees",	3064,
	"trident",	1057,
	"troll",	1033,
	"troll",	1034,
	"tunnel",	23,
	"turn",		2011,
	"u",		29,
	"unlock",	2004,
	"up",		29,
	"upstream",	4,
	"upward",	29,
	"utter",	2003,
	"valley",	9,
	"vase",		1058,
	"velvet",	1010,
	"vending",	1038,
	"view",		28,
	"volcano",	1037,
	"w",		44,
	"wake",		2029,
	"walk",		2011,
	"wall",		53,
	"water",	1021,
	"wave",		2009,
	"west",		44,
	"xyzzy",	62,
	"y2",		55
	};

/*
	Database variables
*/
struct	trav	travel[MAXTRAV];
FILE	*fd1, *fd2, *fd3, *fd4;
int	actmsg[32];		/* action messages	*/

/*
	English variables
*/
int	verb,object,motion;
char	word1[WORDSIZE],word2[WORDSIZE];

/*
	Play variables
*/
int	turns;
int	loc,oldloc,oldloc2,newloc;	/* location variables */
int	cond[MAXLOC];		/* location status	*/
int	place[MAXOBJ];		/* object location	*/
int	fixed[MAXOBJ];		/* second object loc	*/
int	visited[MAXLOC];	/* >0 if has been here	*/
int	prop[MAXOBJ];		/* status of object	*/
int	tally,tally2;		/* item counts		*/
int	limit;			/* time limit		*/
int	lmwarn;			/* lamp warning flag	*/
int	wzdark,closing,closed;	/* game state flags	*/
int	holding;		/* count of held items	*/
int	detail;			/* LOOK count		*/
int	knfloc;			/* knife location	*/
int	clock1,clock2,panic;	 /* timing variables	 */
int	dloc[DWARFMAX];		/* dwarf locations	*/
int	dflag;			/* dwarf flag		*/
int	dseen[DWARFMAX];	/* dwarf seen flag	*/
int	odloc[DWARFMAX];	/* dwarf old locations	*/
int	daltloc;		/* alternate appearance	*/
int	dkill;			/* dwarves killed	*/
int	chloc,chloc2;		/* chest locations	*/
int	bonus;			/* to pass to end	*/
int	numdie;			/* number of deaths	*/
int	object1;		/* to help intrans.	*/
int	gaveup;			/* 1 if he quit early	*/
int	foobar;			/* fie fie foe foo...	*/
int	saveflg;		/* if game being saved	*/
int	dbugflg;		/* if game is in debug	*/

int	lastglob;		/* to get space req.	*/

main(argc, argv)
int	argc;
char	**argv;
{
	int	rflag;		/* user restore request option	*/

	rflag = 0;
	dbugflg = 0;
	while (--argc > 0) {
		++argv;
		if (**argv !=  '-')
			break;
		if (isascii(argv[0][1]))
		{
			if (isupper(argv[0][1]))
				argv[0][1] = tolower(argv[0][1]);
		}
		switch(argv[0][1]) {
		case 'r':
			++rflag;
			continue;
		case 'd':
			++dbugflg;
			continue;
		default:
			printf("unknown flag: %c\n", argv[0][1]);
			continue;
		}				/* switch	*/
	}					/* while	*/
	if (dbugflg < 2)
		dbugflg = 0;	/* must request three times	*/
	opentxt();
	initplay();
	if (rflag)
		restore();
	else if (yes(65, 1, 0))
		limit = 1000;
	else
		limit = 330;
	saveflg = 0;
	xrndseed();				/* seed random	*/
	while(!saveflg)
		turn();
	if (saveflg)
		saveadv();
	fclose(fd1);
	fclose(fd2);
	fclose(fd3);
	fclose(fd4);
	exit(0);				/* exit = ok	*/
}						/* main		*/

/* ************************************************************	*/

/*
	Initialize integer arrays with sscanf
*/
scanint(pi, str)
int	*pi;
char	*str;
{

	while (*str) {
		if  ((sscanf(str, "%d,", pi++)) < 1)
			bug(41);	/* failed before EOS	*/
		while (*str++ != ',')	/* advance str pointer	*/
			;
	}
	return;
}

/*
	Initialization of adventure play variables
*/
initplay()
{
	turns = 0;

	/* initialize location status array */
	setmem(cond, (sizeof(int))*MAXLOC, 0);
	scanint(&cond[1], "5,1,5,5,1,1,5,17,1,1,");
	scanint(&cond[13], "32,0,0,2,0,0,64,2,");
	scanint(&cond[21], "2,2,0,6,0,2,");
	scanint(&cond[31], "2,2,0,0,0,0,0,4,0,2,");
	scanint(&cond[42], "128,128,128,128,136,136,136,128,128,");
	scanint(&cond[51], "128,128,136,128,136,0,8,0,2,");
	scanint(&cond[79], "2,128,128,136,0,0,8,136,128,0,2,2,");
	scanint(&cond[95], "4,0,0,0,0,1,");
	scanint(&cond[113], "4,0,1,1,");
	scanint(&cond[122], "8,8,8,8,8,8,8,8,8,");

	/* initialize object locations */
	setmem(place, (sizeof(int))*MAXOBJ, 0);
	scanint(&place[1], "3,3,8,10,11,0,14,13,94,96,");
	scanint(&place[11], "19,17,101,103,0,106,0,0,3,3,");
	scanint(&place[23], "109,25,23,111,35,0,97,");
	scanint(&place[31], "119,117,117,0,130,0,126,140,0,96,");
	scanint(&place[50], "18,27,28,29,30,");
	scanint(&place[56], "92,95,97,100,101,0,119,127,130,");

	/* initialize second (fixed) locations */
	setmem(fixed, (sizeof(int))*MAXOBJ, 0);
	scanint(&fixed[3], "9,0,0,0,15,0,-1,");
	scanint(&fixed[11], "-1,27,-1,0,0,0,-1,");
	scanint(&fixed[23], "-1,-1,67,-1,110,0,-1,-1,");
	scanint(&fixed[31], "121,122,122,0,-1,-1,-1,-1,0,-1,");
	scanint(&fixed[62], "121,-1,");

	/* initialize default verb messages */
	scanint(actmsg, "0,24,29,0,33,0,33,38,38,42,14,");
	scanint(&actmsg[11], "43,110,29,110,73,75,29,13,59,59,");
	scanint(&actmsg[21], "174,109,67,13,147,155,195,146,110,13,13,");

	/* initialize various flags and other variables */
	setmem(visited, (sizeof(int))*MAXLOC, 0);
	setmem(prop, (sizeof(int))*MAXOBJ, 0);
	setmem(&prop[50], (sizeof(int))*(MAXOBJ-50), 0xff);
	wzdark = closed = closing = holding = detail = 0;
	limit = 100;
	tally = 15;
	tally2 = 0;
	newloc = 3;
	loc = oldloc = oldloc2 = 1;
	knfloc = 0;
	chloc = 114;
	chloc2 = 140;
/*	dloc[DWARFMAX-1] = chloc				*/
	scanint(dloc, "0,19,27,33,44,64,114,");
	scanint(odloc, "0,0,0,0,0,0,0,");
	dkill = 0;
	scanint(dseen, "0,0,0,0,0,0,0,");
	clock1 = 30;
	clock2 = 50;
	panic = 0;
	bonus = 0;
	numdie = 0;
	daltloc = 18;
	lmwarn = 0;
	foobar = 0;
	dflag = 0;
	gaveup = 0;
	saveflg = 0;
	return;
}

/*
	Open advent?.txt files
*/
opentxt()
{
	fd1 = fopen("advent1.txt", "r");
	if (!fd1) {
		printf("Sorry, I can't open advent1.txt...\n");
		exit(0);
	}
	fd2 = fopen("advent2.txt", "r");
	if (!fd2) {
		printf("Sorry, I can't open advent2.txt...\n");
		exit(0);
	}
	fd3 = fopen("advent3.txt", "r");
	if (!fd3) {
		printf("Sorry, I can't open advent3.txt...\n");
		exit(0);
	}
	fd4 = fopen("advent4.txt", "r");
	if (!fd4) {
		printf("Sorry, I can't open advent4.txt...\n");
		exit(0);
	}
}

/*
		save adventure game
*/
saveadv()
{
	char	*sptr;
	FILE	*savefd;
	char	username[13];
	int i;

	printf("What do you want to name the saved game? ");
	scanf("%s", username);
	if (sptr = strchr(username, '.'))
		*sptr = '\0';		/* kill extension	*/
	if (strlen(username) > 8)
		username[8] = '\0';	/* max 8 char filename	*/
	strcat(username, ".adv");
	savefd = fopen(username, "wb");
	if (savefd == NULL) {
		printf("Sorry, I can't create the file...%s\n", username);
		exit(0);
	}

	putw(turns,savefd);	/* mh 30-Aug-1988 */
	putw(loc,savefd);
	putw(oldloc,savefd);
	putw(oldloc2,savefd);
	putw(newloc,savefd);
	for(i=0;i<MAXLOC;i++) putw(cond[i],savefd);
	for(i=0;i<MAXOBJ;i++) putw(place[i],savefd);
	for(i=0;i<MAXOBJ;i++) putw(fixed[i],savefd);
	for(i=0;i<MAXLOC;i++) putw(visited[i],savefd);
	for(i=0;i<MAXOBJ;i++) putw(prop[i],savefd);
	putw(tally,savefd);
	putw(tally2,savefd);
	putw(limit,savefd);
	putw(lmwarn,savefd);
	putw(wzdark,savefd);
	putw(closing,savefd);
	putw(closed,savefd);
	putw(holding,savefd);
	putw(detail,savefd);
	putw(knfloc,savefd);
	putw(clock1,savefd);
	putw(clock2,savefd);
	putw(panic,savefd);
	for(i=0;i<DWARFMAX;i++) putw(dloc[i],savefd);
	putw(dflag,savefd);
	for(i=0;i<DWARFMAX;i++) putw(dseen[i],savefd);
	for(i=0;i<DWARFMAX;i++) putw(odloc[i],savefd);
	putw(daltloc,savefd);
	putw(dkill,savefd);
	putw(chloc,savefd);
	putw(chloc2,savefd);
	putw(bonus,savefd);
	putw(numdie,savefd);
	putw(object1,savefd);
	putw(gaveup,savefd);
	putw(foobar,savefd);
	putw(saveflg,savefd);
	putw(dbugflg,savefd);

	if (fclose(savefd)  ==  -1) {
		printf("Sorry, I can't close the file...%s\n", username);
		exit(0);
	}
	printf("OK -- \"C\" you later...\n");
}

/*
	restore saved game handler
*/
restore()
{
	char	username[13];
	FILE	*restfd;
	int	i;
	char	*sptr;

	printf("What is the name of the saved game? ");
	scanf("%s", username);
	if (sptr = strchr(username, '.'))
		*sptr = '\0';		/* kill extension	*/
	if (strlen(username) > 8)
		username[8] = '\0';	/* max 8 char filename	*/
	strcat(username, ".adv");
	restfd = fopen(username, "rb");
	if (restfd == NULL) {
		printf("Sorry, I can't open the file...%s\n", username);
		exit(0);
	}

	turns=getw(restfd);	/* mh 30-Aug-1988 */
	loc=getw(restfd);
	oldloc=getw(restfd);
	oldloc2=getw(restfd);
	newloc=getw(restfd);
	for(i=0;i<MAXLOC;i++) cond[i]=getw(restfd);
	for(i=0;i<MAXOBJ;i++) place[i]=getw(restfd);
	for(i=0;i<MAXOBJ;i++) fixed[i]=getw(restfd);
	for(i=0;i<MAXLOC;i++) visited[i]=getw(restfd);
	for(i=0;i<MAXOBJ;i++) prop[i]=getw(restfd);
	tally=getw(restfd);
	tally2=getw(restfd);
	limit=getw(restfd);
	lmwarn=getw(restfd);
	wzdark=getw(restfd);
	closing=getw(restfd);
	closed=getw(restfd);
	holding=getw(restfd);
	detail=getw(restfd);
	knfloc=getw(restfd);
	clock1=getw(restfd);
	clock2=getw(restfd);
	panic=getw(restfd);
	for(i=0;i<DWARFMAX;i++) dloc[i]=getw(restfd);
	dflag=getw(restfd);
	for(i=0;i<DWARFMAX;i++) dseen[i]=getw(restfd);
	for(i=0;i<DWARFMAX;i++) odloc[i]=getw(restfd);
	daltloc=getw(restfd);
	dkill=getw(restfd);
	chloc=getw(restfd);
	chloc2=getw(restfd);
	bonus=getw(restfd);
	numdie=getw(restfd);
	object1=getw(restfd);
	gaveup=getw(restfd);
	foobar=getw(restfd);
	saveflg=getw(restfd);
	dbugflg=getw(restfd);

	    if (fclose(restfd)	==  -1) {
		printf("Warning -- can't close save file...%s\n", username);
	}
}

/*
	Routine to fill travel array for a given location
*/
gettrav(loc)
int	loc;
{
	int	i;
	long	t;
	char*   aptr;
	char	atrav[256];

	strcpy(atrav, cave[loc - 1]);
	while (aptr = rindex(atrav, ','))
		*aptr = '\0';		/* terminate substring	*/
	aptr = &atrav[0];
	for (i = 0; i < MAXTRAV; ++i) {
		t = atol(aptr); 	/* convert to long int	*/
		travel[i].tcond = (t % 1000);
		t /= 1000;
		travel[i].tverb = (t % 1000);
		t /= 1000;
		travel[i].tdest = (t % 1000);
		while (*aptr++)
			;		/* to next substring	*/
		if (!(*aptr)) {
		    travel[++i].tdest = -1;/* end of array	*/
		    if (dbugflg)
			for (i = 0; i < MAXTRAV; ++i)
				printf("cave[%d] = %d %d %d\n", loc, travel[i].tdest, travel[i].tverb, travel[i].tcond);
		    return;		/* terminate for loop	*/
		}
	}
	bug(33);
}

/*
	Function to scan a file up to a specified
	point and either print or return a string.
*/
rdupto(fdi, uptoc, print, string)
FILE	*fdi;
char	uptoc, print, *string;
{
	int	c;

	while ((c = fgetc(fdi)) != uptoc) {
		if (c == EOF)
			return(0);
		if (c == '\r')
			continue;
		if (print)
			fputc(c, stdout);
		else
			*string++ = c;
	}
	if (!print)
		*string = '\0';
	return(1);
}

/*
	Function to read a file skipping
	a given character a specified number
	of times, with or without repositioning
	the file.
*/
rdskip(fdi, skipc, n, rewind)
FILE	*fdi;
char	skipc, rewind;
int	n;
{
	int	c;

	if (rewind)
		if (fseek(fdi, 0, 0) == -1)
			bug(31);
	while (n--)
		while ((c = fgetc(fdi)) != skipc)
			if (c == EOF)
				bug(32);
}

/*
	Routine to request a yes or no answer to a question.
*/
yes(msg1, msg2, msg3)
int	msg1, msg2, msg3;
{
	char	answer[80];

	if (msg1)
		rspeak(msg1);
	fputc('>', stdout);
	fgets(answer, 80, stdin);
	if (isascii(answer[0]))
	{
		if (isupper(answer[0]))
			answer[0] = tolower(answer[0]);
	}
	if (answer[0] == 'n') {
		if (msg3)
			rspeak(msg3);
		return(0);
	}
	if (msg2)
		rspeak(msg2);
	return(1);
}

/*
	Print a location description from "advent4.txt"
*/
rspeak(msg)
int	msg;
{
	if (msg == 54)
		printf("ok.\n");
	else {
		if (dbugflg)
		    printf("Seek loc msg #%d @ %ld\n", msg, idx4[msg]);
		fseek(fd4, idx4[msg - 1], 0);
		rdupto(fd4, '#', 1, 0);
	}
	return;
}

/*
	Print an item message for a given state from "advent3.txt"
*/
pspeak(item, state)
int	item, state;
{
	fseek(fd3, idx3[item - 1], 0);
	rdskip(fd3, '/', state+2, 0);
	rdupto(fd3, '/', 1, 0);
}

/*
	Print a long location description from "advent1.txt"
*/
desclg(loc)
int	loc;
{
	fseek(fd1, idx1[loc - 1], 0);
	rdupto(fd1, '#', 1, 0);
}

/*
	Print a short location description from "advent2.txt"
*/
descsh(loc)
int	loc;
{
	fseek(fd2, idx2[loc - 1], 0);
	rdupto(fd2, '#', 1, 0);
}

binary(w, wctable, maxwc)
char	*w;
int	maxwc;
struct	wac	wctable[];
{
	int	lo, mid, hi, check;

	lo = 0;
	hi = maxwc - 1;
	while (lo <= hi) {
		mid = (lo + hi) / 2;
		if ((check = strcmp(w, wctable[mid].aword)) < 0)
			hi = mid - 1;
		else if (check > 0)
			lo = mid + 1;
		else
			return(mid);
	}
	return(-1);
}

/*
	look-up vocabulary word in lex-ordered table.  words may have
	two entries with different codes. if minimum acceptable value
	= 0, then return minimum of different codes.  last word CANNOT
	have two entries(due to binary sort).
	word is the word to look up.
	val  is the minimum acceptable value,
		if != 0 return %1000
*/
vocab(word, val)
char	*word;
int	val;
{
	int	v1, v2;

	if ((v1 = binary(word, wc, MAXWC)) >= 0) {
		v2 = binary(word, wc, MAXWC-1);
		if (v2 < 0)
			v2 = v1;
		if (!val)
			return(wc[v1].acode < wc[v2].acode ? wc[v1].acode : wc[v2].acode);
		if (val <= wc[v1].acode)
			return(wc[v1].acode % 1000);
		else if (val <= wc[v2].acode)
			return(wc[v2].acode % 1000);
		else
			return(-1);
	}
	else
		return(-1);
}

/*
	Utility Routines
*/

/*
	Routine to tell if an item is being carried.
*/
toting(item)
int	item;
{
	return(place[item] == -1);
}

/*
	Routine to tell if an item is present.
*/
here(item)
int	item;
{
	return(place[item] == loc || toting(item));
}

/*
	Routine to test for darkness
*/
dark()
{
	return(!(cond[loc] & LIGHT) &&
		(!prop[LAMP] ||
		!here(LAMP)));
}

/*
	Routine to tell if a location causes
	a forced move.
*/
forced(atloc)
int	atloc;
{
	return(cond[atloc] == 2);
}

/*
	Routine true x% of the time.
*/
pct(x)
int	x;
{
	return(xrnd() % 100 < x);
}

/*
	Routine to tell if player is on
	either side of a two sided object.
*/
at(item)
int	item;
{
	return(place[item] == loc || fixed[item] == loc);
}

/*
	Routine to destroy an object
*/
dstroy(obj)
int	obj;
{
	move(obj, 0);
}

/*
	Routine to move an object
*/
move(obj, where)
int	obj, where;
{
	int	from;

	from = (obj<MAXOBJ) ? place[obj] : fixed[obj];
	if (from>0 && from<=300)
		carry(obj, from);
	drop(obj, where);
}

/*
	Juggle an object
	currently a no-op
*/
juggle(loc)
int	loc;
{
}

/*
	Routine to carry an object
*/
carry(obj, where)
int	obj, where;
{
	if (obj<MAXOBJ){
		if (place[obj] == -1)
			return;
		place[obj]=-1;
		++holding;
	}
}

/*
	Routine to drop an object
*/
drop(obj, where)
int	obj, where;
{
	if (obj<MAXOBJ) {
		if (place[obj] == -1)
			--holding;
		place[obj]=where;
	}
	else
		fixed[obj-MAXOBJ]=where;
}

/*
	routine to move an object and return a
	value used to set the negated prop values
	for the repository.
*/
put(obj, where, pval)
int	obj, where, pval;
{
	move(obj, where);
	return((-1)-pval);
}
/*
	Routine to check for presence
	of dwarves..
*/
dcheck()
{
	int	i;

	for (i =1; i < (DWARFMAX-1); ++i)
		if (dloc[i] == loc)
			return(i);
	return(0);
}

/*
	Convert  0 to WATER
		 1 to nothing
		 2 to OIL
*/
liq2(pbottle)
int	pbottle;
{
	return((1-pbottle)*WATER+(pbottle>>1)*(WATER+OIL));
}

/*
	Determine liquid in the bottle
*/
liq()
{
	int	i, j;
	i=prop[BOTTLE];
	j=-1-i;
	return(liq2(i>j ? i : j));
}

/*
	Determine liquid at a location
*/
liqloc(loc)
int	loc;
{
	if (cond[loc]&LIQUID)
		return(liq2(cond[loc]&WATOIL));
	else
		return(liq2(1));
}

/*
	Fatal error routine
*/
bug(n)
int	n;
{
	printf("Fatal error number %d\n", n);
	exit(1);
}

/*
		Routine to analyze a word.
*/
analyze(word, type, value)
char	*word;
int	*type, *value;
{
	int	wordval, msg;

	/* make sure I understand */
	if ((wordval = vocab(word, 0)) == -1) {
		switch(xrnd() % 3) {
		case 0:
			msg = 60;
			break;
		case 1:
			msg = 61;
			break;
		default:
			msg = 13;
		}
		rspeak(msg);
		return(0);
	}
	*type = wordval/1000;
	*value = wordval%1000;
	return(1);
}

/*
	Analyze a two word sentence
*/
english()
{

	char	*msg;
	int	type1, type2, val1, val2;

	verb = object = motion = 0;
	type2 = val2 = -1;
	type1 = val1 = -1;
	msg = "bad grammar...";

	getwords();

	if (!(*word1))
		return(0);		/* ignore whitespace	*/
	if (!analyze(word1, &type1, &val1))	/* check word1	*/
		return(0);		/* didn't know it	*/

	if (type1 == 2 && val1 == SAY) {
		verb = SAY;	/* repeat word & act upon if..	*/
		object = 1;
		return(1);
	}

	if (*word2)
		if (!analyze(word2, &type2, &val2))
			return(0);	/* didn't know it	*/

	/* check his grammar */
	if ((type1 == 3) && (type2 == 3) && (val1 == 51) && (val2 == 51)) {
		outwords();
		return(0);
	}
	else if (type1 == 3) {
		rspeak(val1);
		return(0);
	}
	else if (type2 == 3) {
		rspeak(val2);
		return(0);
	}
	else if (type1 == 0) {
		if (type2 == 0) {
			printf("%s\n", msg);
			return(0);
		}
		else
			motion = val1;
	}
	else if (type2 == 0)
		motion = val2;
	else if (type1 == 1) {
		object = val1;
		if (type2 == 2)
			verb = val2;
		if (type2 == 1) {
			printf("%s\n", msg);
			return(0);
		}
	}
	else if (type1 == 2) {
		verb = val1;
		if (type2 == 1)
			object = val2;
		if (type2 == 2) {
			printf("%s\n", msg);
			return(0);
		}
	}
	else
		bug(36);
	return(1);
}

/*
	retrieve input line (max 80 chars), convert to lower case
	 & rescan for first two words (max. WORDSIZE-1 chars).
*/
getwords()
{
	char	words[80], *wptr;

	fputc('>', stdout);
	word1[0] = word2[0] = '\0';
	fgets(words, 80, stdin);
	wptr = words;
	while (*wptr)
	{
		if (isascii(*wptr))
		{
			if (isupper(*wptr))
				*wptr = tolower(*wptr);
		}
		wptr++;
	}
	sscanf(words, "%19s %19s", word1, word2);
	if (dbugflg)
		printf("WORD1 = %s, WORD2 = %s\n", word1, word2);
	return;
}

/*
	output adventure word list (motion/0xxx & verb/2xxx) only
	6 words/line pausing at 20th line until keyboard active
*/
outwords()
{
	int	i, j, line;
	char	words[80];

	j = line = 0;
	for (i = 0; i < MAXWC; ++i) {
		if ((wc[i].acode < 1000) || ((wc[i].acode < 3000) && (wc[i].acode > 1999))) {
			printf("%-12s", wc[i].aword);
			if ((++j == 6) || (i == MAXWC-1)) {
				j = 0;
				fputc('\n', stdout);
				if (++line == 20) {
					line = 0;
					printf("\n\007Enter <RETURN>");
					printf(" to continue\n\n");
					fgets(words, 80, stdin);
				}
			}
		}
	}
}

/*
	Routines to process intransitive verbs
*/
itverb()
{
	switch(verb) {
	case DROP:
	case SAY:
	case WAVE:
	case CALM:
	case RUB:
	case THROW:
	case FIND:
	case FEED:
	case BREAK:
	case WAKE:
		needobj();
		break;
	case TAKE:
		ivtake();
		break;
	case OPEN:
	case LOCK:
		ivopen();
		break;
	case NOTHING:
		rspeak(54);
		break;
	case ON:
	case OFF:
	case POUR:
		trverb();
		break;
	case WALK:
		actspk(verb);
		break;
	case KILL:
		ivkill();
		break;
	case EAT:
		iveat();
		break;
	case DRINK:
		ivdrink();
		break;
	case QUIT:
		ivquit();
		break;
	case FILL:
		ivfill();
		break;
	case BLAST:
		vblast();
		break;
	case SCORE:
		score();
		break;
	case FOO:
		ivfoo();
		break;
	case SUSPEND:
		saveflg = 1;
		break;
	case INVENTORY:
		inventory();
		break;
	case READ:
		ivread();
		break;
	default:
		printf("This intransitive not implemented yet\n");
	}
}

/*
	CARRY, TAKE etc.
*/
ivtake()
{
	int anobj,item;

	anobj = 0;
	for(item=1;item<MAXOBJ;++item) {
		if(place[item]==loc) {
			if(anobj != 0) {
				needobj();
				return;
			}
			anobj = item;
		}
	}
	if(anobj==0|| (dcheck() && dflag>=2)) {
		needobj();
		return;
	}
	object = anobj;
	vtake();
}

/*
	OPEN, LOCK, UNLOCK
*/
ivopen()
{
	if(here(CLAM))
		object=CLAM;
	if(here(OYSTER))
		object=OYSTER;
	if(at(DOOR))
		object=DOOR;
	if(at(GRATE))
		object=GRATE;
	if(here(CHAIN)) {
		if(object != 0) {
			needobj();
			return;
		}
		object=CHAIN;
	}
	if(object==0) {
		rspeak(28);
		return;
	}
	vopen();
}

/*
	ATTACK, KILL etc
*/
ivkill()
{
	object1 = 0;
	if(dcheck() && dflag >=2)
		object=DWARF;
	if(here(SNAKE))
		addobj(SNAKE);
	if(at(DRAGON) && prop[DRAGON]==0)
		addobj(DRAGON);
	if(at(TROLL))
		addobj(TROLL);
	if(here(BEAR) && prop[BEAR]==0)
		addobj(BEAR);
	if(object1 != 0) {
		needobj();
		return;
	}
	if(object != 0) {
		vkill();
		return;
	}
	if(here(BIRD) && verb!= THROW)
		object=BIRD;
	if(here(CLAM) || here(OYSTER))
		addobj(CLAM);
	if(object1 != 0) {
		needobj();
		return;
	}
	vkill();
}

/*
	EAT
*/
iveat()
{
	if(!here(FOOD))
		needobj();
	else {
		object=FOOD;
		veat();
	}
}

/*
	DRINK
*/
ivdrink()
{
	if(liqloc(loc) != WATER &&
	  (liq()!= WATER || !here(BOTTLE)))
		needobj();
	else {
		object=WATER;
		vdrink();
	}
}

/*
	QUIT
*/
ivquit()
{
	if(gaveup=yes(22,54,54))
		normend();
}

/*
	FILL
*/
ivfill()
{
	if(!here(BOTTLE))
		needobj();
	else {
		object=BOTTLE;
		vfill();
	}
}

/*
	Handle fee fie foe foo...
*/
ivfoo()
{
	char k,msg;
	k = vocab(word1,3000);
	msg = 42;
	if (foobar != 1-k) {
		if (foobar != 0)
			msg = 151;
		rspeak(msg);
		return;
	}
	foobar = k;
	if (k != 4)
		return;
	foobar = 0;
	if (place[EGGS] == 92 ||
	   (toting(EGGS) && loc == 92)) {
		rspeak(msg);
		return;
	}
	if (place[EGGS] == 0 && place[TROLL] == 0 &&
	    prop[TROLL] == 0)
		prop[TROLL] = 1;
	if (here(EGGS))
		k = 1;
	else if (loc == 92)
		k = 0;
	else
		k = 2;
	move(EGGS,92);
	pspeak(EGGS,k);
	return;
}

/*
	read etc...
*/
ivread()
{
	if (here(MAGAZINE))
		object = MAGAZINE;
	if (here(TABLET))
		object = object*100 + TABLET;
	if (here(MESSAGE))
		object = object*100 + MESSAGE;
	if (object > 100 || object == 0 || dark()) {
		needobj();
		return;
	}
	vread();
}

/*
	INVENTORY 
*/
inventory()
{
	char msg;
	int i;

	msg = 98;
	for (i=1; i<=MAXOBJ; ++i) {
		if (i==BEAR || !toting(i))
			continue;
		if (msg)
			rspeak(99);
		msg = 0;
		pspeak(i,-1);
	}
	if (toting(BEAR))
		msg = 141;
	if (msg)
		rspeak(msg);
}

/*
	ensure uniqueness as objects are searched
	out for an intransitive verb
*/
addobj(obj)
{
	if(object1 != 0)
		return;
	if(object != 0) {
		object1 = -1;
		return;
	}
	object = obj;
}

/*
	Routine to process a transitive verb
*/
trverb()
{
	switch(verb){
	case CALM:
	case WALK:
	case QUIT:
	case SCORE:
	case FOO:
	case BRIEF:
	case SUSPEND:
	case HOURS:
	case LOG:
		actspk(verb);
		break;
	case TAKE:
		vtake();
		break;
	case DROP:
		vdrop();
		break;
	case OPEN:
	case LOCK:
		vopen();
		break;
	case SAY:
		vsay();
		break;
	case NOTHING:
		rspeak(54);
		break;
	case ON:
		von();
		break;
	case OFF:
		voff();
		break;
	case WAVE:
		vwave();
		break;
	case KILL:
		vkill();
		break;
	case POUR:
		vpour();
		break;
	case EAT:
		veat();
		break;
	case DRINK:
		vdrink();
		break;
	case RUB:
		if (object != LAMP)
			rspeak(76);
		else
			actspk(RUB);
		break;
	case THROW:
		vthrow();
		break;
	case FEED:
		vfeed();
		break;
	case FIND:
	case INVENTORY:
		vfind();
		break;
	case FILL:
		vfill();
		break;
	case READ:
		vread();
		break;
	case BLAST:
		vblast();
		break;
	case BREAK:
		vbreak();
		break;
	case WAKE:
		vwake();
		break;
	default:
		printf("This verb is not implemented yet.\n");
	}
}

/*
	CARRY TAKE etc.
*/
vtake()
{
	char msg;
	int i;

	if (toting(object)) {
		actspk(verb);
		return;
	}
	/*
	   special case objects and fixed objects
	*/
	msg = 25;
	if (object == PLANT && prop[PLANT] <= 0)
		msg = 115;
	if (object == BEAR && prop[BEAR] == 1)
		msg = 169;
	if (object == CHAIN && prop[BEAR] != 0)
		msg = 170;
	if (fixed[object]) {
		rspeak(msg);
		return;
	}
	/*
	   special case for liquids
	*/
	if (object == WATER || object == OIL) {
		if (!here(BOTTLE) || liq() != object) {
			object = BOTTLE;
			if (toting(BOTTLE)&&prop[BOTTLE] == 1){
				vfill();
				return;
			}
			if (prop[BOTTLE] != 1)
				msg = 105;
			if (!toting(BOTTLE))
				msg = 104;
			rspeak(msg);
			return;
		}
		object = BOTTLE;
	}
	if (holding >= 7) {
		rspeak(92);
		return;
	}
	/*
	   special case for bird.
	*/
	if (object == BIRD && prop[BIRD] == 0) {
		if (toting(ROD)) {
			rspeak(26);
			return;
		}
		if (!toting(CAGE)) {
			rspeak(27);
			return;
		}
		prop[BIRD] = 1;
	}
	if ( (object == BIRD || object == CAGE) &&
		prop[BIRD] != 0)
		carry((BIRD+CAGE)-object,loc);
	carry(object,loc);
	/*
	   handle liquid in bottle
	*/
	i = liq();
	if (object == BOTTLE && i != 0)
		place[i] = -1;
	rspeak(54);
}

/*
	DROP etc.
*/
vdrop()
{
	int i;

	/*
	   check for dynamite
	*/
	if (toting(ROD2) && object == ROD && !toting(ROD))
		object = ROD2;
	if (!toting(object)){
		actspk(verb);
		return;
	}
	/*
	   snake and bird
	*/
	if (object == BIRD && here(SNAKE)) {
		rspeak(30);
		if (closed)
			dwarfend();
		dstroy(SNAKE);
		prop[SNAKE] = -1;
	}
	/*
	   coins and vending machine
	*/
	else if (object == COINS && here(VEND)) {
		dstroy(COINS);
		drop(BATTERIES,loc);
		pspeak(BATTERIES,0);
		return;
	}
	/*
	   bird and dragon (ouch!!)
	*/
	else if (object == BIRD && at(DRAGON) && prop[DRAGON] == 0){
		rspeak(154);
		dstroy(BIRD);
		prop[BIRD] = 0;
		if (place[SNAKE] != 0)
			++tally2;
		return;
	}
	/*
	   Bear and troll
	*/
	if (object == BEAR && at(TROLL)) {
		rspeak(163);
		move(TROLL,0);
		move((TROLL+MAXOBJ),0);
		move(TROLL2,117);
		move((TROLL2+MAXOBJ),122);
		juggle(CHASM);
		prop[TROLL] = 2;
	}
	/*
	   vase
	*/
	else if (object == VASE) {
		if (loc == 96)
			rspeak(54);
		else {
			prop[VASE] = at(PILLOW) ? 0 : 2;
			pspeak(VASE,prop[VASE]+1);
			if (prop[VASE] != 0)
				fixed[VASE] = -1;
		}
	}
	/*
	   handle liquid and bottle
	*/
	i = liq();
	if (i == object)
		object = BOTTLE;
	if (object == BOTTLE && i != 0)
		place[i] = 0;
	/*
	   handle bird and cage
	*/
	if (object == CAGE && prop[BIRD] != 0)
		drop(BIRD,loc);
	if (object == BIRD)
		prop[BIRD] = 0;
	drop(object,loc);
}

/*
	LOCK, UNLOCK, OPEN, CLOSE etc.
*/
vopen()
{
	char msg,oyclam;

	switch(object) {
	case CLAM:
	case OYSTER:
		oyclam = (object == OYSTER ? 1 : 0);
		if (verb == LOCK)
			msg = 61;
		else if (!toting(TRIDENT))
			msg = 122+oyclam;
		else if (toting(object))
			msg = 120+oyclam;
		else {
			msg = 124+oyclam;
			dstroy(CLAM);
			drop(OYSTER,loc);
			drop(PEARL,105);
		}
		break;
	case DOOR:
		msg = (prop[DOOR] == 1 ? 54 : 111);
		break;
	case CAGE:
		msg = 32;
		break;
	case KEYS:
		msg = 55;
		break;
	case CHAIN:
		if (!here(KEYS))
			msg = 31;
		else if (verb == LOCK) {
			if (prop[CHAIN] != 0)
				msg = 34;
			else if (loc != 130)
				msg = 173;
			else {
				prop[CHAIN] = 2;
				if (toting(CHAIN))
					drop(CHAIN,loc);
				fixed[CHAIN] =  -1;
				msg = 172;
			}
		}
		else {
			if (prop[BEAR] == 0)
				msg = 41;
			else if (prop[CHAIN] == 0)
				msg = 37;
			else {
				prop[CHAIN] = 0;
				fixed[CHAIN] = 0;
				if (prop[BEAR] != 3)
					prop[BEAR] = 2;
				fixed[BEAR] = 2-prop[BEAR];
				msg = 171;
			}
		}
		break;
	case GRATE:
		if (!here(KEYS))
			msg = 31;
		else if (closing) {
			if (!panic) {
				clock2 = 15;
				++panic;
			}
			msg = 130;
		}
		else {
			msg = 34+prop[GRATE];
			prop[GRATE] = (verb == LOCK ? 0 : 1);
			msg += 2*prop[GRATE];
		}
		break;
	default:
		msg = 33;
	}
	rspeak(msg);
}

/*
	SAY etc.
*/
vsay()
{
	int wtype,wval;

	analyze(word1,&wtype,&wval);
	printf("Okay.\n%s\n",wval == SAY ? word2 : word1);
}

/*
	ON etc.
*/
von()
{
	if (!here(LAMP))
		actspk(verb);
	else if (limit<0)
		rspeak(184);
	else {
		prop[LAMP] = 1;
		rspeak(39);
		if (wzdark) {
			wzdark = 0;
			describe();
		}
	}
}

/*
	OFF etc.
*/
voff()
{
	if (!here(LAMP))
		actspk(verb);
	else {
		prop[LAMP] = 0;
		rspeak(40);
	}
}

/*
	WAVE etc.
*/
vwave()
{
	if (!toting(object) &&
	   (object != ROD || !toting(ROD2)))
		rspeak(29);
	else if (object != ROD || !at(FISSURE) ||
		!toting(object) || closing)
		actspk(verb);
	else {
		prop[FISSURE] = 1-prop[FISSURE];
		pspeak(FISSURE,2-prop[FISSURE]);
	}
}

/*
	ATTACK, KILL etc.
*/
vkill()
{
	char msg;
	int i;

	switch(object) {
	case BIRD:
		if (closed)
			msg = 137;
		else {
			dstroy(BIRD);
			prop[BIRD] = 0;
			if (place[SNAKE] == 19)
				++tally2;
			msg = 45;
		}
		break;
	case 0:
		msg = 44;
		break;
	case CLAM:
	case OYSTER:
		msg = 150;
		break;
	case SNAKE:
		msg = 46;
		break;
	case DWARF:
		if (closed)
			dwarfend();
		msg = 49;
		break;
	case TROLL:
		msg = 157;
		break;
	case BEAR:
		msg = 165+(prop[BEAR]+1)/2;
		break;
	case DRAGON:
		if (prop[DRAGON] != 0) {
			msg = 167;
			break;
		}
		if (!yes(49,0,0))
			break;
		pspeak(DRAGON,1);
		prop[DRAGON] = 2;
		prop[RUG] = 0;
		move((DRAGON+MAXOBJ),-1);
		move((RUG+MAXOBJ),0);
		move(DRAGON,120);
		move(RUG,120);
		for(i = 1;i<MAXOBJ;++i)
			if (place[i] == 119 || place[i] == 121)
				move(i,120);
		newloc = 120;
		return;
	default:
		actspk(verb);
		return;
	}
	rspeak(msg);
}

/*
	POUR
*/
vpour()
{
	if (object == BOTTLE || object == 0)
		object = liq(0);
	if (object == 0) {
		needobj();
		return;
	}
	if (!toting(object)) {
		actspk(verb);
		return;
	}
	if (object != OIL && object != WATER) {
		rspeak(78);
		return;
	}
	prop[BOTTLE] = 1;
	place[object] = 0;
	if (at(PLANT)) {
		if (object != WATER)
			rspeak(112);
		else {
			pspeak(PLANT,prop[PLANT]+1);
			prop[PLANT] = (prop[PLANT]+2)%6;
			prop[PLANT2] = prop[PLANT]/2;
			describe();
		}
	}
	else if (at(DOOR)) {
		prop[DOOR] = (object == OIL ? 1 : 0);
		rspeak(113+prop[DOOR]);
	}
	else
		rspeak(77);
}

/*
	EAT
*/
veat()
{
	char msg;

	switch(object) {
	case FOOD:
		dstroy(FOOD);
		msg = 72;
		break;
	case BIRD: case SNAKE: case CLAM: case OYSTER:
	case DWARF: case DRAGON: case TROLL: case BEAR:
		msg = 71;
		break;
	default:
		actspk(verb);
		return;
	}
	rspeak(msg);
}

/*
	DRINK
*/
vdrink()
{
	if (object != WATER)
		rspeak(110);
	else if (liq() != WATER || !here(BOTTLE))
		actspk(verb);
	else {
		prop[BOTTLE] = 1;
		place[WATER] = 0;
		rspeak(74);
	}
}

/*
	THROW etc.
*/
vthrow()
{
	char msg;
	int i;

	if (toting(ROD2) && object == ROD && !toting(ROD))
		object = ROD2;
	if (!toting(object)) {
		actspk(verb);
		return;
	}
	/*
	   treasure to troll
	*/
	if (at(TROLL) && object >= 50 && object<MAXOBJ) {
		rspeak(159);
		drop(object,0);
		move(TROLL,0);
		move((TROLL+MAXOBJ),0);
		drop(TROLL2,117);
		drop((TROLL2+MAXOBJ),122);
		juggle(CHASM);
		return;
	}
	/*
	   feed the bears...
	*/
	if (object == FOOD && here(BEAR)) {
		object = BEAR;
		vfeed();
		return;
	}
	/*
	   if not axe, same as drop...
	*/
	if (object != AXE) {
		vdrop();
		return;
	}
	/*
	   AXE is THROWN
	*/
	/*
	   at a dwarf...
	*/
	if (i = dcheck()) {
		msg = 48;
		if (pct(33)) {
			dseen[i] = dloc[i] = 0;
			msg = 47;
			++dkill;
			if (dkill == 1)
				msg = 149;
		}
	}
	/*
	   at a dragon...
	*/
	else if (at(DRAGON) && prop[DRAGON] == 0) 
		msg = 152;
	/*
	   at the troll...
	*/
	else if (at(TROLL))
		msg = 158;
	/*
	   at the bear...
	*/
	else if (here(BEAR) && prop[BEAR] == 0) {
		rspeak(164);
		drop(AXE,loc);
		fixed[AXE] = -1;
		prop[AXE] = 1;
		juggle(BEAR);
		return;
	}
	/*
	   otherwise it is an attack
	*/
	else {
		verb = KILL;
		object = 0;
		itverb();
		return;
	}
	/*
	   handle the left over axe...
	*/
	rspeak(msg);
	drop(AXE,loc);
	describe();
}

/*
	INVENTORY, FIND etc.
*/
vfind()
{
	char msg;
	if (toting(object))
		msg = 24;
	else if (closed)
		msg = 138;
	else if (dcheck() && dflag >= 2 && object == DWARF)
		msg = 94;
	else if (at(object) ||
		(liq() == object && here(BOTTLE)) ||
		object == liqloc(loc))
		msg = 94;
	else {
		actspk(verb);
		return;
	}
	rspeak(msg);
}

/*
	FILL
*/
vfill()
{
	char msg;
	int i;

	switch(object) {
	case BOTTLE:
		if (liq() != 0)
			msg = 105;
		else if (liqloc(loc) == 0)
			msg = 106;
		else {
			prop[BOTTLE] = cond[loc] & WATOIL;
			i = liq();
			if (toting(BOTTLE))
				place[i] = -1;
			msg = (i == OIL ? 108 : 107);
		}
		break;
	case VASE:
		if (liqloc(loc) == 0) {
			msg = 144;
			break;
		}
		if (!toting(VASE)) {
			msg = 29;
			break;
		}
		rspeak(145);
		vdrop();
		return;
	default:
		msg = 29;
	}
	rspeak(msg);
}

/*
	FEED
*/
vfeed()
{
	char msg;

	switch(object) {
	case BIRD:
		msg = 100;
		break;
	case DWARF:
		if (!here(FOOD)) {
			actspk(verb);
			return;
		}
		++dflag;
		msg = 103;
		break;
	case BEAR:
		if (!here(FOOD)) {
			if (prop[BEAR] == 0)
				msg = 102;
			else if (prop[BEAR] == 3)
				msg = 110;
			else {
				actspk(verb);
				return;
			}
		break;
		}
		dstroy(FOOD);
		prop[BEAR] = 1;
		fixed[AXE] = 0;
		prop[AXE] = 0;
		msg = 168;
		break;
	case DRAGON:
		msg = (prop[DRAGON] != 0 ? 110 : 102);
		break;
	case TROLL:
		msg = 182;
		break;
	case SNAKE:
		if (closed || !here(BIRD)) {
			msg = 102;
			break;
		}
		msg = 101;
		dstroy(BIRD);
		prop[BIRD] = 0;
		++tally2;
		break;
	default:
		msg = 14;
	}
	rspeak(msg);
}

/*
	READ etc.
*/
vread()
{
	char msg;
	int	wtype, wval;

	msg = 0;
	if (dark()) {
		analyze(word1, &wtype, &wval);
		printf("I see no %s here.\n",wtype == 1 ? word1 : word2);
		return;
	}
	switch(object) {
	case MAGAZINE:
		msg = 190;
		break;
	case TABLET:
		msg = 196;
		break;
	case MESSAGE:
		msg = 191;
		break;
	case OYSTER:
		if (!toting(OYSTER) || !closed)
			break;
		yes(192,193,54);
		return;
	default:
		;
	}
	if (msg)
		rspeak(msg);
	else
		actspk(verb);
}

/*
	BLAST etc.
*/
vblast()
{
	if (prop[ROD2] < 0 || !closed)
		actspk(verb);
	else {
		bonus = 133;
		if (loc == 115)
			bonus = 134;
		if (here(ROD2))
			bonus = 135;
		rspeak(bonus);
		normend();
	}
}

/*
	BREAK etc.
*/
vbreak()
{
	char msg;
	if (object == MIRROR) {
		msg = 148;
		if (closed) {
			rspeak(197);
			dwarfend();
		}
	}
	else if (object == VASE && prop[VASE] == 0) {
		msg = 198;
		if (toting(VASE))
			drop(VASE,loc);
		prop[VASE] = 2;
		fixed[VASE] = -1;
	}
	else {
		actspk(verb);
		return;
	}
	rspeak(msg);
}

/*
	WAKE etc.
*/
vwake()
{
	if (object != DWARF || !closed)
		actspk(verb);
	else {
		rspeak(199);
		dwarfend();
	}
}

/*
	Routine to speak default verb message
*/
actspk(verb)
int verb;
{
	char i;

	if (verb<1 || verb>31)
		bug(39);
	i = actmsg[verb];
	if (i)
		rspeak(i);
}

/*
	Routine to indicate no reasonable
	object for verb found.  Used mostly by
	intransitive verbs.
*/
needobj()
{
	int wtype,wval;

	analyze(word1,&wtype,&wval);
	printf("%s what?\n",wtype == 2 ? word1 : word2);
}

/*
	Routine to take 1 turn
*/
turn()
{
	char	i;
	/*
		if closing, then he can't leave except via
		the main office.
	*/
	if (newloc < 9 && newloc != 0 && closing) {
		rspeak(130);
		newloc = loc;
		if (!panic)
			clock2 = 15;
		panic = 1;
	}
	/*
		see if a dwarf has seen him and has come
		from where he wants to go.
	*/
	if (newloc != loc && !forced(loc) && cond[loc]&NOPIRAT  ==  0)
		for (i = 1; i< (DWARFMAX-1); ++i)
			if (odloc[i] == newloc && dseen[i]) {
				newloc  =  loc;
				rspeak(2);
				break;
			}

	dwarves();	/* & special dwarf(pirate who steals)	*/

	/* added by BDS C conversion */
	if (loc != newloc) {
		++turns;
		loc = newloc;
/*	causes occasional "move" with two describe & descitem	*/
/*	}	*/			/* if (loc != newloc)	*/

	/* check for death */
	if (loc == 0) {
		death();
		return;
	}

	/* check for forced move */
	if (forced (loc)) {
		describe();
		domove();
		return;
	}

	/* check for wandering in dark */
	if (wzdark && dark() && pct(35)) {
		rspeak(23);
		oldloc2 = loc;
		death();
		return;
	}

	/* describe his situation */
	describe();
	if (!dark ()) {
		++visited[loc];
		descitem();
	}
/*	causes occasional "move" with no describe & descitem	*/
	}				/* if (loc != newloc)	*/

	if (closed) {
		if (prop[OYSTER] < 0 && toting (OYSTER))
			pspeak(OYSTER, 1);
		for (i = 1; i <= MAXOBJ; ++i)
			if (toting (i) && prop[i] < 0)
				prop[i] = -1-prop[i];
	}

	wzdark = dark();
	if (knfloc > 0 && knfloc != loc)
		knfloc  =  0;

	if (stimer())	/* as the grains of sand slip by	*/
		return;

	while (!english())	/* retrieve player instructions	*/
		;

	if (dbugflg)
		printf("loc = %d, verb = %d, object = %d, motion = %d\n", loc, verb, object, motion);

	if (motion)		/* execute player instructions	*/
		domove();
	else if (object)
		doobj();
	else
		itverb();
}

/*
	Routine to describe current location
*/
describe()
{
	if (toting(BEAR))
		rspeak(141);
	if (dark())
		rspeak(16);
	else if (visited[loc])
		descsh(loc);
	else
		desclg(loc);
	if (loc == 33 && pct(25) && !closing)
		rspeak(8);
}

/*
	Routine to describe visible items
*/
descitem()
{
	int	i, state;

	for (i = 1;i<MAXOBJ; ++i) {
		if (at(i)) {
			if (i == STEPS && toting(NUGGET))
				continue;
			if (prop[i]<0) {
				if (closed)
					continue;
				else {
					prop[i] = 0;
					if (i == RUG || i == CHAIN)
						++prop[i];
					--tally;
				}
			}
			if (i == STEPS && loc == fixed[STEPS])
				state = 1;
			else
				state = prop[i];
			pspeak(i, state);
		}
	}
	if (tally == tally2 && tally != 0 && limit > 35)
		limit = 35;
}

/*
	Routine to handle motion requests
*/
domove()
{
	gettrav(loc);
	switch(motion) {
	case NULLX:
		break;
	case BACK:
		goback();
		break;
	case LOOK:
		if (detail++<3)
			rspeak(15);
		wzdark = 0;
		visited[loc] = 0;
		newloc = loc;
		loc = 0;
		break;
	case CAVE:
		if (loc<8)
			rspeak(57);
		else
			rspeak(58);
		break;
	default:
		oldloc2 = oldloc;
		oldloc = loc;
		dotrav();
	}
}

/*
	Routine to handle request to return
	from whence we came!
*/
goback()
{
	int	kk, k2, want, temp;
	struct trav strav[MAXTRAV];

	if (forced(oldloc))
		want = oldloc2;
	else
		want = oldloc;
	oldloc2 = oldloc;
	oldloc = loc;
	k2 = 0;
	if (want == loc) {
		rspeak(91);
		return;
	}
	copytrv(travel, strav);
	for (kk = 0; travel[kk].tdest != -1; ++kk) {
		if (!travel[kk].tcond && travel[kk].tdest == want) {
			motion = travel[kk].tverb;
			dotrav();
			return;
		}
		if (!travel[kk].tcond) {
			k2 = kk;
			temp = travel[kk].tdest;
			gettrav(temp);
			if (forced(temp) && travel[0].tdest == want)
				k2 = temp;
			copytrv(strav, travel);
		}
	}
	if (k2) {
		motion = travel[k2].tverb;
		dotrav();
	}
	else
		rspeak(140);
}

/*
	Routine to copy a travel array
*/
copytrv(trav1, trav2)
struct trav *trav1, *trav2;
{
	int	i;

	for (i = 0; i<MAXTRAV; ++i) {
		trav2 -> tdest = trav1 -> tdest;
		trav2 -> tverb = trav1 -> tverb;
		trav2 -> tcond = trav1 -> tcond;
	}
}

/*
	Routine to figure out a new location
	given current location and a motion.
*/
dotrav()
{
	char	mvflag, hitflag, kk;
	int	rdest, rverb, rcond, robject;
	int	pctt;

	newloc = loc;
	mvflag = hitflag = 0;
	pctt = xrnd()%100;

	for (kk = 0; travel[kk].tdest>=0 && !mvflag; ++kk) {
		rdest = travel[kk].tdest;
		rverb = travel[kk].tverb;
		rcond = travel[kk].tcond;
		robject = rcond%100;

		if (dbugflg)
			printf("rdest = %d, rverb = %d, rcond = %d, robject = %d in dotrav\n", rdest, rverb, rcond, robject);
		if ((rverb != 1) && (rverb != motion) && !hitflag)
			continue;
		++hitflag;
		switch(rcond/100) {
		case 0:
			if ((rcond == 0) || (pctt < rcond))
				++mvflag;
			if (rcond && dbugflg)
				printf("%% move %d %d\n",
					pctt, mvflag);
			break;
		case 1:
			if (robject == 0)
				++mvflag;
			else if (toting(robject))
				++mvflag;
			break;
		case 2:
			if (toting(robject) || at(robject))
				++mvflag;
			break;
		case 3:
		case 4:
		case 5:
		case 7:
			if (prop[robject] != (rcond/100)-3)
				++mvflag;
			break;
		default:
			bug(37);
		}
	}
	if (!mvflag)
		badmove();
	else if (rdest>500)
		rspeak(rdest-500);
	else if (rdest>300)
		spcmove(rdest);
	else {
		newloc = rdest;
		if (dbugflg)
			printf("newloc in dotrav = %d\n", newloc);
	}
}

/*
	The player tried a poor move option.
*/
badmove()
{
	int	msg;

	msg = 12;
	if (motion >= 43 && motion <=50) msg = 9;
	if (motion == 29 || motion == 30) msg = 9;
	if (motion == 7 || motion == 36 || motion == 37) msg = 10;
	if (motion == 11 || motion == 19) msg = 11;
	if (verb == FIND || verb == INVENTORY) msg = 59;
	if (motion == 62 || motion == 65) msg = 42;
	if (motion == 17) msg = 80;
	rspeak(msg);
}

/*
	Routine to handle very special movement.
*/
spcmove(rdest)
int	rdest;
{
	switch(rdest-300) {
	case 1:  /* plover movement via alcove */
		if (!holding || (holding == 1 && toting(EMERALD)))
			newloc = (99+100)-loc;
		else
			rspeak(117);
		break;
	case 2:  /* trying to remove plover, bad route */
		drop(EMERALD, loc);
		break;
	case 3:  /* troll bridge */
		if (prop[TROLL] == 1) {
			pspeak(TROLL, 1);
			prop[TROLL] = 0;
			move(TROLL2, 0);
			move((TROLL2+MAXOBJ), 0);
			move(TROLL, 117);
			move((TROLL+MAXOBJ), 122);
			juggle(CHASM);
			newloc = loc;
		}
		else {
			newloc = (loc == 117 ? 122 : 117);
			if (prop[TROLL] == 0)
				++prop[TROLL];
			if (!toting (BEAR))
				return;
			rspeak(162);
			prop[CHASM] = 1;
			prop[TROLL] = 2;
			drop(BEAR, newloc);
			fixed[BEAR] = -1;
			prop[BEAR] = 3;
			if (prop[SPICES]<0)
				++tally2;
			oldloc2 = newloc;
			death();
		}
		break;
	default:
		bug(38);
	}
}


/*
	Routine to handle player's demise via
	waking up the dwarves...
*/
dwarfend()
{
	death();
	normend();
}

/*
	normal end of game
*/
normend()
{
	score();
	exit(0);
}

/*
	scoring
*/
score()
{
	int	t, i, k, s;
	s = t = 0;
	for (i = 50; i<=MAXTRS; ++i) {
		if (i == CHEST)
			k = 14;
		else if (i > CHEST)
			k = 16;
		else 
			k = 12;
		if (prop[i] >= 0)
			t += 2;
		if (place[i] == 3 && prop[i] == 0)
			t += k-2;
	}
	printf("%-20s%d\n", "Treasures:", s = t);
	t = (MAXDIE - numdie)*10;
	if (t)
		printf("%-20s%d\n", "Survival:", t);
	s += t;
	if (!gaveup)
		s += 4;
	t = dflag ? 25 : 0;
	if (t)
		printf("%-20s%d\n", "Getting well in:", t);
	s += t;
	t = closing ? 25 : 0;
	if (t)
		printf("%-20s%d\n", "Masters section:", t);
	s += t;
	if (closed) {
		if (bonus == 0)
			t = 10;
		else if (bonus == 135)
			t = 25;
		else if (bonus == 134)
			t = 30;
		else if (bonus == 133)
			t = 45;
		printf("%-20s%d\n", "Bonus:", t);
		s += t;
	}
	if (place[MAGAZINE] == 108)
		s += 1;
	s += 2;
	printf("%-20s%d\n", "Score:", s);
}

/*
	Routine to handle the passing on of one
	of the player's incarnations...
*/
death()
{
	char	yea, i, j;

	if (!closing) {
		yea = yes(81+numdie*2, 82+numdie*2, 54);
		if (++numdie >= MAXDIE || !yea)
			normend();
		place[WATER] = 0;
		place[OIL] = 0;
		if (toting(LAMP))
			prop[LAMP] = 0;
		for (j = 1; j<101; ++j) {
			i = 101-j;
			if (toting (i))
				drop(i, i == LAMP ? 1:oldloc2);
		}
		newloc = 3;
		oldloc = loc;
		return;
	}
	/*
	   closing -- no resurrection...
	*/
	rspeak(131);
	++numdie;
	normend();
}

/*
	Routine to process an object.
*/
doobj()
{
	int	wtype, wval;

	/*
	   is object here?  if so, transitive
	*/
	if (fixed[object] == loc || here(object))
		trobj();
	/*
		did he give grate as destination?
	*/
	else if (object == GRATE) {
		if (loc == 1 || loc == 4 || loc == 7) {
			motion = DEPRESSION;
			domove();
		}
		else if (loc>9 && loc<15) {
			motion = ENTRANCE;
			domove();
		}
	}
	/*
		is it a dwarf he is after?
	*/
	else if (dcheck() && dflag >= 2) {
		object = DWARF;
		trobj();
	}
	/*
	   is he trying to get/use a liquid?
	*/
	else if ((liq() == object && here(BOTTLE)) ||
		 liqloc(loc) == object)
		trobj();
	else if (object == PLANT && at(PLANT2) &&
		prop[PLANT2] == 0) {
		object = PLANT2;
		trobj();
	}
	/*
	   is he trying to grab a knife?
	*/
	else if (object == KNIFE && knfloc == loc) {
		rspeak(116);
		knfloc = -1;
	}
	/*
	   is he trying to get at dynamite?
	*/
	else if (object == ROD && here(ROD2)) {
		object = ROD2;
		trobj();
	}
	else  {
/*
		printf("I see no %s here.\n", probj(object));
*/
          analyze(word1,&wtype,&wval);
          printf("I see no %s here.\n",wtype == 1 ? word1 : word2);
       }

}


/*
	Routine to process an object being
	referred to.
*/
trobj()
{
	int	wtype, wval;

	if (verb)
		trverb();
	else  {
/*
		printf("What do you want to do with the %s?\n",
			probj(object));
*/
                analyze(word1,&wtype,&wval);
                printf("What do you want to do with the %s?\n",
                       wtype == 1 ? word1 : word2);
       }
}

/*
	Routine to print word corresponding to object
probj(object)
{
	int	wtype, wval;
	analyze(word1, &wtype, &wval);
	return (wtype == 1 ? word1 : word2);
}
*/

/*
	dwarf stuff.
*/
dwarves()
{
	int	i, j, k, try, attack, stick, dtotal;
	/*
		see if dwarves allowed here
	*/
	if (newloc == 0 || forced(newloc) || cond[newloc]&NOPIRAT)
		return;
	/*
		see if dwarves are active.
	*/
	if (!dflag) {
		if (newloc > 15)
			++dflag;
		return;
	}
	/*
		if first close encounter (of 3rd kind)
		kill 0, 1 or 2
	*/
	if (dflag == 1) {
		if (newloc < 15 || pct (95))
			return;
		++dflag;
		for (i = 1; i<3; ++i)
			if (pct (50))
				dloc[xrnd()%5+1] = 0;
		for (i = 1; i< (DWARFMAX-1); ++i) {
			if (dloc[i] == newloc)
				dloc[i] = daltloc;
			odloc[i] = dloc[i];
		}
		rspeak(3);
		drop(AXE, newloc);
		return;
	}
	dtotal = attack = stick = 0;
	for (i = 1; i<DWARFMAX; ++i) {
		if (dloc[i] == 0)
			continue;
		/*
			move a dwarf at random.  we don't
			have a matrix around to do it
			as in the original version...
		*/
		for (try = 1; try<20; ++try) {
			j = xrnd()%106+15; /* allowed area */
			if (j != odloc[i] && j != dloc[i] &&
			    !(i == (DWARFMAX-1) && cond[j]&NOPIRAT == 1))
				break;
		}
		if (j == 0)
			j = odloc[i];
		odloc[i] = dloc[i];
		dloc[i] = j;
		if ((dseen[i] && newloc >= 15) ||
		    dloc[i] == newloc || odloc[i] == newloc)
			dseen[i] = 1;
		else
			dseen[i] = 0;
		if (!dseen[i])
			continue;
		dloc[i] = newloc;
		if (i == 6)
			dopirate();
		else {
			++dtotal;
			if (odloc[i] == dloc[i]) {
				++attack;
				if (knfloc >= 0)
					knfloc = newloc;
				if (xrnd()%1000 < 95*(dflag-2))
					++stick;
			}
		}
	}
	if (dtotal == 0)
		return;
	if (dtotal > 1)
		printf("There are %d threatening little dwarves in the room with you!\n", dtotal);
	else
		rspeak(4);
	if (attack == 0)
		return;
	if (dflag == 2)
		++dflag;
	if (attack > 1) {
		printf("%d of them throw knives at you!!\n", attack);
		k = 6;
	}
	else {
		rspeak(5);
		k = 52;
	}
	if (stick <= 1) {
		rspeak(stick+k);
		if (stick == 0)
			return;
	}
	else
		printf("%d of them get you !!!\n", stick);
	oldloc2 = newloc;
	death();
}
/*
	pirate stuff
*/
dopirate()
{
	int	j, k;
	if (newloc == chloc || prop[CHEST] >= 0)
		return;
	k = 0;
	for (j = 50; j<=MAXTRS; ++j)
		if (j != PYRAMID ||
		    (newloc != place[PYRAMID] &&
		     newloc != place[EMERALD])) {
			if (toting(j))
				goto stealit;
			if (here(j))
				++k;
		}
	if (tally == tally2+1 && k == 0 && place[CHEST] == 0 &&
	    here(LAMP) && prop[LAMP] == 1) {
		rspeak(186);
		move(CHEST, chloc);
		move(MESSAGE, chloc2);
		dloc[6] = chloc;
		odloc[6] = chloc;
		dseen[6] = 0;
		return;
	}
	if (odloc[6] != dloc[6] && pct(20)) {
		rspeak(127);
		return;
	}
	return;

stealit:

	rspeak(128);
	if (place[MESSAGE] == 0)
		move(CHEST, chloc);
	move(MESSAGE, chloc2);
	for (j = 50; j<=MAXTRS; ++j) {
		if (j == PYRAMID &&
		    (newloc == place[PYRAMID] ||
		     newloc == place[EMERALD]))
			continue;
		if (at(j) && fixed[j] == 0)
			carry(j, newloc);
		if (toting(j))
			drop(j, chloc);
	}
	dloc[6] = chloc;
	odloc[6] = chloc;
	dseen[6] = 0;
}
/*
	special time limit stuff...
*/
stimer()
{
	int	i;
	foobar = foobar > 0 ?  -foobar : 0;
	if (tally == 0 && loc >= 15 && loc != 33)
		--clock1;
	if (clock1 == 0) {
		/*
			start closing the cave
		*/
		prop[GRATE] = 0;
		prop[FISSURE] = 0;
		for (i = 1; i<DWARFMAX; ++i)
			dseen[i] = 0;
		move(TROLL, 0);
		move((TROLL+MAXOBJ), 0);
		move(TROLL2, 117);
		move((TROLL2+MAXOBJ), 122);
		juggle(CHASM);
		if (prop[BEAR] != 3)
			dstroy(BEAR);
		prop[CHAIN] = 0;
		fixed[CHAIN] = 0;
		prop[AXE] = 0;
		fixed[AXE] = 0;
		rspeak(129);
		clock1 = -1;
		closing = 1;
		return(0);
	}
	if (clock1 < 0)
		--clock2;
	if (clock2 == 0) {
		/*
			set up storage room...
			and close the cave...
		*/
		prop[BOTTLE] = put(BOTTLE, 115, 1);
		prop[PLANT] = put(PLANT, 115, 0);
		prop[OYSTER] = put(OYSTER, 115, 0);
		prop[LAMP] = put(LAMP, 115, 0);
		prop[ROD] = put(ROD, 115, 0);
		prop[DWARF] = put(DWARF, 115, 0);
		loc = 115;
		oldloc = 115;
		newloc = 115;
		put(GRATE, 116, 0);
		prop[SNAKE] = put(SNAKE, 116, 1);
		prop[BIRD] = put(BIRD, 116, 1);
		prop[CAGE] = put(CAGE, 116, 0);
		prop[ROD2] = put(ROD2, 116, 0);
		prop[PILLOW] = put(PILLOW, 116, 0);
		prop[MIRROR] = put(MIRROR, 115, 0);
		fixed[MIRROR] = 116;
		for (i = 1; i<= MAXOBJ; ++i)
			if (toting(i))
				dstroy(i);
		rspeak(132);
		closed = 1;
		return(1);
	}
	if (prop[LAMP] == 1)
		--limit;
	if (limit <= 30 &&
	    here(BATTERIES) && prop[BATTERIES] == 0 &&
	    here(LAMP)) {
		rspeak(188);
		prop[BATTERIES] = 1;
		if (toting(BATTERIES))
			drop(BATTERIES, loc);
		limit += 2500;
		lmwarn = 0;
		return(0);
	}
	if (limit == 0) {
		--limit;
		prop[LAMP] = 0;
		if (here(LAMP))
			rspeak(184);
		return(0);
	}
	if (limit < 0 && loc <= 8) {
		rspeak(185);
		gaveup = 1;
		normend();
	}
	if (limit <= 30) {
		if (lmwarn || !here(LAMP))
			return(0);
		lmwarn = 1;
		i = 187;
		if (place[BATTERIES] == 0)
			i = 183;
		if (prop[BATTERIES] == 1)
			i = 189;
		rspeak(i);
		return(0);
	}
	return(0);
}
