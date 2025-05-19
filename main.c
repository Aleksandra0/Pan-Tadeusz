#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LENGTH 999999//max dlugosc pliku
#define MAX_WORD_LENGTH 100 //max dlugosc slowa

//Definicja slowa
struct Word
{
    char *startPtr; //adres poczatku slowa
    int lengthBytes; //dlugosc slowa
    struct Word *nextWordPtr; //wskaznik na kolejne slowo
};

//Zwracanie struktury slowa (pustego miejsca na slowo)
struct Word *getWord(char *startPtr)
{
    struct Word *newWord = malloc(sizeof(struct Word)); //zarezerwowanie pamieci na slowo
    newWord->startPtr = startPtr; //adres poczatku slowa
    newWord->lengthBytes = 0; //Zerowanie dlugosci slowa
    newWord->nextWordPtr = NULL; //Zerowanie nextWorda
    return newWord;
}

//Stworzenie "stringa" ze slowa
char *wordToString(struct Word *wordPtr)
{
    char *stringPtr = malloc(MAX_WORD_LENGTH * sizeof(char)); //zarezerwowanie pamieci na slowo
    memcpy(stringPtr, wordPtr->startPtr, wordPtr->lengthBytes); //kopiowanie wartosci z adresow komorek gdzie sa zapisane poszcegolne litery w slowie do stringPtr
    stringPtr[wordPtr->lengthBytes] = '\0'; //Zapewnienie ze kazdy "string" bedzie konzcyl sie \0 - printowanie/porownywanie
    return stringPtr;
}

//Dodawanie slowa na poczatek listy slow - zalety: nie trzeba isc po calej tablicy aby dodac slowo na jej koneic
void prependWord(struct Word **wordListPPtr, struct Word *newWordPtr)
{
    if (*wordListPPtr != NULL)
    {
        //To slow ktore wpychamy na poczatek musi wskazywac na slowo, ktore jest obecnie pierwsze (jako nastepne)
        newWordPtr->nextWordPtr = *wordListPPtr;
    }
    //Poczatek listy - wskaznik na nowe slowo
    *wordListPPtr = newWordPtr;
}

//Pojedynczy element w drzewie
struct BstNode
{
    struct Word *wordPtr; //typ danych w dzrewie
    struct BstNode *leftPtr; //wskaznik na element po lewej
    struct BstNode *rightPtr; //wskaznik na elemnt po prawej
};

//Zwracanie struktury elementu drzewa (pustego miejsca na element drzewa)
struct BstNode *getBstNode(struct Word *wordPtr)
{
    struct BstNode *newNode = malloc(sizeof(struct BstNode));
    newNode->wordPtr = wordPtr;
    newNode->leftPtr = NULL;
    newNode->rightPtr = NULL;
    return newNode;
}

void insertBstNode(struct BstNode** rootPPtr, struct Word *wordPtr) //** - wskaznik na wskaznik - przekazujemy w funkcji wskaznik na wskaznik na root aby wartosc zapisac w zmiennej rootPtr z maina
{
    //Jezeli nie ma korzenia - utworz go
    if (*rootPPtr == NULL)
    {
        *rootPPtr = getBstNode(wordPtr);
        return;
    }

    //Do celow ypisywania i porownywania stringow - zmainiamy slowa na "stringi"
    char *newWordStringPtr = wordToString(wordPtr);
    char *rootWordStringPtr = wordToString((*rootPPtr)->wordPtr);

    //printf("root: %s <> nowe slowo: %s\n", rootWordStringPtr, newWordStringPtr);

    if (strcmp(newWordStringPtr, rootWordStringPtr) < 0) //wtawianie na lewo (slowo "mniejsze lub rowne" od korzenia)
    {
        //printf("%s -> left\n", newWordStringPtr);
        insertBstNode(&((*rootPPtr)->leftPtr), wordPtr);
    }
    else //wtawianie na prawo (slowo "wieksze" od korzenia)
    {
        //printf("%s -> right\n", newWordStringPtr);
        insertBstNode(&((*rootPPtr)->rightPtr), wordPtr);
    }

    free(newWordStringPtr);
    free(rootWordStringPtr);
}

