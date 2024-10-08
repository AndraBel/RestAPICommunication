# PROTOCOALE DE COMUNICAŢIE. Tema 4. Client Web. Comunicaţie cu REST API.

Belceanu Andra-Maria 

Grupa 321CA

# Schelet de cod

Pentru implementarea acestui proiect, am pornit de la scheletul de cod furnizat în exercițiile de laborator. Am utilizat și adaptat fișierele `helpers.c` și `buffer.c` fără a le modifica. În special, am folosit extensiv funcțiile din `helpers.c`, cum ar fi `open_connection`, `close_connection`, `send_to_server`, `receive_from_server` și `basic_extract_json_response`.

În plus, am utilizat fișierul `requests.c` din exercițiile de laborator, dar l-am modificat și l-am implementat conform cerințelor proiectului. În acest fișier, am implementat următoarele funcții:

- `compute_get_request`: Composează o cerere de tip GET care trebuie trimisă la server. Include parametrii cum ar fi gazda, URL-ul, parametrii de interogare, token-ul de autentificare, cookie-urile și numărul lor.

- `compute_post_request`: Composează o cerere de tip POST care trebuie trimisă la server. Include parametrii cum ar fi gazda, URL-ul, tipul de conținut, token-ul de autentificare, încărcătura (corpul cererii), cookie-urile și numărul lor.

- `compute_delete_request`: Composează o cerere de tip DELETE care trebuie trimisă la server. Include parametrii cum ar fi gazda, URL-ul, parametrii de interogare, token-ul de autentificare, cookie-urile și numărul lor.

# Biblioteca JSON

Am ales biblioteca JSON nlohmann pentru manipularea obiectelor JSON datorită documentației bune și ușurinței de utilizare. Pentru a imprima un obiect JSON, am folosit funcția `dump(1)` cu un nivel de indentare de 1 pentru o mai bună citire. Adăugarea unui câmp într-un obiect JSON este simplă, similară cu lucrul cu un vector, iar parsarea este făcută folosind funcția `parse`. Pentru căutarea în obiectele JSON, am folosit funcția `contains`.

## Implementare Client

Pentru implementarea clientului, am asigurat o alocare și dezalocare adecvată a memoriei ori de câte ori a fost necesar. În cazul în care alocarea memoriei a eșuat, este afișat un mesaj de eroare.

Pentru fiecare comandă, am implementat o funcție separată:

### Înregistrare Client

- Funcția `handle_register` permite înregistrarea unui cont nou.
- Se citește de la tastatură numele de utilizator și parola.
- Conexiunea către server se deschide și se creează un obiect JSON cu datele introduse.
- Cererea de tip POST este trimisă către server.
- Răspunsul serverului este verificat și se afișează mesaje corespunzătoare.
- Conexiunea se închide la final.

### Autentificare Client

- Funcția `handle_login` efectuează autentificarea într-un cont existent.
- Numele de utilizator și parola sunt citite de la tastatură.
- Se creează un obiect JSON cu datele introduse și se trimite o cerere de tip POST pentru autentificare către server.
- Răspunsul serverului este verificat, iar cookie-ul de sesiune este salvat pentru utilizare ulterioară.
- Conexiunea se închide la final.

### Acces la Bibliotecă

- Funcția `handle_access` solicită accesul la bibliotecă.
- Se trimite o cerere de tip GET către server cu cookie-ul de sesiune.
- Răspunsul serverului este verificat, iar token-ul este salvat pentru utilizare ulterioară.
- Conexiunea se închide la final.

### Informații despre Cărți

- Funcția `get_books` afișează informațiile despre toate cărțile din bibliotecă.
- Se trimite o cerere de tip GET către server cu token-ul de acces.
- Lista cărților sau un mesaj de eroare este afișat în funcție de răspunsul serverului.
- Conexiunea se închide la final.

### Informații despre o Anumită Carte

- Funcția `get_book` afișează informațiile despre o anumită carte.
- ID-ul cărții este citit de la tastatură și trimis în cadrul cererii de tip GET.
- Răspunsul serverului este verificat, iar informațiile despre carte sau un mesaj de eroare sunt afișate corespunzător.
- Conexiunea se închide la final.

### Adăugare Carte

- Funcția `add_book` adaugă o carte nouă în bibliotecă.
- Detaliile despre carte sunt citite de la tastatură și transformate într-un obiect JSON.
- Cererea de tip POST este trimisă către server cu token-ul de acces și datele despre carte.
- Răspunsul serverului este verificat, iar un mesaj corespunzător este afișat.
- Conexiunea se închide la final.

### Ștergere Carte

- Funcția `delete_book` șterge o carte din bibliotecă.
- ID-ul cărții de șters este citit de la tastatură și trimis în cadrul cererii de tip DELETE.
- Cererea este trimisă către server cu token-ul de acces.
- Răspunsul serverului este verificat, iar un mesaj corespunzător este afișat.
- Conexiunea se închide la final.

### Deconectare Client

- Funcția `handle_logout` efectuează deconectarea clientului.
- Se trimite o cerere de tip GET către server pentru a închide sesiunea.
- Răspunsul serverului este verificat, iar un mesaj corespunzător este afișat.
- Conexiunea se închide la final.
- Se sterg din memorie cookie-ul de sesiune si token-ul de access

Programul principal rulează într-o buclă infinită unde citește comenzile de la utilizator și apelează funcția corespunzătoare în consecință.
