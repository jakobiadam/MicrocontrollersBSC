#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

int Button1=3;
int Button2=4;
int Button3=5;
int Button4=6;

int soundSensor=2;

int buzzer=7;

int LED1=9;
int LED2=10;
int LED3=11;
int LED4=12;
int LED5=13;

int time_s_goal=500;
int time_s=0;
unsigned long time_in_interrupt=0;
unsigned long last_interrupt2_time=micros();

int jumps_goal=500;
int jumps=0;

int ciklus_feltetel=1;

int d_cycl=10;
int d_t_ac=30;
int d_t_pa=60;

void setup() {
  lcd.begin();
  lcd.backlight();

  Serial.begin(9600);

  pinMode(Button1,INPUT);
  pinMode(Button2,INPUT);
  pinMode(Button3,INPUT);
  pinMode(Button4,INPUT);
  pinMode(soundSensor,INPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(Button1), interrupt1, RISING);
  attachInterrupt(digitalPinToInterrupt(soundSensor), interrupt2, RISING);

  menu_print();
}

void loop() {
  if (digitalRead(Button1)==true) {
    delay(300);
    setup_A();
  } else if (digitalRead(Button2)==true) {
    delay(300);
    setup_B();
  } else if (digitalRead(Button3)==true) {
    delay(300);
    setup_C();
  } else if (digitalRead(Button4)==true) {
    delay(300);
    setup_D();
  }
}

void interrupt1() {
  ciklus_feltetel=0;
}

void interrupt2() {
  unsigned long interrupt2_time_start=micros();
  unsigned long interrupt_delta_t=interrupt2_time_start-last_interrupt2_time;
  if (interrupt_delta_t>150000){ // átfordulásnál hamis, ha nem pont átfordulás előtt történt ugrás, akkor egyel kevesebbet számol.
    jumps+=1;                    // 70 percenként fordul csak át a micros, így valószínűleg 70 percenként
  }                              // hibázik a program egy ugrást a felhasználó ellenére.
  last_interrupt2_time=micros();
  unsigned long time_in_this_interrupt=last_interrupt2_time-interrupt2_time_start;
  if (time_in_this_interrupt>0) { //ha pont ebben az interruptban fordul át a micros, akkor egy interruptnyi idő nem lesz számontartva.
    time_in_interrupt+=(time_in_this_interrupt/1000);
  }
}

void menu_print() {
  lcd.setCursor(0,0);
  lcd.print("      MENU      ");
  lcd.setCursor(0,1);
  lcd.print("  A   B   C   D ");
}

void setup_print(int jt) {
  lcd.setCursor(12,1);
  lcd.print("    ");
  lcd.setCursor(12,1);
  lcd.print(jt,DEC);
}

void ABC_print() {
  lcd.setCursor(5,1);
  lcd.print(time_s,DEC);
  lcd.setCursor(11,1);
  lcd.print(jumps,DEC);
}

int setup_jt(int previous_goal,int lepes) {
  int jt=previous_goal;
  boolean B_gomb=false;
  while(true) {
    if (digitalRead(Button2)) {
      if (B_gomb) {
        B_gomb=false;
        lepes=lepes/10;
        lcd.setCursor(3,1);
        lcd.print("   ");
        lcd.setCursor(3,1);
        lcd.print(lepes,DEC);
        
      } else {
        B_gomb=true;
        lepes*=10;
        lcd.setCursor(3,1);
        lcd.print("   ");
        lcd.setCursor(3,1);
        lcd.print(lepes,DEC);
      }
      delay(500);
    } else if (digitalRead(Button1)) {
      jt=(jt-lepes+10000)%10000;
      setup_print(jt);
      delay(200);
    } else if (digitalRead(Button3)) {
      jt=(jt+lepes)%10000;
      setup_print(jt);
      delay(200);
    } else if (digitalRead(Button4)) {
      return jt;
    }
  }
}

void setup_A() {
  lcd.setCursor(0,0);
  lcd.print("     Ready      ");
  lcd.setCursor(0,1);
  lcd.print("  Starts: 5s    ");
  delay(5000);
  A();
}

void setup_B() {
  lcd.setCursor(0,0);
  lcd.print("Setup jumps:    ");
  lcd.setCursor(0,1);
  lcd.print("+/-10  Goal:    ");
  lcd.setCursor(12,1);
  lcd.print(jumps_goal,DEC);
  jumps_goal=setup_jt(jumps_goal,10);
  lcd.setCursor(0,1);
  lcd.print("Start: 5 seconds");
  delay(5000);
  B();
}

