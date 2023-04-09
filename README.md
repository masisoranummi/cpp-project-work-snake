# C++ Lopputyö: Snake/Matopeli

Tämä projekti on oma versioni komentorivillä pyörivästä matopeli-pelistä, käyttäen C++-ohjelmointikieltä.

## Käyttöohjeet

Peli on kehitetty ja testattu windowsin command prompt-komentorivillä, suosittelen sen käyttämistä pelin pelaamiseen.
Pelataksesi peliä sinun pitää ensin kääntää ohjelmat .exe muotoon. Itse käytin g++ kääntämiseen, jolloin komentorivillä annoin komennon:
`g++ -o snake.exe source.cpp SnakeHead.cpp SnakeBody.cpp`
Kun olet kääntänyt, voit ajaa peliä komennolla `snake.exe`, tai tuplaklikkaamalla snake.exeä file-explorerissä.
Pelin käynnistämisen jälkeen voit painaa 'x' käynnistääksesi pelin, painaa 'z' nähdäksesi high scoret tai painaa 'c' poistuaksesi pelistä.
Pelissä ideana on kerätä hedelmiä('\*'-merkki), ja yrittää olla törmäämättä itseensä tai seiniin. Voit ohjata käärmettä käyttämällä nuolinäppäimiä. Hedelmän kerätessä käärmeen pituus kasvaa ja nopeus suurenee, jolloin seinien ja käärmeen muun vartalon vältteleminen vaikenee. Peli loppuu, kun törmäät johonkin, jonka jälkeen peli pyytää sinun laittamaan nimesi. Tämän jälkeen se tallentaa sen scores.txt tiedostoon, jonka jälkeen peli näyttää 5 korkeinta tulosta, jos löytyy 5. Tämän jälkeen peli palaa päävalikkoon.

## Arviointi

Kaikki arviointikriteerit toteutuvat, en aluksi löytänyt perinnälle oikein mitään paikkaa projektissa, mutta tein nyt semi-"turhan" kantaluokan SnakeHead- ja SnakeBody-luokille. Omasta mielestä selviydyin projektista kiitettävästi. Vähän olisin mielellään vähentänyt koodintoistoa varsinkin inputin lukemiseen liittyvissä asioissa, ja rehellisesti sanottuna en olisi varmaan ilman googlen apua saanut inputtia tai koko konsolin buffer-systeemiä ikinä yksin selville. En vieläkään ymmärrä 100% mitä jokainen rivi niistä tekee ja miten, mutta ainakin ymmärrän mitä ne tekevät yhdessä ja osaan tarpeeksi hyvin manipuloida niitä tällaiseen projektiin. Kaikenkaikkiaan tyytyväinen projektin tulokseen, hyvää oppimista c++-kielestä.
