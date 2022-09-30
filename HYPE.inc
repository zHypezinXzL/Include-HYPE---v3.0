/* * * * * * * * * * * * *( HYPE 3.0 )* * * * * * * * * * * * *\
 *                                                            *
 *          (C) Copyright 2022 by zHypezinXzL (R)             *
 *                                                            *
 * @country  : Brasil                                         *
 * @author   : zHypezinXzL                                    *
 * @github   : https://github.com/zHypezinXzL                 *
 * @date     : 7th September 2022 (Independence Brasil)       *
 * @update   : 30th September 2022                            *
 *                               							  *
 *			     	    Improved Speed						  *
 *                New Function ( HYPE_FCopy )                 *
 *             New Function ( HYPE_FMakeBackup )              *
 *                                                            *
 *     Time to Drop DOF2 and Dini and start using HYPE        *
 *                                                            *
\* * * * * * * * * * * * *( HYPE 3.0 )* * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <a_samp>             /*  Dowload:  https://github.com/zHypezinXzL/include-a_samp.git  */
#include <string>             /*  Dowload:  https://github.com/zHypezinXzL/include-string.git  */
#include <float>              /*  Dowload:  https://github.com/zHypezinXzL/include-float.git   */
#include <file>               /*  Dowload:  https://github.com/zHypezinXzL/include-file.git    */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * */
#if defined _hype_included
    #endinput
#endif
#define _hype_included
#pragma library hype
/* * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * */
#if defined MAX_LINE 
    #define HYPE_MAX_LINE MAX_LINE
#else 
    #define HYPE_MAX_LINE (250)
