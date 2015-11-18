/***********  main.c **********************

VOICI LE PROGRAMME MAIN QUI VA GERER LA COM
ZIGBEE AINSI QUE LES ECHANGES AVEC LA PAGE
WEB PAR WEBSOCKET

*******************************************/
// include standard
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <string.h>
#include <stdint.h>
#include <poll.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>


// include persos
#include "dessinterminal/drawterminal.h"
#include "jansson/jansson.h"

// POUR INVERSER LES CARACTERES (vu qu'ils arrivent dans le desordre)
void swap(uint8_t* s, int i, int j)
{
    uint8_t temp = s[i];
    s[i] = s[j];
    s[j] = temp;
}
 
void rev(uint8_t* s, uint8_t taille)
{
    int i = 0, j = taille-1;
    for(; i < j; ++i, --j)
        swap(s, i, j);
}

// Pour transformer les bouts de trame en vraie valeur a afficher 
// TODO : fonction a affiner en fonction des formats et tout... etc...
int computeData(uint8_t* s, uint8_t taille){
	uint32_t result = 0;
	for(int i = 0; i < taille ; i++){
		printf("Resultat %d = %d\n", i, result);
		result += s[i] << (i*8);
	}
	return result;
}



void add_2array_to_json( json_t* obj, const char* name, const int*
marr, size_t dim1, size_t dim2 )
{
    size_t i, j;
    json_t* jarr1 = json_array();

    for( i=0; i<dim1; ++i ) {
        json_t* jarr2 = json_array();

        for( j=0; j<dim2; ++j ) {
            int val = marr[ i*dim2 + j ];
            json_t* jval = json_integer( val );
            json_array_append_new( jarr2, jval );
        }
        json_array_append_new( jarr1, jarr2 );
    }
    json_object_set_new( obj, name, jarr1 );
    return;
}





// MAIN
int main(void){

	
	//POUR AFFICHER UN TRUC SYMPA AU LANCEMENT DU PROGRAMME
	char *filename = "main/image2.txt";
	FILE *fptr = NULL;
	if((fptr = fopen(filename,"r")) == NULL)
	  {
		fprintf(stderr,"error opening %s\n",filename);
		return 1;
	  }
	print_image(fptr);
	fclose(fptr);
	fprintf(stderr,"\n");
	printf("Lancement du programme main\n");
	
	uint8_t testTrame[2];
	testTrame[0] = 0x00;
	testTrame[1] = 0xFF;

	rev(testTrame, 2);
	int result = computeData(testTrame, 2);


	 json_t* jdata;
    char* s;
    int arr1[2][3] = { {1,2,3}, {4,5,6} };
    int arr2[4][4] = { {1,2,3,4}, {5,6,7,8}, {9,10,11,12}, {13,14,15,16} };

    jdata = json_object();
    add_2array_to_json( jdata, "arr1", &arr1[0][0], 2, 3 );
    add_2array_to_json( jdata, "arr2", &arr2[0][0], 4, 4 );

    s = json_dumps( jdata, 0 );
    puts( s );
    free( s );
    //json_decref( jdata );


/*json_error_t error;  
 json_t *root = json_loads(jdata, 0, &error );  
 if( root )  
 {  
   json_t *jsonData = json_object_get( root, "data" );  
   if( json_is_array( jsonData ) )  
   {  
     const unsigned int length = json_array_size( jsonData );  
     for( int i=0; i<length; ++i ) // Iterates over the sequence elements.  
     {  
       json_t *jsonObject = json_array_get( jsonData, i );  
         
       json_t *jsonID = json_object_get( jsonObject, "id" );  
       const char *jsonStringID = json_string_value( jsonID );  
         
       json_t *jsonName = json_object_get( jsonObject, "name" );  
       const char *jsonStringName = json_string_value( jsonName );  
         
	char * test;
	test = json_dumps (jsonStringName, 0);
	puts(test);
       // We can now do something with our Name and ID  
     }  
   }  
   json_decref( root ); 
 }  */
	

	printf("Resultat : %d\n", result);
    printf("Fin Du Programme. Merci d'avoir participe au test!\n");
	return EXIT_SUCCESS;

}

