PROGRAM Complex;

TYPE
    complex = RECORD
                  re, im : real
              END;
              
    mystring = ARRAY[1..3] OF char;

VAR
    x, y, z : complex;
    
PROCEDURE print(name : mystring; VAR z : complex);
    BEGIN
        write(name, ' = (', z.re:0:5, ', ', z.im:0:5, ') ');
    END;
    
PROCEDURE add(VAR x, y, z : complex);   
    BEGIN
        z.re := x.re + y.re;
        z.im := x.im + y.im;
    END;
    
PROCEDURE subtract(VAR x, y, z : complex);
    BEGIN
        z.re := x.re - y.re;
        z.im := x.im - y.im;
    END;
    
PROCEDURE multiply(VAR x, y, z: complex);
    BEGIN
        z.re := x.re*y.re - x.im*y.im;
        z.im := x.re*y.im + x.im*y.re;
    END;
    
PROCEDURE divide(VAR x, y, z : complex);
    VAR
        denom : real;
    
    BEGIN
        denom := sqr(y.re) + sqr(y.im);
    
        z.re := (x.re*y.re + x.im*y.im)/denom;
        z.im := (x.im*y.re - x.re*y.im)/denom;
    END;

BEGIN {ComplexTest}
    x.re := 3; x.im := 2;  print('  x', x);
    y.re := 8; y.im := -5; print('  y', y);
    add(x, y, z);          print('x+y', z);
    writeln;

    print('  x', x);
    print('  y', y);
    subtract(x, y, z); print('x-y', z);
    writeln;
    
    x.re := 4; x.im := -2; print('  x', x);
    y.re := 1; y.im := -5; print('  y', y);
    multiply(x, y, z);     print('x*y', z);
    writeln;
    
    x.re := -3; x.im := 2;  print('  x', x);
    y.re := 3;  y.im := -6; print('  y', y);
    divide(x, y, z);        print('x/y', z);
    writeln;
    
    x.re := 5; x.im := 0; print('  x', x);
    y.re := 3; y.im := 2; print('  y', y);
    add(x, y, z);         print('x+y', z);
    writeln;
    
    x.re := 5; x.im := 4; print('  x', x);
    y.re := 2; y.im := 0; print('  y', y);
    multiply(x, y, z);    print('x*y', z);
    writeln;
    
    x.re := -2; x.im := -4; print('  x', x);
    y.re := 0;  y.im :=  1; print('  y', y);
    divide(x, y, z);        print('x/y', z);
    writeln;
END {ComplexTest}.
