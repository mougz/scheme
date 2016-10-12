
/**
 * @file read.c
 * @author François Cayre <cayre@yiking.(null)>
 * @date Fri Jun 15 18:31:05 2012
 * @brief Read routine for SFS.
 *
 * Read routine for SFS.
 */

#include <stdio.h>
#include <ctype.h>

#include "read.h"



void flip( uint *i ) {

    if ( *i == FALSE ) {
        *i = TRUE;
    }
    else {
        *i = FALSE;
    }
}

/*
 * @fn char* first_usefull_char(char* line)
 *
 * @brief retourne un pointeur sur le premier caractere utile dans line
 * ou NULL si line ne contient que des espaces et des commentaires
 */
char* first_usefull_char(char* line) {

    int i=0;
    if (line == NULL) {
        return NULL;
    }
    i = 0;
    /* on saute les espaces */
    while(line[i] != '\0' && isspace(line[i])) {
        i++;
    }
    /* si fin de ligne => ligne inutile */
    if(line[i] == '\0') {
        return NULL;
    }
    /* si premier caractere non espace est ';' => ligne inutile */
    if(line[i] == ';') {
        return NULL;
    }
    return line + i; /* ligne utile */
}

/**
 * @fn uint  sfs_get_sexpr( char *input, FILE *fp )
 *
 * @brief extrait la prochaine S-Expression dans le flux fp et la stocke dans input
 * (meme si elle est repartie sur plusieurs lignes)
 * @param fp (FILE *) flux d'entree (ouvert en ecriture, mode texte)
 * @param input (char *) chaine allouee de taille BIGSTRING, dans laquelle la S-Expression sera stockée
 *
 * @return S_OK si une S-Expression apparemment valide a ete trouvee
 * @return S_KO si on n'a pas trouve de S-Expression valide
 * @return S_END si fin de fichier atteinte sans avoir lu de caractere utile.
 *
 * sfs_get_sexpr commence par lire une ligne dans fp,
 * puis compte le nombre de parentheses ouvrantes et fermantes sur la ligne.
 * Les parentheses dans des chaines et les caracteres Scheme #\( et #\)
 * ne sont pas comptes.
 *
 * Si le compte devient zéro et que
 *        - la ligne est fini, la fonction retourne S_OK
 * 				- la ligne n'est pas fini la fonction retourne S_KO
 *
 * S'il y a plus de parentheses fermantes qu'ouvrantes,
 * la fonction retourne S_KO.
 * Les commentaires et espaces qui figurent a la fin de chaque ligne
 * sont remplacés par un espace.
 * Les espaces qui figurent a la fin de la S-Expression (y compris '\n')
 * sont supprimés.
 *
 * Attention : cette fonction refuse les S-Expression multiple sur une seule ligne. Ainsi :
 *    a b c
 *    (qqchose) (autrechose)
 *    (qqchose) 78
 * seront considereees comme des erreurs
 * et la fonction retournera S_KO
 *
 * @pre fp doit etre prealablement ouvert en lecture
 * @pre input doit etre prealablement alloue en memoire, de taille BIGSTRING
 */

typedef enum {
    NOTHING,        /* rien n'a ete trouve encore.. */
    STRING_ATOME,   /* la premiere trouvee dans la ligne semble etre un atome */
    BASIC_ATOME,    /* la premiere trouvee dans la ligne semble etre d'une chaine */
    S_EXPR_PARENTHESIS, /* la premiere trouvee dans la ligne semble etre une expression parenthesee */
    FINISHED        /* on a trouve une S-Expr bien formee */
} EXPRESSION_TYPE_T;

