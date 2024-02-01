#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
//공룡 캐릭터
byte dino [8]
{
  B00000, 
  B00111, 
  B00101, 
  B10111, 
  B11100, 
  B11111, 
  B01101, 
  B01100,
} ;
//나무 캐릭터
byte tree [8]
{
  B00011,
  B11011,
  B11011, 
  B11011, 
  B11011, 
  B11111, 
  B01110, 
  B01110
};

const int BUTTON_ENTER = 2;
const int LCD_COLUMN = 16;
const int TREE_CHAR = 6;
const int DINO_CHAR = 7;

boolean isPlaying = true;
boolean isDinoOnGround = true;

// 게임 실행 상태로 초기화
int score = 0;
boolean buttonPressed = false; // 스위치 입력 여부를 확인하기 위한 변수

void setup() {
  Wire.begin() ; 
  lcd.init() ;
  lcd.backlight() ;
  lcd.createChar (DINO_CHAR, dino) ;
  lcd.createChar (TREE_CHAR, tree) ;
  pinMode(BUTTON_ENTER, INPUT_PULLUP) ;
}
void loop () 
{
  lcd. clear ();
  if (isPlaying) {
    handleGame () ;
  } else {
  // 게임이 종료된 경우 스위치가 눌릴 때까지 대기
    if (digitalRead (BUTTON_ENTER) == LOW && !buttonPressed) {
      buttonPressed = true;
      isPlaying = true; // 게임 재시작
      delay (500) ; // 디바운스를 위한 딜레이
    } else if (digitalRead(BUTTON_ENTER)==HIGH && buttonPressed){
      buttonPressed = false;
    }
  }
  delay(300);
}

void handleGame(){
  lcd.clear();
   
  int buttonPressedTimes = 0;

  int secondPosition = random(4,9);
  int thirdPosition = random(4,9);
  int firstTreePosition = LCD_COLUMN;
  const int columnValueToStopMoveTrees = -(secondPosition + thirdPosition);

  for(; firstTreePosition >=columnValueToStopMoveTrees; firstTreePosition--){
    lcd.setCursor(13,0);
    lcd.print(score);

    defineDinoPosition();

    int secondTreePosition = firstTreePosition + secondPosition;
    int thirdTreePosition = secondTreePosition + thirdPosition;

    showTree(firstTreePosition);
    showTree(secondTreePosition);
    showTree(thirdTreePosition);

    if(isDinoOnGround){
      if(firstTreePosition == 1 || secondTreePosition ==1 || thirdTreePosition ==1){
        handleGameOver();
        isPlaying = false;
        delay(5000);
        break;
      }
      buttonPressedTimes = 0;
    }
    else{
      if(buttonPressedTimes > 2){
        score-=3;
      }
      buttonPressedTimes++;
    }
    score++;
    delay(500);
  }
}

void handleGameOver(){
  lcd.clear();
  lcd.print("GAME OVER");

  lcd.setCursor(0,1);
  lcd.print("SCORE : ");
  lcd.print(score);

  score = 0;

  delay(2000);
}

void showTree(int position){
  lcd.setCursor(position, 1);
  lcd.write(TREE_CHAR);
  lcd.setCursor(position + 1, 1);
  lcd.print(" ");
}

void defineDinoPosition(){
  if(digitalRead(BUTTON_ENTER)==LOW){
    putDinoOnAir();
  } else {
    putDinoGround();
  }
}

void putDinoGround(){
  lcd.setCursor(1,1);
  lcd.write(DINO_CHAR);
  lcd.setCursor(1,0);
  lcd.print(" ");

  isDinoOnGround = true;
}

void putDinoOnAir(){
  lcd.setCursor(1,0);
  lcd.write(DINO_CHAR);
  lcd.setCursor(1,1);
  lcd.print(" ");

  isDinoOnGround = false;
}
