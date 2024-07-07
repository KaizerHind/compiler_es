/*  Pawn compiler - Error message system
 *  In fact a very simple system, using only 'panic mode'.
 *
 *  Copyright (c) ITB CompuPhase, 1997-2006
 *
 *  This software is provided "as-is", without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from
 *  the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1.  The origin of this software must not be misrepresented; you must not
 *      claim that you wrote the original software. If you use this software in
 *      a product, an acknowledgment in the product documentation would be
 *      appreciated but is not required.
 *  2.  Altered source versions must be plainly marked as such, and must not be
 *      misrepresented as being the original software.
 *  3.  This notice may not be removed or altered from any source distribution.
 *
 *  Version: $Id: sc5.c 3579 2006-06-06 13:35:29Z thiadmer $
 */

#include <assert.h>
#if defined	__WIN32__ || defined _WIN32 || defined __MSDOS__
  #include <io.h>
#endif
#if defined LINUX || defined __GNUC__
  #include <unistd.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>     /* ANSI standardized variable argument list functions */
#include <string.h>
#if defined FORTIFY
  #include <alloc/fortify.h>
#endif
#include "sc.h"

static char *errmsg[] = {
/*001*/  "\n> Se esperaba un/a \"%s\" y se encontro \"%s\". (001)\n> Te recomendamos revisar dicha linea. (001)\n \n",
/*002*/  "\n> Al parecer has omitido el uso de '{}' dentro del 'case'.. (002)\n> Para insertar funciones en un case, estas deben estar entre llaves. (002)\n \n",
/*003*/  "\n> La variable esta creada de manera local, sin embargo.. (003)\n> no es usada nunca y solo puede ser usada dentro de esta funcion. (003)\n \n",
/*004*/  "\n> El forward \"%s\" no cuenta con o no tiene un callback(public). 004\n \n",
/*005*/  "\n> La funcion 'main(){}' no debe tener parametros de valor. (005)\n \n",
/*006*/  "\n> La posicion de tu Array es incorrecta, al parecer estas.. (006)\n> usando de manera incorrecta la variable. (006)\n \n",
/*007*/  "\n> Este Operador no puede ser re-definido, corrige tu operador. (007)\n \n",
/*008*/  "\n> Expresion invalida; Tu variable/array debe ser constante. (static const(008))\n \n",
/*009*/  "\n> Tu variable Array no parece tener una dimension asginada, este debe ser mayor a 1 o mas. (009)\n \n",
/*010*/  "\n> La funcion declarada no cumple los parametros. (010)\n> Posiblemente estas omitiendo un valor. (010)\n \n",
/*011*/  "\n> Las etiquetas locales y las declaraciones solo son válidas.. (011)\n> si se usan dentro de funciones.(011)\n \n",
/*012*/  "\n> Este simbolo no parece ser alguna tipo de funcion. (012)\n \n",
/*013*/  "\n> No hay entradas de ningun tipo, el contenido esta vacio. (013)\n> Debes definir alguna funcion para comenzar. (013)\n \n",
/*014*/  "\n> Las declaraciones no son validas.(014)\n \n",
/*015*/  "\n> \"case\" y \"default\" solo son validos dentro de una declaración de cambio. (015)\n",
/*016*/  "\n> multiples 'defaults' en \'switch\' (016)\n \n",
/*017*/  "\n> Variable y/o function \"%s\" no ha sido declarada/definida en el Gamemode. (017)\n \n",
/*018*/  "\n> Array excede la dimension de inicializacion, deberia ser '[][]' o solo '[]' (018).\n \n",
/*019*/  "\n> \"%s\" No parece ser una etiqueta y/o label. (019)\n \n",
/*020*/  "\n> Simbolo \"%s\" es invalido; Verifica la linea, seguramente has.. (020)\n> insertado algun caracter de mas. (020)\n \n",
/*021*/  "\n> La variable/funcion \"%s\" ya se encuentra declarada. (021)\n \n",
/*022*/  "\n> Las variables deben ser constantes ya que su valor no se modifica. (022)\n \n",
/*023*/  "\n> No puedes combinar una operación aritmética con el operador '+='. (023)\n \n",
/*024*/  "\n> 'break' o 'continue' estan fuera de contexto y/o usados de una manera incorrecta. (024)\n \n",
/*025*/  "\n> Faltan parametros en tu Callback. (025)\n> Al parecer estas omitiendo un valor en el. (025)\n \n",
/*026*/  "\n> Se encontro '#endif' pero se omitio '#if defined ...' (026)\n \n",
/*027*/  "\n> Al parecer estas usando caracteres de forma incorrecta, verifica tu codigo. (027)\n \n",
/*028*/  "\n> Variable \"%s\" no es una matriz y/o tiene asignado arrays invalidos.\n \n",
/*029*/  "\n> Expresion invalida; Verifica que no estes Agregando y/o ... (029)\n> Omitiendo algo de mas en tu funcion. (029)\n \n",
/*030*/  "\n> No se ha encontrado una llave \"}\" de cierre; Revisa la linea \"%d\".\n \n",
/*031*/  "\n> Se encontro \"#\", pero no tiene ninguna definicion. \n \n",
/*032*/  "\n> La dimension del Array \"%s\" tiene un valor menor a la cantidad que estas usando. (032)\n \n",
/*033*/  "\n> El Array \"%s\" esta mal indexado; No estas usando la cantidad.. (033)\n> correspondiente en la variable del Array. (033)\n \n",
/*034*/  "\n> El argumento \"%d\" esta siendo omitido en la funcion por un \"_\". (034)\n \n",
/*035*/  "\n> Hacen falta parametros en tu funcion. (035)\n \n",
/*036*/  "\n> Pawn no admite una variable como declaracion vacia.. (036)\n> Te recomendamos corregir la variable en como la estes usando. (036)\n \n",
/*037*/  "\n> Haz omitido/agregado una comilla o \"/\"..\n> en un string y/o funcion. (posiblemente no es un string(037))\n \n",
/*038*/  "\n> Al parecer tienes un caracter/simbolo de mas en esta funcion.\n (038)\n",
/*039*/  "\n> No puedes utilizar una constante con el operador sizeof. (039)\n \n",
/*040*/  "\n> Se encontro una ID \"%d\" duplicada en \"case\" label. (040)\n \n",
/*041*/  "\n> Al parecer no has asignado una dimension a la matriz de la variable. (041)\n> Te recomendamos revisar la linea. (041)\n \n",
/*042*/  "\n> Intentas combinar una clase con una funcion; Ej: \"public static\". (042)\n \n",
/*043*/  "character constant exceeds range for packed string\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(043))\n \n",
/*044*/  "\n> Al parecer estas mezclando parametros nombrados y posicionales. (044)\n \n",
/*045*/  "\n> Las funciones en Pawn estan limitadas limitado a 64. (045)\n \n",
/*046*/  "\n> \"%s\" Posee una dimension de Array desconocida. (046)\n> Tampoco parece ser un \"static const\" o estas declarandola mal. (046)\n \n",
/*047*/  "\n> Tu matriz es desconocida; Debes especificar una...\n> matriz con el mismo tamaño y dimensiones. 047\n \n",
/*048*/  "\n> Las dimensiones del Array no coinciden; Intenta borrando un '[]' o.. (048)\n> Verifica bien el array. (048)\n \n",
/*049*/  "invalid line continuation\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(049))\n \n",
/*050*/  "invalid range\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(050))\n \n",
/*051*/  "invalid subscript, use \"[ ]\" operators on major dimensions\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(051))\n \n",
/*052*/  "\n> El Array que intentas usar, supera o esta por debajo de la cantidad.. (052)\n> que almacena la variable; Corrige el Array o verificalo. (052)\n \n",
/*053*/  "\n> El Array excede el maximo numero maximo de numeros, comprueba la variable. (053)\n \n",
/*054*/  "\n> Se encontro un (\"}\") de mas en esta linea, comprueba y borralo. (054) \n \n",
/*055*/  "\n> Has omitido el cuerpo de la funcion del forward. (public(055))\n \n",
/*056*/  "\n>Caracter \"%s\" es invalido en variables y/o argumentos. (056)\n \n",
/*057*/  "unfinished expression before compiler directive\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(057))\n \n",
/*058*/  "duplicate argument; same argument is passed twice\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(058))\n \n",
/*059*/  "\n>Variable \"%s\" no puede tener un valor por.. (059)\n> defecto dentro de sus argumentos. (059)\n \n",
/*060*/  "\n> Dos o más directivas \"#else\" aparecen en el.. (060)\n> cuerpo entre \"#if ... #endif\" coincidentes. (060)\n \n",
/*061*/  "\"#elseif\" directive follows an \"#else\" directive\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(061))\n \n",
/*062*/  "number of operands does not fit the operator\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(062))\n \n",
/*063*/  "function result tag of operator \"%s\" must be \"%s\"\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(063))\n \n",
/*064*/  "cannot change predefined operators\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(064))\n \n",
/*065*/  "function argument may only have a single tag (argument \"%d\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(065))\n \n",
/*066*/  "function argument may not be a reference argument or an array (argument \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(066))\n \n",
/*067*/  "variable cannot be both a reference and an array (variable \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(067))\n \n",
/*068*/  "invalid rational number precision in #pragma\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(068))\n \n",
/*069*/  "rational number format already defined\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(069))\n \n",
/*070*/  "rational number support was not enabled\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(070))\n \n",
/*071*/  "user-defined operator must be declared before use (function \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(071))\n \n",
/*072*/  "\n> No puedes solicitar el tamaño de una funcion. (072)\n>Seguramente estas usando \"sizeof namefunction\", corrige tu codigo. (072)\n \n",
/*073*/  "function argument must be an array (argument \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(073))\n \n",
/*074*/  "\n> Ya existe un \"#define\" delante de este operador, eliminalo. (074)\n \n",
/*075*/  "input line too long (after substitutions)\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(075))\n \n",
/*076*/  "\n> Al parecer tienes un error de Sintaxis en la utilidad de la funcion. (076)\n \n",
/*077*/  "\n> El archivo \"%s\" se ha corrompido debido al encoding UTF-8. (077) \n \n",
/*078*/  "function uses both \"return\" and \"return <value>\"\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(078))\n \n",
/*079*/  "inconsistent return types (array & non-array)\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(079))\n \n",
/*080*/  "unknown symbol, or not a constant symbol (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(080))\n \n",
/*081*/  "cannot take a tag as a default value for an indexed array parameter (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(081))\n \n",
/*082*/  "user-defined operators and native functions may not have states\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(082))\n \n",
/*083*/  "a function or variable may only belong to a single automaton (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(083))\n \n",
/*084*/  "state confliclsct: one of the states is already assigned to another implementation (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(084))\n \n",
/*085*/  "no states are defined for symbol \"%s\"\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(085))\n \n",
/*086*/  "unknown automaton \"%s\"\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(086))\n \n",
/*087*/  "unknown state \"%s\" for automaton \"%s\"\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(087))\n \n",
/*088*/  "public variables and local variables may not have states (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(088))\n \n",
/*089*/  "state variables may not be initialized (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(089))\n \n",
/*090*/  "public functions may not return arrays (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(090))\n \n",
/*091*/  "ambiguous constant; tag override is required (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(091))\n \n",
/*092*/  "functions may not return arrays of unknown size (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(092))\n \n",
/*093*/  "\"__addressof\" operator is invalid in preprocessor expressions\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(093))\n "
};

