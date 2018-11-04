__Translation of this document is in progress! To see the whole polish version of this work with images included please checkout `CA.pdf` file in this repository!__

# Modeling epidemics of selected infectious diseases using cellular automata

### Abstract
A purpose of this work was to build a cellular-automata based computer program to
simulate epidemic spread and evolution of infectious diseases under different condi-
tions. To fulfill the task, mathematical model describing rules of time and spatial trans-
mission of pathogenic vector was implemented. The socials and geographical features
have been taken into account. The model is constructed to be universal as much as
possible and to enable the user to enter his own set of parameters due to his require-
ments. This allows to show a representation of a simplified model how the detailed
characterized disease would evolve in specific conditions.

```
Key words: cellular automata, epidemy, epidemic model, pathogenic vector
```


## Table of Contents

- 1. [Introduction](#Introduction)
   - 1.1. [Goals](#1.1.-Goals)
   - 1.2. [Cellular Automata (CA)](#1.2.-Cellular-Automata-(CA))
   - 1.3. [Infectious diseases](#1.3.-Infectious-diseases)
   - 1.4. [Epidemy](#1.4.-Epidemy)
- 2. [Review](#2.-Review)
   - 2.1. [Deterministic models](#2.1.-Deterministic-models)
   - 2.2. [Stochastic models](#2.2.-Stochastic-models)
   - 2.3. [Cellular Automata](#2.3.-Cellular-Automata)
      - 2.3.1. [Cell definition](#2.3.1.-Cell-definition)
      - 2.3.2. [Used models](#2.3.2-Used-Models)
      - 2.3.3. [Cell movement](#2.3.3.-Cell-movement)
   - 2.4. [Summary](#2.4.-Summary)
- 3. [Methodology](#3.-Methodology)
   - 3.1. [Epidemiological model](#3.1.-Epidemiological-model)
   - 3.2. [Cellular Automata](#3.2.-Cellular-Automata)
   - 3.3. [Algorithms](#3.3.Algorithms)
      - 3.3.1. [State changes](#3.3.1-State-changes)
      - 3.3.2. [Cell movement](#3.3.2.-Cell-movement)
- 4. [Implementation](#4.-Implementation)
   - 4.1. [Menu](#4.1.-Menu)
   - 4.2. [Run function](#4.2.-run-function)
   - 4.3. [Update function](#4.3.-update-function)
   - 4.4. [State changes](#4.4.-State-changes)
   - 4.5. [Movement](#4.5-Movement)
   - 4.6. [Floyd-Warshall Agorithm](#4.6-floyd-warshall-algorithm)
- 5. [Results and discussion](#5.Results-and-discussion)
   - 5.1. [Random population distribution](#5.1.-Random-population-distribution)
   - 5.2. [Isolation of healthy population](#5.2.-Isolation-of-healthy-population)
   - 5.3. [Statistic tests](#5.3.-Statistic-tests)
- 6. [Conclusions](#6.Conclusions)
- [Bibliography](#Bibliography)
- [List of drawings](#List-of-drawings)
- [List of tables](#List-of-tables)

## 1. Introduction

In the era of overcrowding, when the annual growth is on average 76 million people [14],
Public health is becoming a problem of great importance. High concentration of population
being the domain of the modern world poses a serious threat due to
the fact that densely populated agglomerations become hot spots for the rapidly spreading incidence 
of infectious diseases such as influenza, smallpox and tuberculosis. For this reason, it is extremely important to develop ways to predict the course of such phenomena, depending on many factors, in order to be able to apply the appropriate preventive measures in the most effective way. The use of cellular automata, which can reproduce in silico what we observe in nature, would help to better understand, predict and control potential epidemics.

### 1.1. Goals

The goal of this work was to explore the subject of modeling epidemics of
infectious diseases with the use of programming tools in the form of cellular automatons. For this purpose, the existing model proposals proposed by other authors were reviewed to create a new implementation of the epidemiological model. In addition, the impact of communication networks on the transmission of the disease was taken into consideration.

### 1.2. Cellular Automata (CA)

The creator of the theory of cellular automata was a Hungarian scientist, John von Neumann [15].
Basing on the Turing machine, he tried to create a machine capable of self-duplication. Among other meritorious for developing the concept of cellular automata, we can mention such mathematicians as Stanisław Ulam, John Conway or Stephen Wolfram [8].
A cellular automaton is a system composed of a network of cells `{i}`, which can take a certain state from a finite set of states `Q`. Individual nodes' states are updated in each time unit `t` on a deterministic basis
or the probabilistic function of state `(F)`. The state of each cell at the time `t + 1` depends on the state of this cell and the state of the cells in its vicinity at the time `t`, as shown in the diagram in Figure 1.1.

There are many methods of CA classification. Probably the most popular one so far - based on observation of their evolution from an accidental initial state - proposed by one of the leading popularizers of the CA - Steven
Wolfram - in his work from 1984 [18]. He specified four classes of machines:

- homogeneous machines, where all system nodes take the same state after finite time;
- regular / periodic machines, where after a finite time the system has only stable structures (so-called attractors) or repetitive structures (so-called oscillators);
- chaotic machines, where - according to chaos theory [6] - even small ones changes in the initial state of the system lead to diametrically different final effects;
- complex machines that are characterised by complex behavior.

There are many cellular automata distinguishable in terms of construction in such aspects as:

- space dimension (1D, 2D, 3D, nD);
- number of cell states (k);
- cell neighborhood;
- cell type;
- boundary conditions.

The cellular automaton in the simplest way can be described by providing two parameters - the number of possible cell states `(k)` and the radius of its neighborhood `(r)`, i.e. maximal distance between cells still affecting their states. This notation works best with one-dimensional cellular automata, in which the cells are placed linearly to each other and the neighborhood type is not required for their description. The simplest example of cellular automata are elementary one-dimensional models with two cell states and a neighborhood radius equal to one; they are marked with the entry `(2,1)`. Due to the fact that each cell has only two neighbors and can take one of two specified states (0 or 1), the number of possible configurations of the three nodes is `2 ^ 3 = 8`. Because for each of these configurations, the cell can take one of two states, the number of different cellular automata with the `(2,1)` characteristic is `2 ^ 8 = 256`. Elementary cellular automatons are represented by the number of the rule in decimal notation, which describes the values that the state function takes for certain cell combinations in the order they are taken. For example, rule 110 (interesting because of its completeness in the meaning of Turing, i.e. the ability to perform any algorithm) is presented in table 1.1 and its implementation is illustrated in Fig. 1.2.

Among the elementary machines, it is worth distinguishing two sub-classes:

- legal machines - characterized by symmetry behavior, which means that the value of the state function is the same, for example for the configurations `100` and` 001`, and additionally `F (000) = 0` (so-called vacuum rule);
- voting machines - they take into account only the number of non-zero elements in the neighborhood (and not their mutual location).
Two-dimensional cellular automata introduce more variables on which the space of the machine depends. In this case, it is necessary to determine the type of grid to characterize the space of the machine (Figure 1.3) as well as the neighborhood type, the most popular of which is the vicinity of Moore and von Neuman (Figure 1.4).

~Bardzo istotn ̨a kwesti ̨a w przeprowadzaniu symulacji z wykorzystaniem automatów
komórkowych jest okre ́slenie warunków brzegowych. Wyróznia si ̨e trzy warianty za- ̇
chowania układu na brzegach siatki:~

~— warunki periodyczne — komórka połozona na brzegu siatki pozostaje w interakcji ̇
z komórk ̨a znajduj ̨ac ̨a si ̨e na drugim brzegu siatki, tak jakby ze sob ̨a s ̨asiadowały;~
~— warunki pochłaniaj ̨ace — sygnał po wykroczeniu poza siatk ̨e całkowicie znika z
układu;~
~— warunki odbijaj ̨ace — komórka na brzegu siatki jest ograniczona przez jej kontur i
odbija si ̨e od niego.~

~Automaty komórkowe maj ̨a wszechstronne zastosowanie, mi ̨edzy innymi jako na-
rz ̨edzia do symulowania układów dynamicznych, co pozwala na odtwarzanie rozma-
itych zjawisk fizycznych, chemicznych czy biologicznych.~

### 1.3. Infectious diseases

~Chorob ̨a zaka ́zn ̨a nazywa si ̨e chorob ̨e, b ̨ed ̨ac ̨a skutkiem zakazenia organizmu czyn- ̇
nikiem etiologicznym (zaka ́znym) w postaci wirusów, wiroidów, prionów, bakterii lub
grzybów. Wbrew powszechnym opiniom za chorob ̨e zaka ́zn ̨a nie uznaje si ̨e chorób
wywoływanych działaniem pasozytów, jak np. malaria. Celem wirusa (lub innego ̇
wektora chorobotwórczego) jest namnozenie si ̨e w organizmie swojego gospodarza. ̇
Jednakze organizmy ̇ zywe wykształciły szereg mechanizmów obronnych zwi ̨azanych ̇
z odporno ́sci ̨a, takze jedyn ̨a szans ̨a na przetrwanie dla patogenów jest transmisja na ̇
inne organizmy. Nosiciel patogenu wykazuje symptomy chorobowe, takie jak kaszel,
które pomagaj ̨a patogenowi przenosi ́c si ̨e poza organizm gospodarza i zaraza ́c ko- ̇
lejne osobniki. Zazwyczaj chorob ̨e wywołan ̨a wirusem mozna podzieli ́c na kilka eta- ̇
pów [10] — okresem utajenia nazywa si ̨e czas, kiedy nosiciel patogenu nie jest jeszcze
w stanie przenosi ́c go na inne osobniki. Nast ̨epnie nadchodzi czas, kiedy osobnik za-
czyna zaraza ́c inne jednostki, po czym zdrowieje. Okres po zara ̇ zeniu, ale przed wy- ̇
st ̨apieniem objawów nazywa si ̨e inkubacj ̨a. W tym czasie osobnik moze ju ̇ z zara ̇ za ́c ̇
inne jednostki, samemu nie b ̨ed ̨ac ́swiadomym,ze istnieje takie zagro ̇ zenie (patrz rys. ̇
1.5):~

~W niniejszej pracy rozwazano choroby o jednakowym prawdopodobie ́nstwie trans- ̇
misji w całej populacji, z tego wzgl ̨edu wykluczone zostały m.in. choroby przenoszone
drog ̨a płciow ̨a (sexually transmitted infections: STI), takie jak AIDS, na których przekazy-
wanie wpływ maj ̨a czynniki zwi ̨azane z aktywno ́sci ̨a seksualn ̨a i wiekiem jednostek,
w zwi ̨azku z czym nie mozna zało ̇ zy ́c ich równomiernej dystrybucji.~

### 1.4. Epidemy

~Encyklopedia PWN definiuje termin epidemia jako „wyst ̨apienie u ludzi zachorowa ́n
na okre ́slon ̨a chorob ̨e w okre ́slonym czasie i na okre ́slonym terenie w liczbie przypad-
ków wi ̨ekszej niz przeci ̨etnie” [4]. Oznacza to, ̇ ze o tym, czy dan ̨a liczb ̨e zachorowa ́n ̇
mozna ju ̇ z nazwa ́c epidemi ̨a, decyduje charakter samej choroby — tysi ̨ace zachorowa ́n ̇
na gryp ̨e kazdego roku nie wzbudzaj ̨a niczyich obaw, za ́s ka ̇ zdy przypadek pojawienia ̇
si ̨e ospy jest juz powa ̇ znym zagro ̇ zeniem epidemi ̨a. W przeszło ́sci epidemie powodo- ̇
wały olbrzymie straty demograficzne i ekonomiczne. Dlatego wła ́snie tak wazne jest ̇
rozwijanie technik umozliwiaj ̨acych szybkie przewidywanie i symulowanie transmisji ̇
chorób w okre ́slonych warunkach.~


## 2. Review

~Na przestrzeni lat rozwini ̨eto wiele róznych podej ́s ́c do modelowania przebiegu epi- ̇
demii. Pierwsze próby rozpatrywania tego problemu podj ̨ał juz w 1766 roku Daniel ̇
Bernoulli, bazuj ̨ac na danych pochodz ̨acych z epidemii czarnej ospy we Wrocławiu [2].
Modele bazuj ̨ace pocz ̨atkowo na metodach deterministycznych, zast ̨epowano z cza-
sem podej ́sciem stochastycznym, by wraz z nadej ́sciem ery komputerów wykorzysty-
wa ́c coraz wydajniejsze maszyny do rozwijania technik predykcji w oparciu o progra-
mowanie dynamiczne i modele agentowe.~

### 2.1. Deterministic models

~Zdecydowanie najpopularniejszym modelem epidemiologicznym, jest model SIR za-
proponowany w 1927 roku przez Kermacka i McKendricka [5]. Stworzyli oni model
opisywany przez równania rózniczkowe, zakładaj ̨acy istnienie pewnej populacji ̇ N
podzielonej na trzy subpopulacje:~

~— osobników podatnych (S);~
~— osobników infekuj ̨acych (I);~
~— osobników odpornych (R).~

~bilansujących się do całkowitej populacjiN=S+I+R
Model opisywany jest przez nast ̨epuj ̨ace parametry:~

~— _α_ — cz ̨esto ́s ́c wyzdrowie ́n;~
~— _β_ — cz ̨esto ́s ́c przekazywania patogenu na jeden kontakt;~
~— C— indywidualne kontakty z innymi osobnikami.~

~Przy oznaczeniu przezr= _β_ C/N, model wyrazony jest przez układ równa ́n ró ̇ z- ̇
niczkowych (2.1):~

```
dS
dt =−rS I,
```
```
d I
dt =rS I− α I,
```
```
dR
dt = α I (2.1)
```

~Zgodnie z zapisem równa ́n rózniczkowych (2.1) przepływ z jednej klasy do drugiej ̇
odbywa si ̨e z zachowaniem ilo ́sci osobników. Schemat modelu SIR przedstawia dia-
gram na rysunku 2.1.~

~Innymi przykładami modeli deterministycznych s ̨a modele SEIR oraz SEIAR. Opie-
raj ̨a si ̨e na podobnych nieliniowych układach równa ́n rózniczkowych opisuj ̨acych prze- ̇
pływ pomi ̨edzy klasami, wprowadzaj ̨ac jedynie nowe składniki stanu w postaci sub-
populacji osób z chorob ̨a w stanie utajonym (E) i subpopulacji gospodarzy patogenów
w stanie inkubacji (A). Przedstawiony jako równanie (2.2) układ równa ́n rózniczko- ̇
wych do opisania modelu SEIAR zaproponowali w swoich badaniach Tianmu Chen
i współpracownicy [3]:~

```
dSdt=− β S(I+ κ A)
dEdt = β S(I+ κ A)− ρω ′E−( 1 − ρ ) ω E
d Idt= ( 1 − ρ ) ω E− γ I
d Adt = ρω ′E− γ ′A
dRdt = γ I+ γ ′A
```

##### (2.2)

~Podobnie jak w modelu SIR, takze model SEIAR operuje nieliniowymi funkcjami ̇
przepływu mi ̨edzy populacjami, zachowuj ̨ac całkowit ̨a liczb ̨e osobników. Diagram
przej ́s ́c mi ̨edzy stanami zobrazowano na rysunku 2.2.~

### 2.2. Stochastic models

~Pomimo faktu,ze za pomoc ̨a deterministycznych metod opartych na równaniach ̇
rózniczkowych mo ̇ zna w sposób do ́s ́c precyzyjny estymowa ́c post ̨epy epidemii w du- ̇
zych populacjach, Bruno di Stefano i współpracownicy [13] wskazuj ̨a w swojej pracy, ̇
ze uproszczenia, które zakładaj ̨a te metody „(... ) w mniejszych [populacjach] mog ̨a ̇
prowadzi ́c do nierzetelnych wyników”. Z tego wzgl ̨edu modele probabilistyczne wy-
daj ̨a si ̨e bardziej odpowiednim podej ́sciem do opisywania transmisji chorób zaka ́z-
nych.~

### 2.3. Cellular Automata

~W tej sekcji omówiono i porównano istniej ̨ace automaty komórkowe znane z litera-
tury przedmiotu.~


#### 2.3.1. Cell definition

~W zdecydowanej wi ̨ekszo ́sci prac nad wykorzystaniem automatów komórkowych
do modelowania epidemii mozna spotka ́c si ̨e z poj ̨eciem komórki postrzeganej jako ̇
miejsca w przestrzeni, zajmowanego przez pewn ̨a populacj ̨e o całkowitej liczbie osob-
nikówN[12], [16], [17], przy czym tylko w jednej z prac S. H. White i współpracownicy
zastosowali heterogeniczny rozkład populacji, tak by poszczególne komórki mogły si ̨e
rózni ́c liczb ̨a przebywaj ̨acych w nich osobników [16]. Na zupełnie inne podej ́scie w ̇
kontek ́scie definicji komórki zdecydowali si ̨e L. López i współpracownicy [7]. W ich
pracy kazda komórka systemu reprezentuje indywidualn ̨a jednostk ̨e ludzk ̨a co, jak ̇
przekonuj ̨a, pozwala na dokładniejsze ustalenie róznych scenariuszy zdarze ́n i zasto- ̇
sowanie lepszych strategii kontroli.~

#### 2.3.2. Used models

~Niemal wszystkie omawiane w tej sekcji modele bazuj ̨a na standardowym modelu
SIR, opisanym w rozdziale 2.1. Bardzo niewielu autorów decyduje si ̨e rozszerzy ́c po-
dej ́scie o dodatkowe warianty. Wyj ̨atkiem moze by ́c praca Ahmeda i Agizy [1], którzy ̇
wprowadzili do swojego modelu stan inkubacji i stan utajony choroby. Tak samo po-
st ̨apił López [7] wysun ̨awszy propozycj ̨e modelu SEIAR. Stan utajony (E) to wczesny
okres choroby, w którym jednostka jest juz zara ̇ zona patogenem, ale jeszcze nie zara ̇ za ̇
innych osobników. Stan inkubacji (A) obejmuje czas, w którym komórka jest juz chora ̇
i skutecznie zaraza inne jednostki, ale sama nie wykazuje ̇ zadnych objawów choroby. ̇
Rozwi ̨azanie polegaj ̨ace na pojawianiu si ̨e w populacji nowych jednostek w wyniku
narodzin oraz ́smierci innych zarówno z przyczyn naturalnych jak i w wyniku choro-
by zostało zastosowane tylko w modelu wysuni ̨etym przez Lópeza [7]. W przypadku
pozostałych prac całkowita liczba populacji pozostaje stała przez cały czas trwania
symulacji.~

#### 2.3.3. Cell movement

~Powtarzaj ̨ac za Sirakoulisem [12]: „Jednym z najwazniejszych czynników wpływaj ̨a- ̇
cych na rozwój epidemii w populacji jest ruch jednostek”. Istniej ̨a rozmaite podej ́scia
do tego tematu. Ahmed i Agiza [1] całkowicie zrezygnowali z jego uwzgl ̨ednienia.
Cz ̨e ́s ́c prac [16], [17] wprowadza parametr okre ́slaj ̨acy prawdopodobie ́nstwo,ze chory ̇~


~osobnik przetransportuje si ̨e z jednej populacji do drugiej i współczynnik poł ̨aczenia
zalezny od liczby dost ̨epnych ́srodków transportu mi ̨edzy komórkami. Zmienne te s ̨a ̇
uwzgl ̨edniane w obliczeniach procentowego udziału chorych w komórce. Innym po-
mysłem wykazał si ̨e López [7], którego model opiera si ̨e na pojedynczych osobnikach.
W tym przypadku kazda jednostka systemu z pewnym prawdopodobie ́nstwem mo ̇ ze ̇
zmieni ́c miejsce swojego pobytu na losowo wybran ̨a komórk ̨e ze swojego s ̨asiedztwa
bez wzgl ̨edu na stan w jakim tamta si ̨e znajduje. Autor pracy [7] stwierdza,ze mo ̇ zna ̇
to postrzega ́c tak, jak gdyby dwie jednostki zamieniły si ̨e miejscami.~

### 2.4. Summary

~Omawiane automaty komórkowe nalez ̨a do klasy IV (automaty zło ̇ zone), według ̇
podziału Wolframa [18]. Krótkie podsumowanie dyskutowanych modeli przedstawia
w tabela 2.1.~

~Jak mozna zauwa ̇ zy ́c, zdecydowana wi ̨ekszo ́s ́c obecnie istniej ̨acych automatów ko- ̇
mórkowych słuz ̨acych do modelowania przebiegu epidemii bazuje na modelu Ker- ̇
macka-McKendricka i wykorzystuje metody polegaj ̨ace na wyliczaniu procentowego
udziału chorych w populacji reprezentowanej przez poszczególne komórki. Interesuj ̨a-
cym wydaje si ̨e by ́c podej ́scie opisania kazdego w ̨ezła systemu jako miejsca bytowania ̇
pojedynczego organizmu. Moze to przynie ́s ́c lepsze efekty ze wzgl ̨edu na mo ̇ zliwo ́s ́c ̇
bardziej szczegółowego scharakteryzowania czynników działaj ̨acych lokalnie. Jednocześnie takie rozwiązanie pociąga za sob ̨a wi ̨eksze problemy wynikaj ̨ace z zapotrze-
bowania na moc obliczeniow ̨a, jest o wiele bardziej pami ̨eciochłonne i zasobozerne, a ̇
odwzorowanie g ̨esto ́sci przestrzennej jest ograniczone do mozliwo ́sci danych topolo- ̇
gi ̨a siatki. Dodatkowo wi ̨eksz ̨a uwag ̨e nalezałoby przywi ̨aza ́c do zasad obowi ̨azuj ̨a- ̇
cych ruch jednostek — sposób polegaj ̨acy na losowym zamienianiu ze sob ̨a miejscami
komórek wydaje si ̨e mało odpowiadaj ̨acy rzeczywistym zachowaniom ludzkim.~


## 3. Methodology

~Przedstawiony w pracy algorytm oraz realizuj ̨acy go program komputerowy, jest
odtworzeniem pomysłu dotycz ̨acego symulowania przebiegu epidemii w oparciu o
model uwzgl ̨edniaj ̨acy czynniki działaj ̨ace indywidualnie na kazd ̨a jednostk ̨e w popu- ̇
lacji. Modele epidemiologiczne zawsze obarczone s ̨a pewnym bł ̨edem wynikaj ̨acym z
konieczno ́sci stosowania uproszczonych załoze ́n przy ich wyznaczaniu. Jest to powo- ̇
dowane złozono ́sci ̨a i mnogo ́sci ̨a zmiennych obserwowanych w naturze, które maj ̨a ̇
wpływ na przebieg procesów zachodz ̨acych w naturze. Jak to wykazali M. Shapiro
i E. Delgado-Eckert w swojej pracy [11], zarówno wyznaczanie prawdopodobie ́nstwa
zachorowania jednostki, jak i próba okre ́slenia oczekiwanej liczby chorych w modelach
SIR jest problemem klasy NP-trudnej, czyli takiej, których znalezienie rozwi ̨azania nie
jest mozliwe ze zło ̇ zono ́sci ̨a wielomianow ̨a, a sprawdzenie poprawno ́sci rozwi ̨azania ̇
wymaga co najmniej złozono ́sci obliczeniowej wielomianowej [9]. Z tego wzgl ̨edu ni- ̇
niejsza praca nie stara si ̨e proponowa ́c sztywnych warto ́sci prawdopodobie ́nstw, kła-
d ̨ac bardziej nacisk na dobrze skonstruowany algorytm zalezny od zmiennych mody- ̇
fikowanych przez uzytkownika w ramach jego indywidualnych potrzeb. ̇~

### 3.1. Epidemiological model

~Do zaproponowanego automatu komórkowego zaimplementowano model typu SE-
IAR bazuj ̨ac na modelu zaproponowanym przez L. Lópeza i współpracowników [7].
Model ten jest rozbudowany o dwa dodatkowe stany komórek w stosunku do kla-
sycznych/podstawowych modeli SIR. Wprowadza poj ̨ecie komórki wystawionej na
działanie patogenu (Exposed-E) oraz komórki asymptomatycznej (Asymptomatic-A),
czyli takiej, która mimo wykazywania si ̨e zara ́zliwo ́sci ̨a wobec komórek w swoim s ̨a-
siedztwie, nie przejawia symptomów chorobowych. Takie podej ́scie zapewnia wi ̨ek-
sz ̨a swobod ̨e w dopasowaniu modelu do róznych chorób zaka ́znych i dlatego zostało ̇
obrane na potrzeby programu, który powinien mie ́c mozliwo ́s ́c dostosowania do szerokiej gamy problemów. Dodatkowo model zaproponowany w tej pracy został uzupełniony o mozliwo ́s ́c uwzgl ̨ednienia powrotu komórki ze stanu odporno ́sci ( ̇ R) do stanu podatno ́sci (S) po czasie t ustalonym przez uzytkownika. Jest to istotne ze wzgl ̨edu na ̇
istnienie szeregu chorób zaka ́znych, po przej ́sciu których nabyta odporno ́s ́c na pato-
gen nie jest odporno ́sci ̨a trwał ̨a (jak np. w przypadku zarazenia krztu ́scem). Ogólny ̇
schemat działania modelu przedstawia schemat na rys. 3.1 (por. z rys. 2.1).~

~— S— osobniki podatne;~
~— E— nosiciele patogenu w czasie inkubacji;~
~— I— osobniki chore, przenosz ̨ace infekcj ̨e;~
~— A— osobniki chore, przenosz ̨ace infekcj ̨e, nie wykazuj ̨ace objawów (asymptomatyczne);~
~— R— osobniki ozdrowiałe, odporne na działanie patogenu;~
~— D— pusta komórka (wł ̨aczaj ̨ac osobniki martwe).~

~Ponadto wyrózniono dodatkowy stan komórki komórki ( ̇ P) nie uwzgl ̨edniany przez
model, który reprezentuje przeszkod ̨e w ́swiecie. Jest to komórka niewchodz ̨aca w in-
terakcj ̨e z innymi komórkami, niemaj ̨acazadnego wpływu na stan s ̨asiadów, jedynie ̇
uniemozliwiaj ̨aca jednostkom zaj ̨ecie okre ́slonego miejsca na siatce lub jej przekrocze- ̇
nie (jak np. bariery wodne w ́swiecie rzeczywistym). Parametry charakteryzuj ̨ace dy-
namik ̨e mog ̨a zosta ́c okre ́slone przez uzytkownika i s ̨a to: ̇~

~— _β_ 1 — prawdopodobie ́nstwo zarazenia przez komórk ̨e w stanie ̇ I;~
~— _β_ 2 — prawdopodobie ́nstwo zarazenia przez komórk ̨e w stanie ̇ A;~
~— _e_ — prawdopodobie ́nstwo przej ́scia w stanI;~
~— _ρ_ — prawdopodobie ́nstwo przej ́scia w stanA;~
~— _μ_ — prawdopodobie ́nstwo naturalnej ́smierci i narodzin;~
~— d— prawdopodobie ́nstwo ́smierci w wyniku choroby;~
~— _γ_ — prawdopodobie ́nstwo powrotu do zdrowia;~
~— r— promie ́n s ̨asiedztwa;~
~— x— prawdopodobie ́nstwo wykonania ruchu przez komórk ̨e;~
~— y— czas, po którym komórka zatraci odporno ́s ́c na patogen;~
~— z— prawdopodobie ́nstwo spontanicznego zachorowania.~

~Parametryx,yizs ̨a opcjonalne i mog ̨a zosta ́c wył ̨aczone z symulacji decyzj ̨a uzyt- ̇
kownika. W takim przypadku komórki odpowiednio: nie przemieszczaj ̨a si ̨e w trakcie
trwania symulacji (x=0), nabywaj ̨a odporno ́s ́c trwał ̨a po przej ́sciu choroby (y=0),
nie mog ̨a zachorowa ́c w przypadku braku kontaktu z inn ̨a zarazaj ̨ac ̨a jednostk ̨a ( ̇ z=
0). Parametrrokre ́sla zasi ̨eg komórek mog ̨acych wchodzi ́c ze sob ̨a w interakcje, tj.
maksymaln ̨a liczb ̨e komórek, w promieniu której komórka przenosz ̨aca chorob ̨e ma
szans ̨e na zainfekowanie komórki podatnej. Model korzysta w tym przypadku z otoczenia komórek w postaci s ̨asiedztwa Moore’a. Promie ́n s ̨asiedztwa b ̨edzie si ̨e róznił ̇w zalezno ́sci od tego, czy dana symulacja dotyczy choroby wymagaj ̨acej bezpo ́sred- ̇
niego kontaktu do transmisji, czy chorób wywoływanych patogenami posiadaj ̨acymi
mozliwo ́s ́c przenoszenia si ̨e poza ciałem nosiciela we własnym zakresie (jak np. w ̇
przypadku chorób przenoszonych drog ̨a kropelkow ̨a). Prawdopodobie ́nstwa przej ́s ́c
pomi ̨edzy poszczególnymi stanami komórek przedstawia tabela 3.1.~

### 3.2. Cellular Automata

~Prezentowany w tej pracy model symulacyjny jest probabilistycznym automatem ko-
mórkowym, co oznacza,ze w przeciwie ́nstwie do modeli deterministycznych, funkcja ̇
stanu decyduj ̨aca o zmianie stanów poszczególnych w ̨ezłów systemu jest zalezna od ̇
zmiennych losowych. Dokładny opis funkcji przej ́scia został opisany w rozdziale 3.1.
Symulacja przeprowadzana jest w ́swiecie tworzonym przez dwuwymiarow ̨a, kwa-
dratow ̨a siatk ̨e. Kazda komórka w systemie reprezentuje jednostk ̨e powierzchni, która ̇
moze znajdowa ́c si ̨e w jednym z siedmiu mo ̇ zliwych stanów, opisanych przy okazji ̇
opisu modelu w rozdziale 3.1. Warunki brzegowe wyznacza kontur zbudowany z ko-
mórek w stanie P stanowi ̨ac zamkni ̨ecie odbijaj ̨ace i tworz ̨ace zamkni ̨et ̨a przestrze ́n
do ́swiadczaln ̨a. Ze wzgl ̨edu na charakter symulacji wydaje si ̨e to najrozs ̨adniejszym
rozwi ̨azaniem w przeciwie ́nstwie do innych mozliwo ́sci, które wpływałyby na g ̨esto ́s ́c ̇
populacji (zamkni ̨ecie pochłaniaj ̨ace) lub stwarzałyby nielogiczne sytuacje, nieopowia-
daj ̨ace zachowaniom w ograniczonych obszarach przestrzeni (warunki periodyczne).~


### 3.3. Algorithms

~Symulacja rozwoju epidemii przebiega w nast ̨epuj ̨acych krokach:~

~— etap infekcji — nosiciele patogenu przechodz ̨a w stan infekcyjny lub asymptomatyczny;~
~— etap ekspozycji — osobniki podatne s ̨a zarazane przez chore jednostki;~
~— etap spontanicznych zachorowa ́n — osobniki podatne mog ̨a zachorowa ́c samorzutnie;~
~— etap powrotu do zdrowia — osobniki chore zdrowiej ̨a i nabywaj ̨a odporno ́s ́c;~
~— etap ́smierci z powodu choroby — osobniki chore umieraj ̨a na ́smierteln ̨a chorob ̨e;~
~— etap naturalnych narodzin i ́smierci — pojawiaj ̨a si ̨e nowe podatne jednostki i umieraj ̨a stare;~
~— ruch komórek — osobniki d ̨az ̨a do obranego przez siebie celu.~

#### 3.3.1. State changes

~W tej sekcji omówione zostan ̨a szczegółowo zasady, na jakich komórki automatu
zmieniaj ̨a swoje stany.
Infekcja (rys. 3.2) — dla kazdego w ̨ezła w systemie sprawdzany jest jego stan z po- ̇
przedniej iteracji. Jezeli komórka ta jest nosicielem patogenu, losowana jest pewna licz- ̇
ba z przedziału[0, 1]. Liczba ta jest porównywana z prawdopodobie ́nstwem przej ́scia
komórki w stan zainfekowania (I) i, jezeli to prawdopodobie ́nstwo zostanie spełnione, ̇
obecny stan komórki zostaje ustawiony na I. W przeciwnym wypadku sprawdzane
jest jest prawdopodobie ́nstwo przej ́scia komórki w stan asymptomatyczny (A) i jezeli ̇
wylosowana liczba okaze si ̨e od niego mniejsza, obecny stan komórki jest zmieniany ̇
naA. Jezeli ̇ zadne prawdopodobie ́nstwo nie zostanie spełnione lub komórka miała ̇
inny stan niz ̇ Eprzechodzimy do nast ̨epnego kroku iteracji. Proces ten powtarza dla
kazdego w ̨ezła w systemie. ̇~

~Ekspozycja (rys. 3.3) — dla kazdego w ̨ezła w systemie sprawdzany jest jego stan z ̇
poprzedniej iteracji. Jezeli dana komórka znajduje si ̨e w stanie ̇ Ssprawdzamy stany
wszystkich komórek znajduj ̨acych si ̨e w s ̨asiedztwie tej komórki okre ́slonym promie-
niemr. Dla kazdej komórki o stanie ̇ I(Infected) lubA(Asymptomatic) znajduj ̨acej si ̨e w
zasi ̨egu komórkisi(t− 1 ), losowana jest pewna liczba z przedziału[0, 1]i porównywa-
na z prawdopodobie ́nstwem przej ́scia komórki w stanE( _β_ 1 dla komórek w stanieA
lub _β_ 2 dla komórek w stanieI). Jezeli prawdopodobie ́nstwo to zostanie spełnione, ko- ̇
mórka podatna zmienia si ̨e w nosiciela patogenu. Proces ten powtarza si ̨e dla kazdego ̇
w ̨ezła w systemie.~

~Spontaniczne zachorowania (rys. 3.4) — dla kazdego w ̨ezła w systemie sprawdzany ̇
jest jego stan z poprzedniej iteracji. Jezeli dana komórka znajduje si ̨e w stanie ̇ S, loso-
wana jest pewna liczba z przedziału[0, 1]. Liczba ta jest porównywana z prawdopodo-
bie ́nstwem spontanicznego zachorowania (parametrz). Jezeli prawdopodobie ́nstwo ̇
to zostanie spełnione, komórka przechodzi w stanEi staje si ̨e nosicielem patogenu.
Proces ten powtarza si ̨e dla kazdego w ̨ezła w systemie. ̇~

~Powrót do zdrowia (rys. 3.5) — dla kazdego w ̨ezła w systemie sprawdzany jest jego ̇
stan z poprzedniej iteracji. Jezeli dana komórka znajduje si ̨e w stanie ̇ IlubA, losowana
jest pewna liczba z przedziału[0, 1]. Liczba ta jest porównywana z prawdopodobie ́n-
stwem przej ́scia komórki w stan odporno ́sci. Jezeli prawdopodobie ́nstwo to zostanie ̇
spełnione, komórka chora zdrowieje i staje si ̨e odporna. W przypadku, kiedy odpor-
no ́s ́c nie jest trwała (y=0) zapisywany jest aktualny czastw parametrach opisuj ̨acych
klas ̨e komórki, co pozwoli powróci ́c komórce do stanu podatno ́sci po czasiey. Proces
ten powtarza si ̨e dla kazdego w ̨ezła w systemie.~

~Smier ́c wywołana chorob ̨a (rys. 3.6) — dla ka ́ zdego w ̨ezła w systemie sprawdza- ̇
ny jest jego stan z poprzedniej iteracji. Jezeli dana komórka znajduje si ̨e w stanie ̇ I
lubA, losowana jest pewna liczba z przedziału[0, 1]. Liczba ta jest porównywana z
prawdopodobie ́nstwem,ze komórka umrze w wyniku choroby (parametr ̇ d). Jezeli ̇
prawdopodobie ́nstwo to zostanie spełnione, komórka chora umierasi(t) =D.~

~Naturalne narodziny i ́smier ́c (rys. 3.7) — dla kazdego w ̨ezła w systemie sprawdzany ̇
jest jego stan z poprzedniej iteracji. Dla kazdej komórki, która nie znajduje si ̨e w stanie ̇
P, losowana jest pewna liczba z przedziału[0, 1]i jest ona porównywana z prawdopo-
dobie ́nstwem _μ_. Jezeli ̇ si(t− 1 ) =D, tosi(t) =S, w przeciwnym wypadkusi(t) =D.~

#### 3.3.2. Cell movement

~Algorytm (rys. 3.8) realizowany jest kilkuetapowo i bierze pod uwag ̨e warto ́sci dwóch
zmiennych typuboolopisuj ̨acych własno ́sci ruchu kazdego w ̨ezła w systemie: ̇~

~— znacznikinMotion— dostarcza informacj ̨e , czy dana jednostka aktualnie jest w
trakcie podrózy ( ̇ inMotion = true), czy pozostaje w miejscu (inMotion = 0);
— znacznikmoved— informuje system, czy dana jednostka wykonała juz swój ruch ̇
w obecnej chwilit(moved = 1), czy nie (moved = 0).
Dla kazdego w ̨ezła w systemie sprawdzany jest jego stan w obecnej iteracji. Dla ka ̇ z- ̇
dej komórki, która w danej chwilitsi ̨e nie porusza (inMotion = 0), z wył ̨aczeniem
tych w stanieDlubP, losowana jest pewna liczba z przedziału[0, 1]i porównuje si ̨e
j ̨a z prawdopodobie ́nstwem wykonania ruchu (parametrx). W przypadku spełnienia
tego prawdopodobie ́nstwa, komórce wybierany jest w sposób losowy cel jej podrózy ̇
i oznacza si ̨e j ̨a jako komórk ̨e w ruchu (inMotion = 1). Nast ̨epnie kazda komórka, ̇
która w danej iteracji nie wykonała jeszcze ruchu (moved = 0) jest przesuwana w
kierunku swojego celu.~

## 4. Implementation

~Program został w cało ́sci napisany w j ̨ezyku programowania C++, ze wzgl ̨edu na
jego rozległe zalety, z których najwazniejsze to: ̇
— mozliwo ́s ́c operacji na referencjach i wska ́znikach; ̇
— dost ̨epno ́s ́c wielu bibliotek;
— jest to j ̨ezyk kompilowany, dzi ̨eki czemu program w nim działa szybko.
Zastosowane ́srodowiska programistyczne IDE to Dev C++ oraz CodeBlocks.~

### 4.1. Menu

~Po uruchomieniu programu nast ̨epuje pocz ̨atkowa konfiguracja, która obejmuje mi ̨e-
dzy innymi ustawienie rozmiaru okna, a takze ustawienie punktu startowego dla me- ̇
chanizmu generowania pseudolosowych liczb całkowitych poprzez wywołanie funk-
cjisrand(). Nast ̨epnie wywoływana jest funkcja steruj ̨acaShowMenu(), która wy-
́swietla menu główne programu i zwraca wybór uzytkownika powoduj ̨ac wywoła- ̇
nie odpowiedniej funkcji. Uzytkownik na tym etapie ma mo ̇ zliwo ́s ́c wyboru poprzez ̇
wprowadzenie odpowiedniej opcji z klawiatury:~

~— S - uruchomienie nowej symulacji~
~— H - wywołanie funkcjiHelp()zawieraj ̨acej wskazówki do obsługi programu~
~— I - wywołanie funkcjiInfo()z krótkim opisem programu~
~— T - zako ́nczenie działania programu~

### 4.2. Run function

~Po wybraniu opcji Start wywoływana jest funkcjaRun(), która jest odpowiedzialna
za działanie i sterowanie symulacj ̨a. Na pocz ̨atku tworzony jest nowy lub nadpisywa-
ny istniej ̨acy plik"output.txt"celem przygotowania dokumentu tekstowego zawieraj ̨acego dane dotycz ̨ace aktualnej symulacji. W ramach przygotowania symulacji w funkcjiRun()wywoływane s ̨a kolejno funkcje:~

~— GetConditions()— pobiera od uzytkownika szczegółowe dane dotycz ̨ace parametrów przygotowywanej symulacji i zapisuje je do zmiennych globalnych, a takze zapisuje w katalogu obejmuj ̨acym plik ̇ "log.txt"zawieraj ̨acy informacje na temat wybranego zestawu parametrów.~
~— SetupArrays()— jest odpowiedzialna za dynamiczn ̨a allokacj ̨e pami ̨eci dla tablicy:~
~ cellsb ̨ed ̨acej siatk ̨a komórek tworz ̨acych przestrze ́n automatu;~
~ cellsCopyb ̨ed ̨ac ̨a kopi ̨a siatki komórek z poprzedniej iteracji;~
~ weights— tablica z zapisanymi wagami przej ́s ́c pomi ̨edzy w ̨ezłami systemu;~
~ paths— tablica z zapisanymi najkrótszymi ́sciezkami pomi ̨edzy ̇w ̨ezłami.~
~— SetInitialConditions()— ustala pocz ̨atkowe warto ́sci zmiennych, zeruje liczniki, ustawia warunki brzegowe, losuje populacj ̨e na siatce, wstawia komórki w stanie `I`.~
~— LoadData()— funkcja zostaje wywołana pod warunkiem,ze opcja ruchu komórek nie została wył ̨aczona. Słuzy do wczytania do pami ̨eci komputera z zewn ̨etrznego pliku tablicy ́sciezek wybranego przez u ̇ zytkownika wzorca lub, w przypadku braku takiego pliku, stworzenia go samodzielnie.~
Po ustawieniu wszystkich parametrów funkcje odpowiedzialne za prowadzenie symulacji i prezentacj ̨e wyników wywoływane s ̨a w p ̨etlido ... while, której liczb ̨e powtórze ́n okre ́sla czas trwania symulacji podany przez uzytkownika. Funkcje wywoływane w p ̨etli to:~

~— Update()— uaktualniaj ̨aca stany komórek i steruj ̨aca ich poruszaniem si ̨e;~
~— DrawGrid()— odpowiedzialna za prezentacj ̨e graficzn ̨a układu;~
~— ShowLog()— wy ́swietlaj ̨aca parametry charakteryzuj ̨ace populacje (liczby stanów komórek).~

### 4.3. Update function

~FunkcjaUpdate()zbudowana jest z dwóch zagniezd ̇ zonych p ̨etli ̇ forprzebiega-
j ̨acych po elementach tablicycellsz wykorzystaniem zmiennych z zapisan ̨a liczb ̨a
wierszy (rows) i kolumn (cols). Przy kazdym wywołaniu kolejne elementy tablicy ̇
ulegaj ̨a kolejno operacjom:~

~— znacznikowi ruchu (moved) kazdej komórki jest przypisywana warto ́s ́c 0;~
~— wykonywana jest kopia tablicy komórek z poprzedniej iteracji (cellsCopy[i][j] = cells [i][j]);~
~— instrukcja warunkowaswitchwywołuje odpowiedni algorytm (z opisanych w rozdziale 3.3.1) w zalezno ́sci od stanu danej komórki; ̇~
~— dla kazdej komórki wywoływana jest funkcja ̇ NaturalDeathAndBirth();~
~— jezeli opcja ruchu nie została wył ̨aczona decyzj ̨a u ̇ zytkownika, wywoływana jes funkcjaMove(), której opis znajdzie si ̨e w rozdziale 4.5;~
~— na koniec stany wszystkich komórek zostaj ̨a zliczone i zapisane do pliku"output.txt".~

~Uzytkownik w trakcie symulacji ma mo ̇ zliwo ́s ́c jej chwilowego wstrzymania poprzez ̇
naci ́sni ̨ecie spacji lub całkowite przerwanie p ̨etli i powrót do menu, a takze wyj ́scie z ̇
programu.~

### 4.4. State changes

~Z poziomu funkcjiUpdatewywoływane s ̨a funkcje steruj ̨ace zmian ̨a stanów komó-
rek. S ̨a to:~

~— Infection(i,j);~
~— Exposition(i,j);~
~— SpontaneousInfection(i,j);~
~— Recovery(i,j);~
~— EndResistance(i,j);~
~— DeathByIllness(i,j);~
~— NaturalDeathAndBirth(i,j).~

~Funkcje te działaj ̨a w sposób analogiczny, zatem zastan ̨a opisane zbiorowo. Indywidualne opisy algorytmów mozna znale ́z ́c w rozdziale 3.3.1. Funkcja jest wywoływana ̇tylko dla komórek w okre ́slonym stanie. Przyjmuje ona na wej ́sciu dwie zmienne typu
integer, okre ́slaj ̨ace pozycj ̨e komórki na siatce. Na pocz ̨atku losowana jest liczba z przedziału[0, 1], a nast ̨epnie jest ona porównywana z prawdopodobie ́nstwem odpowiednim dla danego przej ́scia. Jezeli wylosowana liczba b ̨edzie mniejsza ni ̇ z to prawdopodobie ́nstwo, komórka zmieni stan. W przypadku funkcjiRecovery(i,j)do zmiennejrecDayopisuj ̨acej komórk ̨e zapisywany jest stan obecnej iteracji,zeby komórka straciła odporno ́s ́c po upływie czasu zdefiniowanym przez uzytkownika (je ̇ zeli ̇
odporno ́s ́c nie jest stała).~

### 4.5. Movement

~Do sterowania ruchem komórek słuzy funkcja ̇ Move()modyfikuj ̨aca połozenie ko- ̇
mórek na siatce w obecnej iteracji. Funkcja przy załozeniu, ̇ ze opcja ruchu nie została ̇
wył ̨aczona, wywoływana jest dla kazdego w ̨ezła w systemie. Najpierw sprawdzany ̇
jest znacznik komórkiinMotion. Jezeli jest on równy 0, losowana jest pewna liczba ̇
z przedziału[0, 1]i porównywana z prawdopodobie ́nstwem wykonania ruchu usta-
lonym przez uzytkownika. Je ̇ zeli prawdopodobie ́nstwo zostanie spełnione, wywoły- ̇
wana jest funkcjaSetDirection(), która losuje punkt na siatce, do którego dana ko-
mórka b ̨edzie si ̨e zbliza ́c wraz z inkrementacj ̨a zmiennej ̇ t. Obranie przez komórk ̨e celu
odbywa si ̨e z uwzgl ̨ednieniem zasi ̨egu komórki okre ́slonego przez uzytkownika pod- ̇
czas ustawiania parametrów symulacji (parametrm). Nast ̨epnie znacznikinMotion
jest ustawiany na warto ́s ́ctrue, informuj ̨ac system,ze dana komórka jest w trakcie ̇
podrózy. Kolejnym etapem jest wykonanie ruchu przez komórk ̨e w kierunku obra- ̇
nego przez ni ̨a celu. Kazda komórka opisana parametrem ̇ inMotion = truejest
sprawdzana pod k ̨atem tego, czy wykonała juz ruch w obecnej iteracji. Je ̇ zeli flaga ̇
mooveddanego w ̨ezła przyjmuje warto ́s ́cfalse, wywoływana jest dla niego funkcja
MoveCell(). Funkcja ta ma na celu:~

~— wyznaczenie optymalnego kroku, który zblizy jednostk ̨e do swojego celu; ̇~
~— przesuni ̨ecie jednostki w wyznaczone miejsce.~

~Obranie kierunku przez komórk ̨e odbywa si ̨e z wykorzystaniem algorytmu Floyda-
-Warshalla (patrz rozdział 4.6) do wyznaczenia najkrótszej ́sciezki do miejsca docelo- ̇
wego z uwzgl ̨ednieniem macierzy odległo ́sci pomi ̨edzy poszczególnymi w ̨ezłami systemu zaimplementowanej do programu. Kolejnym zadaniem funkcji jest poruszenie
jednostki z zachowaniem odpowiednich zasad — jednostka ma prawo przesun ̨a ́c si ̨e
na wyznaczone miejsce tylko w dwóch przypadkach:~

~— jezeli wyznaczone pole, na które ma si ̨e przesun ̨a ́c, nie jest zajmowane przez ̇ zaden inny obiekt (stan =D);~
~— jezeli na wyznaczonym do przesuni ̨ecia polu znajduje si ̨e inna ̇ zywa komórka w ru- ̇
chu i jej nast ̨epnym krokiem b ̨edzie zaj ̨ecie pola zajmowanego przez obecnie rozpa-
trywan ̨a jednostk ̨e. W tym przypadku dochodzi do wymini ̨ecia komórek (zamiany
miejscami) i obie jednostki s ̨a rozpatrywane jako takie, które wykonały ruch.
Jezeli ̇ zaden z powy ̇ zszych warunków nie zostaje spełniony, komórka ma jeszcze ̇
prawo do znalezienia obej ́scia, tak by nie dochodziło do zatorów. W tym wypadku
Komórka szuka wolnego pola w swoim najblizszym s ̨asiedztwie i je ̇ zeli takie znajdzie, ̇
ma prawo zaj ̨a ́c tak ̨a przestrze ́n, ale tylko pod warunkiem,ze nie oddali jej to od celu. ̇~

### 4.6. Floyd-Warshall algorithm

~W pracy zwrócono uwag ̨e na wpływ ruchu jednostek na przebieg epidemii. Posta-
wiono pytanie, czy ukierunkowanie ruchu za pomoc ̨a ́sciezek transportu preferowa- ̇
nych przez jednostki wpłynie na zachowanie systemu. W tym celu w programie zaim-
plementowano trzy wzorce transportu:~

~— Wzorzec 1 — z wykorzystaniem macierzy ́sciezek bez zró ̇ znicowania wagi przej ́s ́c ̇pomi ̨edzy komórkami, gdzie koszt ruchu w kazdym kierunku jest taki sam (rys. ̇4.1 a)~
~— Wzorzec 2 — z wykorzystaniem macierzy ́sciezek z prost ̨a tras ̨a szybkiego ruchu, ̇gdzie koszt poruszania si ̨e pomi ̨edzy komórkami poza wyznaczon ̨a tras ̨a jest wyzszy (rys. 4.1 b)~
~— Wzorzec 3 — z wykorzystaniem macierzy ́sciezek ze skrzy ̇ zowaniem szybkiego ruchu, gdzie koszt poruszania si ̨e pomi ̨edzy komórkami poza wyznaczon ̨a tras ̨a jest wyzszy (rys. 4.1 c)~
~Do wyznaczenia optymalnej trasy posłuzono si ̨e algorytmem Floyda-Warshalla, wykorzystuj ̨acym programowanie dynamiczne do odnalezienia najkrótszych ́sciezek między wszystkimi parami w grafie. Polega na porównaniu kosztu bezpo ́sredniego przej-
́scia z wierzchołkaidojz kosztem trasy uwzgl ̨edniaj ̨acej po ́sredni wierzchołekk w oparciu o dostarczon ̨a mu macierzWzawieraj ̨ac ̨a koszty przej ́s ́c pomi ̨edzy wierzchołkami [20]. Algorytm wyrazony matematycznie przedstawia równanie (4.1).~ ̇

## 5. Results and discussion

~Zaproponowany model pozwala ustali ́c i sprawdzi ́c rózne scenariusze przebiegu ̇
epidemii poprzez bardzo szczegółowe ustalenie poszczególnych parametrów maj ̨a-
cych wpływ na jej rozwój, a zatem umozliwia okre ́slenie sposobu w jaki ka ̇ zdy czyn- ̇
nik z osobna wpływa na populacj ̨e. Szczególn ̨a uwag ̨e po ́swi ̨econo temu, w jaki spo-
sób na rozprzestrzenianie si ̨e epidemii wpłynie obecno ́s ́c pewnych preferowanych tras
w ̨edrówki jednostek. W celu sprawdzenia zasadno ́sci uwzgl ̨ednienia preferowanych
́sciezek obieranych przez osobników w symulowaniu przebiegu epidemii uruchomio- ̇
no symulacj ̨e dla trzech zaproponowanych wzorców ́sciezek z zastosowaniem tych ̇
samych parametrów wej ́sciowych. Parametry uzyte w trakcie symulacji przedstawia ̇
tabela 5.1.~

### 5.1. Random population distribution

~W pierwszym etapie sprawdzono, czy ́sciezki komunikacyjne b ̨ed ̨a miały wpływ na ̇
transmisj ̨e choroby przy załozeniu równomiernego rozkładu populacji na siatce. Wy- ̇
kresy na rysunkach 5.2 i 5.4 przedstawiaj ̨a przebieg przykładowej symulacji. Jak wyni-
ka z wykresów, w przypadku istnienia pojedynczej sieci transportowej epidemia miała
bardzo podobny przebieg pod wzgl ̨edem czasu rozpocz ̨ecia do całkowitego wyga ́sni ̨e-
cia, jak bez zastosowania preferowanych ́sciezek, ale liczba zainfekowanych jednostek ̇
osi ̨aga wyzsze maksimum lokalne. Wyra ́znie równie ̇ z wida ́c, ̇ ze sumaryczna liczba ̇
jednostek, które przeszły chorob ̨e, była zdecydowanie wyzsza. W przypadku scena- ̇
riusza zakładaj ̨acego istnienie dwóch dróg krzyzuj ̨acych si ̨e w centralnej cz ̨e ́sci siatki, ̇
dostrzegalne s ̨a wyra ́zne róznice. W tym wypadku najwy ̇ zszy zapis dotycz ̨acy liczby ̇
zainfekowanych komórek pojawia si ̨e duzo pr ̨edzej (około dnia 50. w porównaniu do 90. i 110. w poprzednich scenariuszach), ale równiez szybciej wygasa. Zarówno maksimum jak i sumaryczna liczba chorych jest znacz ̨aco wyzsza ni ̇ z w innych modelach ̇
́sciezek. Porównanie liczby zara ̇ zonych dla wszystkich wzorców ́scie ̇ zek przedstawia wykres na rys. 5.1.~

### 5.2. Isolation of healthy population

~Kolejnym etapem było okre ́slenie wpływu obecno ́sci cz ̨esto ucz ̨eszczanych tras na
zarazanie jednostek przez odległe ́zródło. W tym celu odseparowano zdrow ̨a popula- ̇
cj ̨e poprzez umieszczenie jej na połowie siatki od chorej jednostki, która pocz ̨atkowo
zajmowała miejsce na drugiej połowie siatki. Pocz ̨atkowe rozstawienie osobników ob-
razuje rys. 5.3.
Przy takim załozeniu istnienie preferowanych ́scie ̇ zek ma jeszcze wi ̨eksze znaczenie ̇
dla rozwoju epidemii. Wielokrotnie powtarzane symulacje wykazały,ze w przypadku ̇
braku cz ̨esto obieranej wspólnej drogi (wzorzec 1) epidemia rozwija si ̨e ́srednio 1 raz
na 10 prób. Jej przebieg za ́s w porównaniu do symulacji przeprowadzonych na innych
wzorcach ́sciezek jest podobny do tego, w których jednostki nie były od siebie po- ̇
cz ̨atkowo izolowane, tj. trwa najdłuzej i charakteryzuje si ̨e ni ̇ zsz ̨a szczytow ̨a warto ́sci ̨a ̇
zarazonych jednostek oraz sumaryczn ̨a liczb ̨a osobników, które przeszły chorob ̨e. W przypadku istnienia pojedynczej, prostej ́sciezki transportowej rozwój epidemii, tak jak w poprzednim przypadku, bardzo przypominał ten bez preferowanych ́sciezek. ̇
Szanse na rozwini ̨ecie si ̨e epidemii były niewiele wyzsze, poniewa ̇ z wyst ̨epowała ona ̇
́srednio w 2 na 10 przypadków. Najcz ̨e ́sciej rozwój epidemii obserwowano w przy-
padku zastosowania trzeciego wzorca ́sciezek, zakładaj ̨acego przecinanie si ̨e 2 cz ̨esto ̇
ucz ̨eszczanych dróg. W tym scenariuszu epidemia pojawiała si ̨e w 6 na 10 prób. Po-
nownie zostały potwierdzone wyniki z poprzedniego etapu, gdzie g ̨estsza sie ́c komu-
nikacyjna spowodowała szybsz ̨a transmisj ̨e choroby, wi ̨eksz ̨a liczb ̨e zainfekowanych
osobników i wcze ́sniejsze wyga ́sni ̨ecie epidemii. Rezultaty tych symulacji przedsta-
wiaj ̨a wykresy na rys. 5.4.~

### 5.3. Statistic tests

~W celu weryfikacji załoze ́n na temat wpływu sieci komunikacyjnych na charakter ̇
rozwoju epidemii, wykonano testowanie statystyczne w oparciu o dane uzyskane z
dwudziestu symulacji przeprowadzonych dla kazdego z zaproponowanych wzorców ̇
́sciezek. Otrzymane dane składały si ̨e z 365 wpisów zawieraj ̨acych informacje o liczbie ̇
poszczególnych stanów komórek w kolejnych jednostkach czasu symulacji. Dane po-
chodziły z rozkładu normalnego, ale nie wykazywały równo ́sci wariancji, dlatego po-
słuzono si ̨e testem Welcha-Aspina przyjmuj ̨ac za poziom ufno ́sci ̇ _α_ =0,05. W pierwszej
kolejno ́sci sprawdzono hipotez ̨e zakładaj ̨ac ̨a równo ́s ́c ́sredniego czasu trwania epide-
mii (tj. od pierwszego zachorowania do całkowitego wyeliminowania patogenu z po-
pulacji) przy alternatywie,ze jest mniejszy w próbie o bardziej g ̨estej infrastrukturze ̇
drogowej:~


```
~H 0 :Sredni czas trwania epidemii jest taki sam dla obu prób;~
~H 1 :Srednia czas trwania epidemii jest mniejszy w przypadku wi ̨ekszej liczby dróg.~
```

~Test Welcha-Aspina nie wykazał podstaw do odrzucenia hipotezy zerowej dla wzor-
ców 1 i 2, natomiast pomi ̨edzy ́srednimi w próbach dla wzorców 1 i 3 oraz 2 i 3 wyst ̨api-
ły wysoce istotne róznice statystyczne, co potwierdza zało ̇ zenie, ̇ ze wyst ̨epowanie w ̨e- ̇
złów komunikacyjnych przyspiesza rozwój epidemii. Sprawdzono równiez, czy wy- ̇
st ̨epowanie preferowanych dróg transportu zwi ̨eksza liczb ̨e zachorowa ́n. W tym celu
porównano ze sob ̨a ́srednie sumarycznej liczby osobników, które przeszły chorob ̨e, jak
równiez najwy ̇ zszej odnotowanej liczby zainfekowanych jednostek w poszczególnych ̇
symulacjach. Testowano hipotez ̨e:~

```
~H 0 :Srednia liczba zachorowa ́n jest taka sama dla obu prób; ́~
~H 1 :Srednia liczba zachorowa ́n jest mniejsza w przypadku wi ̨ekszej liczby dróg. ́~
```
~Podobnie jak w poprzednim przypadku test Welcha-Aspina nie wykazał róznic po- ̇
mi ̨edzy ́srednimi w próbach dla wzorców 1 i 2, ale pomi ̨edzy wzorcami 1 i 3 oraz
2 i 3 wyst ̨apiły róznice wysoce istotne, zatem równie ̇ z i w tym wypadku potwierdzono ̇
wpływ wyst ̨epowania w ̨ezłów komunikacyjnych na transmisj ̨e choroby.~


## 6. Conclusions

~Modele epidemiologiczne oparte na automatach komórkowych, które s ̨a w stanie
przewidzie ́c stan kazdej jednostki z osobna w zale ̇ zno ́sci od działaj ̨acych na ni ̨a czyn- ̇
ników ́srodowiskowych, mog ̨a okaza ́c si ̨e niezwykle uzytecznym narz ̨edziem do sy- ̇
mulacji ze wzgl ̨edu na ich zdolno ́s ́c do uwzgl ̨edniania bardzo szczegółowych danych
i istotnych czynników maj ̨acych wpływ na jednostk ̨e. W niniejszej pracy zapropono-
wano model SEIAR uwzgl ̨edniaj ̨acy osobników w okresie utajenia patogenu, a tak-
ze tych z czynnikiem chorobotwórczym w trakcie inkubacji, którzy mimo nie wyka- ̇
zywania objawów odgrywaj ̨a wazn ̨a rol ̨e w zara ̇ zaniu podatnych jednostek. Nale ̇ zy ̇
jednak zauwazy ́c, ̇ ze u ̇ zytkownikowi dana jest mo ̇ zliwo ́s ́c samodzielnego ustawienia ̇
wszystkich parametrów z osobna, co w rezultacie pozwala mu na wykluczenie z sy-
mulacji poszczególnych stanów, takze z powodzeniem mo ̇ zliwe jest przeprowadzanie ̇
symulacji w oparciu o inne modele, takie jak SEIR. Implementacja algorytmu do wy-
szukiwania najkrótszych ́sciezek umo ̇ zliwia dodatkowo uwzgl ̨ednienie tak istotnych ̇
czynników dla transmisji choroby jak istnienie głównych sieci tranzytowych czy ba-
rier geograficznych, co moze wpływa ́c na dokładniejsze i bardziej realistyczne analizy ̇
uwzgl ̨edniaj ̨ace zachowanie ludzi w ruchu. Przedstawione wyniki dotycz ̨a konkret-
nego zestawu parametrów przedstawionych w tabeli 5.1 i potwierdzaj ̨a,ze obecno ́s ́c ̇
preferowanych tras ruchu ma wpływ na przebieg epidemii. Wyzszy odsetek zara ̇ ze ́n, ̇
którego wzrost widoczny jest wraz ze wzrostem g ̨esto ́sci poł ̨acze ́n jest wynikiem gro-
madzenia si ̨e wi ̨ekszej liczby osobników w miejscach głównych w ̨ezłów komunikacyj-
nych. W tych obszarach, gdzie jednostki podrózuj ̨ace w ró ̇ znych kierunkach przecina- ̇
j ̨a swoje drogi nast ̨epuje znaczne lokalne zag ̨eszczenie (patrz rys. 6.1), co wpływa na
wi ̨eksze prawdopodobie ́nstwo zaraze ́n mi ̨edzy osobnikami. Z tego wzgl ̨edu epidemia ̇
miała duzo szybszy i gwałtowniejszy przebieg, kiedy dwie trasy krzy ̇ zowały si ̨e ze ̇
sob ̨a, anizeli w przypadku istnienia jednej prostej drogi. ̇~

## Bibliography

~[1] Ahmed E., Agiza H. N. [1998]:On modeling epidemics. Including latency, incubation and
variable susceptibility, Physica A 253, 347–352.
[2] Bernoulli D. [1766]:Essai dèune nouvelle analyse de la mortalite causee par la petite verole,
Mem Math Phy Acad Roy Sci Paris.
[3] Chen T., Huang Y., Liu R., Xie Z., Chen S., Hu G. [2017]:Evaluating the effects of common
control measures for influenza A (H1N1) outbreak at school in China: A modeling study, PLoS
One, 12(5)a
[4] Praca zbiorowa:Encyklopedia, Wydawnictwo Naukowe PWN,
[http://encyklopedia.pwn.pl/haslo/epidemia;3898232.html](http://encyklopedia.pwn.pl/haslo/epidemia;3898232.html) (25.04.2017).
[5] Kermack O., McKendrick A. G. [1927]:Contributions to the mathematical theory of epidemics,
part I, Proc. R. Soc. Edin. A, 115, 700–721.
[6] Lorenz E. [2000]:The butterfly effect, WORLD SCIENTIFIC SERIES ON NONLINEAR
SCIENCE SERIES A, 39, 91–94.
[7] López L., Burguerner G., Giovanini L. [2014]:Addressing population heterogeneity and
distribution in epidemics models using a cellular automata approach, BMC Research Notes,
7:234.
[8] Malarz K.:Automaty komórkowe, notatki do wykładu,
[http://www.zis.agh.edu.pl/ak/ca.pdf](http://www.zis.agh.edu.pl/ak/ca.pdf) (25.04.2017).
[9] Papadimitriou C. H.:Złozono ́s ́c obliczeniowa ̇ , Wydawnictwa Naukowo-Techniczne, 2002,
182–192.
[10] Scott S., Duncan C. J. [2001]:Biology of Plagues: Evidence from Historical Populations,
Cambridge University Press.
[11] Shapiro M., Delgado-EckertE. [2012]:Finding the probability of infection in an SIR network is
NP-Hard, Math Biosci., 240(2): 77–84.
[12] Sirakoulis G. Ch., Karafyllidis I., Thanailakis A. [2000]:A cellular automaton model for the
effects of population movement and vaccination on epidemic propagation, Ecol. Model. 133,
209–223.
[13] Stefano B. D., Fuks H., Lawniczak A. T. [2000]:Object-oriented implementation of CA/LGCA
modelling applied to the spread of epidemics, In 2000 Canadian Conference on Electrical and
Computer Engineering, vol. 1, IEEE, 26–31.
[14] The Ecology Global Network, [http://www.ecology.com/birth-death-rates/](http://www.ecology.com/birth-death-rates/) (25.04.2017).
[15] Von Neumann J., Burks A. W. [1966]:Theory of self-reproducing automata. IEEE Transactions
on Neural Networks, 5(1), 3–14.
[16] White S. H., del Rey A. M., Sánchez G. R. [2007]Modeling epidemics using cellular
automata, Appl Math Comput 186: 193–202.
[17] White S. H., del Rey A. M., Sánchez G. R. [2009]:Using cellular automata to simulate
epidemic diseases, Applied Mathematical Sciences, 3(20), 959–968.
[18] Wolfram S. [1984]:Universality and complexity in cellular automata, Physica D: Nonlinear
Phenomena, 10(1–2), 1–35.
[19] Wikipedia, the free encyclopedia, https://en.wikipedia.org/wiki/Rule_110 (08.06.2017).
[20] Wróblewski P. [2010]:Algorytmy, struktury danych i techniki programowania, Helion, wyd.
IV: 224–225.~

## List of drawings

~1.1 Schemat funkcji stanu(F).................................. 12~
~1.2 Przykładowy wynik reguły 110 ( ́zródło: Wikipedia [19])................ 14~
~1.3 Przykładowe rodzaje siatek................................. 15~
~1.4 Typy s ̨asiedztwa....................................... 15~
~1.5 Etapy infekcji wirusowej................................... 17~

~2.1 Schemat modelu SIR..................................... 19~
~2.2 Schemat modelu SEIAR................................... 20~

~3.1 Model SEIAR z prawdopodobie ́nstwami przej ́s ́c mi ̨edzy stanami........... 25~
~3.2 Algorytm infekcji....................................... 29~
~3.3 Algorytm ekspozycji..................................... 29~
~3.4 Algorytm spontanicznych zachorowa ́n.......................... 30~
~3.5 Algorytm powrotu do zdrowia............................... 31~
~3.6 Algorytm ́smierci wywołanej chorob ̨a........................... 32~
~3.7 Algorytm naturalnych narodzin i ́smierci......................... 33~
~3.8 Algorytm ruchu........................................ 34~

~4.1 Wzorce transportu...................................... 40~

~5.1 Liczba zarazonych jednostek................................ ̇ 43~
~5.2 Rozwój epidemii w czasie.................................. 45~
~5.3 Pocz ̨atkowa dystrybucja populacji............................. 46~
~5.4 Rozwój epidemii w czasie.................................. 48~

~6.1 Koncentracja jednostek w miejscu w ̨ezła komunikacyjnego............... 51~

## List of tables

~1.1 Reguła 110........................................... 13~

~2.1 Podsumowanie modeli................................... 22~

~3.1 Prawdopodobie ́nstwa przej ́s ́c pomi ̨edzy stanami.................... 27~

~5.1 Parametry symulacji..................................... 41~ 
