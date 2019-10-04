# Blokų grandinių technologijos


## Hash generatorius

## Algoritmo idėja
	Šis algoritmas yra 128 bit'ų. Pagrindinė idėja - bet kokį inputą pasidaryti į stringą, kurio ilgis dalintųsi iš 64. Tai daroma taip, kaip rekomendavo internete - append'inant '/0'. Tuomet jau galime sukti ciklą kiekvienam 64 ilgio blokui. Maišymui naudoju paprastus XOR'us, sudėtį ir vieną & operaciją. Iš esmės viskas buvo nuspręsta testų metu. Pamačiau, kad pridėjus daug bitų operacijų visi inputai sueina į vieną ir tą patį, todėl sumažinau kol galiausiai palikau tris skirtingas funkcijas. Maišymas vyksta naudojant keturias random reikšmes. Jas siunčiame į funkcijas, kuriose jau dalyvauja kartu su input'ais. Funkcijose yra panaudotas bitwise shifting algoritmas, kurį nemanau, kad būčiau pats sugalvojęs, tačiau StackOverflow žmogus net paaiškino ką daro. Baigus maišymą gauname 4 32 bitų blokus, kuriuos sudėję turime hash'ą.
# Testai
 1.Paveiksle matote rezultatą suhashinus 'a', '4', 128KB failą, tokį patį tik pakeistą vieną simbolį. Šie rezultatai atsiranda "rez.txt" faile.
![Pirmas](https://imgur.com/XvJmZNo) 
	2. Konstituciją programa suhash'ino per 0.03502 sekundės.
![Antras](https://imgur.com/B56PgXk)
	3. Susigeneravau 2mln. string'ų ir lyginau 1 su 2, 3 su 4 ir t.t. Kolizijų neatsirado.
![Trecias](https://imgur.com/fLkzHeu)
	4. Vidutinis pasikeitimas gavosi ~49 proc., mažiausiai pasikeitė ~28%, daugiausiai - 68,75%.
![Ketvirtas](https://imgur.com/b5Vx7PR)

# Makefile: Guide to using the program
** pavadinimas.exe readFilepav.txt <tipas> readFilepav2.txt <tipas> ... n ** 
	Tipai
1. failas - hashinama visas failas
2. eilute - hashinama eilutėmis
3. compare - lyginami vienas paskui kitą einantys hashai
4. testas - atliekamas ketvirtas testas. Suhashinama ir palyginami bitai.

Komandos:
	makefile main
	makefile gen
	main 1.1.txt failas 1.2.txt failas didelisfailas.txt failas didelisfailaspakeistas.txt failas
	main konstitucija.txt eilute
	main stringpairhashes1.txt compare
	main 4testas.txt testas
```
CC=g++
main:
	$(CC) -o main Hash.cpp
gen:
	$(CC) -o ketvirtas 4testas.cpp 
clean:
	rm *.o *.exe