static char *fatalmsg[] = {
/*100*/  "\n> El include/archivo \"%s\" no pudo ser leido y/o procesado. (100)\n \n",
/*101*/  "\n> El archivo \"%s\" no pudo ser re-escrito y/o editado.\n \n",
/*102*/  "table overflow: \"%s\"\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(102))\n \n",
          /* table can be: loop table
           *               literal table
           *               staging buffer
           *               option table (response file)
           *               peephole optimizer table
           */
/*103*/  "Memoria Insuficiente. (103)\n \n",
/*104*/  "invalid assembler instruction \"%s\"\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(104))\n \n",
/*105*/  "numeric overflow, exceeding capacity\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(105))\n \n",
/*106*/  "\n> El script compilado excede el volumen máximo de memoria(%ld bytes (106))\n \n",
/*107*/  "\n> Al parecer tienes una o varias deficiencias en esta funcion. (107)\n \n",
/*108*/  "codepage mapping file not found\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(108))\n \n",
/*109*/  "La ruta \"%s\" no parece ser valida. (109)\n \n",
/*110*/  "assertion failed: %s\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(110))\n "
/*111*/  "user error: %s\n"
};

static char *warnmsg[] = {
/*200*/  "\n> El simbolo o funcion \"%s\" esta limitado a \"%d\" caracteres. (200)\n \n",
/*201*/  "\n> \"%s\" Ya se encuentra declarado en otra parte del codigo. (201)\n \n",
/*202*/  "\n> La funcion que intentas usar le sobran y/o tiene parametros de mas, compruebalo. (202)\n \n",
/*203*/  "\n> La variable y/o funcion \"%s\" no esta siendo utilizada. (203)\n \n",
/*204*/  "\n> Variable \"%s\" sin uso alguno; Remuevela o comprueba su uso real. (204)\n \n",
/*205*/  "redundant code: constant expression is zero\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(205))\n \n",
/*206*/  "redundant test: constant expression is non-zero\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(206))\n \n",
/*207*/  "\n> \"#pragma\" desconocido; No has añadido nada o esta vacio. (207)\n \n",
/*208*/  "function with tag result used before definition, forcing reparse\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(208))\n \n",
/*209*/  "\n> function y/o command \"%s\" no esta retornando ningun valor. (209) \n \n",
/*210*/  "possible use of symbol before initialization: \"%s\"\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(210))\n \n",
/*211*/  "\n> Asignacion \"=\" incorrecta; Intenta con \"==\". (211)\n \n",
/*212*/  "possibly unintended bitwise operation\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(212))\n \n",
/*213*/  "\n> %s desconocida o vacia%s, al parecer has omitido %s en tu argumento. (213)\n \n",
/*214*/  "possibly a \"const\" array argument was intended: \"%s\"\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(214))\n \n",
/*215*/  "\n> Posiblemente tienes un caracter/simbolo de mas en la funcion. (215)\n> Por lo que esta expresion no causa ningun efecto. (215)\n \n",
/*216*/  "nested comment\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(216))\n \n",
/*217*/  "\n> Parece ser que has omitido algun \"{}\" o \"()\" cerca de esta linea. (217)\n> Posiblemente tambien tu codigo este mal tabulado. (217)\n \n",
/*218*/  "old style prototypes used with optional semicolumns\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(218))\n \n",
/*219*/  "\n> \"%s\" Ya existe de manera local y global, debes eliminar una de las dos. (219)\n \n",
/*220*/  "expression with tag override must appear between parentheses\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(220))\n \n",
/*221*/  "label name \"%s\" shadows tag name\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(221))\n \n",
/*222*/  "number of digits exceeds rational number precision\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(222))\n \n",
/*223*/  "redundant \"sizeof\": argument size is always 1 (symbol \"%s\")\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(223))\n \n",
/*224*/  "\n> Variable \"%s\" tiene una dimension interminado o mal asignado, comprueba la sintaxis. (224)\n \n",
/*225*/  "\n> Al parecer has omitido una llave en un callback, funcion o comando. (225)\n \n",
/*226*/  "\n> Variable \"%s\" esta siendo auto asignada asi misma \"%s = %s\". (226)\n \n",
/*227*/  "more initiallers than enum fields\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(227))\n \n",
/*228*/  "\n> La dimension del Array excede el enum o tu Array no tiene valor asignado. (228)\n \n",
/*229*/  "index tag mismatch (symbol \"%s\"): expected tag %s but found %s\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(229))\n \n",
/*230*/  "no implementation for state \"%s\" in function \"%s\", no fall-back\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(230))\n \n",
/*231*/  "state specification on forward declaration is ignored\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(231))\n \n",
/*232*/  "output file is written, but with compact encoding disabled\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(232))\n \n",
/*233*/  "state variable \"%s\" shadows a global variable\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(233))\n \n",
/*234*/  "function is deprecated (symbol \"%s\") %s\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(234))\n \n",
/*235*/  "\n> El Callback/function \"%s\" es erroneo, no coincide con los nativos. (235)\n \n",
/*236*/  "unknown parameter in substitution (incorrect #define pattern)\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(236))\n \n",
/*237*/  "user warning: %s\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(237))\n \n",
/*238*/  "meaningless combination of class specifiers (%s)\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(238))\n \n",
/*239*/  "literal array/string passed to a non-const parameter\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(238))\n"
};

