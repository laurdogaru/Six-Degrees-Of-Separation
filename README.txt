README Tema3 - Dogaru Laurentiu 313CC

-Am implementat pentru rezolvarea primelor doua cerinte

-Structura "graf" are trei campuri : size - capacitatea grafului, **noduri, 
pentru listele de adiacenta ale nodurilor, si **nume, care reprezinta numele
care corespunde unui nod 

-Pentru implementarea bfs-ului, am folosit o structura de tip queue

-Am implementat functii pentru initializarea unui graf, crearea unui arc intre 
doua noduri, verificarea existentei unui arc intre doua noduri, si realocarea
memoriei pentru un graf

-Functia "create_graf" returneaza un graf, primind un fisier ca parametru( am
detaliat mai mult procesul de formare al grafului in comentarii)

-Pentru implementarea functiilor de parcurgere( in adancime si in latime ), 
m-am folosit de indicatiile din laboratorul de la acest capitol, inclusiv
pseudocodul primit

-Pentru prima cerinta am determinat productia maxima folosind parcurgerea in
adancime. Practic, plecand de la primult nod, se parcurge graful si fiecare 
nod vizitat este marcat cu ajutorul variabilei "vazut". Dupa aceea, incepe
parcurgerea din urmatorul nod nevizitat si tot asa

-Cu ajutorul variabilelor "inceput" si "mx" se retine indicele actorului de
unde poate incepe parcurgerea pentru a obtine parcurgerea maxima, respectiv
numarul de actori din productia maxima

-Se parcurge iar graful in adancime din nodul "inceput" si se memoreaza indicii
nodurilor vizitate. Dupa, se creeaza o matrice cu numele actorilor, se sorteaza
si se afiseaza

-Am implementat functii pentru initializare, introducere si scoatere din coada, 
folosindu-ma de indicatiile de la laborator si curs

-Cu ajutorul parcurgerii in latime, se poate obtine vectorul "d", care
contine distantele de la un anumit vector la toate celelalte. Astfel, se
creeaza acest vector pentru indicele unuia din actori, iar distanta dintre 
acesta si celalat este d[indice al doilea actor].
