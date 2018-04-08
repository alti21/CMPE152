PROGRAM parms3;

TYPE
    cube = ARRAY[0..1, 0..2, 0..3] OF integer;

VAR
    vvv : cube;

PROCEDURE doCube1(VAR c : cube);
    VAR
        i, j, k : integer;

    BEGIN
        FOR i := 0 TO 1 DO BEGIN
            FOR j := 0 TO 2 DO BEGIN
                FOR k := 0 TO 3 DO BEGIN
                    c[i,j][k] := 100*i + 10*j +k;
                END;
            END;
        END;
    END;

PROCEDURE doCube2(c : cube);
    VAR
        i, j, k : integer;

    BEGIN
        FOR i := 0 TO 1 DO BEGIN
            FOR j := 0 TO 2 DO BEGIN
                FOR k := 0 TO 3 DO BEGIN
                    c[i,j][k] := 200*i + 10*j +k;
                END;
            END;
        END;
    END;

PROCEDURE doCube3(VAR c : cube);
    VAR
        i, j, k : integer;

    BEGIN
        FOR i := 0 TO 1 DO BEGIN
            FOR j := 0 TO 2 DO BEGIN
                FOR k := 0 TO 3 DO BEGIN
                    c[i,j][k] := 300*i + 10*j +k;
                END;
            END;
        END;
    END;

PROCEDURE printCube(VAR c : cube);
    VAR
        i, j, k : integer;

    BEGIN
        FOR j := 0 TO 2 DO BEGIN
            FOR i := 0 TO 1 DO BEGIN
                FOR k := 0 TO 3 DO BEGIN
                    write(c[i][j,k]:4);
                END;
                write('     ');
            END;
            writeln;
        END;
        writeln;
    END;

BEGIN
    write('Cube 1 (reference) ... ');
    doCube1(vvv);
    writeln('done!');
    printCube(vvv);

    write('Cube 2 (value) ... ');
    doCube2(vvv);
    writeln('done!');
    printCube(vvv);

    write('Cube 3 (reference) ... ');
    doCube3(vvv);
    writeln('done!');
    printCube(vvv);
END.
