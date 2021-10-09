
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {

    //initilize empty list by asigning NULL to all his parameters
    list->lastElement = list->activeElement = list->firstElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

    //check if list have elements
    if(list->firstElement != NULL) {

        DLLElementPtr active = list->firstElement;

        //loop through elements end free each one
        do {

            DLLElementPtr next = active->nextElement;
            free(active);
            active = next;

        } while (active != NULL);
    }

    //assign NULL to all list parameters
    list->lastElement = list->activeElement = list->firstElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    //allocate memory for the new element
    DLLElementPtr newElem = malloc(sizeof(struct DLLElement));

    //check if there is enough memory for allocation
    if (newElem == NULL) {

        DLL_Error();

    } else {

        newElem->data = data;
        newElem->previousElement = NULL;

        //check if list isnt empty
        if (list->firstElement != NULL) {

            newElem->nextElement = list->firstElement;
            list->firstElement->previousElement = newElem;

        //in case of empty list asign new element as last
        } else {

            newElem->nextElement = NULL;
            list->lastElement = newElem;
        }

        //asign new element as first in the list
        list->firstElement = newElem;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

    //allocate memory for the new element
    DLLElementPtr newElem = malloc(sizeof(struct DLLElement));

    //check if there is enough memory for allocation
    if (newElem == NULL) {

        DLL_Error();

    } else {

        newElem->data = data;
        newElem->nextElement = NULL;

        //check if list isnt empty
        if (list->lastElement != NULL) {

            newElem->previousElement = list->lastElement;
            list->lastElement->nextElement = newElem;

        } else {

            //in case of empty list asign new element as first
            newElem->previousElement = NULL;
            list->firstElement = newElem;
        }
        //asign new element last in the list
        list->lastElement = newElem;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    //set first element of list as active
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

    //set last element of list as active
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    //return error in case of empty list
    if (list->firstElement == NULL) {

        DLL_Error();

    //in other case return value of the first element it the list
    } else { *dataPtr = list->firstElement->data; }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

    //return error in case of empty list
    if (list->lastElement == NULL) {

        DLL_Error();

    //in other case return value of the last element it the list
    } else { *dataPtr = list->lastElement->data; }
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    //check if list isnt empty
    if (list->firstElement != NULL) {

        //list lose active element if first element is active
        if (list->firstElement == list->activeElement) { list->activeElement = NULL; }

        DLLElementPtr temp = list->firstElement->nextElement;
        //free deleted element
        free(list->firstElement);

        //check if element isnt alone in the list
        if (temp != NULL) {

            list->firstElement = temp;
            list->firstElement->previousElement = NULL;

        //in case of only one element in the list, list will be empty
        } else { list->firstElement = list->lastElement = NULL; }
    }
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    //check if list isnt empty
    if (list->lastElement != NULL) {

        //list lose active element if last element is active
        if (list->lastElement == list->activeElement) { list->activeElement = NULL; }

        DLLElementPtr temp = list->lastElement->previousElement;
        //free deleted element
        free(list->lastElement);

        //check if element isnt alone in the list
        if (temp != NULL) {

            list->lastElement = temp;
            list->lastElement->nextElement = NULL;

        //in case of only one element in the list, list will be empty
        } else { list->firstElement = list->lastElement = NULL; }
        
    }
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

    //check if there is active element and he is not last in the list
    if (list->activeElement != NULL && list->activeElement->nextElement != NULL) {

        //define helping variables
        DLLElementPtr temp = list->activeElement->nextElement;
        DLLElementPtr tempNext = temp->nextElement;

        //check if deleted element isnt last in the list
        if (tempNext != NULL) {

            list->activeElement->nextElement = tempNext;
            tempNext->previousElement = list->activeElement;

        //in case of deleted element was last in the list
        } else {

            list->lastElement = list->activeElement;
            list->activeElement->nextElement = NULL;
        }
        //free element after active one
        free(temp);
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

    //check if there is active element and he is not first in the list
    if (list->activeElement != NULL && list->activeElement->previousElement != NULL) {

        //define helping variables
        DLLElementPtr temp = list->activeElement->previousElement;
        DLLElementPtr tempPrev = temp->previousElement;

        //check if deleted element isnt first in the list
        if (tempPrev != NULL) {

            list->activeElement->previousElement = tempPrev;
            tempPrev->nextElement = list->activeElement;

        //in case of deleted element was first in the list
        } else {

            list->firstElement = list->activeElement;
            list->activeElement->previousElement = NULL;
        }
        //free element before active one
        free(temp);
    }
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    //check if there is active element in the list
    if (list->activeElement != NULL) {

        //allocate memory for the new element
        DLLElementPtr newElem = malloc(sizeof(struct DLLElement));
        //check if there is enough memory for the new element
        if (newElem == NULL) {

            DLL_Error();
        
        } else {

            //assign value and place for the new element
            newElem->previousElement = list->activeElement;
            newElem->data = data;
            list->activeElement->nextElement = newElem;
            
            //check if active element isnt last in the list
            if (list->activeElement != list->lastElement) {

                newElem->nextElement = list->activeElement->nextElement;
                list->lastElement->nextElement = newElem;

            //in case of new element will be last in the list
            //assign him as last
            } else {

                newElem->nextElement = NULL;
                list->lastElement = newElem;
            }
        }
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    //check if there is active element in the list
    if (list->activeElement != NULL) {

        //allocate memory for the new element
        DLLElementPtr newElem = malloc(sizeof(struct DLLElement));
        //check if there is enough memory for the new element
        if (newElem == NULL) {

            DLL_Error();
        
        } else {

            //assign value and place for the new element
            newElem->nextElement = list->activeElement;
            newElem->data = data;
            list->activeElement->previousElement = newElem;
            
            //check if active element isnt first in the list
            if (list->activeElement != list->firstElement) {

                newElem->previousElement = list->activeElement->previousElement;
                list->firstElement->previousElement = newElem;

            //in case of new element will be first in the list
            //assign him as first
            } else {

                newElem->previousElement = NULL;
                list->firstElement = newElem;
            }
        }
    }
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    //call error in case of no active element in the list
    if (list->activeElement == NULL) {

        DLL_Error();

    //assign value of the active element to the pointer
    } else { *dataPtr = list->activeElement->data; }
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

    //check if there is active element
    //and assign given value as new value of the active element
    if (list->activeElement != NULL) { list->activeElement->data = data; }
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

    //check if there is active element
    //and if there is, shift active element to the next one in the list
    if (list->activeElement != NULL) { list->activeElement = list->activeElement->nextElement; }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

    //check if there is active element
    //and if there is, shift active element to the previous one in the list
    if (list->activeElement != NULL) { list->activeElement = list->activeElement->previousElement; }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {

    //return 1 in case there is active element in the list
    //return 0 in case of no active element in the list
    return (list->activeElement != NULL);
}

/* Konec c206.c */