static char *noticemsg[] = {
/*001*/  "\n> Quizas quisiste decir \'%s\'?\n \n",
/*002*/  "; state variable out of scope\n> ([UC] Por favor informa acerca de este Error/Warn en Unlimited Pawn(238))\n"
};

#define NUM_WARNINGS    (sizeof warnmsg / sizeof warnmsg[0])
static struct s_warnstack {
  unsigned char disable[(NUM_WARNINGS + 7) / 8]; /* 8 flags in a char */
  struct s_warnstack *next;
} warnstack;

static int errflag;
static int errstart;    /* line number at which the instruction started */
static int errline;     /* forced line number for the error message */
static int errwarn;

/*  error
 *
 *  Outputs an error message (note: msg is passed optionally).
 *  If an error is found, the variable "errflag" is set and subsequent
 *  errors are ignored until lex() finds a semicolumn or a keyword
 *  (lex() resets "errflag" in that case).
 *
 *  Global references: inpfname   (reffered to only)
 *                     fline      (reffered to only)
 *                     fcurrent   (reffered to only)
 *                     errflag    (altered)
 */
SC_FUNC int error(long number,...)
{
static char *prefix[3]={ "error", "fatal error", "warning" };
static int lastline,errorcount;
static short lastfile;
  char *msg,*pre;
  va_list argptr;
  char string[128];
  int notice;

  /* split the error field between the real error/warning number and an optional
   * "notice" number
   */
  notice=(unsigned long)number >> (sizeof(long)*4);
  number&=(~(unsigned long)0) >> (sizeof(long)*4);
  assert(number>0 && number<300);

  /* errflag is reset on each semicolon.
   * In a two-pass compiler, an error should not be reported twice. Therefore
   * the error reporting is enabled only in the second pass (and only when
   * actually producing output). Fatal errors may never be ignored.
   */
  if ((errflag || sc_status!=statWRITE) && (number<100 || number>=200))
    return 0;

  /* also check for disabled warnings */
  if (number>=200) {
    int index=(number-200)/8;
    int mask=1 << ((number-200)%8);
    if ((warnstack.disable[index] & mask)!=0)
      return 0;
  } /* if */

  if (number<100) {
    assert(number>0 && number<(1+sizeof(errmsg)/sizeof(errmsg[0])));
    msg=errmsg[number-1];
    pre=prefix[0];
    errflag=TRUE;       /* set errflag (skip rest of erroneous expression) */
    errnum++;
  } else if (number<200) {
    assert(number>=100 && number<(100+sizeof(fatalmsg)/sizeof(fatalmsg[0])));
    msg=fatalmsg[number-100];
    pre=prefix[1];
    errnum++;           /* a fatal error also counts as an error */
  } else if (errwarn) {
    assert(number>=200 && number<(200+sizeof(warnmsg)/sizeof(warnmsg[0])));
    msg=warnmsg[number-200];
    pre=prefix[0];
    errflag=TRUE;
    errnum++;
  } else {
    assert(number>=200 && number<(200+sizeof(warnmsg)/sizeof(warnmsg[0])));
    msg=warnmsg[number-200];
    pre=prefix[2];
    warnnum++;
  } /* if */

  if (notice!=0) {
    assert(notice>0 && notice<(1+sizeof(noticemsg)/sizeof(noticemsg[0])) && noticemsg[notice-1][0]!='\0');
    strcpy(string,msg);
    strcpy(&string[strlen(string)-1],noticemsg[notice-1]);
    msg=string;
  } /* if */

  assert(errstart<=fline);
  if (errline>0)
    errstart=errline;
  else
    errline=fline;
  assert(errstart<=errline);
  va_start(argptr,number);
  if (errfname[0]=='\0') {
    int start=(errstart==errline) ? -1 : errstart;
    if (pc_error((int)number,msg,inpfname,start,errline,argptr)) {
      if (outf!=NULL) {
        pc_closeasm(outf,TRUE);
        outf=NULL;
      } /* if */
      longjmp(errbuf,3);        /* user abort */
    } /* if */
  } else {
    FILE *fp=fopen(errfname,"a");
    if (fp!=NULL) {
      if (errstart>=0 && errstart!=errline)
        fprintf(fp,"%s(%d -- %d) : %s %03d: ",inpfname,errstart,errline,pre,(int)number);
      else
        fprintf(fp,"%s(%d) : %s %03d: ",inpfname,errline,pre,(int)number);
      vfprintf(fp,msg,argptr);
      fclose(fp);
    } /* if */
  } /* if */
  va_end(argptr);

  if ((number>=100 && number<200) || errnum>25){
    if (errfname[0]=='\0') {
      va_start(argptr,number);
      pc_error(0,"\nCompilation aborted.\n\n",NULL,0,0,argptr);
      va_end(argptr);
    } /* if */
    if (outf!=NULL) {
      pc_closeasm(outf,TRUE);
      outf=NULL;
    } /* if */
    longjmp(errbuf,2);          /* fatal error, quit */
  } /* if */

  errline=-1;
  /* check whether we are seeing many errors on the same line */
  if ((errstart<0 && lastline!=fline) || lastline<errstart || lastline>fline || fcurrent!=lastfile)
    errorcount=0;
  lastline=fline;
  lastfile=fcurrent;
  if (number<200 || errwarn)
    errorcount++;
  if (errorcount>=3)
    error(107);         /* too many error/warning messages on one line */

  return 0;
}

