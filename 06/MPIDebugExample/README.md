# MPI Debugging Example
Ein kleines Beispiel, das Debugging mit MPI zeigt. Das Beispielprogramm sollte mit 2 Prozessen ausgeführt werden.

Das hier vorgestellte Setup wurde mit Ubuntu 22.04, cmake 3.22, gcc12, gdb 12.1, vscode 1.84.2 getestet.

## VSCode setup
In diesem Beispiel ist bereits eine passende `launch.json` vorhanden. Allgemeines vorgehen:
1. [.vscode/launch.json](.vscode/launch.json) erstellen (z.B. über den "Run and Debug"-Tab -> "create a launch.json file").
1. Neue Configuration `gdb Attach` erstellen (`gdb` schreiben und den vorgeschlagenen `gdb Attach` Eintrag wählen).
1. Pfad zur executable in `program` eintragen.
1. Eintrag `processId` erstellen mit Wert `"${command:pickProcess}"`.
1. (optional) Name der Configuration anpassen.

## MPI Code einrichten und debuggen
1. Eine unendliche while-Schleife an den Anfang des Programms setzen, die auf den Debugger wartet.
1. Programm im Debug-Modus compilen (command palette -> cmake: select variant -> Debug).
1. MPI-Programm wie üblich starten - wird dann in der while-Schleife warten.
1. Für jeden Prozess den Debugger starten und an die jeweiligen Prozesse anhängen.
1. Im Debugger den Prozess pausieren, Variable der while-Schleife anpassen.
1. Wie üblich debuggen.
1. Zuletzt die unendliche while-Schleife wieder entfernen.