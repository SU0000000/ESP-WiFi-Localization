��    L      |  e   �      p  �   q  ,   l  5   �  N   �  7     \   V  _   �  `   	  u   t	  b   �	  V   M
  �   �
  %   4     Z     q     �     �     �     �     �  $        5     G     b     s     |  #   �     �     �     �     �            H   &     o     �  !   �     �     �     �       #   %     I     c  $   �     �     �  #   �  2        8      L     m     �  *   �  *   �                 0  #   >  #   b  &   �     �     �  ,   �          !  -   6     d     s     �     �     �     �     �     �  �  	  "  �  -   �  ?   (  W   h  ?   �  X      c   Y  \   �  r     e   �  Y   �  [   M  +   �     �     �          '  +   >  #   j  7   �  *   �     �          1     B  *   a  (   �  "   �     �  4   �          0     J  P   ^     �     �  4   �          6  +   T      �  "   �     �  )   �  0     ,   =  !   j  ,   �  &   �     �     �  '     '   @  =   h  .   �  $   �     �       #     #   <  )   `     �  *   �  6   �              6   ;      r            �      �      �      �      �   1   !     6      +   =   .   2   H   )       #                           *                          3   C   5          >      I   	       (   /   E   <       4   %   J          G       "       L                ,   -              B       K      1   &                      0          ?   A   '           F          !   9                $   
                ;      7   8   :   D   @    