SC_FUNC void errorset(int code,int line)
{
  switch (code) {
  case sRESET:
    errflag=FALSE;      /* start reporting errors */
    break;
  case sFORCESET:
    errflag=TRUE;       /* stop reporting errors */
    break;
  case sEXPRMARK:
    errstart=fline;     /* save start line number */
    break;
  case sEXPRRELEASE:
    errstart=-1;        /* forget start line number */
    errline=-1;
    break;
  case sSETPOS:
    errline=line;
    break;
  } /* switch */
}

/* pc_enablewarning()
 * Enables or disables a warning (errors cannot be disabled).
 * Initially all warnings are enabled. The compiler does this by setting bits
 * for the *disabled* warnings and relying on the array to be zero-initialized.
 *
 * Parameter enable can be:
 *  o  0 for disable
 *  o  1 for enable
 *  o  2 for toggle
 */
int pc_enablewarning(int number,int enable)
{
  int index;
  unsigned char mask;

  if (number<200)
    return FALSE;       /* errors and fatal errors cannot be disabled */
  number-=200;
  if (number>=NUM_WARNINGS)
    return FALSE;

  index=number/8;
  mask=(unsigned char)(1 << (number%8));
  switch (enable) {
  case 0:
    warnstack.disable[index] |= mask;
    break;
  case 1:
    warnstack.disable[index] &= (unsigned char)~mask;
    break;
  case 2:
    warnstack.disable[index] ^= mask;
    break;
  } /* switch */

  return TRUE;
}