void setup_C() {
  lcd.setCursor(0,0);
  lcd.print("Setup time_:    ");
  lcd.setCursor(0,1);
  lcd.print("+/-10  Goal:    ");
  lcd.setCursor(12,1);
  lcd.print(time_s_goal,DEC);
  time_s_goal=setup_jt(time_s_goal,10);
  lcd.setCursor(0,0);
  lcd.print("Setup jumps:    ");
  lcd.setCursor(0,1);
  lcd.print("+/-10  Goal:    ");
  lcd.setCursor(12,1);
  lcd.print(jumps_goal,DEC);
  delay(1000);
  jumps_goal=setup_jt(jumps_goal,10);
  lcd.setCursor(0,1);
  lcd.print("Start: 5 seconds");
  delay(5000);
  C();
}

void setup_D() {
  lcd.setCursor(0,0);
  lcd.print("Setup t_active: ");
  lcd.setCursor(0,1);
  lcd.print("+/-1   t_ac:    ");
  lcd.setCursor(12,1);
  lcd.print(d_t_ac,DEC);
  d_t_ac=setup_jt(d_t_ac,1);
  lcd.setCursor(0,0);
  lcd.print("Setup t_passive:");
  lcd.setCursor(0,1);
  lcd.print("+/-1   t_pa:    ");
  lcd.setCursor(12,1);
  lcd.print(d_t_pa,DEC);
  delay(1000);
  d_t_pa=setup_jt(d_t_pa,1);
  lcd.setCursor(0,0);
  lcd.print("Setup ciklusok: ");
  lcd.setCursor(0,1);
  lcd.print("+/-1   cycl:    ");
  lcd.setCursor(12,1);
  lcd.print(d_cycl,DEC);
  delay(1000);
  d_cycl=setup_jt(d_cycl,1);
  lcd.setCursor(0,1);
  lcd.print("Start: 5 seconds");
  delay(5000);
  D();
}

void A() {
  //kijelzo kezdo allas
  lcd.setCursor(0,0);
  lcd.print("Goal:----s;----j");
  lcd.setCursor(0,1);
  lcd.print(" Now:0   s;0   j");

  tone(buzzer, 200, 200); //E4
  delay(200);
  unsigned long time_start=millis();
  unsigned long time_now=millis();
  
  jumps=0;
  time_s=0;
  ciklus_feltetel=1;
  time_in_interrupt=0;
  
  do
  {
    time_now=millis();
    time_s=(time_now-time_start+time_in_interrupt)/1000;
    ABC_print();
  } while(ciklus_feltetel==1);
  if (fin_or_quit()) {
    finish(2);
  } else {
    exit_workout();
  }
}

void B() {
  lcd.setCursor(0,0);
  lcd.print("Goal:----s;    j");
  lcd.setCursor(0,1);
  lcd.print(" Now:0   s;0   j");
  lcd.setCursor(11,0);
  lcd.print(jumps_goal,DEC);

  tone(buzzer, 200, 200); //E4
  delay(200);
  unsigned long time_start=millis();
  unsigned long time_now=millis();
  
  jumps=0;
  time_s=0;
  int jumps_kor=jumps_goal/5;
  ciklus_feltetel=1;
  time_in_interrupt=0;
  
  for (int i=0;i<5;++i) {
    do
    {
      time_now=millis();
      time_s=(time_now-time_start+time_in_interrupt)/1000;
      ABC_print();
      if (ciklus_feltetel==0) {
        exit_workout();
        return;
      }
    } while(jumps-i*jumps_kor<jumps_kor);
    digitalWrite(LED1+i,HIGH);
  }
  finish(1);
}

void C() {
  lcd.setCursor(0,0);
  lcd.print("Goal:    s;    j");
  lcd.setCursor(0,1);
  lcd.print(" Now:0   s;0   j");
  lcd.setCursor(5,0);
  lcd.print(time_s_goal,DEC);
  lcd.setCursor(11,0);
  lcd.print(jumps_goal,DEC);

  tone(buzzer, 200, 200); //E4
  delay(200);
  unsigned long time_start=millis();
  unsigned long time_now=millis();
  
  jumps=0;
  time_s=0;
  int time_s_kor=time_s_goal/5;
  ciklus_feltetel=1;
  time_in_interrupt=0;
  
  for (int i=0;i<5;++i) {
    do
    {
      time_now=millis();
      time_s=(time_now-time_start+time_in_interrupt)/1000;
      ABC_print();
      if (ciklus_feltetel==0) {
        exit_workout();
        return;
      }
    } while(time_s-i*time_s_kor<time_s_kor);
    digitalWrite(LED1+i,HIGH);
  }
  finish(jumps>=jumps_goal);
}

void D_print(int i) {
  lcd.setCursor(2,1);
  lcd.print((i+1),DEC);
  lcd.setCursor(12,1);
  lcd.print(time_s,DEC);
}

