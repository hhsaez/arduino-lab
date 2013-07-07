int columns[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int rows[] = { 12, 11, 10 };
 
void setup()
{
  for ( int c = 0; c < 9; c++ ) {
    pinMode( columns[ c ], OUTPUT ); 
  }
 
  for (int r = 0; r < 3; r++ ) {
    pinMode( rows[ r ], OUTPUT ); 
  }

  for ( int c = 0; c < 9; c++ ) {
    for ( int r = 0; r <= 3; r++ ) {
        digitalWrite( rows[ r ], LOW);
        digitalWrite( columns[ r ], LOW);
    }
  }
}
 
void loop()
{
  for ( int i = 0; i < 5; i++ ) {
    test1();
  }
  delay( 1000 );

  for ( int i = 0; i < 10; i++ ) {
    test2();
  }
  delay( 1000 );
  
  for ( int i = 0; i < 3; i++ ) {
    test3();
  }
  delay( 1000 );
}

void test1() {
  for ( int r = 0; r < 3; r++ ) {
    for (int c = 0; c < 9; c++ ) {
      digitalWrite( rows[ r ], HIGH );
      digitalWrite( columns[ c ], HIGH );
    }
    
    delay( 500 );
    
    for (int c = 0; c < 0; c++ ) {
      digitalWrite( rows[ r ], HIGH );
      digitalWrite( columns[ c ], HIGH );
    }
  }
}
 
void test2() {
  for ( int i = 2; i >= 0; i-- ) {
    for ( int column = 1; column <= 3; column++ ) {
      for (int row = 10; row <= 12; row++) {
        digitalWrite(row, HIGH);
        digitalWrite(i * 3 + column, HIGH);
      }
    }
    delay(100);
      
    for (int column = 1; column <= 3; column++) {
      for (int row = 10; row <= 12; row++) {
        digitalWrite(row, LOW);
        digitalWrite(i * 3 + column, LOW);
      }
    }
  }
}

void test3() {
  for (int column = 1; column <= 9; column++) {
    for (int row = 10; row <= 12; row++) {
        digitalWrite(row, HIGH);
        digitalWrite(column, HIGH);
    }
  }

  delay(1000);

  for (int column = 1; column <= 9; column++) {
    for (int row = 10; row <= 12; row++) {
        digitalWrite(row, LOW);
        digitalWrite(column, LOW);
    }
  }

  delay(100);
}