/* pc_pushwarnings()
 * Saves currently disabled warnings, used to implement #pragma warning push
 */
int pc_pushwarnings()
{
  void *p;
  p=calloc(sizeof(struct s_warnstack),1);
  if (p==NULL) {
    error(103); /* insufficient memory */
    return FALSE;
  }
  memmove(p,&warnstack,sizeof(struct s_warnstack));
  warnstack.next=p;
  return TRUE;
}

/* pc_popwarnings()
 * This function is the reverse of pc_pushwarnings()
 */
int pc_popwarnings()
{
  void *p;
  if (warnstack.next==NULL)
    return FALSE; /* nothing to do */
  p=warnstack.next;
  memmove(&warnstack,p,sizeof(struct s_warnstack));
  free(p);
  return TRUE;
}

/* pc_seterrorwarnings()
 * Make warnings errors (or not).
 */
void pc_seterrorwarnings(int enable)
{
  errwarn = enable;
}

int pc_geterrorwarnings()
{
  return errwarn;
}

/* Implementation of Levenshtein distance, by Lorenzo Seidenari
 */
static int minimum(int a,int b,int c)
{
  int min=a;
  if(b<min)
    min=b;
  if(c<min)
    min=c;
  return min;
}

static int levenshtein_distance(const char *s,const char*t)
{
  //Step 1
  int k,i,j,cost,*d,distance;
  int n=strlen(s);
  int m=strlen(t);
  assert(n>0 && m>0);
  d=(int*)malloc((sizeof(int))*(m+1)*(n+1));
  m++;
  n++;
  //Step 2
  for (k=0;k<n;k++)
    d[k]=k;
  for (k=0;k<m;k++)
    d[k*n]=k;
  //Step 3 and 4
  for (i=1;i<n;i++) {
    for (j=1;j<m;j++) {
      //Step 5
      cost= (tolower(s[i-1])!=tolower(t[j-1]));
      //Step 6
      d[j*n+i]=minimum(d[(j-1)*n+i]+1,d[j*n+i-1]+1,d[(j-1)*n+i-1]+cost);
    } /* for */
  } /* for */
  distance=d[n*m-1];
  free(d);
  return distance;
}