void D() {
  lcd.setCursor(0,0);
  lcd.print("ta:   tp:   c:  ");
  lcd.setCursor(0,1);
  lcd.print("c=  ; act t=    ");
  lcd.setCursor(3,0);
  lcd.print(d_t_ac,DEC);
  lcd.setCursor(9,0);
  lcd.print(d_t_pa,DEC);
  lcd.setCursor(14,0);
  lcd.print(d_cycl,DEC);

  tone(buzzer, 200, 200); //E4
  delay(200);

  time_s=0;
  ciklus_feltetel=1;
  time_in_interrupt=0;
  unsigned long time_start;
  unsigned long time_now;
  
  for (int i=0;i<d_cycl;++i) {
    time_start=millis();
    time_now=millis();
    time_in_interrupt=0;
    lcd.setCursor(0,1);
    lcd.print("c=  ; act t=    ");
    do
    {
      time_now=millis();
      time_s=(time_now-time_start+time_in_interrupt)/1000;
      D_print(i);
      if (ciklus_feltetel==0) {
        exit_workout();
        return;
      }
    } while(time_s<d_t_ac);
    tone(buzzer, 200, 200);
    time_start=millis();
    time_now=millis();
    time_in_interrupt=0;
    lcd.setCursor(0,1);
    lcd.print("c=  ; pas t=    ");
    do
    {
      time_now=millis();
      time_s=(time_now-time_start+time_in_interrupt)/1000;
      D_print(i);
      if (ciklus_feltetel==0) {
        exit_workout();
        return;
      }
    } while(time_s<d_t_pa and i<d_cycl-1);
    tone(buzzer, 400, 200);
  }
  finish(1);
}

void set_back() {
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,LOW);
  digitalWrite(LED3,LOW);
  digitalWrite(LED4,LOW);
  digitalWrite(LED5,LOW);
  menu_print();
}

void serial_message() {
  Serial.print(time_s,DEC);
  Serial.print("    ");
  Serial.print(jumps,DEC);
  Serial.print("\n");
}

void light_two_LED_with_delay(int t_delay) {
  
  int led_first=random(LED1,LED5+1);
  int led_second=0;
  do
  {
    led_second=random(LED1,LED5+1);
  } while(led_first==led_second);
  digitalWrite(led_first,HIGH);
  digitalWrite(led_second,HIGH);
  delay(t_delay);
  digitalWrite(led_first,LOW);
  digitalWrite(led_second,LOW);
}

void victory() {
  for (int i=0;i<4;++i) {
    lcd.setCursor(0,0);
    lcd.print("    VICTORY     ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    tone(buzzer, 330, 330); //E4
    light_two_LED_with_delay(330);
    tone(buzzer, 330, 330); //E4
    light_two_LED_with_delay(330);
    tone(buzzer, 330, 330); //E4
    light_two_LED_with_delay(330);
    tone(buzzer, 330, 330); //E4
    light_two_LED_with_delay(330);
    tone(buzzer, 330, 330); //E4
    light_two_LED_with_delay(330);
    tone(buzzer, 349, 330); //F4
    light_two_LED_with_delay(330);
    tone(buzzer, 294, 600); //D4
    light_two_LED_with_delay(600);
    tone(buzzer, 294, 330); //D4
    light_two_LED_with_delay(330);
    tone(buzzer, 262, 330); //C4
    light_two_LED_with_delay(330);
    tone(buzzer, 262, 600); //C4
    light_two_LED_with_delay(600);
    tone(buzzer, 262, 330); //C4
    light_two_LED_with_delay(330);
    tone(buzzer, 235, 330); //A#3 akar lenni, de at kellett irnom, mert rossz volt a "helyen"
    light_two_LED_with_delay(330);
    tone(buzzer, 235, 330);
    light_two_LED_with_delay(600);
  }
}

void defeat() {
  for (int i=0;i<4;++i) {
    lcd.setCursor(0,0);
    lcd.print("     DEFEAT     ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    tone(buzzer, 330, 330); //E4
    light_two_LED_with_delay(600);
    tone(buzzer, 311, 330); //D#4
    light_two_LED_with_delay(600);
    tone(buzzer, 294, 330); //D4
    light_two_LED_with_delay(600);
    tone(buzzer, 277, 500); //C#4
    light_two_LED_with_delay(900);
  }
}

void finish(int vicdef) {
  serial_message();
  if (vicdef==1) {
    victory();
  } else if (vicdef==0){
    defeat();
  } else {
    lcd.setCursor(0,0);
    lcd.print("    FINISHED    ");
    lcd.setCursor(0,1);
    lcd.print("                ");
    for (int i=0;i<50;++i) {
      light_two_LED_with_delay(100);
    }
  }
  set_back();
}

void exit_workout() {
  lcd.setCursor(0,0);
  lcd.print("   GOOD BYE!    ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  delay(2000);
  set_back();
}

boolean fin_or_quit() {
  lcd.setCursor(0,0);
  lcd.print("B: save result  ");
  lcd.setCursor(0,1);
  lcd.print("C: abort result ");
  while(true) {
    if (digitalRead(Button2)) {
      return true;
    } else if (digitalRead(Button3)) {
      return false;
    }
  }
}
