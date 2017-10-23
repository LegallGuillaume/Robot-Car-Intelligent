# Utiles de Savoir !

Valeur qui peut etre Négative !! /!\ ATTENTION, PRENDRE LA VALEUR ASSOCIER

## Nombres
---

> Valeur signé:

> int8_t = valeur de -128 à 127

> int16_t = valeur de -32 768 à 32 767

> int32_t = valeur de -2 147 483 648 à 2 147 483 647

> Valeur non signé:

> uint8_t = valeur de 0 à 255

> uint16_t = valeur de 0 à 65 535

> uint32_t = valeur de 0 à 4 294 967 295

## Vector

---

- 10 elements dans le tableau.
```cpp
#include <vector>
std::vector<int8_t> *ptr = new std::vector<int8_t>();
std::vector<int8_t> non_ptr;

ptr->reserve(10);
non_ptr.reserve(10);

delete ptr;
```

## Allocation dynamique

---

- Création tableau à 2 dimensions de 20x20.

- -128 < 20 < 127 --> int8_t

```cpp
int8_t **tableau;
uint8_t allocmem = 0;
try{ 
    tableau = new int8_t * [ 20 ];
    std::fill_n( tableau, 20, static_cast<int8_t*>(0)); /*permet d'initialiser le tableau à 0*/
    for(allocmem = 0; allocmem < 20; ++allocmem) 
    { 
        tableau[ allocmem ] = new int8_t[ 20 ]; 
    } 
} catch( const std::bad_alloc &) { /*si l'allocation ne s'est pas bien passé, alors on libère l'espace utilisé*/
    for( uint8_t i = 0; i < allocmem; ++i ){ 
        delete [] tableau[ i ]; 
    } 
    delete [] tableau; 
    throw; 
}
```

- Destruction du tableau 20x20

```cpp
for(int8_t i=0; i<20; i++){
    delete [] tableau[i];
    tableau[i] = nullptr;
}
delete []tableau;
tableau = nullptr;
```

## Pair
---

- position x3 & y5

```cpp
#include <utility>

std::pair<int8_t, int8_t> position;
position = std::make_pair(3, 5);

std::pair<int8_t, int8_t> *pair;
std::pair<int,int> pt;
pt = std::make_pair(3, 5);
paire = &pt;

printf("Valeur de x: %d, y: %d", pair->first, position.second);

delete pair;
pair = nullptr;
```

``Valeur de x: 3, y: 5``