//Przeszukiwanie drzewa binarnego
int findBstNode(struct BstNode** rootPPtr, char *searchedWordStringPtr)
{
    if (*rootPPtr == NULL)
    {
        printf("Wystapil blad.\n");
        return -1;
    }

    int pathLength; //Dlugosc sciezki - (ilosc galezi od korzenia do znalezionego elementu)

    //Zamiana korzenia na stringa (w celach porownanaia slow)
    char *currentWordStringPtr = malloc(MAX_WORD_LENGTH * sizeof(char));
    currentWordStringPtr = wordToString((*rootPPtr)->wordPtr);

    if (strcmp(searchedWordStringPtr, currentWordStringPtr) == 0) //Slowa sa takie same - zwracamy 0 (dlugosc od obecnego korzenia do slowa to 0)
    {
        return 0;
    }
    else if (strcmp(searchedWordStringPtr, currentWordStringPtr) < 0) //Jezeli porownywane slowo jest "mniejsze" od obecnego slowa - przejdz do slowa po lewej (wez je jako korzen) i porownuj slowo z nim
    {
        pathLength = findBstNode(&((*rootPPtr)->leftPtr), searchedWordStringPtr);
    }
    else //Jezeli porownywane slowo jest "wieksze" od obecnego slowa - przejdz do slowa po prawej (wez je jako korzen) i porownuj slowo z nim
    {
        pathLength = findBstNode(&((*rootPPtr)->rightPtr), searchedWordStringPtr);
    }

    if(pathLength != -1)
    {
        pathLength++;
        return pathLength;
    }
    else
    {
        pathLength = -1;
        return pathLength;
    }

    free(currentWordStringPtr);
}

//Stworzenie drzewa binarnego z listy slow
struct BstNode *createBst(struct Word *wordListPtr)
{
    struct BstNode *rootPtr = NULL; //Wskaznik na korzen - na poczatku puste drzewo
    struct Word *wordCursorPtr = wordListPtr; //Wskaznik na pierwsze slowo

    while (wordCursorPtr != NULL) {
        insertBstNode(&rootPtr, wordCursorPtr); //wrzucenie elementu w odpowiednie miejsce drzewa
        wordCursorPtr = wordCursorPtr->nextWordPtr; //kolejne slowo z listy
    }
    return  rootPtr;
};

//Zaladowanie pliku do zmiennej stringPtr
char *loadStringFromFile(char *fileNamePtr)
{
    FILE *filePtr = fopen(fileNamePtr, "r");
    char *stringPtr = malloc(MAX_LENGTH * sizeof(char));
    fread(stringPtr, sizeof(char), MAX_LENGTH, filePtr);
    fclose(filePtr);
    return stringPtr;
};

//Sprawdzenie czy znak jest litera (Czyli czy moze nalezec do slowa)
bool isWordChar(char * cPtr)
{
    if ((*cPtr >= (int)'A' && *cPtr <= (int)'Z') ||
        (*cPtr >= (int)'a' && *cPtr <= (int)'z')  ||
        (*cPtr & 0b10000000)) //Wlaczenie w to znakow utf-8 - jesli jest to znak utf-8 to w zapisie binarnym bedzie sie zaczynal od 1 na tym miejscu
    {
        return true;
    }
    else
    {
        return false;
    }
};

//tworzenie slow z calego pliku
struct Word *createWords(char *stringPtr)
{
    char *cPtr = stringPtr; //jeden znak w tekscie - na poczatek pierwszy
    struct Word *wordListPtr = NULL;
    struct Word *wordBufferPtr = NULL;

