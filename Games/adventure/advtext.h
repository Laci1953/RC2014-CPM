
/*      header: ADVTEXT.H                                       */


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