static int get_max_dist(const char *name)
{
  int max_dist=strlen(name)/2; /* for short names, allow only a single edit */
  if (max_dist>MAX_EDIT_DIST)
    max_dist=MAX_EDIT_DIST;
  return max_dist;
}

static int find_closest_symbol_table(const char *name,const symbol *root,int symboltype,symbol **closest_sym)
{
  int dist,max_dist,closest_dist=INT_MAX;
  char symname[2*sNAMEMAX+16];
  symbol *sym;
  assert(closest_sym!=NULL);
  *closest_sym =NULL;
  assert(name!=NULL);
  max_dist=get_max_dist(name);
  for (sym=root->next; sym!=NULL; sym=sym->next) {
    if (sym->fnumber!=-1 && sym->fnumber!=fcurrent)
      continue;
    if ((sym->usage & uDEFINE)==0 && (sym->ident!=iFUNCTN || (sym->usage & (uNATIVE | uPROTOTYPED))!=uPROTOTYPED))
      continue;
    switch (sym->ident)
    {
    case iLABEL:
      if ((symboltype & esfLABEL)==0)
        continue;
      break;
    case iCONSTEXPR:
      if ((symboltype & esfCONST)==0)
        continue;
      break;
    case iVARIABLE:
    case iREFERENCE:
      if ((symboltype & esfVARIABLE)==0)
        continue;
      break;
    case iARRAY:
    case iREFARRAY:
      if ((symboltype & esfARRAY)==0)
        continue;
      break;
    case iFUNCTN:
    case iREFFUNC:
      if ((symboltype & (((sym->usage & uNATIVE)!=0) ? esfNATIVE : esfFUNCTION))==0)
        continue;
      break;
    default:
      assert(0);
    } /* switch */
    funcdisplayname(symname,sym->name);
    if (symname[0] == '\0')
      continue;
    dist=levenshtein_distance(name,symname);
    if (dist>max_dist || dist>=closest_dist)
      continue;
    *closest_sym=sym;
    closest_dist=dist;
    if (closest_dist<=1)
      break;
  } /* for */
  return closest_dist;
}