uint  sfs_get_sexpr( char *input, FILE *fp ) {
    int       parlevel = 0;
    uint      in_string = FALSE;
    uint      s = 0;
    char      k[BIGSTRING];
    char      *ret = NULL;
    char      *chunk = NULL;
    uint      i = 0;
    string    sfs_prompt;

    EXPRESSION_TYPE_T typeOfExpressionFound = NOTHING;

    parlevel = 0;
    memset( input, '\0', BIGSTRING );

    do {
        ret = NULL;
        chunk=k;
        memset( chunk, '\0', BIGSTRING );

        /* si en mode interactif*/
        if ( stdin == fp ) {
            uint nspaces = 2*parlevel;

            init_string( sfs_prompt );

            /* le prompt indique le niveau de parenthese
               et decale la prochaine entrée en fonction
               de ce niveau (un peu à la python)*/
            sprintf( sfs_prompt, "SFS:%u > ", parlevel );

            for ( i= 0; i< nspaces; i++ ) {
                sfs_prompt[strlen(sfs_prompt)] = ' ';
            }

            /* si sur plusieurs lignes, le \n équivaut à un espace*/
            if (nspaces>0) {
                input[strlen(input)+1] = '\0';
                input[strlen(input)] = ' ';
            }

            /*saisie de la prochaine ligne à ajouter dans l'input*/
            printf("%s",sfs_prompt);
            ret = fgets( chunk, BIGSTRING, fp );
            if (ret && chunk[strlen(chunk)-1] == '\n') chunk[strlen(chunk)-1] = '\0';

        }
        /*si en mode fichier*/
        else {
            ret = fgets( chunk, BIGSTRING, fp );

            if ( NULL == ret ) {
                /* fin de fichier...*/
                if ( parlevel != 0 ) {
                    WARNING_MSG( "Parse error: missing ')'" );
                    return S_KO;
                }
                return S_END;
            }

            if (strlen(chunk) == BIGSTRING-1
                    && chunk[BIGSTRING-1] != '\n'
                    && !feof(fp)) {
                WARNING_MSG( "Too long line for this interpreter!" );
                return S_KO;
            }
        }

        /* si la ligne est inutile
        	=> on va directement à la prochaine iteration */
        if (first_usefull_char(chunk) == NULL) {
            continue;
        }


        s = strlen( chunk );

        if ( s > 0 ) {
            if (strlen(input) + s > BIGSTRING-1 ) {
                WARNING_MSG( "Too long a S-expression for this interpreter!" );
                return S_KO;
            }

            for ( i = 0; i< strlen(chunk); i++ ) {
                /* si la fin de la ligne chunk est inutile,
                   on ajoute un espace dans input et on sort de la boucle*/
                if ( in_string == FALSE && first_usefull_char(chunk + i) == NULL ) {
                    chunk[i]='\0';
                    input[strlen(input)] = ' ';
                    break;
                }


                switch(chunk[i]) {
                case '(':
                    if (in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) ) {
                        parlevel++;
                        typeOfExpressionFound = S_EXPR_PARENTHESIS;
                    }
                    break;
                case ')':
                    if ( in_string == FALSE
                            && ! ( i>1 && chunk[i-1] == '\\' && chunk[i-2] == '#' ) ) {
                        parlevel--;
                        if (parlevel == 0 ) {
                            typeOfExpressionFound = FINISHED;
                        }
                        if ( parlevel < 0 ) {
                            WARNING_MSG( "Parse error : cannot start with ')'" );
                            return S_KO;
                        }
                    }
                    break;
                case '"':
                    if ( i<2 || chunk[i-1] != '\\' ) {
                        if ( in_string == FALSE ) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                WARNING_MSG("Parse error: invalid string after atom : '%s'", chunk+i);
                                return S_KO;
                            }
                            in_string = TRUE;
                            if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                                typeOfExpressionFound = STRING_ATOME;
                            }
                        }
                        else {
                            in_string = FALSE;
                            if(typeOfExpressionFound == STRING_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        }
                    }
                    break;
                default:
                    if(in_string == FALSE) {
                        if(isspace(chunk[i])) {
                            if(typeOfExpressionFound == BASIC_ATOME) {
                                typeOfExpressionFound = FINISHED;
                            }
                        } else if(typeOfExpressionFound != S_EXPR_PARENTHESIS) {
                            typeOfExpressionFound = BASIC_ATOME;
                        }
                    }
                    break;
                }


                if(typeOfExpressionFound == FINISHED) {
                    char *first_useful = first_usefull_char(chunk + i + 1);
                    if( first_useful != NULL) {
                        if(*first_useful == ')' ) {
                            WARNING_MSG( "Parse error: too many closing parenthesis')'" );
                        }
                        else {
                            WARNING_MSG("Parse error: invalid trailing chars after S-Expr : '%s'", chunk+i);
                        }
                        return S_KO;
                    }
                }

                /* recopie char par char*/
                input[strlen(input)] = chunk[i];
            }
            if(in_string == TRUE) {
                WARNING_MSG( "Parse error: non terminated string on line %s", chunk );
                return S_KO;
            }
        }


        if ( parlevel > 0 && fp != stdin ) {
            if ( feof( fp ) ) {
                WARNING_MSG( "Parse error: missing ')'" );
                return S_KO;
            }

            if (input[strlen(input)-1] == '\n') input[strlen(input)-1] = ' ';
        }
    } while ( parlevel > 0 );

    /* Suppression des espaces restant a la fin de l'expression, notamment le dernier '\n' */
    while (isspace(input[strlen(input)-1])) input[strlen(input)-1] = '\0';

    return S_OK;
}