#endif
/* * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#define HYPE_FExists fexist
#define HYPE_FRemove fremove
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FCreate(const file[])
{
	if(fexist(file)) return false;
	new File:fnew = fopen(file, io_write);
	if(fnew) return fclose(fnew);
	else return false;
}
stock HYPE_FCopy(const fcopy[], const fnecklace[])
{
	if(fexist(fnecklace) || !fexist(fcopy)) return false;
	else return HYPE_FCopyText(fcopy, fnecklace);
}
stock HYPE_FMakeBackup(const file[])
{
	new fbackup[HYPE_MAX_LINE], fdate[3]; getdate(fdate[0], fdate[1], fdate[2]);
    format(fbackup, sizeof fbackup, "%s.backup[%02d-%02d-%02d]", file, fdate[2], fdate[1], fdate[0]);
    return HYPE_FCopy(file, fbackup);
}
stock HYPE_FRename(const oldname[], const newname[])
{
	if(fexist(newname)) return false;
    new File:fnew = fopen(newname, io_write);
    if(fnew) return HYPE_FCopyText(oldname, newname), fclose(fnew), fremove(oldname);
	return false;
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FIsSet(const file[], const key[])
{
	if(strlen(key) == 0 || (strlen(key)+2) > HYPE_MAX_LINE) return false;
	new File:firh = fopen(file, io_read), fsec[HYPE_MAX_LINE];
	if(!firh) return false;
	while(fread(firh, fsec))
    {
		if(fsec[strlen(key)] == '=' && !strcmp(fsec, key, true, strlen(key))) return true, fclose(firh);
	}
	return fclose(firh);
}
stock HYPE_FUnSet(const file[], const key[])
{
	if(strlen(key) == 0 || (strlen(key)+2) > HYPE_MAX_LINE) return false;
	new File:firh, File:fiwh, fsec[HYPE_MAX_LINE];
	format(fsec, HYPE_MAX_LINE, "%s.hype", file); fremove(fsec); firh = fopen(file, io_read);
	if(!firh) return false; fiwh = fopen(fsec, io_write);
	if(!fiwh) return false; fclose(firh);
	while(fread(firh, fsec))
	{
		if(fsec[strlen(key)] == '=' && !strcmp(fsec, key, true, strlen(key))) { } 
		else HYPE_FNewLine(fsec), fwrite(fiwh, fsec); fwrite(fiwh, "\r\n");
	}
	fclose(firh); fclose(fiwh); format(fsec, HYPE_MAX_LINE, "%s.hype", file);
	if(HYPE_FCopyText(fsec, file)) return fremove(fsec);
	return false;
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetString(const file[], const key[], const string[])
{
	if(strlen(key) == 0 || (strlen(key)+strlen(string)+2) > HYPE_MAX_LINE) return false;
	new File:firh, File:fiwh, bool:configured = false, fsec[HYPE_MAX_LINE];
	format(fsec, sizeof fsec, "%s.hype", file); fremove(fsec); firh = fopen(file, io_read);
	if(!firh) return false; fiwh = fopen(fsec, io_write);
	if(!fiwh) return fclose(firh), fclose(fiwh); 
	while(fread(firh, fsec)) 
	{
		if(!configured && fsec[strlen(key)] == '=' && !strcmp(fsec, key, true, strlen(key))) format(fsec, sizeof fsec,"%s=%s", key, string), configured = true;
		else HYPE_FNewLine(fsec);
		fwrite(fiwh, fsec); fwrite(fiwh, "\r\n");
	}
	if(!configured) format(fsec, sizeof fsec,"%s=%s", key, string), fwrite(fiwh, fsec), fwrite(fiwh, "\r\n");
	fclose(firh); fclose(fiwh); format(fsec, sizeof fsec, "%s.hype", file);
	if(HYPE_FCopyText(fsec, file)) return fremove(fsec);
	return false;
}
stock HYPE_FGetString(const file[], const key[])
{
	new fsec[HYPE_MAX_LINE];
	if(strlen(key) == 0 || (strlen(key)+2) > HYPE_MAX_LINE) return fsec;
	new File:firh = fopen(file, io_read);
	if(!firh) return fsec;
	while(fread(firh, fsec))
    {
		if(fsec[strlen(key)] == '=' && !strcmp(fsec, key, true, strlen(key))) 
        {
			HYPE_FNewLine(fsec); strmid(fsec, fsec, (strlen(key) + 1), strlen(fsec), HYPE_MAX_LINE);
			fclose(firh); return fsec;
		}
	}
	fclose(firh);
	return fsec;
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetInt(const file[], const key[], int)
{
    new Int[HYPE_MAX_LINE]; valstr(Int, int);
    return HYPE_FSetString(file, key, Int);
}
stock HYPE_FGetInt(const file[], const key[])
{
    return strval(HYPE_FGetString(file, key)); 
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetFloat(const file[], const key[], Float:float)
{
    new Float[HYPE_MAX_LINE]; format(Float, sizeof Float, "%f", float);
    return HYPE_FSetString(file, key, Float);
}
stock Float:HYPE_FGetFloat(const file[], const key[])
{
    return floatstr(HYPE_FGetString(file, key));
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetBool(const file[], const key[], bool:bool)
{
	if(bool == true) return HYPE_FSetString(file, key, "true");
    else return HYPE_FSetString(file, key, "false");
}
stock bool:HYPE_FGetBool(const file[], const key[])
{
	if(strcmp(HYPE_FGetString(file, key), "true") == 0) return true;
	else return false;
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FSetHex(const file[], const key[],  const hex[])
{
	if(!HYPE_FValidHex(hex)) return false;
	return HYPE_FSetString(file, key, hex);
}
stock HYPE_FGetHex(const file[], const key[])
{
    return HYPE_FHexToInt(HYPE_FGetString(file, key));
} 
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
stock HYPE_FValidHex(const hex[])
{
	if(strlen(hex) != 10) return false;
	new Mid[50], Mtt[50], c = 2; strmid(Mid, hex, 0, 2, sizeof Mid);
    if(strcmp(Mid, "0x", true) == 0)
    {
        while(c < 11)
        {
            strmid(Mtt, hex, c, c+1, sizeof Mtt);
            if(strcmp(Mtt, "0") || strcmp(Mtt, "1") || strcmp(Mtt, "2") || strcmp(Mtt, "3") ||
               strcmp(Mtt, "4") || strcmp(Mtt, "5") || strcmp(Mtt, "6") || strcmp(Mtt, "7") ||
               strcmp(Mtt, "8") || strcmp(Mtt, "9") || strcmp(Mtt, "a") || strcmp(Mtt, "b") ||
               strcmp(Mtt, "c") || strcmp(Mtt, "d") || strcmp(Mtt, "e") || strcmp(Mtt, "f") )
               {
				   c++;
			       if(c == 10) return true; 
			   }
               else return false;
        }
    }
    return false;
}
/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
HYPE_FHexToInt(string[])  // const return error / no const return sucess / Code by Forum SA-MP
{
	if(string[0] == 0) return false;
	new i = strlen(string), cur = 1, res = 0;
	while(i > 0) 
	{
		if(string[i - 1] < 58) res = res + cur*(string[i - 1] - 48); 
		else res = res + cur*(string[i - 1] - 65 + 10); cur = cur*16;
	    i--;
  	}
  	return res;
} 
HYPE_FNewLine(code[])  // const return error / no const return sucess
{
	new size = strlen(code);
	if(code[0] == 0) return ;
	if((code[size-1] == '\n') || (code[size - 1] == '\r'))
    {
		code[size - 1] = 0;
		if(code[0] == 0) return ;
		else if((code[size - 2] == '\n') || (code[size - 2] == '\r'))
        {
            code[size - 2] = 0;
            if(code[0] == 0) return ;
        }
	}
}
HYPE_FCopyText(const oldname[], const newname[])
{
	new File:firh = fopen(oldname, io_read), File:fiwh = fopen(newname, io_write);
	if(!firh) return false;
	if(!fiwh) return fclose(firh);
	new fsec[HYPE_MAX_LINE];
	while(fread(firh, fsec))
    {
		HYPE_FNewLine(fsec); format(fsec, sizeof fsec, "%s\r\n", fsec);
		fwrite(fiwh, fsec);
	}
	return fclose(firh), fclose(fiwh);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * /
native <--=[C]=-->
native HYPE_FCopy(const fcopy[], const fnecklace[]);                        // FCO ( New )
native HYPE_FCreate(const file[]);                                          // FCR

native <--=[E]=-->
native HYPE_FExists(const file[]);                                          // FE

native <--=[G]=-->
native HYPE_FGetBool(const file[], const key[]);                            // FGB
native HYPE_FGetFloat(const file[], const key[]);                           // FGF
native HYPE_FGetHex(const file[], const key[]);                             // FGH
native HYPE_FGetInt(const file[], const key[]);                             // FGI
native HYPE_FGetString(const file[], const key[]);                          // FGS

native <--=[I]=-->
native HYPE_FIsSet(const file[], cosnt key[]);                              // FI

native <--=[M]=-->
native HYPE_FMakeBackup(const file[]);										// FM  ( New )

native <--=[S]=-->
native HYPE_FSetBool(const file[], const key[], bool:bool);                 // FSB
native HYPE_FSetFloat(const file[], const key[], Float:float);              // FSF
native HYPE_FSetHex(const file[], const key, const hex[]);                  // FSH 
native HYPE_FSetInt(const file[], const key[], int);                        // FSI
native HYPE_FSetString(const file[], const key[], const str[]);             // FSS

native <--=[R]=-->
native HYPE_FRemove(const file[]);                                          // FRM
native HYPE_FRename(const file[]);                                          // FRN 

native <--=[U]=-->
native HYPE_FUnSet(const file[], const key[]);                              // FU

native <--=[V]=-->
native HYPE_FValidHex(const hex[]);											// FV
/ * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */     
