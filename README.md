## Wspolbiezne-Projekt-Samochod
# Projekt na przedmiot programowanie współbieżne na temat prowadzenia pojazdu przez wiele osób

Projekt dostal ocene 5.0, jeżeli coś nie jest zrozumiałe/nie działa proszę pisać w Issue a spróbuje wytłumaczyć. Kod napisany w xlib na gniazdach BSD wykorzystujący wielowątkowość. Ten projekt nie jest napisany optymalnie/wykorzystując najlepszych sposobów, gdyż był pisany w krótkim okresie czasu a ja nie jestem/nie byłem specjalnie obeznany w bibliotekach xlib, a w c pisałem porządnie ostatnio jakiś rok temu/dwa lata temu.

Autor programu: Michał Olkiewicz
Temat: Symulacja prowadzenia pojazdu przez wiele osób naraz

1.Krótki opis programu

Program ma symulować prowadzenie pewnego pojazdu np. samochodu.
Jest on reprezentowany przez czerwony prostokat z szybą(przodem samochodu) reprezentowaną przez jasno niebieski prostokąt.
Ilość graczy nie malże nie ograniczona(ustawiona na stale na 999 graczy, mozna powiększyć).
Pojazdem można skręcać(obraca się) za pomocą strzałek lewo i prawo, i jechać do przodu i do tyłu za pomocą odpowiadającyh strzałek.

2. Merytoryczne uzasadnienie wyboru mechanizmu komunikacji międzyprocesowej
Proces programu podzielony na dwa pliki, serwer i klient. Komunikują się za pomocą gniazd BSD.

Wykorzystywana jest też wielowątkowość, aby proces serwera jak i klienta mógł wysyłać do siebie informacje, podczas gdy główna pętla programu ma za zadanie otrzymywać ciągle informacje przesyłane.

Serwer ma informacje dotyczące nachylenia jak i koordynaty pojazdu w danej chwili i przesyła je do wszystkich klientów połączonych z serwerem.

3. Opis użytkowania programu

Kompilacja:
gcc -o klient klient.c -lm -pthread -lX11
gcc -o serwer serwer.c -lm -pthread

Serwer trzeba skompilować tylko raz i odpalić go jako pierwszy. Na każdym terminale trzeba osobno otworzyć program klienta podając ip serwera. Każdy gracz może za pomocą strzałek poruszać pojazd i informacje są przetwarzane przez serwer i pokazywane na ekranie każdego klienta.

4. Sytuacje wyjątkowe

Sytuacja: Nie podano adresu ip przy wywołania klienta
Zachowanie: Komunikat o braku argumentu

Sytuacja: Zmiana wielkości ekranu
Zachowanie: Zablokowanie ekranu do danej rozdzielczości 800x600

Sytuacja: Brak odpalenia aplikacji serwera
Zachowanie: Samochód nie jest rysowany po stronie klienta