    while (*cPtr != 0)
    {
        if (isWordChar(cPtr)) //jezeli znak jest litera to zaliczamy go do slowa
        {
            if (wordBufferPtr == NULL) { //jezeli bufor jest pusty, a znak jest liera - w tym miejscu zaczynamy slowo
                wordBufferPtr = getWord(cPtr);
            }
            wordBufferPtr->lengthBytes += 1; //zwiekszamy dlugosc slowa o 1 (Tylko to sie dzieje jesli bufor nie jest pusty)
        } else { // jezeli znak nie jest litera (Napotykalismy koniec slowa albo cos innego)
            if (wordBufferPtr != NULL) { //i bufor nie jest pusty (Czyli wczesniej bylo slowo)
                prependWord(&wordListPtr, wordBufferPtr); //Dodajemy slowo na poczatek listy slow
                wordBufferPtr = NULL;
            }
        }
        cPtr += 1;
    }


    if (wordBufferPtr != NULL) //Dodanie ostatniego slowa
    {
        prependWord(&wordListPtr, wordBufferPtr);
    }
    return wordListPtr;
}

//Odfiltrowanie stopwordow z glownej listy i stworzenie nowej przefiltrowanej listy (juz bez stopwordow)
struct Word *filterStopWords(struct Word *wordList, struct Word *stopList)
{
    struct Word *filteredListPtr = NULL;
    struct Word *wordCursorPtr = wordList;
    struct Word *stopCursorPtr = NULL;

    while (wordCursorPtr != NULL)
    {
        stopCursorPtr = stopList;
        bool isStopWord = false; //flaga czy slowo jest stopwordem
        //Porownanie obecnie rozwazanego slowa z kazdym slowem na stopliscie
        while (stopCursorPtr != NULL)
        {
            if (wordCursorPtr->lengthBytes == stopCursorPtr->lengthBytes &&
                memcmp(wordCursorPtr->startPtr, stopCursorPtr->startPtr, wordCursorPtr->lengthBytes) == 0) //jezeli zgadza sie dlugosc slowa i kolejene znaki w konkretnych komorkach pamieci sa takie same to slowo jest na stopliscie
            {
                isStopWord = true;
                break;
            }
            stopCursorPtr = stopCursorPtr->nextWordPtr;
        }

        //Jezeli slowo nie jest stopwordem - dodajemy je na przefiltrowana liste
        if (!isStopWord)
        {
            struct Word *copiedWordPtr = getWord(wordCursorPtr->startPtr);
            copiedWordPtr->lengthBytes = wordCursorPtr->lengthBytes;
            char *tmp = wordToString(copiedWordPtr);
            //printf("Slowo: %s\n", tmp);
            prependWord(&filteredListPtr, copiedWordPtr);
        }
        wordCursorPtr = wordCursorPtr->nextWordPtr;
    }
    //printf("Lista zostala przefiltrowana.\n");
    return filteredListPtr;
};


int main ()
{
    char *mainTextPtr = loadStringFromFile("pan-tadeusz.txt");
    char *stopTextPtr = loadStringFromFile("stop.txt");
    //char *mainTextPtr = loadStringFromFile("test.txt"); //Plik testowy
    //char *stopTextPtr = loadStringFromFile("stop_test.txt"); //Testowa stoplista

    char *utf8WordPtr = loadStringFromFile("utf8_word.txt"); //Wczytywanie polskiego słowa z pliku

    struct Word *wordListPtr = createWords(mainTextPtr);
    struct Word *stopListPtr = createWords(stopTextPtr);
    struct Word *filteredList = filterStopWords(wordListPtr, stopListPtr);


    struct BstNode *rootPtr = createBst(filteredList);

    char wordToSearch[MAX_WORD_LENGTH];
    printf("Podaj slowo do wyszukania: ");
    scanf("%s", wordToSearch);

    //printf("Dlugosc sciezki od korzenia do elementu %s: %d\n", wordToSearch, findBstNode(&rootPtr, wordToSearch)); //Znajdowanie zwykłego słowa
    printf("Dlugosc sciezki od korzenia do elementu %s: %d\n", utf8WordPtr, findBstNode(&rootPtr, utf8WordPtr)); //Znajdowanie słowa utf-8

};
