PROGRAM ComplexBuiltIn;

TYPE
    mystring = ARRAY[1..3] OF char;

VAR
    x, y, z : complex;
    
PROCEDURE print(expr : mystring; VAR z : complex);
    BEGIN
        write(expr, ' = (', z.re:0:5, ', ', z.im:0:5, ') ');
    END;

BEGIN {ComplexTest}
    x.re := 3; x.im := 2;  print('  x', x);
    y.re := 8; y.im := -5; print('  y', y);
    z := x + y;            print('x+y', z);
    writeln;

    print('  x', x);
    print('  y', y);
    z := x - y; print('x-y', z);
    writeln;
    
    x.re := 4; x.im := -2; print('  x', x);
    y.re := 1; y.im := -5; print('  y', y);
    z := x*y;              print('x*y', z);
    writeln;

    x.re := -3; x.im := 2;  print('  x', x);
    y.re := 3;  y.im := -6; print('  y', y);
    z := x/y;               print('x/y', z);
    writeln;
    
    x.re := 5; x.im := 0; print('  x', x);
    y.re := 3; y.im := 2; print('  y', y);
    z := x + y;           print('x+y', z);
    writeln;
    
    x.re := 5; x.im := 4; print('  x', x);
    y.re := 2; y.im := 0; print('  y', y);
    z := x*y;             print('x*y', z);
    writeln;
    
    x.re := -2; x.im := -4; print('  x', x);
    y.re := 0;  y.im :=  1; print('  y', y);
    z := x/y;               print('x/y', z);
    writeln;
END {ComplexTest}.