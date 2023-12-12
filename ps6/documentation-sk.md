# Conwayova hra o živote Dokumentácia

## Obsah

1. **Úvod**
    1. Účel
    2. Prehľad
2. **Pravidlá hry**
    1. Mriežka
    2. Bunky
    3. Narodenie a smrť
3. **Hlavné funkcie**
    1. `play`
    2. `tickArena`
    3. `drawNewFrame`
4. **Použitie**
    1. Kompilácia hry
    2. Konfigurácia počiatočného stavu
    3. Pozastavenie a obnovenie hry
    4. Zmena rýchlosti simulácie
    5. Prispôsobenie mriežky
    6. Ukončenie hry
5. **Mapy**

## 1. Úvod

### 1.1 Účel

Conwayova hra života je bunkový automat, ktorý simuluje vývoj buniek na dvojrozmernej mriežke. Táto dokumentácia obsahuje informácie o pravidlách hry, podrobnosti o implementácii a o tom, ako simuláciu používať a konfigurovať.

### 1.2 Prehľad

Hra pozostáva z mriežky buniek, z ktorých každá môže byť v jednom z dvoch stavov: živá alebo mŕtva. Stav bunky sa vyvíja v priebehu generácií na základe súboru pravidiel.

## 2. Pravidlá hry

### 2.1 Mriežka

Hra sa hrá na dvojrozmernej mriežke buniek. Každá bunka má osem susedov: horizontálne, vertikálne a diagonálne susediacich.

### 2.2 Bunky

- **Živé**: Živá bunka pokračuje v živote aj v ďalšej generácii.
- **Mŕtva**: Mŕtva bunka sa môže stať živou v ďalšej generácii.

### 2.3 Zrod a smrť

- **Zrodenie**: Mŕtva bunka s presne tromi živými susedmi sa stane živou v nasledujúcej generácii.
- **Smrť**: Živá bunka, ktorá má menej ako dvoch živých susedov, zomiera v dôsledku nedostatočného osídlenia a živá bunka, ktorá má viac ako troch živých susedov, zomiera v dôsledku premnoženia.

## 3. Hlavné funkcie

- `void play(char* mapPath);`: Nastaví počiatočnú mriežku na základe navrhnutej mapy.
- `bool tickArena(ARENA* arena, SETTINGS* settings)`: Posunie simuláciu do ďalšej generácie na základe nastavení hry.
- `void drawNewFrame(ARENA* arena, SETTINGS* settings, unsigned int tickCount, bool isAlive)`: Nakreslí aktuálny stav mriežky na základe nastavení hry

## 4. Použitie

### 4.1 Kompilácia hry

Ak chcete skompilovať hru, zadajte

`gcc -Ofast -march=native -Werror=vla -Wall ps6/main.c ps6/life/engine.c ps6/life/graphic.c ps6/utils/utils.c -o lifeGame -lcurses`

#### Príznak `-march=native` je mimoriadne dôležitý. Kód obsahuje funkcie, ktoré pre čo najlepšiu kompiláciu potrebujú podporu aspoň inštrukcií avx2.

### 4.2 Konfigurácia počiatočného stavu

Ak chcete spustiť hru, zadajte do terminálu:

`./lifeGame ./ps6/maps/acron.txt`

Taktiež si môžete vytvoriť a používať vlastné mapy.

### 4.3 Pozastavenie a pokračovanie

Ak chcete hru pozastaviť, stlačte jedenkrát tlačidlo `q`
Ak chcete pokračovať v hre, stlačte znova `q`

### 4.4 Zmena rýchlosti simulácie

Na zvýšenie alebo zníženie rýchlosti simulácie použite `=` alebo `-`.

### 4.5 Prispôsobenie mriežky

Potom hru pri pozastavení (`q` stlačený jedenkrát) stlačte `space` pre **_Pridanie bunky_** alebo **_Odstránenie bunky_** z aktuálnej pozície kurzora.

Ak chcete zmeniť pozíciu kurzora, použite `šipky` na klávesnici.

Ak chcete prepnúť `Režim kreslenia`, stlačte 1-krát `Enter`.

Ak chcete zmeniť polohu `Kamery`, použite štandardné klávesy wasd.

### 4.6 Ukončenie hry

Ak chcete ukončiť hru, stlačte 1-krát tlačidlo `ESC`.

## 5. Mapy

1. acron.txt
2. lietadlo-nosič.txt
3. barka.txt
4. beacon.txt
5. včelí úľ.txt
6. blinkers.txt
7. block.txt
8. boat.txt
9. osmička.txt
10. klzák-žrút.txt
11. glider.txt
12. gosper-glider-gun-inf.txt
13. gosper-glider-gun.txt
14. heavyweight-spaceship-HWSS.txt
15. light-weight-spaceship-LWSS.txt
16. loaf.txt
17. long-boat.txt
18. dlhá-loď.txt
19. stredne ťažká-vesmírna-loď-MWSS.txt
20. penta-decathlon.txt
21. rybník.txt
22. pulsar.txt
23. queen-bee.txt
24. ship.txt
25. simkin-glider-gun.txt
26. zátišie-20.txt
27. toad.txt
28. tub.txt