static symbol *find_closest_symbol(const char *name,int symboltype)
{
  symbol *symloc,*symglb;
  int distloc,distglb;

  if (sc_status==statFIRST)
    return NULL;
  assert(name!=NULL);
  if (name[0]=='\0')
    return NULL;
  distloc=find_closest_symbol_table(name,&loctab,symboltype,&symloc);
  if (distloc<=1)
    distglb=INT_MAX; /* don't bother searching in the global table */
  else
    distglb=find_closest_symbol_table(name,&glbtab,symboltype,&symglb);
  return (distglb<distloc) ? symglb : symloc;
}

static constvalue *find_closest_automaton(const char *name)
{
  constvalue *ptr=sc_automaton_tab.first;
  constvalue *closest_match=NULL;
  int dist,max_dist,closest_dist=INT_MAX;

  assert(name!=NULL);
  max_dist=get_max_dist(name);
  while (ptr!=NULL) {
    if (ptr->name[0]!='\0') {
      dist=levenshtein_distance(name,ptr->name);
      if (dist<closest_dist && dist<=max_dist) {
        closest_match=ptr;
        closest_dist=dist;
        if (closest_dist<=1)
          break;
      } /* if */
    } /* if */
    ptr=ptr->next;
  } /* while */
  return closest_match;
}

