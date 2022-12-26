/******************************************************
*     Decompiled by Mark Ogden & Andrey Nikitin       *
* Adapted to 128 KB RAM RC2014's by Ladislau Szilagyi *
*		    December 2022		      *
******************************************************/

#include "p1.h"

uint8_t yylex(void);
void sub_51cf(register sym_t *st);
void prError(int, ...);

/**************************************************
 * 135: 5DD1 PMO +++
 * use of uint8_t arg
 * some optimisations different some better some worse
 **************************************************/
uint8_t sub_5dd1(uint8_t *pscType, register s8_t *attr) {
    uint8_t scType;
    uint8_t dataType;
    int16_t sizeIndicator; /* -1 short, 0 int, 1 long */
    bool isUnsigned;
    uint8_t tok;
    uint8_t scFlags; /* storage class */
    sym_t *var9;
    sym_t *ps;
    struct _s8 tmp;

    attr->i4        = 0;
    attr->i_sym     = 0;
    attr->i_nextSym = (sym_t*)NULL;
    attr->c7        = 0;
    scType = dataType = 0;
    sizeIndicator     = 0;
    isUnsigned        = false;
    scFlags           = 0;

    for (;;) {
        if ((tok = yylex()) == (char)S_CLASS) {
            if (pscType == NULL)
                prError(31);
            else {
                switch (tok = yylval.yVal) {
                case T_REGISTER:
                    scFlags |= 4;
                    break;
                case T_AUTO:
                    if (!depth)
                        prError(30);
                    /* FALLTHRU */
                case T_STATIC:
                case T_TYPEDEF:
                    scFlags |= 1;
                    /* FALLTHRU */
                case T_EXTERN:
                    if (scType && scType != tok)
                        prError(32);
                    else
                        scType = tok;
                    break;
                }
            }
        } else if (tok == (char)S_TYPE) { /* 5e78 */
            switch (tok = yylval.yVal) {
            case T_SHORT:
                sizeIndicator--;
                break;
            case T_LONG:
                sizeIndicator++;
                break;
            case T_UNSIGNED:
                isUnsigned = true;
                break;
            case T_UNION:
                dataType        = DT_UNION;
                attr->i_nextSym = sub_60db(D_UNION);
                if (attr->i_nextSym)
                    sub_51cf(attr->i_nextSym);
                break;
            case T_STRUCT:
                dataType        = DT_STRUCT;
                attr->i_nextSym = sub_60db(D_STRUCT);
                if (attr->i_nextSym)
                    sub_51cf(attr->i_nextSym);
                break;
            case T_ENUM:
                dataType        = DT_ENUM;
                attr->i_nextSym = sub_6360();
                sub_51cf(attr->i_nextSym);
                break;
            case T_CHAR:
            case T_DOUBLE:
            case T_FLOAT:
            case T_INT:
            case T_VOID:
                if (dataType)
                    prError(29);
                else
                    dataType = tok == T_INT     ? DT_INT
                               : tok == T_CHAR  ? DT_CHAR
                               : tok == T_VOID  ? DT_VOID
                               : tok == T_FLOAT ? DT_FLOAT
                                                : DT_DOUBLE;
                break;
            }
        } else if (tok == (char)T_ID &&
		 /* yylval.ySym->m20 */ GetByte((char*)(yylval.ySym)+OFF_m20) == (char)T_TYPEDEF &&
		 dataType == (char)0) { /* 5f68 */
            ps = yylval.ySym;
            sub_51cf(ps);
            var9 = ps;
            if (/* var9->a_c7 */ GetByte((char*)var9+OFF_s8_t_c7)) {
                dataType        = DT_POINTER;
                attr->i_nextSym = ps;
            } else 
	    {
		ReadMem((char*)&tmp, (char*)var9, sizeof(struct _s8));
                dataType = /* var9->a_dataType */ tmp.dataType;
                attr->u1 = /* var9->attr.u1; */ tmp.u1;  
                attr->u2 = /* var9->attr.u2; */ tmp.u2;
                attr->i4 = /* var9->attr.i4; */ tmp.i4;
            }
        } else
            break;

    } /* 6003 */
    ungetTok = tok;
    if (scType == (char)0) {
        scType = depth ? byte_a299 : T_EXTERN;
        scFlags |= 1;
    }
    if ((scFlags & 4) &&
	 scType != (char)T_AUTO &&
	 scType != (char)D_6 &&
	 scType != (char)D_14 &&
	 scType != (char)D_15)
        prError(28);
    if (dataType == (char)0)
        dataType = DT_INT;
    if (sizeIndicator > 0) {
        if (dataType == (char)DT_FLOAT || dataType == (char)DT_INT)
            dataType += 2; /* to DT_DOUBLE or DT_LONG*/
        else
            prError(27);
    } else if (sizeIndicator < 0) {
        if (dataType == (char)DT_INT)
            dataType = DT_SHORT;
        else
            prError(26);
    }
    if (isUnsigned) {
        if (dataType < (unsigned char)DT_FLOAT)
            dataType |= 1;
        else
            prError(25);
    }
    if (pscType)
        *pscType = scType;
    attr->dataType = dataType;
    return scFlags;
}