If no -e, --expression, -f, or --file option is given, then the first
non-option argument is taken as the sed script to interpret.  All
remaining arguments are names of input files; if no input files are
specified, then the standard input is read.

       --help     display this help and exit
       --version  output version information and exit
   --follow-symlinks
                 follow symlinks when processing in place
   --posix
                 disable all GNU extensions.
   -R, --regexp-perl
                 use Perl 5's regular expressions syntax in the script.
   -b, --binary
                 open files in binary mode (CR+LFs are not processed specially)
   -e script, --expression=script
                 add the script to the commands to be executed
   -f script-file, --file=script-file
                 add the contents of script-file to the commands to be executed
   -l N, --line-length=N
                 specify the desired line-wrap length for the `l' command
   -n, --quiet, --silent
                 suppress automatic printing of pattern space
   -u, --unbuffered
                 load minimal amounts of data from the input files and flush
                 the output buffers more often
 %s: -e expression #%lu, char %lu: %s
 %s: can't read %s: %s
 %s: file %s line %lu: %s
 : doesn't want any addresses Invalid back reference Invalid character class name Invalid collation character Invalid content of \{\} Invalid preceding regular expression Invalid range end Invalid regular expression Memory exhausted No match No previous regular expression Premature end of regular expression Regular expression too big Success Trailing backslash Unmatched ( or \( Unmatched ) or \) Unmatched \{ Usage: %s [OPTION]... {script-only-if-no-other-script} [input-file]...

 `e' command not supported `}' doesn't want any addresses can't find label for jump to `%s' cannot remove %s: %s cannot rename %s: %s cannot stat %s: %s command only uses one address comments don't accept any addresses couldn't attach to %s: %s couldn't edit %s: is a terminal couldn't edit %s: not a regular file couldn't follow symlink %s: %s couldn't open file %s: %s couldn't open temporary file %s: %s delimiter character is not a single-byte character error in subprocess expected \ after `a', `c' or `i' expected newer version of sed extra characters after command invalid reference \%d on `s' command's RHS invalid usage of +N or ~N as first address invalid usage of line address 0 missing command multiple `!'s multiple `g' options to `s' command multiple `p' options to `s' command multiple number options to `s' command no input files no previous regular expression number option to `s' command may not be zero option `e' not supported read error on %s: %s strings for `y' command are different lengths unexpected `,' unexpected `}' unknown command: `%c' unknown option to `s' unmatched `{' unterminated `s' command unterminated `y' command unterminated address regex Project-Id-Version: sed 4.2.0
Report-Msgid-Bugs-To: bug-gnu-utils@gnu.org
POT-Creation-Date: 2018-03-31 18:40-0700
PO-Revision-Date: 2008-01-16 12:44+0100
Last-Translator: Paolo Bonzini <bonzini@gnu.org>
Language-Team: Italian <tp@lists.linux.it>
Language: it
MIME-Version: 1.0
Content-Type: text/plain; charset=ISO-8859-1
Content-Transfer-Encoding: 8-bit
X-Bugs: Report translation errors to the Language-Team address.
Plural-Forms: nplurals=2; plural=n != 1;
 
Se non � usata nessuna delle opzioni -e, --expression, -f o --file allora il
primo argomento che non � una opzione sar� usato come lo script sed da
interpretare. Tutti gli argomenti rimanenti sono nomi di file di input; se non
sono specificati file di input sar� letto lo standard input.

       --help     mostra questo aiuto ed esce
       --version  stampa le informazioni sulla versione ed esce
   --follow-symlinks
                 segue i link simbolici quando viene utilizzato -i
   --posix
                 disabilita tutte le estensioni GNU.
   -R, --regexp-perl
                 usa la sintassi Perl 5 per le espressioni regolari
   -b, --binary
                 apre i file in modo binario (lasciando le sequenze CR+LF immutate)
   -e script, --expression=script
                 aggiunge lo script ai comandi da eseguire
   -f script-file, --file=file-script
                 aggiunge il contenuto di file-script ai comandi da eseguire
   -l N, --line-length=N
                 specifica la lunghezza delle linee generate dal comando `l'
   -n, --quiet, --silent
                 sopprime la stampa automatica del pattern space
   -u, --unbuffered
                 carica e visualizza i dati una a pezzetti piu' piccoli
 %s: espressione -e #%lu, carattere %lu: %s
 %s: impossibile leggere %s: %s
 %s: file %s riga %lu: %s
 : non accetta indirizzi Riferimento non valido Nome non valido per una classe di caratteri Carattere di ordinamento non valido numero di ripetizioni specificato tra graffe non valido Espressione regolare precedente non valida Fine dell'intervallo non valida Espressione regolare non valida Memoria esaurita Nessuna corrispondenza trovata Occorre un'espressione regolare precedente Fine prematura dell'espressione regolare Espressione regolare troppo grande Successo Barra rovesciata alla fine dell'espressione regolare `(' o `\(' non bilanciata `)' o `\)' non bilanciata `\{' non bilanciata Utilizzo: %s [OPZIONE]... {script-se-nessun-altro-specificato} [input-file]...

 comando `e' non supportato `}' non accetta indirizzi impossibile trovare un'etichetta per il salto a `%s' impossibile rimuovere %s: %s impossibile rinominare %s: %s impossibile ottenere informazioni su %s: %s il comando usa solo un indirizzo i commenti non accettano indirizzi impossibile accedere a %s: %s impossibile modificare %s: � un terminale impossibile modificare %s: non � un file normale impossibile seguire il link simbolico %s: %s impossibile aprire il file %s: %s impossibile aprire il file temporaneo %s: %s il carattere delimitatore � multi-byte errore in un sottoprocesso atteso \ dopo `a', `c' o `i' attesa una versione piu' recente di sed ci sono altri caratteri dopo il comando riferimento non valido \%d nel secondo membro del comando `s' impossibile usare +N o ~N come primo indirizzo utilizzo non valido dell'indirizzo 0 manca il comando `!' multipli opzioni `g' multiple al comando `s' opzioni `p' multiple al comando `s' opzioni numeriche multiple al comando `s' nessun file in ingresso occorre un'espressione regolare precedente l'opzione numerica del comando `s' non pu� essere zero opzione `e' non supportata errore di lettura su %s: %s le stringhe per il comandi `y' hanno lunghezze diverse `,' inattesa `}' inattesa comando sconosciuto: `%c' opzione di `s' sconosciuta `{' non bilanciata comando `s' non terminato comando `y' non terminato espressione regolare non terminata nell'indirizzo 