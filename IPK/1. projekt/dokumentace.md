## Popis problému
Extrahování dat počasí z open API https://openweathermap.org/api a následná interpretace dat uživateli.
___
## Návrh řešení
Při úspěšněm zpracování argumentů dojde k vytvoření socketu příkazem `tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)`. Tento socket odešle odpověď cílovému serveru. Na straně serveru dojde k vytvoření server socketu, který poslouchá na portu 80. Server socket neodesílá žádná data, ani nezpracovává, pouze produkuje client sockety, které jsou vytvořený na základě příchozích jiných client socketu od uživatel (příkaz `tcp_socket.connect((SERVER, PORT))`). Poté dojde k „propojení“ těchto socketů, které mezi sebou můžou komunikovat. Požadavkem GET si vyžádáme konkrétní data od serveru. Protokol HTTP využívá socket pouze pro 1 přenost, tedy poté co odešleme požadavek a přijmeme data, je socket zničen. Klient tedy dokáže detekovat konec odpovědi tím, že obdrží 0 bitů příkazem `recv`.  
Poté co jsou data úspěněně doručena dojde k jejich zpracování a následné interpretaci uživateli.
___
## Instalace a překlad aplikace
- Pro správný běh aplikace je zapotřebí mít nainstalovanou Python verze 3.x. Odkazy ke stažení zde :
  - *[Windows](https://www.python.org/downloads/windows/)*
  - *[Linux/UNIX](https://www.python.org/downloads/source/)*
  - *[Mac OS X](https://www.python.org/downloads/mac-osx/)*
  - *[ostatní (např. Solaris, iOS...)](https://www.python.org/download/other/)*
___

## Jak spustit aplikaci

### Potřebné kroky před spuštěním skriptu
- Uživatel potřebuje vlastní API klíč, přes který se bude autorizovat u serveru.
  - Po registraci na openweathermap.org uživatel obdrží API klíč. Odkaz k registraci *[zde](https://home.openweathermap.org/users/sign_up)*
- Uživatel musí mít oprávnění spouštět daný skript. V případě Linux/UNIX systému musí mít skript spustitelný přiznak 
  - Přidání spustitelného příznaku pomocí příkazu: `chmod +x <jméno_souboru>`.
- Soubory *weather<i>.</i>py* a *Makefile* se musí nácházet ve stejné složce pro správnou funkčnost.

### Spuštění skriptu
Spuštění skriptu probíhá pomocí příkazu `make run arg=<hodnota>`, kde argumentů může být více a pro různe metody vyhledávání se používají různé argumenty (viz. dále). Na pořadí argumentů nezáleží.
___
#### Vyhledávání podle názvu města.
Vhodné, pokud má město celosvětově unikátní název. Např. vyhledání měst Praha a Tokio.  
>`make run api_key=b774f6086c60501d6e38bb770beee97f city=Brno`  
>`make run api_key=b774f6086c60501d6e38bb770beee97f city=Tokyo`    

Při vyhledávání více slovného města se argument *city* použije následujícím způsobem `city="hodnota1 hodnota2 hodnota3..."` Např.  
>`make run api_key=b774f6086c60501d6e38bb770beee97f city="Nové Město nad Metují"` 
___
#### Vyhledávání podle názvu města + ISO 3166-1 alpha-2.
Může nastat situace, kdy se město může nacházet ve více státech se stejným názvem. K identifikaci státu se tedy použije kód země podle ISO 3166-1. Standard ISO 3166-1 je k dispozici např. na adrese https://cs.wikipedia.org/wiki/ISO_3166-1 nebo https://www.iso.org/obp/ui/#search  

V 1. případě město Varšava ve státu Polsko, v 2. případě Varšava ve státu Indiana ve Spojených státech amerických.
>`make run api_key=b774f6086c60501d6e38bb770beee97f city=Warsaw,pl`   
>`make run api_key=b774f6086c60501d6e38bb770beee97f city=Warsaw,us`

Ani tato metoda neřeší problematiku, kdy se město s jedním názvem může v nacházet hned několikrát v jednom státu. Vhodnější je tedy použít metodu hledání podle ID, kdy každé město má svoje vlastní unikatní ID nebo vyhledávat podle zeměpisných souřadnic.
___
#### Vyhledávání podle ID
Každé  město má vygenerováno unikatní ID, podle kterého se dá vyhledat. Seznam všech ID je dostupný ve formátu JSON ke stažení zde: http://bulk.openweathermap.org/sample/city.list.json.gz

V 1. případě město Cairns ve státě Queensland v Austrálii. V 2. příapdě město Hässelby ve Švédsku.
>`make run api_key=b774f6086c60501d6e38bb770beee97f id=2172797`  
>`make run api_key=b774f6086c60501d6e38bb770beee97f id=7839202`
___
#### Vyhledávání podle zeměpisných souřadnic.
Společně s vyhledávním podle ID se jedná o nejspolehlivější metodu. Zeměpisná souřadnice se skládá ze 2 parametru - *LAT* a *LON*. Tyto údaje jde vyhldeat buď v souboru obsahující ID měst (odkaz výše) nebo online vygenerovat např. zde: https://www.latlong.net/. 

V 1. případě New York ve státě New York ve Spojených státech amerických. V 2. případě Kapské Město v Jihoafrické republice.
>`make run lat=40.7127 lon=-74.005 api_key=b774f6086c60501d6e38bb770beee97f`  
>`make run lat=--33.942572 lon=18.420463 api_key=b774f6086c60501d6e38bb770beee97f`


