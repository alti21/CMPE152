PROGRAM prog;
VAR i, j : integer;

PROCEDURE proc1;
    VAR i : integer;

    PROCEDURE proc2b; forward;

    PROCEDURE proc2a;
        VAR i, j : integer;

        BEGIN {proc2a}
            i := 100;
            j := 200;
            proc2b;
        END;

    PROCEDURE proc2b;
        VAR k : integer;
        
        BEGIN {proc2b}
            k := i + j;
            writeln(k);
        END;

    BEGIN {proc1}
        i := 10;
        proc2a;
    END;

BEGIN {prog}
    i := 1;
    j := 2;
    proc1;
END.
