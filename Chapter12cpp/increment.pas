PROGRAM increment;

VAR i : integer;

BEGIN
    i := 0;
    REPEAT
        writeln('i = ', i);
        i := i + 1;
    UNTIL i > 5;
    writeln('Done!');
END.