object sfs_read( char *input, uint *here ) {

    while ((input[(*here)]==' ' || input[*here]=='\t') && input[(*here)]!='\0') {
	(*here)++;
    }

    if ( input[*here] == '(' ) {

	while ((input[(*here)+1]==' ' || input[(*here)+1]=='\t') && input[(*here)+1]!='\0') {
		(*here)++;
    	}

        if ( input[(*here)+1] == ')' ) {
            *here += 2;
            return nil;
        }
        else {
            (*here)++;
            return sfs_read_pair( input, here );
        }
    }
    else {
        return sfs_read_atom( input, here );
    }
}

object sfs_read_atom( char *input, uint *here ) {

    object atom = NULL;
    uint step=*here;
    int signe=0;

    while ((input[step]==' ' || input[step]=='\t' )&& input[step]!='\0') {
        step++;
	(*here)++;
    }


    switch (input[step]) {
    case '#':
    {	
	int compteur=0;
        if (input[step+1]=='\\')
        {
            if (isspace(input[step+2]))
            {
                WARNING_MSG("Not valid atom");
                return NULL;
            }
            else
            {
                if (isspace(input[step+3]) || input[step+3]=='\0' || input[step+3]=='\n' || input[step+3]==' ' || input[step+3]=='\t')
                {
                    atom=make_character(input[step+2]);
		    compteur+=1;
		    *here+=3;
                }
                if (input[step+2]=='s' && input[step+3]=='p' && input[step+4]=='a' && input[step+5]=='c' && input[step+6]=='e' && (isspace(input[step+7])|| input[step+7]=='\0' || input[step+7]=='\t' ))
                {
                    atom=make_character(' ');
		    compteur+=1;
		    *here+=7;
                }
                if (input[step+2]=='n' && input[step+3]=='e' && input[step+4]=='w' && input[step+5]=='l' && input[step+6]=='i' && input[step+7]=='n' && input[step+8]=='e' && (isspace(input[step+9]) || input[step+9]=='\0'|| input[step+9]=='\t' ))
                {
                    atom=make_character('\n');
		    compteur+=1;
		    *here+=9;
                }
		if (compteur>=1)
		{
			compteur=0;
		}
                else {	
                    WARNING_MSG("Not valid character");
                    return NULL;
                }
            }
        }
        if (input[step+1]=='t') {
            if (isspace(input[step+2]) || input[step+2]=='\0' || input[step+2]==')' || input[step+2]=='(' || input[step+2]=='\t') {
                atom=vrai;
		*here+=2;
            }
            else {
                WARNING_MSG("Not valid atom");
                return NULL;
            }
        }
        if (input[step+1]=='f')
        {
            if (isspace(input[step+2]) || input[step+2]=='\0'|| input[step+2]==')' || input[step+2]=='('|| input[step+2]=='\t' )
            {
                atom=faux;
		*here+=2;
            }
            else {
                WARNING_MSG("Not valid atom");
                return NULL;
            }
        }
        break;
    }
    case '+':
	if (isdigit(input[step+1]))
	{	
		step++;
		(*here)++;
		goto NUM;
	}
	else
	{
		goto SINON;
	}
    break;

    case '-':
    {	
	if (isdigit(input[step+1]))
	{	
		step++;
		signe=1;
		(*here)++;
		goto NUM;
	}
	else
	{
		goto SINON;
	}
    }
    break;



    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
NUM:   
    {

        int number;
        char* pend;
	errno=0;
        number=strtol(&input[step],&pend,0);
	if (errno==ERANGE)
	{
		number=inf;
	}
	if (*pend!=0 && *pend!=32 && *pend!=41 &&*pend!=9 && *pend!=40 && *pend!=34)
	{
		WARNING_MSG("Not valid number");
	  	return NULL;
	}

  	if (signe==1)
	{
		number=-number;
	}
        atom=make_integer(number);
	*here+=pend-&input[step];
    }
    break;

    case '"':
    {
	int k=0;
	int i;
	int length=lengthstring(input+(*here)+1);
	char     str[BIGSTRING]="";
	char chaine[BIGSTRING]="";
	for (i=0;i<=length;i++)
	{
		chaine[i]=input[i+1+(*here)];
	}
	extraire_chaine(chaine,str,k,length,here);
	atom=make_string(str);
	*here+=strlen(str)+2;
    }
    break;

    default :
SINON:
    { 
        int i=1;
        string str = "";
        str[0]=input[*here];
        while (input[(*here)+i] != '\0'&& !isspace(input[(*here)+i])&& input[(*here)+i]!=')' && input[(*here)+i]!='\t')
        {
                str[i]=input[(*here)+i];
                i++;
        }
        atom = make_symbol(str);
        (*here)+=i;
    }
    break;

    }
    
