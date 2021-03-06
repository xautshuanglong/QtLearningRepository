/***** This file contains automatically generated code ******
**
** The code in this file has been automatically generated by
**
**   sqlite/tool/mkkeywordhash.c
**
** The code in this file implements a function that determines whether
** or not a given identifier is really an SQL keyword.  The same thing
** might be implemented more directly using a hand-written hash table.
** But by using this automatically generated code, the size of the code
** is substantially reduced.  This is important for embedded applications
** on platforms with limited memory.
*/
/* Hash score: 214 */
/* zKWText[] encodes 950 bytes of keyword text in 629 bytes */
/*   REINDEXEDESCAPEACHECKEYBEFOREIGNOREGEXPLAINSTEADDATABASELECT       */
/*   ABLEFTHENDEFERRABLELSEXCLUDELETEMPORARYCONSTRAINTERSECTIES         */
/*   AVEPOINTOFFSETRANSACTIONATURALTERAISEXCEPTRIGGEREFERENCES          */
/*   UNIQUERYWITHOUTERELEASEXCLUSIVEXISTSATTACHAVINGLOBEGINNERANGE      */
/*   BETWEENOTHINGROUPSCASCADETACHCASECOLLATECREATECURRENT_DATE         */
/*   IMMEDIATEJOINSERTLIKEMATCHPLANALYZEPRAGMABORTUPDATEVALUES          */
/*   VIRTUALIMITWHENOTNULLWHERECURSIVEAFTERENAMEANDEFAULT               */
/*   AUTOINCREMENTCASTCOLUMNCOMMITCONFLICTCROSSCURRENT_TIMESTAMP        */
/*   ARTITIONDEFERREDISTINCTDROPRECEDINGFAILFILTEREPLACEFOLLOWING       */
/*   FROMFULLIFISNULLORDERESTRICTOTHERSOVERIGHTROLLBACKROWS             */
/*   UNBOUNDEDUNIONUSINGVACUUMVIEWINDOWBYINITIALLYPRIMARY               */
static const char zKWText[628] = {
  'R','E','I','N','D','E','X','E','D','E','S','C','A','P','E','A','C','H',
  'E','C','K','E','Y','B','E','F','O','R','E','I','G','N','O','R','E','G',
  'E','X','P','L','A','I','N','S','T','E','A','D','D','A','T','A','B','A',
  'S','E','L','E','C','T','A','B','L','E','F','T','H','E','N','D','E','F',
  'E','R','R','A','B','L','E','L','S','E','X','C','L','U','D','E','L','E',
  'T','E','M','P','O','R','A','R','Y','C','O','N','S','T','R','A','I','N',
  'T','E','R','S','E','C','T','I','E','S','A','V','E','P','O','I','N','T',
  'O','F','F','S','E','T','R','A','N','S','A','C','T','I','O','N','A','T',
  'U','R','A','L','T','E','R','A','I','S','E','X','C','E','P','T','R','I',
  'G','G','E','R','E','F','E','R','E','N','C','E','S','U','N','I','Q','U',
  'E','R','Y','W','I','T','H','O','U','T','E','R','E','L','E','A','S','E',
  'X','C','L','U','S','I','V','E','X','I','S','T','S','A','T','T','A','C',
  'H','A','V','I','N','G','L','O','B','E','G','I','N','N','E','R','A','N',
  'G','E','B','E','T','W','E','E','N','O','T','H','I','N','G','R','O','U',
  'P','S','C','A','S','C','A','D','E','T','A','C','H','C','A','S','E','C',
  'O','L','L','A','T','E','C','R','E','A','T','E','C','U','R','R','E','N',
  'T','_','D','A','T','E','I','M','M','E','D','I','A','T','E','J','O','I',
  'N','S','E','R','T','L','I','K','E','M','A','T','C','H','P','L','A','N',
  'A','L','Y','Z','E','P','R','A','G','M','A','B','O','R','T','U','P','D',
  'A','T','E','V','A','L','U','E','S','V','I','R','T','U','A','L','I','M',
  'I','T','W','H','E','N','O','T','N','U','L','L','W','H','E','R','E','C',
  'U','R','S','I','V','E','A','F','T','E','R','E','N','A','M','E','A','N',
  'D','E','F','A','U','L','T','A','U','T','O','I','N','C','R','E','M','E',
  'N','T','C','A','S','T','C','O','L','U','M','N','C','O','M','M','I','T',
  'C','O','N','F','L','I','C','T','C','R','O','S','S','C','U','R','R','E',
  'N','T','_','T','I','M','E','S','T','A','M','P','A','R','T','I','T','I',
  'O','N','D','E','F','E','R','R','E','D','I','S','T','I','N','C','T','D',
  'R','O','P','R','E','C','E','D','I','N','G','F','A','I','L','F','I','L',
  'T','E','R','E','P','L','A','C','E','F','O','L','L','O','W','I','N','G',
  'F','R','O','M','F','U','L','L','I','F','I','S','N','U','L','L','O','R',
  'D','E','R','E','S','T','R','I','C','T','O','T','H','E','R','S','O','V',
  'E','R','I','G','H','T','R','O','L','L','B','A','C','K','R','O','W','S',
  'U','N','B','O','U','N','D','E','D','U','N','I','O','N','U','S','I','N',
  'G','V','A','C','U','U','M','V','I','E','W','I','N','D','O','W','B','Y',
  'I','N','I','T','I','A','L','L','Y','P','R','I','M','A','R','Y',
};
/* aKWHash[i] is the hash value for the i-th keyword */
static const unsigned char aKWHash[127] = {
    75, 111, 127,  73, 108,  29,   0,   0,  83,   0,  77,  63,   0,
    37,  33,  78,  15,   0, 126,  86,  57, 120, 128,  19,   0,   0,
   133,   0, 131, 123,   0,  22,  98,   0,   9,   0,   0, 117,  71,
     0,  69,   6,   0,  49,  95, 140,   0, 129, 106,   0,   0,  54,
     0, 109,  24,   0,  17,   0, 134,  56,  23,  26,   5,  58, 135,
   101,   0,   0, 139, 112,  62, 138,  59, 115,  65,   0,  96,   0,
   105,  45,   0, 104,   0,   0,   0, 100,  97, 102, 107, 119,  14,
    31, 118,   0,  81,   0, 136, 116, 137,  61, 124, 132,  80, 121,
    88,  30,  85,   0,   0,  99,  35, 125, 122,   0, 130,   0,   0,
    41,   0,  91,  89,  90,   0,  20,  87, 113,  82,
};
/* aKWNext[] forms the hash collision chain.  If aKWHash[i]==0
** then the i-th keyword has no more hash collisions.  Otherwise,
** the next keyword with the same hash is aKWHash[i]-1. */
static const unsigned char aKWNext[140] = {
     0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   0,
     0,   2,   0,   0,   0,   0,   0,   0,  13,   0,   0,   0,   0,
     0,   0,   0,  21,   0,   0,  12,   0,   0,   0,   0,   0,   0,
     7,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    51,  28,   0,   0,  38,   0,   0,   0,  44,   0,   0,   0,   3,
     0,   0,  67,   1,  66,   0,   0,   0,  36,   0,  47,   0,   0,
     0,   0,   0,  48,  50,  76,   0,   0,  42,   0,  60,   0,   0,
     0,  43,   0,  16,  55,  10,   0,   0,   0,   0,   0,   0,   0,
    11,  72,  93,   0,   0,   8,   0, 110,   0, 103,  40,  53,  70,
     0, 114,   0,  74,  52,   0,   0,  92,  39,  46,   0,  68,  32,
    84,   0,  34,  27,  25,  18,  94,   0,  64,  79,
};
/* aKWLen[i] is the length (in bytes) of the i-th keyword */
static const unsigned char aKWLen[140] = {
     7,   7,   5,   4,   6,   4,   5,   3,   6,   7,   3,   6,   6,
     7,   7,   3,   8,   2,   6,   5,   4,   4,   3,  10,   4,   7,
     6,   9,   4,   2,  10,   9,   4,   9,   4,   6,   2,   3,  11,
     6,   2,   7,   5,   5,   6,   7,  10,   6,   5,   7,   4,   5,
     7,   9,   6,   6,   6,   4,   5,   5,   5,   7,   7,   6,   5,
     7,   3,   6,   4,   7,   6,  12,   9,   4,   6,   4,   5,   4,
     7,   6,   5,   6,   6,   7,   5,   4,   7,   3,   2,   4,   5,
     9,   5,   6,   3,   7,  13,   2,   2,   4,   6,   6,   8,   5,
    17,  12,   7,   9,   8,   8,   2,   4,   9,   4,   6,   7,   9,
     4,   4,   2,   6,   5,   8,   6,   4,   5,   8,   4,   3,   9,
     5,   5,   6,   4,   6,   2,   2,   9,   3,   7,
};
/* aKWOffset[i] is the index into zKWText[] of the start of
** the text for the i-th keyword. */
static const unsigned short int aKWOffset[140] = {
     0,   2,   2,   8,   9,  14,  16,  20,  23,  25,  25,  29,  33,
    36,  41,  46,  48,  53,  54,  59,  62,  65,  67,  69,  78,  81,
    86,  90,  90,  94,  99, 106, 114, 117, 123, 126, 126, 129, 131,
   136, 140, 141, 146, 150, 154, 159, 165, 175, 178, 183, 183, 187,
   191, 197, 205, 211, 216, 221, 224, 227, 231, 236, 242, 248, 248,
   254, 255, 259, 265, 269, 276, 282, 294, 303, 305, 311, 315, 320,
   322, 329, 334, 339, 345, 351, 357, 362, 365, 365, 365, 368, 372,
   375, 384, 388, 394, 396, 403, 405, 407, 416, 420, 426, 432, 440,
   445, 445, 445, 461, 470, 477, 478, 485, 488, 497, 501, 506, 513,
   522, 526, 530, 532, 538, 542, 550, 556, 559, 564, 572, 572, 576,
   585, 590, 595, 601, 604, 607, 610, 612, 617, 621,
};
/* aKWCode[i] is the parser symbol code for the i-th keyword */
static const unsigned char aKWCode[140] = {
  TK_REINDEX,    TK_INDEXED,    TK_INDEX,      TK_DESC,       TK_ESCAPE,     
  TK_EACH,       TK_CHECK,      TK_KEY,        TK_BEFORE,     TK_FOREIGN,    
  TK_FOR,        TK_IGNORE,     TK_LIKE_KW,    TK_EXPLAIN,    TK_INSTEAD,    
  TK_ADD,        TK_DATABASE,   TK_AS,         TK_SELECT,     TK_TABLE,      
  TK_JOIN_KW,    TK_THEN,       TK_END,        TK_DEFERRABLE, TK_ELSE,       
  TK_EXCLUDE,    TK_DELETE,     TK_TEMP,       TK_TEMP,       TK_OR,         
  TK_CONSTRAINT, TK_INTERSECT,  TK_TIES,       TK_SAVEPOINT,  TK_INTO,       
  TK_OFFSET,     TK_OF,         TK_SET,        TK_TRANSACTION,TK_ACTION,     
  TK_ON,         TK_JOIN_KW,    TK_ALTER,      TK_RAISE,      TK_EXCEPT,     
  TK_TRIGGER,    TK_REFERENCES, TK_UNIQUE,     TK_QUERY,      TK_WITHOUT,    
  TK_WITH,       TK_JOIN_KW,    TK_RELEASE,    TK_EXCLUSIVE,  TK_EXISTS,     
  TK_ATTACH,     TK_HAVING,     TK_LIKE_KW,    TK_BEGIN,      TK_JOIN_KW,    
  TK_RANGE,      TK_BETWEEN,    TK_NOTHING,    TK_GROUPS,     TK_GROUP,      
  TK_CASCADE,    TK_ASC,        TK_DETACH,     TK_CASE,       TK_COLLATE,    
  TK_CREATE,     TK_CTIME_KW,   TK_IMMEDIATE,  TK_JOIN,       TK_INSERT,     
  TK_LIKE_KW,    TK_MATCH,      TK_PLAN,       TK_ANALYZE,    TK_PRAGMA,     
  TK_ABORT,      TK_UPDATE,     TK_VALUES,     TK_VIRTUAL,    TK_LIMIT,      
  TK_WHEN,       TK_NOTNULL,    TK_NOT,        TK_NO,         TK_NULL,       
  TK_WHERE,      TK_RECURSIVE,  TK_AFTER,      TK_RENAME,     TK_AND,        
  TK_DEFAULT,    TK_AUTOINCR,   TK_TO,         TK_IN,         TK_CAST,       
  TK_COLUMNKW,   TK_COMMIT,     TK_CONFLICT,   TK_JOIN_KW,    TK_CTIME_KW,   
  TK_CTIME_KW,   TK_CURRENT,    TK_PARTITION,  TK_DEFERRED,   TK_DISTINCT,   
  TK_IS,         TK_DROP,       TK_PRECEDING,  TK_FAIL,       TK_FILTER,     
  TK_REPLACE,    TK_FOLLOWING,  TK_FROM,       TK_JOIN_KW,    TK_IF,         
  TK_ISNULL,     TK_ORDER,      TK_RESTRICT,   TK_OTHERS,     TK_OVER,       
  TK_JOIN_KW,    TK_ROLLBACK,   TK_ROWS,       TK_ROW,        TK_UNBOUNDED,  
  TK_UNION,      TK_USING,      TK_VACUUM,     TK_VIEW,       TK_WINDOW,     
  TK_DO,         TK_BY,         TK_INITIALLY,  TK_ALL,        TK_PRIMARY,    
};
/* Check to see if z[0..n-1] is a keyword. If it is, write the
** parser symbol code for that keyword into *pType.  Always
** return the integer n (the length of the token). */
static int keywordCode(const char *z, int n, int *pType){
  int i, j;
  const char *zKW;
  if( n>=2 ){
    i = ((charMap(z[0])*4) ^ (charMap(z[n-1])*3) ^ n) % 127;
    for(i=((int)aKWHash[i])-1; i>=0; i=((int)aKWNext[i])-1){
      if( aKWLen[i]!=n ) continue;
      j = 0;
      zKW = &zKWText[aKWOffset[i]];
#ifdef SQLITE_ASCII
      while( j<n && (z[j]&~0x20)==zKW[j] ){ j++; }
#endif
#ifdef SQLITE_EBCDIC
      while( j<n && toupper(z[j])==zKW[j] ){ j++; }
#endif
      if( j<n ) continue;
      testcase( i==0 ); /* REINDEX */
      testcase( i==1 ); /* INDEXED */
      testcase( i==2 ); /* INDEX */
      testcase( i==3 ); /* DESC */
      testcase( i==4 ); /* ESCAPE */
      testcase( i==5 ); /* EACH */
      testcase( i==6 ); /* CHECK */
      testcase( i==7 ); /* KEY */
      testcase( i==8 ); /* BEFORE */
      testcase( i==9 ); /* FOREIGN */
      testcase( i==10 ); /* FOR */
      testcase( i==11 ); /* IGNORE */
      testcase( i==12 ); /* REGEXP */
      testcase( i==13 ); /* EXPLAIN */
      testcase( i==14 ); /* INSTEAD */
      testcase( i==15 ); /* ADD */
      testcase( i==16 ); /* DATABASE */
      testcase( i==17 ); /* AS */
      testcase( i==18 ); /* SELECT */
      testcase( i==19 ); /* TABLE */
      testcase( i==20 ); /* LEFT */
      testcase( i==21 ); /* THEN */
      testcase( i==22 ); /* END */
      testcase( i==23 ); /* DEFERRABLE */
      testcase( i==24 ); /* ELSE */
      testcase( i==25 ); /* EXCLUDE */
      testcase( i==26 ); /* DELETE */
      testcase( i==27 ); /* TEMPORARY */
      testcase( i==28 ); /* TEMP */
      testcase( i==29 ); /* OR */
      testcase( i==30 ); /* CONSTRAINT */
      testcase( i==31 ); /* INTERSECT */
      testcase( i==32 ); /* TIES */
      testcase( i==33 ); /* SAVEPOINT */
      testcase( i==34 ); /* INTO */
      testcase( i==35 ); /* OFFSET */
      testcase( i==36 ); /* OF */
      testcase( i==37 ); /* SET */
      testcase( i==38 ); /* TRANSACTION */
      testcase( i==39 ); /* ACTION */
      testcase( i==40 ); /* ON */
      testcase( i==41 ); /* NATURAL */
      testcase( i==42 ); /* ALTER */
      testcase( i==43 ); /* RAISE */
      testcase( i==44 ); /* EXCEPT */
      testcase( i==45 ); /* TRIGGER */
      testcase( i==46 ); /* REFERENCES */
      testcase( i==47 ); /* UNIQUE */
      testcase( i==48 ); /* QUERY */
      testcase( i==49 ); /* WITHOUT */
      testcase( i==50 ); /* WITH */
      testcase( i==51 ); /* OUTER */
      testcase( i==52 ); /* RELEASE */
      testcase( i==53 ); /* EXCLUSIVE */
      testcase( i==54 ); /* EXISTS */
      testcase( i==55 ); /* ATTACH */
      testcase( i==56 ); /* HAVING */
      testcase( i==57 ); /* GLOB */
      testcase( i==58 ); /* BEGIN */
      testcase( i==59 ); /* INNER */
      testcase( i==60 ); /* RANGE */
      testcase( i==61 ); /* BETWEEN */
      testcase( i==62 ); /* NOTHING */
      testcase( i==63 ); /* GROUPS */
      testcase( i==64 ); /* GROUP */
      testcase( i==65 ); /* CASCADE */
      testcase( i==66 ); /* ASC */
      testcase( i==67 ); /* DETACH */
      testcase( i==68 ); /* CASE */
      testcase( i==69 ); /* COLLATE */
      testcase( i==70 ); /* CREATE */
      testcase( i==71 ); /* CURRENT_DATE */
      testcase( i==72 ); /* IMMEDIATE */
      testcase( i==73 ); /* JOIN */
      testcase( i==74 ); /* INSERT */
      testcase( i==75 ); /* LIKE */
      testcase( i==76 ); /* MATCH */
      testcase( i==77 ); /* PLAN */
      testcase( i==78 ); /* ANALYZE */
      testcase( i==79 ); /* PRAGMA */
      testcase( i==80 ); /* ABORT */
      testcase( i==81 ); /* UPDATE */
      testcase( i==82 ); /* VALUES */
      testcase( i==83 ); /* VIRTUAL */
      testcase( i==84 ); /* LIMIT */
      testcase( i==85 ); /* WHEN */
      testcase( i==86 ); /* NOTNULL */
      testcase( i==87 ); /* NOT */
      testcase( i==88 ); /* NO */
      testcase( i==89 ); /* NULL */
      testcase( i==90 ); /* WHERE */
      testcase( i==91 ); /* RECURSIVE */
      testcase( i==92 ); /* AFTER */
      testcase( i==93 ); /* RENAME */
      testcase( i==94 ); /* AND */
      testcase( i==95 ); /* DEFAULT */
      testcase( i==96 ); /* AUTOINCREMENT */
      testcase( i==97 ); /* TO */
      testcase( i==98 ); /* IN */
      testcase( i==99 ); /* CAST */
      testcase( i==100 ); /* COLUMN */
      testcase( i==101 ); /* COMMIT */
      testcase( i==102 ); /* CONFLICT */
      testcase( i==103 ); /* CROSS */
      testcase( i==104 ); /* CURRENT_TIMESTAMP */
      testcase( i==105 ); /* CURRENT_TIME */
      testcase( i==106 ); /* CURRENT */
      testcase( i==107 ); /* PARTITION */
      testcase( i==108 ); /* DEFERRED */
      testcase( i==109 ); /* DISTINCT */
      testcase( i==110 ); /* IS */
      testcase( i==111 ); /* DROP */
      testcase( i==112 ); /* PRECEDING */
      testcase( i==113 ); /* FAIL */
      testcase( i==114 ); /* FILTER */
      testcase( i==115 ); /* REPLACE */
      testcase( i==116 ); /* FOLLOWING */
      testcase( i==117 ); /* FROM */
      testcase( i==118 ); /* FULL */
      testcase( i==119 ); /* IF */
      testcase( i==120 ); /* ISNULL */
      testcase( i==121 ); /* ORDER */
      testcase( i==122 ); /* RESTRICT */
      testcase( i==123 ); /* OTHERS */
      testcase( i==124 ); /* OVER */
      testcase( i==125 ); /* RIGHT */
      testcase( i==126 ); /* ROLLBACK */
      testcase( i==127 ); /* ROWS */
      testcase( i==128 ); /* ROW */
      testcase( i==129 ); /* UNBOUNDED */
      testcase( i==130 ); /* UNION */
      testcase( i==131 ); /* USING */
      testcase( i==132 ); /* VACUUM */
      testcase( i==133 ); /* VIEW */
      testcase( i==134 ); /* WINDOW */
      testcase( i==135 ); /* DO */
      testcase( i==136 ); /* BY */
      testcase( i==137 ); /* INITIALLY */
      testcase( i==138 ); /* ALL */
      testcase( i==139 ); /* PRIMARY */
      *pType = aKWCode[i];
      break;
    }
  }
  return n;
}
int sqlite3KeywordCode(const unsigned char *z, int n){
  int id = TK_ID;
  keywordCode((char*)z, n, &id);
  return id;
}
#define SQLITE_N_KEYWORD 140
int sqlite3_keyword_name(int i,const char **pzName,int *pnName){
  if( i<0 || i>=SQLITE_N_KEYWORD ) return SQLITE_ERROR;
  *pzName = zKWText + aKWOffset[i];
  *pnName = aKWLen[i];
  return SQLITE_OK;
}
int sqlite3_keyword_count(void){ return SQLITE_N_KEYWORD; }
int sqlite3_keyword_check(const char *zName, int nName){
  return TK_ID!=sqlite3KeywordCode((const u8*)zName, nName);
}
