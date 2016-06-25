# LED-Pattern Sprache

In dieser Datei geht es um die kleine Sprache mit der die LEDs angesteuert werden sollen. In der Ideen Datei wurde ja schon grob folgendes vorgestellt:

```

+-------------------------------------------------------------------+
Start|0|1|1->255|Start|0|1|2->255|Start|0|1|3->255|Start|0|1|4->255|
+-------------------------------------------------------------------+

```

Fuer die erste Testimplementierung mache ich aber noch ein paar Änderungen. 

Startsymbol: $
Trennsymbol: |
Adressierung: Dezimalzahl
Zeitangabe: 0-999 Sekunden.
Adresse:0-999
Zuweisung Adresse nach Farbe: >
Farbe: hier wirds tricky. Es gibt sowohl Farbige als auch einfarbige LEDs. Mein Vorschlag Einfarbige LEDs werden ueber eine Dezimalzahl 0-255 angesprochen. Farbige LEDs bekommen drei Dezimalzahlen getrennt durch ein Komma zugewiesen. Jetzt die Fehlerfälle: Bekommt eine einfarbige LEDs 3 Werte zugewiesen so wird der erste genommen. Bekommt eine farbige LED nur einen Wert zugewiesen so gilt dieser fuer Rot, Grün und Blau. Werden zwei Werte zugewiesen -> Error. Das komplette Eingabealphabet umfasst damit diese Symbole: $ | > , 0 1 2 3 4 5 6 7 8 9 Es werden also nur 14 Symbole genutzt. So dass man alle anderen Dinge weiterhin verwenden kann um Kommentare zu machen.

Nich lang schnacken regex machen:
(\$\|[0-9]{1,3}\|[0-9]{1,3}\|[0-9]{1,3}>([0-9]{1,3}\|)|([0-9]{1,3},[0-9]{1,3},[0-9]{1,3}\|))*