    return atom;
}





object sfs_read_pair( char *stream, uint *here ) {

    object o_pair=make_object(SFS_PAIR);

    while (stream[*here]==' ' || stream[*here]=='\t')
    {
    	(*here)++;
    }

    o_pair->this.pair.car=sfs_read(stream,here);

    while (stream[*here]==' ' || stream[*here]=='\t')
    {
    	(*here)++;
    }

    if (stream[*here]==')')
    {
	o_pair->this.pair.cdr= nil;
	(*here)++;
    }
    else
    {  	
	o_pair->this.pair.cdr=sfs_read_pair(stream,here);
    }
    o_pair=make_pair(car(o_pair),cdr(o_pair));


    return o_pair;
}




char * extraire_chaine(char * chaine, char* str,int k,int length,uint *here)
{	
	char *p=NULL;
	char *q=strchr(chaine,'\\');
	int longueur=0;
	if (q!=NULL)
	{
		if (*(q+1)!='"')
		{
			WARNING_MSG("Not valid string1");
		}
		q=NULL;
	}	
	p=strchr(chaine,'"');
	/*printf("%d\n",p-chaine);*/
	if (p==NULL)
	{		
		WARNING_MSG("Not valid string2");
	}
	if (p-chaine==length)
	{
		strncat(str,chaine,length);
		return str;	
	}
	else
	{
		if(*(p-1)!='\\')
		{					
			WARNING_MSG("Not valid string3");
		}
		else
		{
			longueur=p-chaine-1;
			strncat(str,chaine,longueur);
			k+=longueur;
			str[k]='\\';
			k++;
			str[k]='"';
			p=NULL;
			extraire_chaine(chaine+longueur+2,str,k,length-longueur-2,here);
			(*here)++;
		}		
	}
	return str;
}

int lengthstring(char * chaine){
	int n=1;
	int m=0;
	while (n<strlen(chaine)){
		if (chaine[n]=='"' && chaine[n-1]!='\\')
		{
			m=n;
			while (m<strlen(chaine)){
				if (chaine[m+1]=='"' && isspace(chaine[m+2]) )
				{
					ERROR_MSG("Not valid string1");
				}
				m++;
			}
			return n;
		}
		else {
			n++;
		}
	}
	return n;
}




	