static constvalue *find_closest_state(const char *name,int fsa)
{
  constvalue *ptr=sc_state_tab.first;
  constvalue *closest_match=NULL;
  int dist,max_dist,closest_dist=INT_MAX;

  assert(name!=NULL);
  max_dist=get_max_dist(name);
  while (ptr!=NULL) {
    if (ptr->index==fsa && ptr->name[0]!='\0') {
      dist=levenshtein_distance(name,ptr->name);
      if (dist<closest_dist && dist<=max_dist) {
        closest_match=ptr;
        closest_dist=dist;
        if (closest_dist<=1)
          break;
      } /* if */
    } /* if */
    ptr=ptr->next;
  } /* while */
  return closest_match;
}

static constvalue *find_closest_automaton_for_state(const char *statename,int fsa)
{
  constvalue *ptr=sc_state_tab.first;
  constvalue *closest_match=NULL;
  constvalue *automaton;
  const char *fsaname;
  int dist,max_dist,closest_dist=INT_MAX;

  assert(statename!=NULL);
  max_dist=get_max_dist(statename);
  automaton=automaton_findid(ptr->index);
  assert(automaton!=NULL);
  fsaname=automaton->name;
  while (ptr!=NULL) {
    if (fsa!=ptr->index && ptr->name[0]!='\0' && strcmp(statename,ptr->name)==0) {
      automaton=automaton_findid(ptr->index);
      assert(automaton!=NULL);
      dist=levenshtein_distance(fsaname,automaton->name);
      if (dist<closest_dist && dist<=max_dist) {
        closest_match=automaton;
        closest_dist=dist;
        if (closest_dist<=1)
          break;
      } /* if */
    } /* if */
    ptr=ptr->next;
  } /* while */
  return closest_match;
}

SC_FUNC int error_suggest(int number,const char *name,const char *name2,int type,int subtype)
{
  char string[sNAMEMAX*2+2]; /* for "<automaton>:<state>" */
  const char *closest_name=NULL;
  symbol *closest_sym;

  /* don't bother finding the closest names on errors
   * that aren't going to be shown on the 1'st pass
   */
  if ((errflag || sc_status!=statWRITE) && (number<100 || number>=200))
    return 0;

  if (type==estSYMBOL) {
  find_symbol:
    closest_sym=find_closest_symbol(name,subtype);
    if (closest_sym!=NULL) {
      closest_name=closest_sym->name;
      if ((subtype & esfVARIABLE)!=0 && closest_sym->states!=NULL && strcmp(closest_name,name)==0) {
        assert(number==17); /* undefined symbol */
        error(makelong(number,2),name);
        return 0;
      } /* if */
    } /* if */
  } else if (type==estNONSYMBOL) {
    if (tMIDDLE<subtype && subtype<=tLAST) {
      extern char *sc_tokens[];
      name=sc_tokens[subtype-tFIRST];
      subtype=esfVARCONST;
      goto find_symbol;
    } /* if */
  } else if (type==estAUTOMATON) {
    constvalue *closest_automaton=find_closest_automaton(name);
    if (closest_automaton!=NULL)
      closest_name=closest_automaton->name;
  } else if (type==estSTATE) {
    constvalue *closest_state=find_closest_state(name,subtype);
    if (closest_state!=NULL) {
      closest_name=closest_state->name;
    } else {
      constvalue *closest_automaton=find_closest_automaton_for_state(name,subtype);
      if (closest_automaton!=NULL) {
        sprintf(string,"%s:%s",closest_automaton->name,name);
        closest_name=string;
      } /* if */
    } /* if */
  } else {
    assert(0);
  } /* if */

  if (closest_name==NULL) {
    error(number,name,name2);
  } else if (name2!=NULL) {
    error(makelong(number,1),name,name2,closest_name);
  } else {
    error(makelong(number,1),name,closest_name);
  } /* if */
  return 0;
}
