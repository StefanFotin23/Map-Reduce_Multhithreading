# Map-Reduce_Multhithreading


Scopul temei este folosirea paradigmei Map-Reduce, utilizand Multhithreading
si limbajul C pentru a realiza procesare si calcule computationale
foarte "heavy" din punct de vedere al timpului de executie (eficienta).
Programul primeste ca parametri un fisier din care citeste o lista
de fisiere pe care va urma sa le deschida si sa proceseze toate numerele
din fiecare si mai primeste numarul exact de thread-uri map si reduce 
pe care le putem folosi la rulare, cu scopul de a calcula timpul de executie
al programului pe 1 thread sau mai multe si observam cat de mult impacteaza
utilizarea thread-urilor in plus. Optimizarea este aproximativ liniara,
daca folosim 'x' thread-uri, timpul de executie va fi timpul de executie
pe un singur thread impartit la 'x'.

Implentare:
Tema cuprinde 2 parti efective de map si reduce.
Map implica procesarea numerelor si determinarea celor care pot fi scrise
sub forma a ^ b si punerea lor in liste specifice puterii la care pot fi
scrise (ex. 2,3,4,5,6 etc..)
Algoritmul ales de mine si explicat punctual in cod, urmareste ideea de a
scrie fiecare numar n sub forma x ^ y cu y maxim.
Ex. 64 = 4 ^ 3, dar apoi apelam recursiv pt 4 si observam ca acesta se
scrie 2 ^ 2, deci puterea finala maxima a lui 64 este 2 ^ (2 * 3).
In cazul apelului pt numere ca 2 (nu se poate scrie ca x ^ y),
functia "is_power(number)" returneaza 0.
Caz special, number = 1, va returna 1, pe care il vom baga in toate listele
deoarece acesta poate fi scris ca 1 = 1 ^ k, cu k o infinitate de numere
naturale.

Folosim vectori de numere globale specifice pt fiecare putere (2,3,4,5,6 etc.)
si in urma apelului is_power din map, bagam direct numarul in vectorul
corespunzator.

Dupa ce toate thread-urile map si-au terminat treaba, thread-urile reduce
incep efectiv treaba lor (au acces de a trece peste barrier declarata in cod).

Threadurile reduce iau direct vectorii globali si ii ofera spre procesare
pentru a afla numarul de elemente distincte din acestia, care va fi si
outputul final al programului in fisier-ul specific.
Ideea functiei reduce este ca fiecare thread sa ia cate un vector neprocesat
si sa il sorteze folosind quicksort, apoi sa itereze prin el si sa retina
intr-un contor numarul de elemente distincte.
La final outputam in fisierele corespunzatoare.

Pentru a pastra paradigma multithreading corect executata si sa evitam
problemele specifice gen Race Condition, suprascrierea valorilor una peste
alta etc.. folosim mutex-uri si variabile de contor care retin daca un fisier
sau vector a fost deja atribuit cuiva, astfel, thread-urile iau in mod
dinamic task-urile de procesat.
Avem cate un mutex pt partea cand atribuim un fisier unui thread, dar
si pentru momentul in care scriem in fiecare vector de puteri. 

Dinamica acestui program este oferita de thread managementul care
permite fiecarui thread sa proceseze concomitent, dar fara sa
se influenteze unul pe celalalt sau datele utilizate comun.
Fiecare thread alege dinamic un task din lista si daca il termina,
alege alt task, daca mai este disponibil vreunul, daca nu,
thread exit.
Dupa ce termina executia toate threadurile map, urmeaza cele reduce,
iar fiecare thread isi primeste task-ul si outputeaza la finalul sau,
apoi analog cu thread-urile map, alege alt task daca este disponibil.

